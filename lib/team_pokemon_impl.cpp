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

#include <pkmnsim/move.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/team_pokemon.hpp>
#include <pkmnsim/database/queries.hpp>
#include <pkmnsim/types/prng.hpp>

#include "team_pokemon_impl.hpp"
#include "team_pokemon_gen1impl.hpp"
#include "team_pokemon_gen2impl.hpp"
#include "team_pokemon_gen345impl.hpp"

using namespace std;

namespace pkmnsim
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
                    return sptr(new team_pokemon_gen345impl(base, game, level,
                                                       move1, move2, move3, move4));
            }
        }
        catch(exception &e)
        {
            cout << "Caught exception: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    team_pokemon_impl::team_pokemon_impl(base_pokemon::sptr base, unsigned int game, unsigned int lvl,
                                         unsigned int move1, unsigned int move2,
                                         unsigned int move3, unsigned int move4): team_pokemon()
    {
        prng rand_gen;
        unsigned int gen = database::get_generation(game);
        
        personality = rand_gen.lcrng_next(gen);
        trainer_id = rand_gen.lcrng_next(gen);
    
        base_pkmn = base_pkmn;
        nickname = base_pkmn->get_species_name();
        trainer_name = "Ash";
        level = lvl;
        from_game = game;
        from_gen = base_pkmn->get_generation();
        held_item = 0;
        ball = PokeBalls::POKE_BALL;

		attributes = dict<string, int>();
        moves = moveset_t(4);
        move_PPs = vla<unsigned int>(4);

        icon_path = base_pkmn->get_icon_path(true);

        moves[0] = move::make(move1, from_game);
        move_PPs[0] = moves[0]->get_base_pp();
        
        moves[1] = move::make(move2, from_game);
        move_PPs[1] = moves[1]->get_base_pp();
        
        moves[2] = move::make(move3, from_game);
        move_PPs[2] = moves[2]->get_base_pp();
        
        moves[3] = move::make(move4, from_game);
        move_PPs[3] = moves[3]->get_base_pp();
    }

    base_pokemon::sptr team_pokemon_impl::get_base_pokemon() const {return base_pkmn;}

    pokemon_text team_pokemon_impl::get_nickname() const {return nickname;}
    
    void team_pokemon_impl::set_nickname(pokemon_text name)
    {
        if(name.std_string().length() == 0) nickname = base_pkmn->get_species_name();
        else if(name.std_string().length() <= 10) nickname = name;
    }
    
    pokemon_text team_pokemon_impl::get_trainer_name() const {return trainer_name;}
    
    void team_pokemon_impl::set_trainer_name(pokemon_text name)
    {
        if(name.std_string().length() == 0) nickname = "Ash"; //Reset to default
        else if(name.std_string().length() <= 7) nickname = name;
    }

    unsigned int team_pokemon_impl::get_level() const {return level;}
    
    void team_pokemon_impl::set_level(unsigned int lvl)
    {
        if(lvl > 0 and lvl <= 100) level = lvl;
    }

    unsigned int team_pokemon_impl::get_met_level() const {return met_level;}
    
    void team_pokemon_impl::set_met_level(unsigned int lvl)
    {
        if(lvl > 0 and lvl <= 100) met_level = lvl;
    }

    moveset_t team_pokemon_impl::get_moves() const {return moves;}
    
    vla<unsigned int> team_pokemon_impl::get_move_PPs() const {return move_PPs;}

    void team_pokemon_impl::set_move(unsigned int move_id, unsigned int pos)
    {
        //Will fail if move is incompatible with this generation
        moves[pos] = move::make(move_id, from_game);
    }
    
    void team_pokemon_impl::set_move_PP(unsigned int new_PP, unsigned int pos)
    {
        //TODO: implement PP Up stats
        if(new_PP <= moves[pos]->get_base_pp()) move_PPs[pos] = new_PP;
    }
    
    unsigned int team_pokemon_impl::get_status() const {return nonvolatile_status;}
    
    void team_pokemon_impl::set_status(unsigned int status)
    {
        if(status >= Statuses::OK and status <= Statuses::SLEEP) nonvolatile_status = status;
        else status = Statuses::OK;
    }
    
    unsigned int team_pokemon_impl::get_personality() const {return personality;}
    
    void team_pokemon_impl::set_personality(unsigned int new_personality) {personality = new_personality;}

    unsigned int team_pokemon_impl::get_generation() const {return from_gen;}

    unsigned int team_pokemon_impl::get_held_item() const {return held_item;}
    
    void team_pokemon_impl::set_held_item(unsigned int item) {held_item = item;}

    unsigned int team_pokemon_impl::get_ball() const {return ball;}

    void team_pokemon_impl::set_ball(unsigned int new_ball) {ball = new_ball;}
    
    string team_pokemon_impl::get_icon_path() const {return icon_path;}

    string team_pokemon_impl::get_sprite_path() const {return sprite_path;}
    
	int team_pokemon_impl::get_attribute(string attribute) const {return attributes[attribute];}

    dict<string, int> team_pokemon_impl::get_attributes() const {return attributes;}

    bool team_pokemon_impl::has_attribute(string attribute) const {return attributes.has_key(attribute);}

	void team_pokemon_impl::set_attribute(string attribute, int value) {attributes[attribute] = value;}
    
    void team_pokemon_impl::set_hidden_ability(bool val) {has_hidden_ability = val;}

    unsigned int team_pokemon_impl::get_trainer_gender() const {return otgender;}

    void team_pokemon_impl::set_trainer_gender(unsigned int new_gender) {otgender = new_gender;}

    unsigned int team_pokemon_impl::get_trainer_id() const {return trainer_id;}

    unsigned short team_pokemon_impl::get_public_trainer_id() const {return tid.public_id;}

    unsigned short team_pokemon_impl::get_secret_trainer_id() const {return tid.secret_id;}

    void team_pokemon_impl::set_trainer_id(unsigned int id) {trainer_id = id;}

    void team_pokemon_impl::set_public_trainer_id(unsigned short id) {tid.public_id = id;}

    void team_pokemon_impl::set_secret_trainer_id(unsigned short id) {tid.secret_id = id;}
    
    vector<string> team_pokemon_impl::get_egg_group_names() const {return base_pkmn->get_egg_group_names();}

    string team_pokemon_impl::get_game_name() const {return database::get_game_name(from_game);}
    
    string team_pokemon_impl::get_species_name() const {return base_pkmn->get_species_name();}
    
    vector<unsigned int> team_pokemon_impl::get_egg_group_ids() const {return base_pkmn->get_egg_group_ids();}

    unsigned int team_pokemon_impl::get_game_id() const {return from_game;}
    
    unsigned int team_pokemon_impl::get_pokemon_id() const {return base_pkmn->get_pokemon_id();}

    unsigned int team_pokemon_impl::get_species_id() const {return base_pkmn->get_species_id();}
    
    dict<unsigned int, unsigned int> team_pokemon_impl::get_types() const {return base_pkmn->get_types();}
    
    dict<unsigned int, unsigned int> team_pokemon_impl::get_base_stats() const {return base_pkmn->get_base_stats();}

    dict<unsigned , unsigned int> team_pokemon_impl::get_ev_yields() const {return base_pkmn->get_ev_yields();}

    bool team_pokemon_impl::is_fully_evolved() const {return base_pkmn->is_fully_evolved();}
}
