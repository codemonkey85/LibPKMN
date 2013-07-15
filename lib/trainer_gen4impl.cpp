/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <sstream>
#include <stdio.h>

#include <boost/format.hpp>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include <pkmnsim/database/queries.hpp>

#include "pokelib/data_tables.h"
#include "sqlitecpp/SQLiteCPP.h"
#include "trainer_gen4impl.hpp"

using namespace std;

namespace pkmnsim
{
    trainer_gen4impl::trainer_gen4impl(SQLite::Database *import_db): trainer()
    {
        SQLite::Database pkmnsim_db(get_database_path().c_str());

        SQLite::Statement trainer_info_query(*import_db, "SELECT * FROM trainer_info");
        trainer_info_query.executeStep();

        from_game = trainer_info_query.getColumn(1);
        trainer_name = trainer_info_query.getColumnStr(2);
        trainer_id = trainer_info_query.getColumnStr(3);
        int party_size = trainer_info_query.getColumn(4);
        money = trainer_info_query.getColumn(5);

        party.clear();

        for(unsigned int i = 0; i < party_size; i++)
        {
            //Grab values from export database necessary to create spec_pkmn
            SQLite::Statement party_query(*import_db, str(boost::format("SELECT * FROM party WHERE id=%d")
                                                                        % i).c_str()
                                         );
            party_query.executeStep();

            int pkmn_id = party_query.getColumn(1);
            int species_id = party_query.getColumn(2);
            string nickname = party_query.getColumn(3);
            int level = party_query.getColumn(4);
            int item_held_id = party_query.getColumn(5);
            int move1_id = party_query.getColumn(6);
            int move2_id = party_query.getColumn(7);
            int move3_id = party_query.getColumn(8);
            int move4_id = party_query.getColumn(9);
            int HP = party_query.getColumn(10);
            int ATK = party_query.getColumn(11);
            int DEF = party_query.getColumn(12);
            int SATK = party_query.getColumn(13);
            int SDEF = party_query.getColumn(14);
            int SPD = party_query.getColumn(15);
            int evHP = party_query.getColumn(16);
            int evATK = party_query.getColumn(17);
            int evDEF = party_query.getColumn(18);
            int evSATK = party_query.getColumn(19);
            int evSDEF = party_query.getColumn(20);
            int evSPD = party_query.getColumn(21);
            int ivHP = party_query.getColumn(22);
            int ivATK = party_query.getColumn(23);
            int ivDEF = party_query.getColumn(24);
            int ivSATK = party_query.getColumn(25);
            int ivSDEF = party_query.getColumn(26);
            int ivSPD = party_query.getColumn(27);

            string identifier, item_held, move1, move2, move3, move4;
            identifier = pkmnsim_db.execAndGetStr(str(boost::format(
                                                      "SELECT name FROM pokemon_species_names WHERE pokemon_species_id=%d AND local_language_id=9")
                                                      % species_id).c_str(), "name"
                                                 );
            if(item_held_id == -1) item_held = "None";
            else
            {
                item_held = pkmnsim_db.execAndGetStr(str(boost::format(
                                                         "SELECT name FROM item_names WHERE item_id=%d")
                                                         % item_held_id).c_str(), "name"
                                                    );
            }
            move1 = pkmnsim_db.execAndGetStr(str(boost::format(
                                                 "SELECT name FROM move_names WHERE move_id=%d AND local_language_id=9")
                                                 % move1_id).c_str(), "name"
                                            );
            if(move2_id == -1) move2 = "None";
            else
            {
                move2 = pkmnsim_db.execAndGetStr(str(boost::format(
                                                     "SELECT name FROM move_names WHERE move_id=%d AND local_language_id=9")
                                                     % move2_id).c_str(), "name"
                                                );
            }
            if(move3_id == -1) move3 = "None";
            else
            {
                move3 = pkmnsim_db.execAndGetStr(str(boost::format(
                                                     "SELECT name FROM move_names WHERE move_id=%d AND local_language_id=9")
                                                     % move3_id).c_str(), "name"
                                                );
            }
            if(move4_id == -1) move4 = "None";
            else
            {
                move4 = pkmnsim_db.execAndGetStr(str(boost::format(
                                                     "SELECT name FROM move_names WHERE move_id=%d AND local_language_id=9")
                                                     % move4_id).c_str(), "name"
                                                );
            }

            spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, 3, level, move1, move2, move3, move4, true);

            //Manually set other values
            s_pkmn->nickname = nickname;
            s_pkmn->HP = HP;
            s_pkmn->ATK = ATK;
            s_pkmn->DEF = DEF;
            s_pkmn->SATK = SATK;
            s_pkmn->SDEF = SDEF;
            s_pkmn->SPD = SPD;
            s_pkmn->evHP = evHP;
            s_pkmn->evATK = evATK;
            s_pkmn->evDEF = evDEF;
            s_pkmn->evSATK = evSATK;
            s_pkmn->evSDEF = evSDEF;
            s_pkmn->evSPD = evSPD;
            s_pkmn->ivHP = ivHP;
            s_pkmn->ivATK = ivATK;
            s_pkmn->ivDEF = ivDEF;
            s_pkmn->ivSATK = ivSATK;
            s_pkmn->ivSDEF = ivSDEF;
            s_pkmn->ivSPD = ivSPD;

            party.push_back(s_pkmn);
        }
    }

    trainer_gen4impl::trainer_gen4impl(string filename, int game): trainer()
    {
        from_game = game;
        size_t size, save_size;

        //Read save file and get size
        FILE* save_file;
        save_file = fopen(filename.c_str(), "r");
        fseek(save_file, 0, SEEK_END);
        size = ftell(save_file);
        rewind(save_file);

        //Determine which size game file actually is
        if(size >= 0x80000) save_size = 0x80000;
        else if(size > 0x40000 and size < 0x80000) save_size = 0x40000;
        else
        {
            cerr << "File is too small to be a proper save file." << endl;
            exit(EXIT_FAILURE);
        }
        save = new PokeLib::Save(size);
        int result = fread(save->data, 1, save_size, save_file);
        if(result != save_size)
        {
            cerr << "Problem reading save file." << endl;
            exit(EXIT_FAILURE);
        }
        fclose(save_file);

        save->parseRawSave();

        party.clear();
        PokeLib::Party* pokelib_party = save->getParty();
        cout << "party size: " << (unsigned int)(pokelib_party->count()) << endl;
        cout << "Save type: " << save->getSaveType() << endl;
        //PokeLib::Pokemon pokelib_pkmn = pokelib_party->getPokemon(0);
        /*for(unsigned int i = 0; i < (unsigned int)(pokelib_party->count()); i++)
        {
            PokeLib::Pokemon pokelib_pkmn = pokelib_party->getPokemon(i);
            party.push_back(convert_to_spec_pkmn(pokelib_pkmn));
        }*/
    }

    void trainer_gen4impl::export_to_file(string filename)
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
                                 "trainer_id VARCHAR(5) NOT NULL,\n"
                                 "party_size INTEGER NOT NULL,\n"
                                 "money INTEGER NOT NULL,\n"
                                 "PRIMARY KEY(id));";
        export_db.exec(export_db_query_string.c_str());
        export_db.exec(str(boost::format("INSERT INTO \"trainer_info\" VALUES(0,'%s','%s','%s',%d,%d)")
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
                                 "PRIMARY KEY(id));";
        export_db.exec(export_db_query_string.c_str());
        for(unsigned int i = 0; i < party.size(); i++)
        {
            dict<string, int> stats = party[i]->get_stats();
            dict<string, int> EVs = party[i]->get_EVs();
            dict<string, int> IVs = party[i]->get_IVs();
            vla<base_move::sptr> moves = party[i]->get_moves();

            //Intermediary variables for ID's
            int pkmn_id, species_id, item_held_id, move1_id, move2_id, move3_id, move4_id;

            pkmn_id = get_pkmn_id(party[i]->get_base_pkmn());
            species_id = get_species_id(party[i]->get_base_pkmn());
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
            if(moves[1]->get_name() == "Struggle") move2_id = -1;
            else
            {
                move2_id = pkmnsim_db.execAndGet(str(boost::format("SELECT move_id FROM move_names WHERE name='%s';")
                                                                   % moves[1]->get_name()).c_str()
                                                );
            }
            if(moves[2]->get_name() == "Struggle") move3_id = -1;
            else
            {
                move3_id = pkmnsim_db.execAndGet(str(boost::format("SELECT move_id FROM move_names WHERE name='%s';")
                                                                   % moves[2]->get_name()).c_str()
                                                );
            }
            if(moves[3]->get_name() == "Struggle") move4_id = -1;
            else
            {
                move4_id = pkmnsim_db.execAndGet(str(boost::format("SELECT move_id FROM move_names WHERE name='%s';")
                                                                   % moves[3]->get_name()).c_str()
                                                );
            }
            export_db.exec(str(boost::format("INSERT INTO party VALUES(%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d);")
                                             % i % pkmn_id % species_id % party[i]->get_nickname() % party[i]->get_level()
                                             % item_held_id % move1_id % move2_id % move3_id % move4_id
                                             % stats["HP"] % stats["ATK"] % stats["DEF"] % stats["SATK"] % stats["SDEF"] % stats["SPD"]
                                             % EVs["HP"] % EVs["ATK"] % EVs["DEF"] % EVs["SATK"] % EVs["SDEF"] % EVs["SPD"]
                                             % IVs["HP"] % IVs["ATK"] % IVs["DEF"] % IVs["SATK"] % IVs["SDEF"] % IVs["SPD"]).c_str()
                          );
        }
        export_db.exec("COMMIT;");
    }

    spec_pkmn::sptr trainer_gen4impl::convert_to_spec_pkmn(PokeLib::Pokemon pokelib_pkmn)
    {
        string identifier, move1, move2, move3, move4;
        int level; 

        cout << "Inside convert_to_spec_pkmn" << endl;

        cout << "size: " << sizeof(pokelib_pkmn) << endl;
        /*identifier = PokeLib::species[int(pokelib_pkmn.pkm->pkm.species)];
        level = int(pokelib_pkmn.getLevel());

        move1 = to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[0])]);
        move2 = to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[1])]);
        move3 = to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[2])]);
        move4 = to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[3])]);

        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, 4, level, move1, move2, move3, move4, true);

        cout << s_pkmn->get_info() << endl << endl;*/
    }
}
