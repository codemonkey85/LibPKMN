/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <iostream>
#include <string>

#include "sqlitecpp/SQLiteCPP.h"
#include "trainer_gen3impl.hpp"
#include "trainer_gen4impl.hpp"

#include <pkmnsim/enums.hpp>
#include <pkmnsim/trainer.hpp>

using namespace std;

namespace pkmnsim
{
    trainer::trainer()
    {
        trainer_id = "?????";
    }

    trainer::sptr trainer::make(string filename, int game)
    {
        try
        {
            ifstream ifile(filename);
            if(!ifile)
            {
                cerr << "Specified file does not exist." << endl;
                exit(EXIT_FAILURE);
            }
            ifile.close();

            try
            {
                //Check for valid SQLite database by querying both tables
                SQLite::Database db(filename.c_str());

                int game = db.execAndGet("SELECT from_game FROM trainer_info");
                int party_id = db.execAndGet("SELECT id FROM party");

                switch(game)
                {
                    case Games::RUBY:
                    case Games::SAPPHIRE:
                    case Games::EMERALD:
                    case Games::FIRE_RED:
                    case Games::LEAF_GREEN:
                        return trainer::sptr(new trainer_gen3impl(&db));

                    case Games::DIAMOND:
                    case Games::PEARL:
                    case Games::PLATINUM:
                    case Games::HEART_GOLD:
                    case Games::SOUL_SILVER:
                        return trainer::sptr(new trainer_gen4impl(&db));

                    default:
                        cerr << "Only Gen 3-4 supported." << endl;
                        exit(EXIT_FAILURE);
                }
            }
            catch(const exception &e)
            {
                //Assume a save file is being passed in

                switch(game)
                {
                    case Games::RUBY:
                    case Games::SAPPHIRE:
                    case Games::EMERALD:
                    case Games::FIRE_RED:
                    case Games::LEAF_GREEN:
                        return trainer::sptr(new trainer_gen3impl(filename, game));

                    case Games::DIAMOND:
                    case Games::PEARL:
                    case Games::PLATINUM:
                    case Games::HEART_GOLD:
                    case Games::SOUL_SILVER:
                        return trainer::sptr(new trainer_gen4impl(filename, game));

                    default:
                        cerr << "Only Gen 3-4 supported." << endl;
                        exit(EXIT_FAILURE);
                }
            }
        }
        catch(const exception &e)
        {
            cerr << "Caught exception: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    int trainer::get_money(void) {return money;}

    void trainer::get_party(vector<spec_pkmn::sptr>& party_vec) {party_vec = party;}

    //Return-by-value for SWIG's benefit
    vector<spec_pkmn::sptr> trainer::get_party_vec(void) {return party;}

    string trainer::get_trainer_name(void) {return trainer_name;}
}
