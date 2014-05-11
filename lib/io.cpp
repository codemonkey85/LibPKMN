/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdint>
#include <fstream>
#include <cstring>

#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/io.hpp>

#include <pkmds/pkmds_g5.h>
#include <pkmds/pkmds_g5_sqlite.h>

#include "library_bridge.hpp"
#include "conversions/pokemon.hpp"

#include "libspec/libspec.h"
#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    namespace io
    {
        static void get_gen3_savetype(unsigned int libpkmn_id, gba_savetype_t &save_type)
        {
            switch(libpkmn_id)
            {
                case Games::RUBY:
                case Games::SAPPHIRE:
                    save_type = GBA_TYPE_RS;
                    break;

                case Games::EMERALD:
                    save_type = GBA_TYPE_E;
                    break;

                case Games::FIRE_RED:
                case Games::LEAF_GREEN:
                    save_type = GBA_TYPE_FRLG;
                    break;

                default:
                    save_type = GBA_TYPE_RS;
                    break;
            }
        }

        void export_to_3gpkm(team_pokemon::sptr t_pkmn, std::string filename)
        {
            //Check to see if t_pkmn is compatible with the .3gpkm format
            unsigned int generation = t_pkmn->get_generation();
            if(generation != 3)
            {
                throw std::runtime_error("Only Pokemon of generation III can be exported to .3gpkm!");
            }

            pk3_t pk3;
            uint8_t pk3_raw[sizeof(pk3_t)];
            gba_savetype_t save_type;
            get_gen3_savetype(t_pkmn->get_game_id(), save_type);
            conversions::export_gen3_pokemon(t_pkmn, &pk3, save_type);
        }

        team_pokemon::sptr import_from_3gpkm(std::string filename)
        {
            pk3_t pk3;
            uint8_t pk3_raw[sizeof(pk3_t)];
            memset(pk3_raw, 0, sizeof(pk3_t));

            std::ifstream ifile;
            ifile.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
            ifile.read((char*)pk3_raw, sizeof(pk3_t));
            ifile.close();
            memcpy(&pk3, pk3_raw, sizeof(pk3_t));

            uint16_t* game_int = reinterpret_cast<uint16_t*>(&(pk3.box.met_loc)+1);
            uint8_t libpkmn_id = hometown_to_libpkmn_game((*game_int & 0x1E) >> 1);
            gba_savetype_t save_type;
            get_gen3_savetype(libpkmn_id, save_type);
            return conversions::import_gen3_pokemon(&pk3, save_type);
        }

        void export_to_pkm(team_pokemon::sptr t_pkmn, std::string filename)
        {
            //Check to see if t_pkmn is compatible with the .pkm format
            unsigned int generation = t_pkmn->get_generation();
            if(generation != 4 and generation != 5)
            {
                throw std::runtime_error("Only Pokemon of generation IV-V can be exported to .pkm!");
            }

            pkm_nds_t pkm;
            uint8_t pkm_raw[sizeof(pkm_nds_t)];
            conversions::export_nds_pokemon(t_pkmn, &pkm);

            std::ofstream ofile;
            ofile.open(filename.c_str(), std::ofstream::out | std::ofstream::binary);
            ofile.write((char*)pkm_raw, sizeof(pokemon_obj));
            ofile.close();
        }

        //TODO: accept party of box Pokemon, check validity
        team_pokemon::sptr import_from_pkm(std::string filename)
        {
            pkm_nds_t pkm;
            uint8_t pkm_raw[sizeof(pkm_nds_t)];
            memset(pkm_raw, 0, sizeof(pkm_nds_t));

            std::ifstream ifile;
            ifile.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
            ifile.read((char*)pkm_raw, sizeof(pkm_nds_t));
            ifile.close();
            memcpy(&pkm, pkm_raw, sizeof(pkm_nds_t));

            return conversions::import_nds_pokemon(&pkm);
        }

        void export_to_pksql(team_pokemon::sptr t_pkmn, std::string filename, std::string title)
        {
            dict<std::string, unsigned int> stats = t_pkmn->get_stats();
            dict<std::string, unsigned int> EVs = t_pkmn->get_EVs();
            dict<std::string, unsigned int> IVs = t_pkmn->get_IVs();
            moveset_t moves;
            t_pkmn->get_moves(moves);

            SQLite::Database pksql_db(filename.c_str(), (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));
            //TODO: attributes
            std::string create_table = "BEGIN TRANSACTION;\n"
                                       "CREATE TABLE pokemon (\n"
                                       "    id INTEGER NOT NULL,\n"
                                       "    title VARCHAR(50) NOT NULL,\n"
                                       "    species_id INTEGER NOT NULL,\n"
                                       "    game_id INTEGER NOT NULL,\n"
                                       "    nickname VARCHAR(20) NOT NULL,\n"
                                       "    otname VARCHAR(20) NOT NULL,\n"
                                       "    held_item INTEGER NOT NULL,\n"
                                       "    ball INTEGER NOT NULL,\n"
                                       "    level INTEGER NOT NULL,\n"
                                       "    met_level INTEGER NOT NULL,\n"
                                       "    ability INTEGER NOT NULL,\n"
                                       "    nature INTEGER NOT NULL,\n"
                                       "    personality INTEGER NOT NULL,\n"
                                       "    trainer_id INTEGER NOT NULL,\n"
                                       "    ev_hp INTEGER NOT NULL,\n"
                                       "    ev_attack INTEGER NOT NULL,\n"
                                       "    ev_defense INTEGER NOT NULL,\n"
                                       "    ev_speed INTEGER NOT NULL,\n"
                                       "    ev_special INTEGER NOT NULL,\n"
                                       "    ev_spatk INTEGER NOT NULL,\n"
                                       "    ev_spdef INTEGER NOT NULL,\n"
                                       "    iv_hp INTEGER NOT NULL,\n"
                                       "    iv_attack INTEGER NOT NULL,\n"
                                       "    iv_defense INTEGER NOT NULL,\n"
                                       "    iv_speed INTEGER NOT NULL,\n"
                                       "    iv_special INTEGER NOT NULL,\n"
                                       "    iv_spatk INTEGER NOT NULL,\n"
                                       "    iv_spdef INTEGER NOT NULL,\n"
                                       "    move1 INTEGER NOT NULL,\n"
                                       "    move2 INTEGER NOT NULL,\n"
                                       "    move3 INTEGER NOT NULL,\n"
                                       "    move4 INTEGER NOT NULL,\n"
                                       "    ot_is_female INTEGER NOT NULL\n"
                                       ");\n"
                                       "COMMIT;";
            std::string pokemon_export =
                str(boost::format("INSERT INTO \"pokemon\" VALUES(0,'%s',%d,%d,'%s','%s',%d,%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d);")
                        % title
                        % t_pkmn->get_species_id()
                        % t_pkmn->get_game_id()
                        % t_pkmn->get_nickname().const_char()
                        % t_pkmn->get_trainer_name().const_char()
                        % t_pkmn->get_held_item()->get_item_id()
                        % t_pkmn->get_ball()
                        % t_pkmn->get_level()
                        % t_pkmn->get_met_level()
                        % t_pkmn->get_ability()
                        % t_pkmn->get_nature()
                        % t_pkmn->get_personality()
                        % t_pkmn->get_trainer_id()
                        % EVs["HP"]
                        % EVs["Attack"]
                        % EVs["Defense"]
                        % EVs["Speed"]
                        % EVs.at("Special", 0)
                        % EVs.at("Special Attack", 0)
                        % EVs.at("Special Defense", 0)
                        % IVs["HP"]
                        % IVs["Attack"]
                        % IVs["Defense"]
                        % IVs["Speed"]
                        % IVs.at("Special", 0)
                        % IVs.at("Special Attack", 0)
                        % IVs.at("Special Defense", 0)
                        % moves[0]->get_move_id()
                        % moves[1]->get_move_id()
                        % moves[2]->get_move_id()
                        % moves[3]->get_move_id()
                        % ((t_pkmn->get_trainer_gender().std_string() == "Female") ? 1 : 0)
                    );

            pksql_db.exec(create_table.c_str());
            pksql_db.exec(pokemon_export.c_str());
        }

        team_pokemon::sptr import_from_pksql(std::string filename)
        {
            SQLite::Database pksql_db(filename.c_str());

            //TODO: check for valid database before attempting to read
            SQLite::Statement query(pksql_db, "SELECT * FROM pokemon");
            query.executeStep();

            team_pokemon::sptr t_pkmn = team_pokemon::make(int(query.getColumn(2)),   //species_id
                                                           int(query.getColumn(3)),   //game_id
                                                           int(query.getColumn(8)),   //level
                                                           int(query.getColumn(28)),  //move1
                                                           int(query.getColumn(29)),  //move2
                                                           int(query.getColumn(30)),  //move3
                                                           int(query.getColumn(31))); //move4

            t_pkmn->set_nickname((const char*)(query.getColumn(4)));
            t_pkmn->set_trainer_name((const char*)(query.getColumn(5)));
            t_pkmn->set_held_item((const char*)(query.getColumn(6)));
            t_pkmn->set_ball((const char*)(query.getColumn(7)));
            t_pkmn->set_met_level(int(query.getColumn(9)));
            t_pkmn->set_ability((const char*)(query.getColumn(10)));
            t_pkmn->set_nature((const char*)(query.getColumn(11)));
            t_pkmn->set_personality(int(query.getColumn(12)));
            t_pkmn->set_trainer_id(int(query.getColumn(13)));
            t_pkmn->set_EV("HP", int(query.getColumn(14)));
            t_pkmn->set_EV("Attack", int(query.getColumn(15)));
            t_pkmn->set_EV("Defense", int(query.getColumn(16)));
            t_pkmn->set_EV("Speed", int(query.getColumn(17)));
            if(t_pkmn->get_generation() == 1) t_pkmn->set_EV("Special", int(query.getColumn(18)));
            else
            {
                t_pkmn->set_EV("Special Attack", int(query.getColumn(19)));
                t_pkmn->set_EV("Special Defense", int(query.getColumn(20)));
            }
            t_pkmn->set_IV("HP", int(query.getColumn(21)));
            t_pkmn->set_IV("Attack", int(query.getColumn(22)));
            t_pkmn->set_IV("Defense", int(query.getColumn(23)));
            t_pkmn->set_IV("Speed", int(query.getColumn(24)));
            if(t_pkmn->get_generation() < 3) t_pkmn->set_IV("Special", int(query.getColumn(25)));
            else
            {
                t_pkmn->set_IV("Special Attack", int(query.getColumn(26)));
                t_pkmn->set_IV("Special Defense", int(query.getColumn(27)));
            }
            t_pkmn->set_move(int(query.getColumn(28)), 1);
            t_pkmn->set_move(int(query.getColumn(29)), 2);
            t_pkmn->set_move(int(query.getColumn(30)), 3);
            t_pkmn->set_move(int(query.getColumn(31)), 4);
            t_pkmn->set_trainer_gender((int(query.getColumn(32)) ? "Female" : "Male"));

            return t_pkmn;
        }
    }
}
