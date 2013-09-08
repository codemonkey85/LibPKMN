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

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/spec_pokemon.hpp>

#include "spec_pokemon_gen1impl.hpp"
#include "spec_pokemon_gen2impl.hpp"
#include "spec_pokemon_gen345impl.hpp"
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    spec_pokemon::spec_pokemon(base_pokemon::sptr b, int m1, int m2,
                               int m3, int m4, int g, int l)
    {
        base = b;
        nickname = base->get_species_name();
        level = l;
        from_game = g;
        from_gen = b->get_generation();
        SQLite::Database db(get_database_path().c_str()); //Filepath given by CMake
        int base_pokemon_id = base->get_pokemon_id();
        int base_species_id = base->get_species_id();

		attributes = dict<string, int>();
        moves = b_move_vla_t(4);

        icon_path = base->get_icon_path(true);

        moves[0] = base_move::make(m1,from_gen);
        num_moves = 1;
        if(m2 != Moves::NONE) moves[1] = base_move::make(m2,from_gen);
        else
        {
            moves[1] = base_move::make(Moves::NONE,from_gen);
            num_moves = 2;
        }
        if(m3 != Moves::NONE) moves[2] = base_move::make(m3,from_gen);
        else
        {
            moves[2] = base_move::make(Moves::NONE,from_gen);
            num_moves = 3;
        }
        if(m4 != Moves::NONE) moves[3] = base_move::make(m4,from_gen);
        else
        {
            moves[3] = base_move::make(Moves::NONE,from_gen);
            num_moves = 4;
        }
    }

    spec_pokemon::sptr spec_pokemon::make(int id, int game, int level, int move1,
                                          int move2, int move3, int move4)
    {
        try
        {
            base_pokemon::sptr base = base_pokemon::make(id, game);

            if(base->get_generation() < 1 or base->get_generation() > 5) throw runtime_error("Gen must be 1-5.");

            switch(base->get_generation())
            {
                case 1:
                    return sptr(new spec_pokemon_gen1impl(base, level, game,
                                                       move1, move2, move3, move4));

                case 2:
                    return sptr(new spec_pokemon_gen2impl(base, level, game,
                                                       move1, move2, move3, move4));

                default:
                    return sptr(new spec_pokemon_gen345impl(base, level, game,
                                                       move1, move2, move3, move4));
            }
        }
        catch(exception &e)
        {
            cout << "Caught exception: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    base_pokemon::sptr spec_pokemon::get_base_pokemon(void) {return base;}

    dict<int, std::string> spec_pokemon::get_types(void) {return base->get_types();}

    string spec_pokemon::get_nickname(void) {return nickname;}

    int spec_pokemon::get_level(void) {return level;}

    b_move_vla_t spec_pokemon::get_moves(void) {return moves;}
    
    int spec_pokemon::get_game_id(void) {return from_game;}
    
    int spec_pokemon::get_generation(void) {return from_gen;}

    string spec_pokemon::get_icon_path(void) {return icon_path;}

    string spec_pokemon::get_sprite_path(void) {return sprite_path;}

	int spec_pokemon::get_attribute(string attribute) {return attributes[attribute];}

    dict<string, int> spec_pokemon::get_attributes(void) {return attributes;}

    bool spec_pokemon::has_attribute(string attribute) {return attributes.has_key(attribute);}

	void spec_pokemon::set_attribute(string attribute, int value) {attributes[attribute] = value;}

    string spec_pokemon::get_species_name(void) {return base->get_species_name();}

    dict<string, int> spec_pokemon::get_base_stats(void) {return base->get_base_stats();}

    dict<string, int> spec_pokemon::get_ev_yields(void) {return base->get_ev_yields();}

    bool spec_pokemon::is_fully_evolved(void) {return base->is_fully_evolved();}

    int spec_pokemon::get_pokemon_id(void) {return base->get_pokemon_id();}

    int spec_pokemon::get_species_id(void) {return base->get_species_id();}
}
