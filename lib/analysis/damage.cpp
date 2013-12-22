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

#include <pkmnsim/enums.hpp>
#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/move.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/analysis/damage.hpp>
#include <pkmnsim/analysis/stats.hpp>

#include "../SQLiteCpp/src/SQLiteC++.h"

using namespace std;

namespace pkmnsim
{
    namespace analysis
    {
        static SQLite::Database db(get_database_path().c_str());
    
        double get_type_damage_mod(unsigned int type1, unsigned int type2, bool gen1)
        {
            if(gen1 and (type1 == Types::DARK or type1 == Types::STEEL
                         or type2 == Types::DARK or type2 == Types::STEEL)) return 1;

            double damage_mod;
            string query_string;

            if(type1 != Types::NONE and type1 != Types::UNKNOWN and type1 != Types::SHADOW
            and type2 != Types::NONE and type2 != Types::UNKNOWN and type2 != Types::SHADOW)
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

        unsigned int get_base_damage(team_pokemon::sptr attacker, team_pokemon::sptr defender,
                                     move::sptr attack)
        {
            unsigned int level = attacker->get_level();
            unsigned int base_power = attack->get_base_power();
            unsigned int attacker_ATK, defender_DEF;

            unsigned int move_damage_class = attack->get_move_damage_class();
            switch(move_damage_class)
            {
                case Move_Classes::PHYSICAL:
                    attacker_ATK = attacker->get_stats()[Stats::ATTACK];
                    defender_DEF = defender->get_stats()[Stats::DEFENSE];
                    break;

                case Move_Classes::SPECIAL:
                    if(attacker->get_generation() == 1)
                    {
                        attacker_ATK = attacker->get_stats()[Stats::SPECIAL];
                        defender_DEF = defender->get_stats()[Stats::SPECIAL];
                    }
                    else
                    {
                        attacker_ATK = attacker->get_stats()[Stats::SPECIAL_ATTACK];
                        defender_DEF = defender->get_stats()[Stats::SPECIAL_DEFENSE];
                    }
                    break;

                default:
                    return 0;
            }

            return (unsigned int)(floor((((2.0 * double(level) + 10.0) / 250.0) *
                   (double(attacker_ATK) / double(defender_DEF)) * double(base_power) * 2.0)));
        }

        unsigned int get_base_damage(unsigned int level, unsigned int attack,
                                     unsigned int defense, unsigned int base_power)
        {
            return (unsigned int)(floor((((2.0 * double(level) + 10.0) / 250.0) *
                   (double(attack) / double(defense)) * double(base_power) * 2.0)));
        }

        //TODO: get rid of duplicate get_damage_range code
        
        void get_damage_range(team_pokemon::sptr attacker, team_pokemon::sptr defender,
                              move::sptr attack, vector<unsigned int> &damage_range_vec)
        {
            damage_range_vec.clear();
            unsigned int attacker_ATK, defender_DEF;
            unsigned int min_damage, max_damage;

            unsigned int gen = attacker->get_generation();
            unsigned int move_damage_class = attack->get_move_damage_class();
            switch(move_damage_class)
            {
                case Move_Classes::PHYSICAL:
                    attacker_ATK = attacker->get_stats()[Stats::ATTACK];
                    defender_DEF = defender->get_stats()[Stats::DEFENSE];
                    break;

                case Move_Classes::SPECIAL:
                    if(gen == 1)
                    {
                        attacker_ATK = attacker->get_stats()[Stats::SPECIAL];
                        defender_DEF = defender->get_stats()[Stats::SPECIAL];
                    }
                    else
                    {
                        attacker_ATK = attacker->get_stats()[Stats::SPECIAL_ATTACK];
                        defender_DEF = defender->get_stats()[Stats::SPECIAL_DEFENSE];
                    }
                    break;

                default:
                    damage_range_vec.push_back(0);
                    damage_range_vec.push_back(0);
                    return;
            }
            min_damage = (unsigned int)(floor(get_base_damage(attacker, defender, attack) * 0.85));
            max_damage = get_base_damage(attacker, defender, attack);

            double type_mod = double(get_type_damage_mod(attack->get_type(),
                                                         defender->get_types()[0], (gen == 1)))
                            * double(get_type_damage_mod(attack->get_type(),
                                                         defender->get_types()[1], (gen == 1)));

            min_damage = (unsigned int)(floor(double(min_damage) * type_mod));
            max_damage = (unsigned int)(floor(double(max_damage) * type_mod));

            
            if(type_mod != 0)
            {
                if(defender->get_species_id() == Species::SHEDINJA)
                {
                    /*
                     * Shedinja's ability, Wonder Guard, blocks any attacks except for
                     * super-effective moves. However, super-effective moves will
                     * automatically make it faint.
                     */
                    if(type_mod > 1.0)
                    {
                        damage_range_vec.push_back(1);
                        damage_range_vec.push_back(1);
                        return;
                    }
                    else
                    {
                        damage_range_vec.push_back(0);
                        damage_range_vec.push_back(0);
                        return;
                    }
                }
                else if(attack->get_move_id() == Moves::SEISMIC_TOSS
                        or attack->get_move_id() == Moves::NIGHT_SHADE)
                {
                    /*
                     * Seismic Toss and Night Shade's damage automatically
                     * matches the attacker's level.
                     */
                    damage_range_vec.push_back(attacker->get_level());
                    damage_range_vec.push_back(attacker->get_level());
                    return;
                }
                else if(attack->get_move_id() == Moves::DRAGON_RAGE)
                {
                    //Dragon Rage always deals 40 damage
                    damage_range_vec.push_back(40);
                    damage_range_vec.push_back(40);
                    return;
                }
                else if(attack->get_move_id() == Moves::SONICBOOM)
                {
                    //Sonicboom always deals 20 damage
                    damage_range_vec.push_back(20);
                    damage_range_vec.push_back(20);
                    return;
                }
                else
                {
                    /*
                     * STAB: Same-type attack bonus
                     * If an attack's type matches the attacker's type,
                     * it's damage will be increased by 150%.
                     */
                    if(attack->get_type() == attacker->get_types()[0] or
                       attack->get_type() == attacker->get_types()[1])
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

                damage_range_vec.push_back(min_damage);
                damage_range_vec.push_back(max_damage);
            }
            else
            {
                damage_range_vec.push_back(0);
                damage_range_vec.push_back(0);
            }
        }
        
        //Abilities not taken unsigned into account
        void get_damage_range(base_pokemon::sptr attacker, base_pokemon::sptr defender, move::sptr attack,
                              unsigned int attacker_level, unsigned int defender_level, vector<unsigned int>& damage_range_vec)
        {
            damage_range_vec.clear();
            unsigned int attacker_min_ATK, attacker_max_ATK;
            unsigned int defender_min_DEF, defender_max_DEF;
            unsigned int min_damage, max_damage;

            unsigned int move_damage_class = attack->get_move_damage_class();
            unsigned int gen = attacker->get_generation();
            switch(move_damage_class)
            {
                case Move_Classes::PHYSICAL:
                    attacker_min_ATK = get_min_possible_stat(attacker, Stats::ATTACK, attacker_level, gen);
                    attacker_max_ATK = get_max_possible_stat(attacker, Stats::ATTACK, attacker_level, gen);
                    defender_min_DEF = get_min_possible_stat(defender, Stats::DEFENSE, defender_level, gen);
                    defender_max_DEF = get_max_possible_stat(defender, Stats::DEFENSE, defender_level, gen);
                    break;

                case Move_Classes::SPECIAL:
                    if(gen == 1)
                    {
                        attacker_min_ATK = get_min_possible_stat(attacker, Stats::SPECIAL, attacker_level, 1);
                        attacker_max_ATK = get_max_possible_stat(attacker, Stats::SPECIAL, attacker_level, 1);
                        defender_min_DEF = get_min_possible_stat(defender, Stats::SPECIAL, defender_level, 1);
                        defender_max_DEF = get_max_possible_stat(defender, Stats::SPECIAL, defender_level, 1);
                    }
                    else
                    {
                        attacker_min_ATK = get_min_possible_stat(attacker, Stats::SPECIAL_ATTACK, attacker_level, gen);
                        attacker_max_ATK = get_max_possible_stat(attacker, Stats::SPECIAL_ATTACK, attacker_level, gen);
                        defender_min_DEF = get_min_possible_stat(defender, Stats::SPECIAL_DEFENSE, defender_level, gen);
                        defender_max_DEF = get_max_possible_stat(defender, Stats::SPECIAL_DEFENSE, defender_level, gen);
                    }
                    break;

                default:
                    damage_range_vec.push_back(0);
                    damage_range_vec.push_back(0);
                    return;
            }
            min_damage = (unsigned int)(floor(get_base_damage(attacker_level, attacker_min_ATK, defender_max_DEF, attack->get_base_power()) * 0.85));
            max_damage = get_base_damage(attacker_level, attacker_max_ATK, defender_min_DEF, attack->get_base_power());

            double type_mod = double(get_type_damage_mod(attack->get_type(), defender->get_types()[0], (gen == 1))) *
                              double(get_type_damage_mod(attack->get_type(), defender->get_types()[1], (gen == 1)));

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
                        damage_range_vec.push_back(1);
                        damage_range_vec.push_back(1);
                        return;
                    }
                    else
                    {
                        damage_range_vec.push_back(0);
                        damage_range_vec.push_back(0);
                        return;
                    }
                }
                else if(attack->get_move_id() == Moves::SEISMIC_TOSS
                        or attack->get_move_id() == Moves::NIGHT_SHADE)
                {
                    /*
                     * Seismic Toss and Night Shade's damage automatically
                     * matches the attacker's level.
                     */
                    damage_range_vec.push_back(attacker_level);
                    damage_range_vec.push_back(attacker_level);
                    return;
                }
                else if(attack->get_move_id() == Moves::DRAGON_RAGE)
                {
                    //Dragon Rage always deal 40 damage
                    damage_range_vec.push_back(40);
                    damage_range_vec.push_back(40);
                    return;
                }
                else if(attack->get_move_id() == Moves::SONICBOOM)
                {
                    //Sonicboom always deals 20 damage
                    damage_range_vec.push_back(20);
                    damage_range_vec.push_back(20);
                    return;
                }
                else
                {
                    /*
                     * STAB: Same-type attack bonus
                     * If an attack's type matches the attacker's type,
                     * it's damage will be increased by 150%.
                     */
                    if(attack->get_type() == attacker->get_types()[0] or
                       attack->get_type() == attacker->get_types()[1])
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

                damage_range_vec.push_back(min_damage);
                damage_range_vec.push_back(max_damage);
            }
            else
            {
                damage_range_vec.push_back(0);
                damage_range_vec.push_back(0);
            }
        }
    } /* namespace analysis */
} /* namespace pkmnsim */