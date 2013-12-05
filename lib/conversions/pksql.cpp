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
#include <pkmnsim/database/queries.hpp>

#include "../library_bridge.hpp"
#include "pksql.hpp"

#include "../SQLiteCpp/src/SQLiteC++.h"

namespace pkmnsim
{
    namespace io
    {
        void pokehack_pokemon_to_pksql(belt_pokemon_t* b_pkmn_t,
                                       pokemon_attacks_t* pkmn_a_t,
                                       pokemon_effort_t* pkmn_e_t,
                                       pokemon_misc_t* pkmn_m_t,
                                       pokemon_growth_t* pkmn_g_t,
                                       std::string filename)
        {
            unsigned int game_id = hometown_to_pkmnsim_game(pkmn_m_t->game);
            uint32_t* IVint = &(pkmn_m_t->IVint);
            uint16_t* metlevel_int = reinterpret_cast<uint16_t*>(&(pkmn_m_t->locationcaught)+1);

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
                        % pkmn_g_t->species
                        % game_id
                        % pokehack_get_text(b_pkmn_t->name, true)
                        % pokehack_get_text(b_pkmn_t->otname, false)
                        % database::get_item_id(pkmn_g_t->held, game_id)
                        % game_ball_to_pkmnsim_ball(get_gen3_ball(metlevel_int))
                        % b_pkmn_t->level
                        % get_gen3_met_level(metlevel_int)
                        % 0 //TODO: ability
                        % 0 //TODO: nature
                        % b_pkmn_t->personality
                        % b_pkmn_t->otid
                        % pkmn_e_t->hp
                        % pkmn_e_t->attack
                        % pkmn_e_t->defense
                        % pkmn_e_t->speed
                        % 0 //No Special in Gen 3
                        % pkmn_e_t->spatk
                        % pkmn_e_t->spdef
                        % modern_get_IV(IVint, Stats::HP)
                        % modern_get_IV(IVint, Stats::ATTACK)
                        % modern_get_IV(IVint, Stats::DEFENSE)
                        % modern_get_IV(IVint, Stats::SPEED)
                        % 0 //No Special in Gen 3
                        % modern_get_IV(IVint, Stats::SPECIAL_ATTACK)
                        % modern_get_IV(IVint, Stats::SPECIAL_DEFENSE)
                        % pkmn_a_t->atk1
                        % pkmn_a_t->atk2
                        % pkmn_a_t->atk3
                        % pkmn_a_t->atk4
                        % 0 //Implement gender
                        % (get_gen3_otgender(metlevel_int) ? 1 : 0)
                    );

            pksql_db.exec(create_table.c_str());
            pksql_db.exec(pokemon_export.c_str());
        }
    }
}
