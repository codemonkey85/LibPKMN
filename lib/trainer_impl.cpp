/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/cast.hpp>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/trainer.hpp>
#include <pkmnsim/database/queries.hpp>
#include <pkmnsim/types/prng.hpp>

#include "team_pokemon_gen1impl.hpp"
#include "team_pokemon_gen2impl.hpp"
#include "team_pokemon_modernimpl.hpp"

#include "trainer_impl.hpp"

using namespace std;

namespace pkmnsim
{
    trainer::sptr trainer::make(unsigned int game, pokemon_text name, unsigned int gender)
    {
        return sptr(new trainer_impl(game, name, gender));
    }

    prng trainer_impl::_rand_gen;

    trainer_impl::trainer_impl(unsigned int game, pokemon_text name, unsigned int gender): trainer()
    {
        _game_id = game;
        _trainer_name = name;
        _gender = gender;
        _money = gender;
        _trainer_id = _rand_gen.lcrng_next(database::get_generation(game));

        _party = pokemon_team_t(6);
        for(int i = 0; i < 6; i++)
        {
            _party[i] = team_pokemon::make(Species::NONE, _game_id, 1, Moves::NONE, Moves::NONE,
                                          Moves::NONE, Moves::NONE);
        }
        
        _item_bag = bag::make(_game_id);
    }

    unsigned int trainer_impl::get_game_id() const {return _game_id;}

    unsigned int trainer_impl::get_money() const {return _money;}

    void trainer_impl::set_money(unsigned int money)
    {
        _money = (money > 999999) ? 999999 : _money;
    }

    pokemon_team_t trainer_impl::get_party() {return _party;}

    team_pokemon::sptr trainer_impl::get_pokemon(unsigned int pos, bool copy)
    {
        team_pokemon::sptr to_return = (pos > 6) ? _party[5] : _party[pos];
        
        if(copy)
        {
            switch(database::get_generation(_game_id))
            {
                case 1:
                {
                    team_pokemon_gen1impl actual1 = *(boost::polymorphic_downcast<team_pokemon_gen1impl*>(to_return.get()));
                    return team_pokemon::sptr(&actual1);
                }
                    
                case 2:
                {
                    team_pokemon_gen2impl actual2 = *(boost::polymorphic_downcast<team_pokemon_gen2impl*>(to_return.get()));
                    return team_pokemon::sptr(&actual2);
                }
                                    
                default:
                {
                    team_pokemon_modernimpl actual345 = *(boost::polymorphic_downcast<team_pokemon_modernimpl*>(to_return.get()));
                    return team_pokemon::sptr(&actual345);
                }
            }
        }
        else return to_return;
    }

    //TODO: check for validity
    void trainer_impl::set_party(pokemon_team_t &team) {_party = team;}

    void trainer_impl::set_pokemon(unsigned int pos, team_pokemon::sptr t_pkmn)
    {
        unsigned int actual_pos = (pos > 6) ? 5 : (pos == 0) ? 0 : (pos-1);
        _party[actual_pos] = t_pkmn;
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

    const bag::sptr trainer_impl::get_bag() const {return _item_bag;}
    
    pokemon_text trainer_impl::get_name() const {return _trainer_name;}

    void trainer_impl::set_name(pokemon_text name)
    {
        _trainer_name = (name.std_string().length() > 10) ? _trainer_name : pokemon_text(name);
    }

    unsigned int trainer_impl::get_gender() const {return _gender;}
    
    void trainer_impl::set_gender(unsigned int gender) {_gender = gender;}
    
    unsigned int trainer_impl::get_id() const {return _trainer_id;}

    unsigned short trainer_impl::get_public_id() const {return _tid.public_id;}

    unsigned short trainer_impl::get_secret_id() const {return _tid.secret_id;}

    void trainer_impl::set_id(unsigned int id) {_trainer_id = id;}

    void trainer_impl::set_public_id(unsigned short id) {_tid.public_id = id;}

    void trainer_impl::set_secret_id(unsigned short id) {_tid.secret_id = id;}
} /* namespace pkmnsim */
