/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <iostream>
#include <string>

#include "trainer_gen3impl.hpp"

#include <pkmnsim/enums.hpp>
#include <pkmnsim/trainer.hpp>

using namespace std;

namespace pkmnsim
{
    trainer::sptr trainer::make(string filename, int game)
    {
        ifstream ifile(filename);
        if(!ifile)
        {
            cerr << "Specified file does not exist." << endl;
            exit(EXIT_FAILURE);
        }
        ifile.close();

        //Only have trainer_gen3impl so far
        return trainer::sptr(new trainer_gen3impl(filename, game));
    }

    int trainer::get_money(void) {return money;}

    void trainer::get_party(vector<spec_pkmn::sptr>& party_vec) {party_vec = party;}

    //Return-by-value for SWIG's benefit
    vector<spec_pkmn::sptr> trainer::get_party_vec(void) {return party;}

    string trainer::get_trainer_name(void) {return trainer_name;}
}
