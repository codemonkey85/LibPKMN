/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <iostream>
#include <string>

#include <boost/format.hpp>

#include "sqlitecpp/SQLiteCPP.h"
#include "trainer_gen3impl.hpp"
#include "trainer_gen4impl.hpp"
#include "trainer_gen5impl.hpp"

#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/trainer.hpp>

using namespace std;

namespace pkmnsim
{
    trainer::trainer()
    {
        trainer_id = 12345;
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

                    case Games::BLACK:
                    case Games::WHITE:
                    case Games::BLACK2:
                    case Games::WHITE2:
                        return trainer::sptr(new trainer_gen5impl(&db));

                    default:
                        cerr << "Only Gen 3-5 supported." << endl;
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

                    case Games::BLACK:
                    case Games::WHITE:
                    case Games::BLACK2:
                    case Games::WHITE2:
                        return trainer::sptr(new trainer_gen5impl(filename, game));

                    default:
                        cerr << "Only Gen 3-5 supported." << endl;
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

    void trainer::get_party(s_pkmn_vec_t &party_vec) {party_vec = party;}

    //Return-by-value for SWIG's benefit
    s_pkmn_vec_t trainer::get_party_vec(void) {return party;}

    string trainer::get_trainer_name(void) {return trainer_name;}

    int trainer::get_trainer_id(void) {return trainer_id;}

    void trainer::export_to_file(string filename)
    {
        //Create or open file
        ofstream ofile;
        ofile.open(filename.c_str());
        ofile.flush();
        ofile.close();

        SQLite::Database pkmnsim_db(get_database_path().c_str());
        SQLite::Database export_db(filename.c_str(), SQLITE_OPEN_READWRITE);
        string pkmnsim_db_query_string, export_db_query_string;

        export_db.exec("PRAGMA foreign_keys=OFF; BEGIN TRANSACTION;");
        export_db_query_string = "CREATE TABLE trainer_info (\n"
                                 "id INTEGER NOT NULL,\n"
                                 "from_game INTEGER NOT NULL,\n"
                                 "trainer_name VARCHAR(7) NOT NULL,\n"
                                 "trainer_id INTEGER NOT NULL,\n"
                                 "party_size INTEGER NOT NULL,\n"
                                 "money INTEGER NOT NULL,\n"
                                 "PRIMARY KEY(id));";
        export_db.exec(export_db_query_string.c_str());
        export_db.exec(str(boost::format("INSERT INTO \"trainer_info\" VALUES(0,'%s','%s',%d,%d,%d)")
                                         % from_game
                                         % trainer_name
                                         % trainer_id
                                         % party.size()
                                         % money).c_str()
                      );
        export_db_query_string = "CREATE TABLE party (\n"
                                 "id INTEGER NOT NULL,\n"
                                 "pkmn_id INTEGER NOT NULL,\n"
                                 "species_id INTEGER NOT NULL,\n"
                                 "nickname VARCHAR(10) NOT NULL,\n"
                                 "level INTEGER NOT NULL,\n"
                                 "item_held_id INTEGER NOT NULL,\n"
                                 "move1_id INTEGER NOT NULL,\n"
                                 "move2_id INTEGER NOT NULL,\n"
                                 "move3_id INTEGER NOT NULL,\n"
                                 "move4_id INTEGER NOT NULL,\n"
                                 "HP INTEGER NOT NULL,\n"
                                 "ATK INTEGER NOT NULL,\n"
                                 "DEF INTEGER NOT NULL,\n"
                                 "SATK INTEGER NOT NULL,\n"
                                 "SDEF INTEGER NOT NULL,\n"
                                 "SPD INTEGER NOT NULL,\n"
                                 "evHP INTEGER NOT NULL,\n"
                                 "evATK INTEGER NOT NULL,\n"
                                 "evDEF INTEGER NOT NULL,\n"
                                 "evSATK INTEGER NOT NULL,\n"
                                 "evSDEF INTEGER NOT NULL,\n"
                                 "evSPD INTEGER NOT NULL,\n"
                                 "ivHP INTEGER NOT NULL,\n"
                                 "ivATK INTEGER NOT NULL,\n"
                                 "ivDEF INTEGER NOT NULL,\n"
                                 "ivSATK INTEGER NOT NULL,\n"
                                 "ivSDEF INTEGER NOT NULL,\n"
                                 "ivSPD INTEGER NOT NULL,\n"
                                 "nature_id INTEGER NOT NULL,\n"
                                 "PRIMARY KEY(id));";
        export_db.exec(export_db_query_string.c_str());
        for(unsigned int i = 0; i < party.size(); i++)
        {
            dict<string, int> stats = party[i]->get_stats();
            dict<string, int> EVs = party[i]->get_EVs();
            dict<string, int> IVs = party[i]->get_IVs();
            b_move_vla_t moves = party[i]->get_moves();

            //Intermediary variables for ID's
            int pkmn_id, species_id, item_held_id, move1_id, move2_id, move3_id, move4_id;

            pkmn_id = party[i]->get_pokemon_id();
            species_id = party[i]->get_species_id();
            if(party[i]->get_held_item() == "None" or party[i]->get_held_item() == "Nothing") item_held_id = -1;
            else
            {
                item_held_id = pkmnsim_db.execAndGet(str(boost::format("SELECT item_id FROM item_names WHERE name='%s';")
                                                                       % party[i]->get_held_item()).c_str()
                                                    );
            }
            move1_id = pkmnsim_db.execAndGet(str(boost::format("SELECT move_id FROM move_names WHERE name='%s';")
                                                               % moves[0]->get_name()).c_str()
                                                );
            if(moves[1]->get_name() == "None") move2_id = -1;
            else
            {
                move2_id = pkmnsim_db.execAndGet(str(boost::format("SELECT move_id FROM move_names WHERE name='%s';")
                                                                   % moves[1]->get_name()).c_str()
                                                );
            }
            if(moves[2]->get_name() == "None") move3_id = -1;
            else
            {
                move3_id = pkmnsim_db.execAndGet(str(boost::format("SELECT move_id FROM move_names WHERE name='%s';")
                                                                   % moves[2]->get_name()).c_str()
                                                );
            }
            if(moves[3]->get_name() == "None") move4_id = -1;
            else
            {
                move4_id = pkmnsim_db.execAndGet(str(boost::format("SELECT move_id FROM move_names WHERE name='%s';")
                                                                   % moves[3]->get_name()).c_str()
                                                );
            }
            export_db.exec(str(boost::format("INSERT INTO party VALUES(%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d);")
                                             % i % pkmn_id % species_id % party[i]->get_nickname() % party[i]->get_level()
                                             % item_held_id % move1_id % move2_id % move3_id % move4_id
                                             % stats["HP"] % stats["ATK"] % stats["DEF"] % stats["SATK"] % stats["SDEF"] % stats["SPD"]
                                             % EVs["HP"] % EVs["ATK"] % EVs["DEF"] % EVs["SATK"] % EVs["SDEF"] % EVs["SPD"]
                                             % IVs["HP"] % IVs["ATK"] % IVs["DEF"] % IVs["SATK"] % IVs["SDEF"] % IVs["SPD"]
                                             % party[i]->get_nature()->get_nature_id()).c_str()
                          );
        }
        export_db.exec("COMMIT;");

    }
}
