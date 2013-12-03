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
                                       "    hp INTEGER NOT NULL,\n"
                                       "    attack INTEGER NOT NULL,\n"
                                       "    defense INTEGER NOT NULL,\n"
                                       "    speed INTEGER NOT NULL,\n"
                                       "    special INTEGER NOT NULL,\n"
                                       "    spatk INTEGER NOT NULL,\n"
                                       "    spdef INTEGER NOT NULL,\n"
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
                                       "    is_female INTEGER NOT NULL,\n"
                                       "    ot_is_female INTEGER NOT NULL\n"
                                       ");\n"
                                       "COMMIT;";
            std::string pokemon_export =
                str(boost::format("INSERT INTO \"pokemon\" VALUES(0,%d,%d,'%s','%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d);")
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
                        % stats[Stats::HP]
                        % stats[Stats::ATTACK]
                        % stats[Stats::DEFENSE]
                        % stats[Stats::SPEED]
                        % stats.get(Stats::SPECIAL, 0)
                        % stats.get(Stats::SPECIAL_ATTACK, 0)
                        % stats.get(Stats::SPECIAL_DEFENSE, 0)
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
                        % ((t_pkmn->get_gender() == Genders::FEMALE) ? 1 : 0)
                        % ((t_pkmn->get_trainer_gender() == Genders::FEMALE) ? 1 : 0)
                    );

            pksql_db.exec(create_table.c_str());
            pksql_db.exec(pokemon_export.c_str());
        }
    }
}
