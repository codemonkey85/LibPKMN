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
    trainer::sptr trainer::make(unsigned int game, pokemon_text name, unsigned int gndr)
    {
        return sptr(new trainer_impl(game, name, gndr));
    }

    trainer_impl::trainer_impl(unsigned int game, pokemon_text name, unsigned int gndr): trainer()
    {
        prng rand_gen;

        game_id = game;
        trainer_name = name;
        gender = gndr;
        money = gender;
        trainer_id = rand_gen.lcrng_next(database::get_generation(game));

        pokemon_team_t party = pokemon_team_t(6);
        for(int i = 0; i < 6; i++)
        {
            party[i] = team_pokemon::make(Species::NONE, game, 1, Moves::NONE, Moves::NONE,
                                          Moves::NONE, Moves::NONE);
        }
        
        item_bag = bag::make(game_id);
    }

    unsigned int trainer_impl::get_game_id() const {return game_id;}

    unsigned int trainer_impl::get_money() const {return money;}

    void trainer_impl::set_money(unsigned int new_money)
    {
        money = (new_money > 999999) ? 999999 : new_money;
    }

    pokemon_team_t trainer_impl::get_party() {return party;}

    team_pokemon::sptr trainer_impl::get_pokemon(unsigned int pos, bool copy)
    {
        team_pokemon::sptr to_return = (pos > 6) ? party[5] : party[pos];
        
        if(copy)
        {
            switch(database::get_generation(game_id))
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

    void trainer_impl::set_party(pokemon_team_t &team) {party = team;}

    void trainer_impl::set_pokemon(unsigned int pos, team_pokemon::sptr t_pkmn)
    {
        unsigned int actual_pos = (pos > 6) ? 5 : (pos == 0) ? 0 : (pos-1);
        party[actual_pos] = t_pkmn;
    }

    void trainer_impl::remove_pokemon(unsigned int pos)
    {
        unsigned int actual_pos = (pos > 6) ? 5 : (pos == 0) ? 0 : (pos-1);
    
        team_pokemon::sptr blank_pokemon = team_pokemon::make(Species::NONE, game_id, 0, Moves::NONE,
                                           Moves::NONE, Moves::NONE, Moves::NONE);

        set_pokemon(actual_pos, blank_pokemon);
        
        //Move over any non-blank Pokemon in later positions
        for(int i = (actual_pos+1); i < 6; i++)
        {
            if(party[i]->get_species_id() == Species::NONE) break;
            else
            {
                party[i-1] = party[i];
                party[i] = blank_pokemon;
            }
        }
    }

    bag::sptr trainer_impl::get_bag() const {return item_bag;}
    
    pokemon_text trainer_impl::get_name() const {return trainer_name;}

    void trainer_impl::set_name(pokemon_text new_name)
    {
        trainer_name = (new_name.std_string().length() > 10) ? trainer_name : pokemon_text(new_name);
    }

    unsigned int trainer_impl::get_gender() const {return gender;}
    
    void trainer_impl::set_gender(unsigned int new_gender) {gender = new_gender;}
    
    unsigned int trainer_impl::get_id() const {return trainer_id;}

    unsigned short trainer_impl::get_public_id() const {return tid.public_id;}

    unsigned short trainer_impl::get_secret_id() const {return tid.secret_id;}

    void trainer_impl::set_id(unsigned int id) {trainer_id = id;}

    void trainer_impl::set_public_id(unsigned short id) {tid.public_id = id;}

    void trainer_impl::set_secret_id(unsigned short id) {tid.secret_id = id;}
}
