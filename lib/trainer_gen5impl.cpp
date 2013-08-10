/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#include <boost/format.hpp>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/conversions.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include <pkmnsim/database/queries.hpp>

#include "sqlitecpp/SQLiteCPP.h"
#include "trainer_gen5impl.hpp"

using namespace std;

namespace pkmnsim
{
    trainer_gen5impl::trainer_gen5impl(SQLite::Database *import_db): trainer()
    {
        SQLite::Database pkmnsim_db(get_database_path().c_str());

        SQLite::Statement trainer_info_query(*import_db, "SELECT * FROM trainer_info");
        trainer_info_query.executeStep();

        from_game = trainer_info_query.getColumn(1);
        trainer_name = trainer_info_query.getColumnStr(2);
        trainer_id = trainer_info_query.getColumn(3);
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

            spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, 3, level, move1, move2, move3, move4);

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

    trainer_gen5impl::trainer_gen5impl(string filename, int game): trainer()
    {
        from_game = game;

        sav = new bw2sav_obj;
        p_pkm = new party_pkm;
        read(filename.c_str(), sav);

        money = 0; //Placeholder until money is implemented in PKMDS
        wstring trainer_name_wide = getsavtrainername(sav->cur);
        char trainer_name_buffer[8];
        memset(trainer_name_buffer,0,8);
        wcstombs(trainer_name_buffer, trainer_name_wide.c_str(), 8);
        trainer_name = trainer_name_buffer;

        party.clear();
        for(unsigned int i = 0; i < sav->cur.party.size; i++)
        {
            p_pkm = &(sav->cur.party.pokemon[i]);
            decryptpkm(p_pkm->pkm_data);
            party.push_back(converter::pkmds_pkmn_to_spec_pkmn(p_pkm));
            encryptpkm(p_pkm->pkm_data);
        }
    }
}
