/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/cast.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/move.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/team_pokemon.hpp>
#include <pkmn/database/queries.hpp>
#include <pkmn/types/prng.hpp>

#include "base_pokemon_gen1impl.hpp"
#include "base_pokemon_gen2impl.hpp"
#include "base_pokemon_modernimpl.hpp"

#include "item_impl.hpp"
#include "item_berryimpl.hpp"
#include "item_machineimpl.hpp"

#include "team_pokemon_impl.hpp"
#include "team_pokemon_gen1impl.hpp"
#include "team_pokemon_gen2impl.hpp"
#include "team_pokemon_modernimpl.hpp"

#include "SQLiteCpp/src/SQLiteC++.h"

using namespace std;

namespace pkmn
{
    team_pokemon::sptr team_pokemon::make(unsigned int id, unsigned int game, unsigned int level,
                                          unsigned int move1, unsigned int move2,
                                          unsigned int move3, unsigned int move4)
    {
        try
        {
            base_pokemon::sptr base = base_pokemon::make(id, game);
            
            if(base->get_generation() < 1 or base->get_generation() > 5) throw runtime_error("Gen must be 1-5.");

            switch(base->get_generation())
            {
                case 1:
                    return sptr(new team_pokemon_gen1impl(base, game, level,
                                                       move1, move2, move3, move4));

                case 2:
                    return sptr(new team_pokemon_gen2impl(base, game, level,
                                                       move1, move2, move3, move4));

                default:
                    return sptr(new team_pokemon_modernimpl(base, game, level,
                                                       move1, move2, move3, move4));
            }
        }
        catch(exception &e)
        {
            cout << "Caught exception: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    SQLite::Database team_pokemon_impl::_db(get_database_path().c_str());
    prng team_pokemon_impl::_rand_gen;
    
    team_pokemon_impl::team_pokemon_impl(base_pokemon::sptr base, unsigned int game, unsigned int level,
                                         unsigned int move1, unsigned int move2,
                                         unsigned int move3, unsigned int move4): team_pokemon()
    {
        unsigned int gen = database::get_generation(game);
        
        _personality = _rand_gen.lcrng_next(gen);
        _trainer_id = _rand_gen.lcrng_next(gen);
    
        _base_pkmn = base;
        _nickname = _base_pkmn->get_name();
        _trainer_name = "Ash";
        _level = level;
        _game_id = game;
        _generation = _base_pkmn->get_generation();
        _held_item = item::make(Items::NONE, _game_id);
        _ball = PokeBalls::POKE_BALL;

		_attributes = dict<string, int>();
        _moves = moveset_t(4);
        _move_PPs = array<unsigned int>(4);

        _icon_path = _base_pkmn->get_icon_path(true);

        _moves[0] = move::make(move1, _game_id);
        _move_PPs[0] = _moves[0]->get_base_pp();
        
        _moves[1] = move::make(move2, _game_id);
        _move_PPs[1] = _moves[1]->get_base_pp();
        
        _moves[2] = move::make(move3, _game_id);
        _move_PPs[2] = _moves[2]->get_base_pp();
        
        _moves[3] = move::make(move4, _game_id);
        _move_PPs[3] = _moves[3]->get_base_pp();

        _nonvolatile_status = Statuses::OK;
    }

    base_pokemon::sptr team_pokemon_impl::get_base_pokemon(bool copy) const
    {
        base_pokemon::sptr to_return = _base_pkmn;
        
        if(copy)
        {
            switch(database::get_generation(_game_id))
            {
                case 1:
                {
                    base_pokemon_gen1impl actual1 = *(boost::polymorphic_downcast<base_pokemon_gen1impl*>(to_return.get()));
                    return base_pokemon::sptr(&actual1);
                }
                    
                case 2:
                {
                    base_pokemon_gen2impl actual2 = *(boost::polymorphic_downcast<base_pokemon_gen2impl*>(to_return.get()));
                    return base_pokemon::sptr(&actual2);
                }
                                    
                default:
                {
                    base_pokemon_modernimpl actual345 = *(boost::polymorphic_downcast<base_pokemon_modernimpl*>(to_return.get()));
                    return base_pokemon::sptr(&actual345);
                }
            }
        }
        else return to_return;
    }

    pokemon_text team_pokemon_impl::get_nickname() const {return _nickname;}
    
    void team_pokemon_impl::set_nickname(pokemon_text name)
    {
        size_t max_len = (_game_id >= Games::X) ? 12 : 10;
        if(name.std_string().length() <= max_len) _nickname = name;
    }
    
    pokemon_text team_pokemon_impl::get_trainer_name() const {return _trainer_name;}
    
    void team_pokemon_impl::set_trainer_name(pokemon_text name)
    {
        if(name.std_string().length() == 0) _trainer_name = "Ash"; //Reset to default
        else if(name.std_string().length() <= 7) _trainer_name = name;
    }

    unsigned int team_pokemon_impl::get_level() const {return _level;}
    
    void team_pokemon_impl::set_level(unsigned int level)
    {
        if(level > 0 and level <= 100) _level = level;
    }

    unsigned int team_pokemon_impl::get_met_level() const {return _met_level;}
    
    void team_pokemon_impl::set_met_level(unsigned int level)
    {
        if(level > 0 and level <= 100) _met_level = level;
    }

    moveset_t team_pokemon_impl::get_moves() const {return _moves;}
    
    array<unsigned int> team_pokemon_impl::get_move_PPs() const {return _move_PPs;}

    void team_pokemon_impl::set_move(unsigned int move_id, unsigned int pos)
    {
        //Will fail if move is incompatible with this generation
        unsigned int actual_pos = (pos > 4) ? 3 : (pos == 0) ? 0 : (pos-1);
        _moves[actual_pos] = move::make(move_id, _game_id);
        _move_PPs[actual_pos] = database::get_move_pp(move_id);
    }
    
    void team_pokemon_impl::set_move_PP(unsigned int PP, unsigned int pos)
    {
        //TODO: implement PP Up stats
        unsigned int actual_pos = (pos > 4) ? 3 : (pos == 0) ? 0 : (pos-1);
        if(PP <= _moves[actual_pos]->get_base_pp()) _move_PPs[actual_pos] = PP;
    }
    
    unsigned int team_pokemon_impl::get_status() const {return _nonvolatile_status;}
    
    void team_pokemon_impl::set_status(unsigned int status)
    {
        if(status >= Statuses::OK and status <= Statuses::SLEEP) _nonvolatile_status = status;
        else status = Statuses::OK;
    }
    
    unsigned int team_pokemon_impl::get_personality() const {return _personality;}
    
    void team_pokemon_impl::set_personality(unsigned int personality) {_personality = personality;}

    void team_pokemon_impl::set_ability(std::string ability)
    {
        //Check if ability is present in given generation
        unsigned int ability_id = database::get_ability_id(ability);

        if(ability_id != 0)
        {
            std::string query_string = "SELECT generation_id FROM abilities WHERE id=" + to_string(ability);
            SQLite::Statement query(_db, query_string.c_str());
            unsigned int generation_id = int(_db.execAndGet(query_string.c_str()));
            if(generation_id <= _generation) _ability = ability;
        }
    }

    void team_pokemon_impl::set_gender(unsigned int gender)
    {
        //Don't change gender if genderless
        if(gender != Genders::GENDERLESS) _gender = (gender == Genders::FEMALE) ?
                                                    Genders::FEMALE : Genders::MALE;
    }

    void team_pokemon_impl::set_nature(unsigned int nature)
    {
        _nature = (nature <= Natures::SERIOUS) ? _nature : Natures::SERIOUS;
    }

    unsigned int team_pokemon_impl::get_generation() const {return _generation;}

    item::sptr team_pokemon_impl::get_held_item(bool copy) const
    {
        item::sptr to_return = _held_item;
        
        unsigned int item_id = to_return->get_item_id();
        
        if(copy) return item::make(item_id, _game_id);
        else return to_return;
    }
    
    void team_pokemon_impl::set_held_item(item::sptr item)
    {
        //Don't set item if it doesn't exist in this game
        if(database::get_item_index(item->get_item_id(), _game_id)) _held_item = item;
    }
    
    void team_pokemon_impl::set_held_item(unsigned int item_id)
    {
        set_held_item(item::make(item_id, _game_id));
    }

    unsigned int team_pokemon_impl::get_ball() const {return _ball;}

    void team_pokemon_impl::set_ball(unsigned int ball) {_ball = ball;}
    
    string team_pokemon_impl::get_icon_path() const {return _icon_path;}

    string team_pokemon_impl::get_sprite_path() const {return _sprite_path;}
    
	int team_pokemon_impl::get_attribute(string attribute) const {return _attributes[attribute];}

    dict<string, int> team_pokemon_impl::get_attributes() const {return _attributes;}

    bool team_pokemon_impl::has_attribute(string attribute) const {return _attributes.has_key(attribute);}

	void team_pokemon_impl::set_attribute(string attribute, int value) {_attributes[attribute] = value;}
    
    void team_pokemon_impl::set_hidden_ability(bool val) {_has_hidden_ability = val;}

    unsigned int team_pokemon_impl::get_trainer_gender() const {return _otgender;}

    void team_pokemon_impl::set_trainer_gender(unsigned int gender) {_otgender = gender;}

    unsigned int team_pokemon_impl::get_trainer_id() const {return _trainer_id;}

    unsigned short team_pokemon_impl::get_public_trainer_id() const {return _tid.public_id;}

    unsigned short team_pokemon_impl::get_secret_trainer_id() const {return _tid.secret_id;}

    void team_pokemon_impl::set_trainer_id(unsigned int id) {_trainer_id = id;}

    void team_pokemon_impl::set_public_trainer_id(unsigned short id) {_tid.public_id = id;}

    void team_pokemon_impl::set_secret_trainer_id(unsigned short id) {_tid.secret_id = id;}

    string team_pokemon_impl::get_species_name() const {return _base_pkmn->get_name();}
    
    void team_pokemon_impl::get_egg_groups(std::vector<std::string>&
                                           egg_group_vec) const
    {
        egg_group_vec.clear();
        _base_pkmn->get_egg_groups(egg_group_vec);
    }
    
    void team_pokemon_impl::get_egg_group_ids(std::vector<unsigned int>
                                              &egg_group_id_vec) const
    {
        egg_group_id_vec.clear();
        _base_pkmn->get_egg_group_ids(egg_group_id_vec);
    }
                                                
    unsigned int team_pokemon_impl::get_game_id() const {return _game_id;}
    
    unsigned int team_pokemon_impl::get_pokemon_id() const {return _base_pkmn->get_pokemon_id();}

    unsigned int team_pokemon_impl::get_species_id() const {return _base_pkmn->get_species_id();}

    std::string team_pokemon_impl::get_form() const {return _base_pkmn->get_form();}
    
    string_pair_t team_pokemon_impl::get_types() const {return _base_pkmn->get_types();}
    
    dict<std::string, unsigned int> team_pokemon_impl::get_base_stats() const {return _base_pkmn->get_base_stats();}

    dict<std::string, unsigned int> team_pokemon_impl::get_ev_yields() const {return _base_pkmn->get_ev_yields();}

    bool team_pokemon_impl::is_fully_evolved() const {return _base_pkmn->is_fully_evolved();}
} /* namespace pkmn */
