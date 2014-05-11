/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cmath>
#include <stdexcept>
#include <ctime>

#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/base_pokemon.hpp>
#include <pkmn/move.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/analysis/damage.hpp>
#include <pkmn/analysis/stats.hpp>
#include <pkmn/database/queries.hpp>

#include "../SQLiteCpp/src/SQLiteC++.h"

using namespace std;

namespace pkmn
{
    namespace analysis
    {
        static SQLite::Database db(get_database_path().c_str());

        //TODO: Account for Gen V-VI differences
        double get_type_damage_mod(std::string type1, std::string type2, unsigned int gen)
        {
            unsigned int type1_id = database::get_type_id(type1);
            unsigned int type2_id = database::get_type_id(type2);

            if((gen == 1) and (type1_id == Types::DARK or type1_id == Types::STEEL
                          or type2_id == Types::DARK or type2_id == Types::STEEL)) return 1;

            double damage_mod;
            string query_string;

            if(type1_id != Types::NONE and type1_id != Types::UNKNOWN and type1_id != Types::SHADOW
            and type2_id != Types::NONE and type2_id != Types::UNKNOWN and type2_id != Types::SHADOW)
            {
                //Get damage mod from database
                query_string = str(boost::format("SELECT damage_factor FROM type_efficacy WHERE damage_type_id=%d AND target_type_id=%d")
                                                 % type1 % type2);
                damage_mod = db.execAndGet(query_string.c_str());
                damage_mod /= 100; //Stored as 50, 100, or 200 in database

                return damage_mod;
            }
            else return 1.0;
        }

        unsigned int get_base_damage(unsigned int level, unsigned int attack,
                                     unsigned int defense, unsigned int base_power)
        {
            return (unsigned int)(floor((((2.0 * double(level) + 10.0) / 250.0) *
                   (double(attack) / double(defense)) * double(base_power) * 2.0)));
        }

        unsigned int get_base_damage(team_pokemon::sptr attacker, team_pokemon::sptr defender,
                                     move::sptr attack)
        {
            unsigned int level = attacker->get_level();
            unsigned int base_power = attack->get_base_power();
            unsigned int attacker_ATK, defender_DEF;

            unsigned int move_damage_class = attack->get_move_damage_class_id();
            switch(move_damage_class)
            {
                case Move_Classes::PHYSICAL:
                    attacker_ATK = attacker->get_stats()["Attack"];
                    defender_DEF = defender->get_stats()["Defense"];
                    break;

                case Move_Classes::SPECIAL:
                    if(attacker->get_generation() == 1)
                    {
                        attacker_ATK = attacker->get_stats()["Special"];
                        defender_DEF = defender->get_stats()["Special"];
                    }
                    else
                    {
                        attacker_ATK = attacker->get_stats()["Special Attack"];
                        defender_DEF = defender->get_stats()["Special Defense"];
                    }
                    break;

                default:
                    return 0;
            }
            return get_base_damage(level, attacker_ATK, defender_DEF, base_power);
        }

        //Abilities not taken unsigned into account
        std::pair<unsigned int, unsigned int> get_damage_range(base_pokemon::sptr attacker, base_pokemon::sptr defender,
                                                               move::sptr attack, unsigned int attacker_level,
                                                               unsigned int defender_level)
        {
            std::pair<unsigned int, unsigned int> damage_pair;

            unsigned int attacker_min_ATK, attacker_max_ATK;
            unsigned int defender_min_DEF, defender_max_DEF;
            unsigned int min_damage, max_damage;

            unsigned int move_damage_class = attack->get_move_damage_class_id();
            unsigned int gen = attacker->get_generation();
            switch(move_damage_class)
            {
                case Move_Classes::PHYSICAL:
                    attacker_min_ATK = get_min_possible_stat(attacker, "Attack", attacker_level, gen);
                    attacker_max_ATK = get_max_possible_stat(attacker, "Attack", attacker_level, gen);
                    defender_min_DEF = get_min_possible_stat(defender, "Defense", defender_level, gen);
                    defender_max_DEF = get_max_possible_stat(defender, "Defense", defender_level, gen);
                    break;

                case Move_Classes::SPECIAL:
                    if(gen == 1)
                    {
                        attacker_min_ATK = get_min_possible_stat(attacker, "Special", attacker_level, 1);
                        attacker_max_ATK = get_max_possible_stat(attacker, "Special", attacker_level, 1);
                        defender_min_DEF = get_min_possible_stat(defender, "Special", defender_level, 1);
                        defender_max_DEF = get_max_possible_stat(defender, "Special", defender_level, 1);
                    }
                    else
                    {
                        attacker_min_ATK = get_min_possible_stat(attacker, "Special Attack", attacker_level, gen);
                        attacker_max_ATK = get_max_possible_stat(attacker, "Special Attack", attacker_level, gen);
                        defender_min_DEF = get_min_possible_stat(defender, "Special Defense", defender_level, gen);
                        defender_max_DEF = get_max_possible_stat(defender, "Special Defense", defender_level, gen);
                    }
                    break;

                default:
                    damage_pair.first = 0;
                    damage_pair.second = 0;
                    return damage_pair;
            }
            min_damage = (unsigned int)(floor(get_base_damage(attacker_level, attacker_min_ATK, defender_max_DEF, attack->get_base_power()) * 0.85));
            max_damage = get_base_damage(attacker_level, attacker_max_ATK, defender_min_DEF, attack->get_base_power());

            double type_mod = double(get_type_damage_mod(attack->get_type(), defender->get_types().first, gen)) *
                              double(get_type_damage_mod(attack->get_type(), defender->get_types().second, gen));

            min_damage = (unsigned int)(floor(double(min_damage) * type_mod));
            max_damage = (unsigned int)(floor(double(max_damage) * type_mod));

            if(type_mod != 0)
            {
                if(defender->get_species_id() == Species::SHEDINJA)
                {
                    if(type_mod > 1.0)
                    {
                        /*
                         * Shedinja's ability, Wonder Guard, blocks any attacks except for
                         * super-effective moves. However, super-effective moves will
                         * automatically make it faint.
                         */
                        damage_pair.first = 1;
                        damage_pair.second = 1;
                        return damage_pair;
                    }
                    else
                    {
                        damage_pair.first = 0;
                        damage_pair.second = 0;
                        return damage_pair;
                    }
                }
                else if(attack->get_move_id() == Moves::SEISMIC_TOSS
                        or attack->get_move_id() == Moves::NIGHT_SHADE)
                {
                    /*
                     * Seismic Toss and Night Shade's damage automatically
                     * matches the attacker's level.
                     */
                    damage_pair.first = attacker_level;
                    damage_pair.second = attacker_level;
                    return damage_pair;
                }
                else if(attack->get_move_id() == Moves::DRAGON_RAGE)
                {
                    //Dragon Rage always deals 40 damage
                    damage_pair.first = 40;
                    damage_pair.second = 40;
                    return damage_pair;
                }
                else if(attack->get_move_id() == Moves::SONICBOOM)
                {
                    //Sonicboom always deals 20 damage
                    damage_pair.first = 20;
                    damage_pair.second = 20;
                    return damage_pair;
                }
                else
                {
                    /*
                     * STAB: Same-type attack bonus
                     * If an attack's type matches the attacker's type,
                     * it's damage will be increased by 150%.
                     */
                    if(attack->get_type() == attacker->get_types().first or
                       attack->get_type() == attacker->get_types().second)
                    {
                        min_damage = (unsigned int)(floor(double(min_damage) * 1.5));
                        max_damage = (unsigned int)(floor(double(max_damage) * 1.5));
                    }
                }
                /*
                 * As long as the attack does any damage, it will do a minimum
                 * of 1, no matter the strength discrepancy.
                 */
                if(min_damage == 0) min_damage = 1;
                if(max_damage == 0) max_damage = 1;

                damage_pair.first = min_damage;
                damage_pair.second = max_damage;
                return damage_pair;
            }
            else
            {
                damage_pair.first = 0;
                damage_pair.second = 0;
                return damage_pair;
            }
        }

        std::pair<unsigned int, unsigned int> get_damage_range(team_pokemon::sptr attacker, team_pokemon::sptr defender,
                                                               move::sptr attack)
        {
            return get_damage_range(attacker->get_base_pokemon(), defender->get_base_pokemon(),
                                    attack, attacker->get_level(), defender->get_level());
        }
    } /* namespace analysis */
} /* namespace pkmn */