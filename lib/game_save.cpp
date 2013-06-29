/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <iostream>
#include <string>

#include "game_save_gen3impl.hpp"

#include <pkmnsim/enums.hpp>
#include <pkmnsim/game_save.hpp>

using namespace std;

namespace pkmnsim
{
    game_save::sptr game_save::make(string filename, int game)
    {
        ifstream ifile(filename);
        if(!ifile)
        {
            cerr << "Specified file does not exist." << endl;
            exit(EXIT_FAILURE);
        }
        ifile.close();

        //Only have game_save_gen3impl so far
        return game_save::sptr(new game_save_gen3impl(filename, game));
    }

    int game_save::get_money(void) {return money;}

    void game_save::get_party(vector<spec_pkmn::sptr>& party_vec) {party_vec = party;}

    //Return-by-value for SWIG's benefit
    vector<spec_pkmn::sptr> game_save::get_party_vec(void) {return party;}

    string game_save::get_trainer_name(void) {return trainer_name;}
}
