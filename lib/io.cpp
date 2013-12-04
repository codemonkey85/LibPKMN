/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdint>
#include <fstream>
#include <string.h>

#include <boost/format.hpp>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/io.hpp>

#include <pkmds/pkmds_g5.h>
#include <pkmds/pkmds_g5_sqlite.h>

#include "library_bridge.hpp"
#include "conversions/pokemon.hpp"

#include "SQLiteCpp/src/SQLiteC++.h"

namespace pkmnsim
{
    namespace io
    {
        void export_to_pkm(team_pokemon::sptr t_pkmn, std::string filename)
        {
            party_pkm* p_pkm = new party_pkm;
            conversions::team_pokemon_to_pkmds_g5_pokemon(t_pkmn, p_pkm);

            uint8_t pkm_contents[sizeof(pokemon_obj)];
            memcpy(&pkm_contents, &(p_pkm->pkm_data), sizeof(pokemon_obj));

            std::ofstream ofile;
            ofile.open(filename.c_str(), std::ofstream::out | std::ofstream::binary);
            ofile.write((char*)pkm_contents, sizeof(pokemon_obj));
            ofile.close();
        }

        team_pokemon::sptr import_from_pkm(std::string filename)
        {
            party_pkm* p_pkm = new party_pkm;
            pokemon_obj* pkmn_obj = new pokemon_obj;

            uint8_t pkm_contents[sizeof(pokemon_obj)];
            memset(pkm_contents, 0, sizeof(pokemon_obj));

            std::ifstream ifile;
            ifile.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
            ifile.read((char*)pkm_contents, sizeof(pokemon_obj));
            ifile.close();
            memcpy(pkmn_obj, pkm_contents, sizeof(pokemon_obj));
            
            pkmnsim_pctoparty(p_pkm, pkmn_obj);
            return conversions::pkmds_g5_pokemon_to_team_pokemon(p_pkm);
        }
        
        void export_to_pkx(team_pokemon::sptr t_pkmn, std::string filename)
        {
            party_pkx* p_pkx = new party_pkx;
            conversions::team_pokemon_to_pkmds_g6_pokemon(t_pkmn, p_pkx);

            uint8_t pkx_contents[sizeof(pokemonx_obj)];
            memcpy(&pkx_contents, &(p_pkx->pkx_data), sizeof(pokemonx_obj));

            std::ofstream ofile;
            ofile.open(filename.c_str(), std::ofstream::out | std::ofstream::binary);
            ofile.write((char*)pkx_contents, sizeof(pokemonx_obj));
            ofile.close();
        }

        team_pokemon::sptr import_from_pkx(std::string filename)
        {
            party_pkx* p_pkm = new party_pkx;
            pokemonx_obj* pkmn_obj = new pokemonx_obj;

            uint8_t pkx_contents[sizeof(pokemonx_obj)];
            memset(pkx_contents, 0, sizeof(pokemonx_obj));

            std::ifstream ifile;
            ifile.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
            ifile.read((char*)pkx_contents, sizeof(pokemonx_obj));
            ifile.close();
            memcpy(pkmn_obj, pkx_contents, sizeof(pokemonx_obj));
            
            pkmnsim_pctopartyx(p_pkm, pkmn_obj);
            return conversions::pkmds_g6_pokemon_to_team_pokemon(p_pkm);
        }

        void export_to_pksql(team_pokemon::sptr t_pkmn, std::string filename)
        {
            dict<unsigned int, unsigned int> stats = t_pkmn->get_stats();
            dict<unsigned int, unsigned int> EVs = t_pkmn->get_EVs();
            dict<unsigned int, unsigned int> IVs = t_pkmn->get_IVs();
            moveset_t moves = t_pkmn->get_moves();

            SQLite::Database pksql_db(filename.c_str(), (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));
            //TODO: attributes
            std::string create_table = "BEGIN TRANSACTION;\n"
                                       "CREATE TABLE pokemon (\n"
                                       "    id INTEGER NOT NULL,\n"
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
                                       "    is_female INTEGER NOT NULL,\n"
                                       "    ot_is_female INTEGER NOT NULL\n"
                                       ");\n"
                                       "COMMIT;";
            std::string pokemon_export =
                str(boost::format("INSERT INTO \"pokemon\" VALUES(0,%d,%d,'%s','%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d);")
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
                        % EVs[Stats::HP]
                        % EVs[Stats::ATTACK]
                        % EVs[Stats::DEFENSE]
                        % EVs[Stats::SPEED]
                        % EVs.get(Stats::SPECIAL, 0)
                        % EVs.get(Stats::SPECIAL_ATTACK, 0)
                        % EVs.get(Stats::SPECIAL_DEFENSE, 0)
                        % IVs[Stats::HP]
                        % IVs[Stats::ATTACK]
                        % IVs[Stats::DEFENSE]
                        % IVs[Stats::SPEED]
                        % IVs.get(Stats::SPECIAL, 0)
                        % IVs.get(Stats::SPECIAL_ATTACK, 0)
                        % IVs.get(Stats::SPECIAL_DEFENSE, 0)
                        % moves[0]->get_move_id()
                        % moves[1]->get_move_id()
                        % moves[2]->get_move_id()
                        % moves[3]->get_move_id()
                        % ((t_pkmn->get_gender() == Genders::FEMALE) ? 1 : 0)
                        % ((t_pkmn->get_trainer_gender() == Genders::FEMALE) ? 1 : 0)
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

            team_pokemon::sptr t_pkmn = team_pokemon::make(int(query.getColumn(1)),   //species_id
                                                           int(query.getColumn(2)),   //game_id
                                                           int(query.getColumn(7)),   //level
                                                           int(query.getColumn(27)),  //move1
                                                           int(query.getColumn(28)),  //move2
                                                           int(query.getColumn(29)),  //move3
                                                           int(query.getColumn(30))); //move4

            t_pkmn->set_nickname((const char*)(query.getColumn(3)));
            t_pkmn->set_trainer_name((const char*)(query.getColumn(4)));
            t_pkmn->set_held_item(int(query.getColumn(5)));
            t_pkmn->set_ball(int(query.getColumn(6)));
            t_pkmn->set_met_level(int(query.getColumn(7)));
            t_pkmn->set_ability(int(query.getColumn(8)));
            t_pkmn->set_nature(int(query.getColumn(9)));
            t_pkmn->set_personality(int(query.getColumn(10)));
            t_pkmn->set_trainer_id(int(query.getColumn(11)));
            t_pkmn->set_EV(Stats::HP, int(query.getColumn(13)));
            t_pkmn->set_EV(Stats::ATTACK, int(query.getColumn(14)));
            t_pkmn->set_EV(Stats::DEFENSE, int(query.getColumn(15)));
            t_pkmn->set_EV(Stats::SPEED, int(query.getColumn(16)));
            if(t_pkmn->get_generation() == 1) t_pkmn->set_EV(Stats::HP, int(query.getColumn(17)));
            else
            {
                t_pkmn->set_EV(Stats::HP, int(query.getColumn(18)));
                t_pkmn->set_EV(Stats::HP, int(query.getColumn(19)));
            }
            t_pkmn->set_IV(Stats::HP, int(query.getColumn(20)));
            t_pkmn->set_IV(Stats::ATTACK, int(query.getColumn(21)));
            t_pkmn->set_IV(Stats::DEFENSE, int(query.getColumn(22)));
            t_pkmn->set_IV(Stats::SPEED, int(query.getColumn(23)));
            if(t_pkmn->get_generation() == 1) t_pkmn->set_IV(Stats::HP, int(query.getColumn(24)));
            else
            {
                t_pkmn->set_IV(Stats::HP, int(query.getColumn(25)));
                t_pkmn->set_IV(Stats::HP, int(query.getColumn(26)));
            }
            t_pkmn->set_gender((int(query.getColumn(31)) ? Genders::FEMALE : Genders::MALE));
            t_pkmn->set_trainer_gender((int(query.getColumn(32)) ? Genders::FEMALE : Genders::MALE));

            return t_pkmn;
        }
    }
}
