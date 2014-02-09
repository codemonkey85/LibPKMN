/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/move.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/team_pokemon.hpp>
#include <pkmn/database/queries.hpp>
#include <pkmn/types/prng.hpp>

#include "copy_sptrs.hpp"

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
            
            if(base->get_generation() < 1 or base->get_generation() > 6) throw runtime_error("Gen must be 1-6.");

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

    team_pokemon::sptr team_pokemon::make(std::string name, std::string game, unsigned int level,
                                          std::string move1, std::string move2,
                                          std::string move3, std::string move4)
    {
        return make(database::get_species_id(name),
                    database::get_game_id(game),
                    level,
                    database::get_move_id(move1),
                    database::get_move_id(move2),
                    database::get_move_id(move3),
                    database::get_move_id(move4));
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
        _trainer_gender = "Male";
        _level = level;
        _game_id = game;
        _generation = _base_pkmn->get_generation();
        _held_item = Items::NONE;
        _ball = database::get_item_name(Items::POKE_BALL);
        _met_level = 1;

		_attributes = pkmn::dict<string, int>();
        _moves = moveset_t(4);
        _move_PPs = pkmn::array<unsigned int>(4);

        _icon_path = _base_pkmn->get_icon_path(true);

        _moves[0] = move::make(move1, _game_id);
        _move_PPs[0] = _moves[0]->get_base_pp();
        
        _moves[1] = move::make(move2, _game_id);
        _move_PPs[1] = _moves[1]->get_base_pp();
        
        _moves[2] = move::make(move3, _game_id);
        _move_PPs[2] = _moves[2]->get_base_pp();
        
        _moves[3] = move::make(move4, _game_id);
        _move_PPs[3] = _moves[3]->get_base_pp();

        _nonvolatile_status = "OK";
    }

    std::string team_pokemon_impl::get_game() const {return database::get_game_name(_game_id);}

    unsigned int team_pokemon_impl::get_generation() const {return _generation;}

    base_pokemon::sptr team_pokemon_impl::get_base_pokemon(bool copy) const
    {
        return (copy) ? copy_base_pokemon(_base_pkmn) : _base_pkmn;
    }

    pokemon_text team_pokemon_impl::get_species_name() const {return _base_pkmn->get_name();}

    pokemon_text team_pokemon_impl::get_nickname() const {return _nickname;}

    string_pair_t team_pokemon_impl::get_types() const {return _base_pkmn->get_types();}

    pkmn::dict<std::string, unsigned int> team_pokemon_impl::get_base_stats() const {return _base_pkmn->get_base_stats();}

    void team_pokemon_impl::set_nickname(pokemon_text nickname)
    {
        unsigned int max_len = (_generation == 6) ? 12 : 10;

        if(nickname.std_string().length() > 0 and nickname.std_string().length() <= max_len) _nickname = nickname;
    }

    pokemon_text team_pokemon_impl::get_trainer_name() const {return _trainer_name;}

    pokemon_text team_pokemon_impl::get_trainer_gender() const {return _trainer_gender;}

    unsigned int team_pokemon_impl::get_trainer_id() const {return _trainer_id;}

    unsigned short team_pokemon_impl::get_trainer_public_id() const {return _tid.public_id;}

    unsigned short team_pokemon_impl::get_trainer_secret_id() const {return _tid.secret_id;}

    pokemon_text team_pokemon_impl::get_ball() const {return _ball;}

    unsigned int team_pokemon_impl::get_met_level() const {return _met_level;}

    void team_pokemon_impl::set_trainer_name(pokemon_text trainer_name)
    {
        if(trainer_name.std_string().length() > 0 and trainer_name.std_string().length() <= 7) _trainer_name = trainer_name;
    }

    void team_pokemon_impl::set_trainer_gender(pokemon_text gender)
    {
        if(gender.std_string() == "Male" or gender.std_string() == "Female") _trainer_gender = gender;
    }

    void team_pokemon_impl::set_trainer_id(unsigned int id) {_trainer_id = id;}

    void team_pokemon_impl::set_trainer_public_id(unsigned short id) {_tid.public_id = id;}

    void team_pokemon_impl::set_trainer_secret_id(unsigned short id) {_tid.secret_id = id;}

    void team_pokemon_impl::set_ball(pokemon_text ball)
    {
        //TODO: add validity check
        _ball = ball;
    }

    void team_pokemon_impl::set_met_level(unsigned int level)
    {
        _met_level = (level <= 100) ? level : _met_level;
    }

    unsigned int team_pokemon_impl::get_personality() const {return _personality;}

    unsigned int team_pokemon_impl::get_level() const {return _level;}

    bool team_pokemon_impl::using_hidden_ability() const {return _has_hidden_ability;}

    void team_pokemon_impl::set_level(unsigned int level)
    {
        _level = level;
        _set_stats();
    }

    void team_pokemon_impl::set_using_hidden_ability(bool value) {_has_hidden_ability = value;}

    pokemon_text team_pokemon_impl::get_status() const {return _nonvolatile_status;}

    item::sptr team_pokemon_impl::get_held_item() const {return item::make(_held_item, _game_id);}

    void team_pokemon_impl::set_status(pokemon_text status)
    {
        //TODO: add validity check
        _nonvolatile_status = status;
    }

    void team_pokemon_impl::set_held_item(pokemon_text item_name)
    {
        _held_item = database::get_item_id(item_name);
    }

    void team_pokemon_impl::set_held_item(item::sptr item_sptr)
    {
        _held_item = item_sptr->get_item_id();
    }

    move::sptr team_pokemon_impl::get_move(unsigned int pos) const
    {
        return (pos >= 1 and pos <= 4) ? copy_move(_moves[pos-1]) : move::make(Moves::NONE, _game_id);
    }

    moveset_t team_pokemon_impl::get_moves() const
    {
        //Copy move sptrs to prevent editing
        moveset_t to_return(4);
        for(size_t i = 0; i < 4; i++) to_return[i] = copy_move(_moves[i]);
        return to_return;
    }

    unsigned int team_pokemon_impl::get_move_PP(unsigned int pos) const
    {
        return(pos >= 1 and pos <= 4) ? _move_PPs[pos-1] : 0;
    }

    pkmn::array<unsigned int> team_pokemon_impl::get_move_PPs() const {return _move_PPs;}

    void team_pokemon_impl::set_move(pokemon_text move_name, unsigned int pos)
    {
        unsigned int move_id = database::get_move_id(move_name.std_string());
        set_move(move_id, pos);
    }

    void team_pokemon_impl::set_move(unsigned int move_id, unsigned int pos)
    {
        std::string query_string(str(boost::format("SELECT generation_id FROM moves WHERE id=%d")
                                     % move_id));
        SQLite::Statement query(_db, query_string.c_str());
        unsigned int generation = (query.executeStep()) ? int(query.getColumn(0)) : 7; //Invalid if invalid move ID is given
        if(generation <= _generation and (pos >= 1 and pos <= 4)) //Position must be valid
        {
            _moves[pos-1] = move::make(move_id, _game_id);
            _move_PPs[pos-1] = database::get_move_pp(move_id);
        }
    }

    void team_pokemon_impl::set_move(move::sptr move_sptr, unsigned int pos)
    {
        set_move(move_sptr->get_move_id(), pos);
    }

    void team_pokemon_impl::set_move_PP(unsigned int PP, unsigned int pos)
    {
        if(pos >= 1 and pos <= 4)
        {
            if(PP <= _moves[pos-1]->get_base_pp()) _move_PPs[pos-1] = PP;
        }
    }

    int team_pokemon_impl::get_attribute(std::string attribute) const
    {
        return _attributes.at(attribute, 0);
    }

    pkmn::dict<std::string,int> team_pokemon_impl::get_attributes() const {return _attributes;}

    bool team_pokemon_impl::has_attribute(std::string attribute) const
    {
        return _attributes.has_key(attribute);
    }

    void team_pokemon_impl::set_attribute(std::string attribute, int value)
    {
        _attributes[attribute] = value;
    }

    std::string team_pokemon_impl::get_icon_path() const
    {
        return _base_pkmn->get_icon_path((_gender == "Male"));
    }

    std::string team_pokemon_impl::get_sprite_path() const
    {
        return _base_pkmn->get_sprite_path((_gender == "Male"), is_shiny());
    }
    
    void team_pokemon_impl::set_form(std::string form)
    {
        _base_pkmn->set_form(form);
        _set_stats();
    }

    void team_pokemon_impl::set_form(unsigned int form)
    {
        _base_pkmn->set_form(form);
        _set_stats();
    }

    unsigned int team_pokemon_impl::get_form_id() const {return _base_pkmn->get_form_id();}

    unsigned int team_pokemon_impl::get_game_id() const {return _game_id;}

    unsigned int team_pokemon_impl::get_pokemon_id() const {return _base_pkmn->get_pokemon_id();}

    unsigned int team_pokemon_impl::get_species_id() const {return _base_pkmn->get_species_id();}

    unsigned int team_pokemon_impl::get_ability_id() const {return database::get_ability_id(_ability);}

    unsigned int team_pokemon_impl::get_item_id() const {return _held_item;}

    unsigned int team_pokemon_impl::get_nature_id() const {return database::get_nature_id(_nature);}
} /* namespace pkmn */