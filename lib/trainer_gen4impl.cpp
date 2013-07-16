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
#include <boost/lexical_cast.hpp>

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
        save_file = fopen(filename.c_str(), "rb");
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

        if(!save->parseRawSave()) cout << "Couldn't load file." << endl;

        PokeLib::Trainer* pokelib_trainer = save->getTrainer();
        money = 0; //Placeholder until money is implemented in PokeLib
        PokeLib::widetext trainer_name_wide = pokelib_trainer->getName();
        char trainer_name_buffer[7];
        memset(trainer_name_buffer,0,7);
        wcstombs(trainer_name_buffer, trainer_name_wide.c_str(), 7);
        trainer_name = trainer_name_buffer;

        party.clear();
        PokeLib::Party* pokelib_party = save->getParty();
        for(unsigned int i = 0; i < (unsigned int)(pokelib_party->count()); i++)
        {
            PokeLib::Pokemon pokelib_pkmn = pokelib_party->getPokemon(i+1);
            party.push_back(convert_to_spec_pkmn(pokelib_pkmn));
        }
    }

    spec_pkmn::sptr trainer_gen4impl::convert_to_spec_pkmn(PokeLib::Pokemon pokelib_pkmn)
    {
        string identifier, move1, move2, move3, move4;
        int level; 

        identifier = PokeLib::species[int(pokelib_pkmn.pkm->pkm.species)];
        level = int(pokelib_pkmn.getLevel());

        move1 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[0])]);
        move2 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[1])]);
        move3 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[2])]);
        move4 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[3])]);

        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, 4, level, move1, move2, move3, move4, true);

        PokeLib::widetext nickname_wide = pokelib_pkmn.getNickname();
        char nickname_buffer[10];
        memset(nickname_buffer,0,10);
        wcstombs(nickname_buffer, nickname_wide.c_str(), 10);
        s_pkmn->nickname = nickname_buffer;

		s_pkmn->held_item = PokeLib::items[pokelib_pkmn.pkm->pkm.held_item];

        s_pkmn->nature = pkmn_nature::make(PokeLib::nature[int(pokelib_pkmn.getNatureValue())]);

        s_pkmn->HP = pokelib_pkmn.pkm->pkm.battle_max_hp;
        s_pkmn->ATK = pokelib_pkmn.pkm->pkm.battle_atk;
        s_pkmn->DEF = pokelib_pkmn.pkm->pkm.battle_def;
        s_pkmn->SATK = pokelib_pkmn.pkm->pkm.battle_satk;
        s_pkmn->SDEF = pokelib_pkmn.pkm->pkm.battle_sdef;
        s_pkmn->SPD = pokelib_pkmn.pkm->pkm.battle_spd;

        s_pkmn->ivHP = pokelib_pkmn.pkm->pkm.iv_hp;
        s_pkmn->ivATK = pokelib_pkmn.pkm->pkm.iv_atk;
        s_pkmn->ivDEF = pokelib_pkmn.pkm->pkm.iv_def;
        s_pkmn->ivSATK = pokelib_pkmn.pkm->pkm.iv_satk;
        s_pkmn->ivSDEF = pokelib_pkmn.pkm->pkm.iv_sdef;
        s_pkmn->ivSPD = pokelib_pkmn.pkm->pkm.iv_spd;

        return s_pkmn;
    }
}
