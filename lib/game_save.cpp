/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>

#include <pkmnsim/game_save.hpp>

#include "game_save_gen3impl.hpp"
#include "game_save_gen4impl.hpp"
#include "game_save_gen5impl.hpp"

using namespace std;

namespace pkmnsim
{
    game_save::sptr game_save::make(std::string filename, int gen)
    {
        try
        {
            //Confirm that file exists
            ifstream ifile(filename.c_str());
            if(!ifile) throw runtime_error("Specified file does not exist.");
            if(gen == 1 or gen == 2) throw runtime_error("Generation 1 and 2 not currently supported.");
            else if(gen >= 3 and gen <= 5)
            {
                switch(gen)
                {
                    case 3:
                        return sptr(new game_save_gen3impl(filename));
                    case 4:
                        return sptr(new game_save_gen4impl(filename));
                    default:
                        return sptr(new game_save_gen5impl(filename));
                }
            }
            else throw runtime_error("Specify a generation between 1-5.");
        }
        catch(exception &e)
        {
            cerr << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    string game_save::get_trainer_name() {return trainer_name;}

    void game_save::get_party(vector<spec_pkmn::sptr>& party_pkmn_vec) {party_pkmn_vec = party;}
}
