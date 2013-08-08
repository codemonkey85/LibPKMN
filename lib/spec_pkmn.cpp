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
    spec_pkmn::spec_pkmn(base_pkmn::sptr b, string m1, string m2,
                         string m3, string m4, int g, int l)
    {
        base = b;
        nickname = base->get_species_name();
        level = l;
        generation = g;
        SQLite::Database db(get_database_path().c_str()); //Filepath given by CMake
        int base_pkmn_id = base->get_pokemon_id();
        int base_species_id = base->get_species_id();

		attributes = dict<string, int>();
        moves = vla<base_move::sptr>(4);

        icon_path = base->get_icon_path();

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

    spec_pkmn::sptr spec_pkmn::make(string identifier, int gen, int level, string move1,
                                    string move2, string move3, string move4)
    {
        try
        {
            base_pkmn::sptr base = base_pkmn::make(identifier, gen);

            if(gen < 1 or gen > 5) throw runtime_error("Gen must be 1-5.");

            switch(gen)
            {
                case 1:
                    return sptr(new spec_pkmn_gen1impl(base, level,
                                                       move1, move2, move3, move4));

                case 2:
                    return sptr(new spec_pkmn_gen2impl(base, level,
                                                       move1, move2, move3, move4));

                default:
                    return sptr(new spec_pkmn_gen345impl(base, level, gen,
                                                       move1, move2, move3, move4));
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

    vla<base_move::sptr> spec_pkmn::get_moves(void) {return moves;}
    
    int spec_pkmn::get_generation(void) {return generation;}

    string spec_pkmn::get_icon_path(void) {return icon_path;}

    string spec_pkmn::get_sprite_path(void) {return sprite_path;}

	int spec_pkmn::get_attribute(string attribute) {return attributes[attribute];}

    dict<string, int> spec_pkmn::get_attributes(void) {return attributes;}

    bool spec_pkmn::has_attribute(string attribute) {return attributes.has_key(attribute);}

	void spec_pkmn::set_attribute(string attribute, int value) {attributes[attribute] = value;}

    string spec_pkmn::get_species_name(void) {return base->get_species_name();}

    dict<string, int> spec_pkmn::get_base_stats(void) {return base->get_base_stats();}

    dict<string, int> spec_pkmn::get_ev_yields(void) {return base->get_ev_yields();}

    bool spec_pkmn::is_fully_evolved(void) {return base->is_fully_evolved();}

    int spec_pkmn::get_pokemon_id(void) {return base->get_pokemon_id();}

    int spec_pkmn::get_species_id(void) {return base->get_species_id();}
}
