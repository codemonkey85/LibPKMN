/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/assign.hpp>
#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/trainer.hpp>
#include <pkmn/database/queries.hpp>
#include <pkmn/types/dict.hpp>

#include "bag_impl.hpp"
#include "copy_sptrs.hpp"
#include "team_pokemon_gen1impl.hpp"
#include "team_pokemon_gen2impl.hpp"
#include "team_pokemon_modernimpl.hpp"

#include "trainer_impl.hpp"

using namespace std;

namespace pkmn
{
    trainer::sptr trainer::make(unsigned int game, pokemon_text name, unsigned int gender)
    {
        return sptr(new trainer_impl(game, name, gender));
    }

    trainer::sptr trainer::make(std::string game, pokemon_text name, std::string gender)
    {
        return make(database::get_game_id(game),
                    name,
                    ((gender == "Female") ? Genders::FEMALE : Genders::MALE));
    }

    prng trainer_impl::_rand_gen;

    trainer_impl::trainer_impl(unsigned int game, pokemon_text name, unsigned int gender): trainer()
    {
        _game_id = game;
        _generation = database::get_generation(_game_id);
        _trainer_name = name;
        _gender_id = gender;
        _money = 0;
        _trainer_id = _rand_gen.lcrng_next(_generation);

        _party = pokemon_team_t(6);
        for(int i = 0; i < 6; i++)
        {
            _party[i] = team_pokemon::make(Species::NONE, _game_id, 1, Moves::NONE, Moves::NONE,
                                          Moves::NONE, Moves::NONE);
        }
        
        _bag = bag::make(_game_id);
    }

    pokemon_text trainer_impl::get_game() const {return database::get_game_name(_game_id);}

    unsigned int trainer_impl::get_generation() const {return _generation;}

    pokemon_text trainer_impl::get_name() const {return _trainer_name;}

    unsigned int trainer_impl::get_money() const {return _money;}

    pokemon_text trainer_impl::get_gender() const
    {
        return (_gender_id == Genders::MALE) ? "Male" : "Female";
    }

    unsigned int trainer_impl::get_id() const {return _trainer_id;}

    unsigned short trainer_impl::get_public_id() const {return _tid.public_id;}

    unsigned short trainer_impl::get_secret_id() const {return _tid.secret_id;}

    void trainer_impl::set_name(pokemon_text name)
    {
        _trainer_name = (name.std_string().length() <= 7) ? name : _trainer_name;
    }

    void trainer_impl::set_money(unsigned int money)
    {
        _money = (money > 999999) ? _money : money;
    }

    void trainer_impl::set_gender(unsigned int gender)
    {
        if(gender == Genders::MALE or gender == Genders::FEMALE) _gender_id = gender;
    }

    void trainer_impl::set_id(unsigned int id) {_trainer_id = id;}

    void trainer_impl::set_public_id(unsigned short id) {_tid.public_id = id;}
    
    void trainer_impl::set_secret_id(unsigned short id) {_tid.secret_id = id;}

    team_pokemon::sptr trainer_impl::get_pokemon(unsigned int pos, bool copy)
    {
        //If invalid position given, return invalid Pokemon
        if(pos == 0 or pos > 6) return team_pokemon::make(Species::INVALID, _game_id, 0, Moves::NONE,
                                                          Moves::NONE, Moves::NONE, Moves::NONE);
        else
        {
            if(copy) return copy_team_pokemon(_party[pos-1]);
            else return _party[pos-1];
        }
    }

    void trainer_impl::set_pokemon(unsigned int pos, team_pokemon::sptr t_pkmn)
    {
        //Check for valid position, don't do anything otherwise
        if(pos >= 1 and pos <= 6)
        {
            //TODO: more through check (items, forms, etc)
            if(database::get_version_group(_game_id) ==
               database::get_version_group(t_pkmn->get_game_id())) _party[pos-1] = copy_team_pokemon(t_pkmn);
        }
    }

    void trainer_impl::remove_pokemon(unsigned int pos)
    {
        unsigned int actual_pos = (pos > 6) ? 5 : (pos == 0) ? 0 : (pos-1);
    
        team_pokemon::sptr blank_pokemon = team_pokemon::make(Species::NONE, _game_id, 0, Moves::NONE,
                                           Moves::NONE, Moves::NONE, Moves::NONE);

        set_pokemon(actual_pos, blank_pokemon);
        
        //Move over any non-blank Pokemon in later positions
        for(int i = (actual_pos+1); i < 6; i++)
        {
            if(_party[i]->get_species_id() == Species::NONE) break;
            else
            {
                _party[i-1] = _party[i];
                _party[i] = blank_pokemon;
            }
        }
    }

    void trainer_impl::get_party(pokemon_team_t& party)
    {
        //Have to copy all Pokemon, so can't simply assign from _party
        party = pokemon_team_t(6);
        for(size_t i = 0; i < 6; i++) party[i] = copy_team_pokemon(_party[i]);
    }

    void trainer_impl::set_party(pokemon_team_t& party)
    {
        //Only set party if party and all Pokemon are valid
        if(party.size() != 6) return;
        if(party[0]->get_game_id() != _game_id or
           party[1]->get_game_id() != _game_id or
           party[2]->get_game_id() != _game_id or
           party[3]->get_game_id() != _game_id or
           party[4]->get_game_id() != _game_id or
           party[5]->get_game_id() != _game_id) return;

        _party = pokemon_team_t(6);
        for(size_t i = 0; i < 6; i++) _party[i] = copy_team_pokemon(party[i]);
    }

    bag::sptr trainer_impl::get_bag(bool copy) const
    {
        if(copy) return copy_bag(_bag);
        else return _bag;
    }

    unsigned int trainer_impl::get_game_id() const {return _game_id;}
} /* namespace pkmn */