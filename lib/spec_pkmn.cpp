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
#include <pkmnsim/paths.hpp>
#include <pkmnsim/spec_pkmn.hpp>

#include "spec_pkmn_gen1impl.hpp"
#include "spec_pkmn_gen2impl.hpp"
#include "spec_pkmn_gen345impl.hpp"
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    spec_pkmn::spec_pkmn(base_pkmn::sptr b, bool i, string m1, string m2,
                         string m3, string m4, int g, int l)
    {
        base = b;
        nickname = base->get_species_name();
        level = l;
        generation = g;
        SQLite::Database db(get_database_path().c_str()); //Filepath given by CMake
        int base_pkmn_id = base->get_pokemon_id();
        int base_species_id = base->get_species_id();

        moves = vla<base_move::sptr>(4);
        vector<base_move::sptr> base_legal_moves = base->get_legal_moves();

        icon_path = base->get_icon_path();

        if(i) //Don't care about legal moves
        {
            moves[0] = base_move::make(m1,g);
            num_moves = 1;
            if(m2 != "None") moves[1] = base_move::make(m2,g);
            else
            {
                moves[1] = base_move::make("struggle",g);
                num_moves = 2;
            }
            if(m3 != "None") moves[2] = base_move::make(m3,g);
            else
            {
                moves[2] = base_move::make("struggle",g);
                num_moves = 3;
            }
            if(m4 != "None") moves[3] = base_move::make(m4,g);
            else
            {
                moves[3] = base_move::make("struggle",g);
                num_moves = 4;
            }
        }
        else
        {
            bool m1_match = false;
            bool m2_match = false;
            bool m3_match = false;
            bool m4_match = false;

            for(unsigned int i = 0; i < base_legal_moves.size(); i++)
            {
                m1_match = (m1 == base_legal_moves[i]->get_name() or m1 == "None");
                m2_match = (m2 == base_legal_moves[i]->get_name() or m2 == "None");
                m3_match = (m3 == base_legal_moves[i]->get_name() or m3 == "None");
                m4_match = (m4 == base_legal_moves[i]->get_name() or m4 == "None");

                if(not m1_match and not m2_match and not m3_match and not m4_match) break;
            }
            if(not m1_match or not m2_match or not m3_match or not m4_match)
            {
                throw runtime_error("All moves must be legal moves.");
            }

            //move1 = base_move::make(m1,g);
            moves[0] = base_move::make(m1,g);
            num_moves = 1;
            if(m2 != "None") moves[1] = base_move::make(m2,g);
            else
            {
                moves[1] = base_move::make("struggle",g);
                num_moves = 2;
            }
            if(m3 != "None") moves[2] = base_move::make(m3,g);
            else
            {
                moves[2] = base_move::make("struggle",g);
                num_moves = 3;
            }
            if(m4 != "None") moves[3] = base_move::make(m4,g);
            else
            {
                moves[3] = base_move::make("struggle",g);
                num_moves = 4;
            }
        }
    }

    spec_pkmn::sptr spec_pkmn::make(string identifier, int gen, int level, string move1,
                                    string move2, string move3, string move4, bool illegal)
    {
        try
        {
            base_pkmn::sptr base = base_pkmn::make(identifier, gen, true);

            if(gen < 1 or gen > 5) throw runtime_error("Gen must be 1-5.");

            switch(gen)
            {
                case 1:
                    return sptr(new spec_pkmn_gen1impl(base, level,
                                                       move1, move2, move3, move4, illegal));

                case 2:
                    return sptr(new spec_pkmn_gen2impl(base, level,
                                                       move1, move2, move3, move4, illegal));

                default:
                    return sptr(new spec_pkmn_gen345impl(base, level, gen,
                                                       move1, move2, move3, move4, illegal));
            }
        }
        catch(exception &e)
        {
            cout << "Caught exception: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    base_pkmn::sptr spec_pkmn::get_base_pkmn(void) {return base;}

    dict<int, std::string> spec_pkmn::get_types(void) {return base->get_types();}

    string spec_pkmn::get_nickname(void) {return nickname;}

    int spec_pkmn::get_level(void) {return level;}

    bool spec_pkmn::is_shiny(void) {return shiny;}

    vla<base_move::sptr> spec_pkmn::get_moves(void) {return moves;}
    
    int spec_pkmn::get_generation(void) {return generation;}

    string spec_pkmn::get_icon_path(void) {return icon_path;}

    string spec_pkmn::get_sprite_path(void) {return sprite_path;}

    string spec_pkmn::get_species_name(void) {return base->get_species_name();}

    dict<string, int> spec_pkmn::get_base_stats(void) {return base->get_base_stats();}

    dict<string, int> spec_pkmn::get_ev_yields(void) {return base->get_ev_yields();}

    bool spec_pkmn::is_fully_evolved(void) {return base->is_fully_evolved();}

    int spec_pkmn::get_pokemon_id(void) {return base->get_pokemon_id();}

    int spec_pkmn::get_species_id(void) {return base->get_species_id();}
}
