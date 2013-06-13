/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <math.h>
#include <stdexcept>
#include <time.h>

#include <pkmnsim/analysis.hpp>
#include <pkmnsim/base_move.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/pkmn_types.hpp>

using namespace std;

//Base stat calculations

namespace pkmnsim
{
    int get_min_possible_stat(base_pkmn::sptr bpkmn, string stat, int level, int gen)
    {
        //Check inputs for errors
        if(not bpkmn->get_base_stats().has_key(stat)) throw runtime_error("Invalid stat specified.");
        if(gen < 1 or gen > 5) throw runtime_error("Invalid generation specified.");

        //Gen 1-2: IV = 0, EV = 0
        //Gen 3-5: IV = 0, EV = 0, disadvantageous nature
        if(stat == "HP")
        {
            if(gen == 1 or gen == 2) return int(floor((((bpkmn->get_base_stats()["HP"] + 50.0) * level) / 50.0) + 10.0));
            else return int(floor((((2.0 * bpkmn->get_base_stats()["HP"]) * level) / 100.0) + 10.0));
        }
        else
        {
            if(gen == 1 or gen == 2) return int(floor((((bpkmn->get_base_stats()[stat] * level) / 50.0) + 5.0)));
            else return int(floor(((((2.0 * bpkmn->get_base_stats()[stat]) * level) / 100.0) + 5.0) * 0.9));
        }
    }

    int get_max_possible_stat(base_pkmn::sptr bpkmn, string stat, int level, int gen)
    {
        //Check inputs for errors
        if(not bpkmn->get_base_stats().has_key(stat)) throw runtime_error("Invalid stat specified.");
        if(gen < 1 or gen > 5) throw runtime_error("Invalid generation specified.");

        //Gen 1-2: IV = 15, EV = 65535
        //Gen 3-5: IV = 31, EV = 255, advantageous nature
        if(gen == 1 or gen == 2)
        {
            if(stat == "HP")
                return int(floor((((15 + bpkmn->get_base_stats()["HP"] + (pow(65535.0, 0.5) / 8.0) + 50) * level) / 50) + 10));
            else
                return int(floor((((15 + bpkmn->get_base_stats()[stat] + (pow(65535.0, 0.5) / 8.0)) * level) / 50) + 5));
        }
        else
        {
            if(stat == "HP")
                return int(floor(((31.0 + (2.0 * bpkmn->get_base_stats()["HP"]) + (255.0 / 4.0)) / 100.0) + 10.0));
            else
                return int(floor(((((31.0 + (2.0 * bpkmn->get_base_stats()[stat]) + (255 / 4)) * level) / 100.0) + 5.0) * 1.1));
        }
    }

    bool is_stat_possible(base_pkmn::sptr bpkmn, int stat_value, string stat, int level, int gen)
    {
        return (stat_value > get_min_possible_stat(bpkmn,stat,level,gen) and stat_value < get_max_possible_stat(bpkmn,stat,level,gen));
    }

    int get_base_damage(int level, int attack, int defense, int base_power)
    {
        return (((2 * level + 10) / 250) + (attack / defense) * base_power * 2);
    }

    //Abilities not taken into account
    void get_damage_range(base_pkmn::sptr attacker, base_pkmn::sptr defender, base_move::sptr move,
                          int attacker_level, int defender_level, int gen, vector<int>& damage_range_vec)
    {
        srand( time(NULL) );

        damage_range_vec.clear();
        int attacker_min_ATK, attacker_max_ATK;
        int defender_min_DEF, defender_max_DEF;
        int min_damage, max_damage;

        int move_damage_class = move->get_move_damage_class();
        switch(move_damage_class)
        {
            case Move_Classes::PHYSICAL:
                attacker_min_ATK = get_min_possible_stat(attacker, "ATK", attacker_level, gen);
                attacker_max_ATK = get_max_possible_stat(attacker, "ATK", attacker_level, gen);
                defender_min_DEF = get_min_possible_stat(defender, "DEF", defender_level, gen);
                defender_max_DEF = get_max_possible_stat(defender, "DEF", defender_level, gen);
                break;

            case Move_Classes::SPECIAL:
                if(gen == 1)
                {
                    attacker_min_ATK = get_min_possible_stat(attacker, "SPCL", attacker_level, 1);
                    attacker_max_ATK = get_max_possible_stat(attacker, "SPCL", attacker_level, 1);
                    defender_min_DEF = get_min_possible_stat(defender, "SPCL", defender_level, 1);
                    defender_max_DEF = get_max_possible_stat(defender, "SPCL", defender_level, 1);
                }
                else
                {
                    attacker_min_ATK = get_min_possible_stat(attacker, "SATK", attacker_level, gen);
                    attacker_max_ATK = get_max_possible_stat(attacker, "SATK", attacker_level, gen);
                    defender_min_DEF = get_min_possible_stat(defender, "SDEF", defender_level, gen);
                    defender_max_DEF = get_max_possible_stat(defender, "SDEF", defender_level, gen);
                }
                break;

            default:
                damage_range_vec.push_back(0);
                damage_range_vec.push_back(0);
                return;
        }
        min_damage = get_base_damage(attacker_level, attacker_min_ATK, defender_max_DEF, move->get_base_power()) * 0.85;
        max_damage = get_base_damage(attacker_level, attacker_max_ATK, defender_min_DEF, move->get_base_power());

        double type_mod = get_type_damage_mod(move->get_type(), defender->get_types()[0], gen) *
                          get_type_damage_mod(move->get_type(), defender->get_types()[1], gen);

        min_damage *= type_mod;
        max_damage *= type_mod;

        if(type_mod != 0)
        {
            if(move->get_name() == "Seismic Toss" or move->get_name() == "Night Shade")
            {
                damage_range_vec.push_back(attacker_level);
                damage_range_vec.push_back(attacker_level);
                return;
            }
            else if(move->get_name() == "Dragon Rage")
            {
                damage_range_vec.push_back(40);
                damage_range_vec.push_back(40);
                return;
            }
            else if(move->get_name() == "SonicBoom")
            {
                damage_range_vec.push_back(20);
                damage_range_vec.push_back(20);
                return;
            }
            else
            {
                if(move->get_type() == attacker->get_types()[0] or move->get_type() == attacker->get_types()[1])
                {
                    min_damage *= 1.5;
                    max_damage *= 1.5;
                }
            }
            damage_range_vec.push_back(min_damage);
            damage_range_vec.push_back(max_damage);
        }
        else
        {
            damage_range_vec.push_back(0);
            damage_range_vec.push_back(0);
        }
    }
}
