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
    int get_min_possible_stat(base_pkmn::sptr b_pkmn, string stat, int level, int gen)
    {
        //Check inputs for errors
        if(not b_pkmn->get_base_stats().has_key(stat)) throw runtime_error("Invalid stat specified.");
        if(gen < 1 or gen > 5) throw runtime_error("Invalid generation specified.");

        //Gen 1-2: IV = 0, EV = 0
        //Gen 3-5: IV = 0, EV = 0, disadvantageous nature
        dict<string,int> stats = b_pkmn->get_base_stats();
        if(gen == 1 or gen == 2)
        {
            if(stat == "HP")
                return int(floor(((stats["HP"]+50.0)*level)/50.0)+10.0);
            else
                return int(floor((stats[stat]*level)/50.0)+5.0);
        }
        else
        {
            if(stat == "HP")
                return int(floor(((((2.0*stats["HP"])+100.0)*level)+10.0)/100.0));
            else
                return int(floor(((((2.0*stats[stat])*level)/100.0)+5.0)*0.9));
        }
    }

    int get_max_possible_stat(base_pkmn::sptr b_pkmn, string stat, int level, int gen)
    {
        //Check inputs for errors
        if(not b_pkmn->get_base_stats().has_key(stat)) throw runtime_error("Invalid stat specified.");
        if(gen < 1 or gen > 5) throw runtime_error("Invalid generation specified.");

        //Gen 1-2: IV = 15, EV = 65535
        //Gen 3-5: IV = 31, EV = 255, advantageous nature
        dict<string,int> stats = b_pkmn->get_base_stats();
        if(gen == 1 or gen == 2)
        {
            if(stat == "HP")
                return int(floor(((((15.0+stats["HP"]+(pow(65535.0,0.5)/8.0)+50.0)*level)/50.0)+10.0)));
            else
                return int(floor((((15.0+stats[stat]+(pow(65535.0,0.5)/8.0))*level)/50.0)+5.0));
        }
        else
        {
            if(stat == "HP")
                return int(floor(((31.0+(2.0*stats[stat]+(255.0/4.0)+100.0)*level)/100.0)+10.0));
            else
                return int(floor((((31.0 + (2.0*stats[stat]+(255.0/4.0)) * level) / 100.0) + 5.0) * 1.1));
        }
    }

    bool is_stat_possible(base_pkmn::sptr b_pkmn, int stat_value, string stat, int level, int gen)
    {
        return (stat_value > get_min_possible_stat(b_pkmn,stat,level,gen) and stat_value < get_max_possible_stat(b_pkmn,stat,level,gen));
    }

    void get_stat_range(base_pkmn::sptr b_pkmn, string stat, int level, int gen, vector<int>& stat_vec)
    {
        stat_vec.clear();

        stat_vec.push_back(get_min_possible_stat(b_pkmn, stat, level, gen));
        stat_vec.push_back(get_max_possible_stat(b_pkmn, stat, level, gen));
    }

    vector<int> get_stat_range_vec(base_pkmn::sptr b_pkmn, string stat, int level, int gen)
    {
        vector<int> stat_vec;
        get_stat_range(b_pkmn, stat, level, gen, stat_vec);

        return stat_vec;
    }

    int get_base_damage(spec_pkmn::sptr attacker, spec_pkmn::sptr defender, base_move::sptr move)
    {
        int level = attacker->get_level();
        int base_power = move->get_base_power();
        int attacker_ATK, defender_DEF;

        int move_damage_class = move->get_move_damage_class();
        switch(move_damage_class)
        {
            case Move_Classes::PHYSICAL:
                attacker_ATK = attacker->get_stats()["ATK"];
                defender_DEF = defender->get_stats()["DEF"];
                break;

            case Move_Classes::SPECIAL:
                if(attacker->get_generation() == 1)
                {
                    attacker_ATK = attacker->get_stats()["SPCL"];
                    defender_DEF = defender->get_stats()["SPCL"];
                }
                else
                {
                    attacker_ATK = attacker->get_stats()["SATK"];
                    defender_DEF = defender->get_stats()["SDEF"];
                }
                break;

            default:
                return 0;
        }

        return int(floor((((2.0 * double(level) + 10.0) / 250.0) *
               (double(attacker_ATK) / double(defender_DEF)) * double(base_power) * 2.0)));
    }

    int get_base_damage(int level, int attack, int defense, int base_power)
    {
        return int(floor((((2.0 * double(level) + 10.0) / 250.0) *
               (double(attack) / double(defense)) * double(base_power) * 2.0)));
    }

    void get_damage_range(spec_pkmn::sptr attacker, spec_pkmn::sptr defender, base_move::sptr move,
                          vector<int>& damage_range_vec)
    {
        damage_range_vec.clear();
        int attacker_ATK, defender_DEF;
        int min_damage, max_damage;

        int gen = attacker->get_generation();
        int move_damage_class = move->get_move_damage_class();
        switch(move_damage_class)
        {
            case Move_Classes::PHYSICAL:
                attacker_ATK = attacker->get_stats()["ATK"];
                defender_DEF = defender->get_stats()["DEF"];
                break;

            case Move_Classes::SPECIAL:
                if(gen == 1)
                {
                    attacker_ATK = attacker->get_stats()["SPCL"];
                    defender_DEF = defender->get_stats()["SPCL"];
                }
                else
                {
                    attacker_ATK = attacker->get_stats()["SATK"];
                    defender_DEF = defender->get_stats()["SDEF"];
                }
                break;

            default:
                damage_range_vec.push_back(0);
                damage_range_vec.push_back(0);
                return;
        }
        min_damage = int(floor(get_base_damage(attacker, defender, move) * 0.85));
        max_damage = get_base_damage(attacker, defender, move);

        double type_mod = double(get_type_damage_mod(move->get_type(), defender->get_types()[0], (gen == 1))) *
                          double(get_type_damage_mod(move->get_type(), defender->get_types()[1], (gen == 1)));

        min_damage = int(floor(double(min_damage) * type_mod));
        max_damage = int(floor(double(max_damage) * type_mod));

        if(type_mod != 0)
        {
            if(defender->get_species_name() == "Shedinja")
            {
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
            else if(move->get_name() == "Seismic Toss" or move->get_name() == "Night Shade")
            {
                damage_range_vec.push_back(attacker->get_level());
                damage_range_vec.push_back(attacker->get_level());
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
                if(move->get_type() == attacker->get_types()[0] or
                   move->get_type() == attacker->get_types()[1])
                {
                    min_damage = int(floor(double(min_damage) * 1.5));
                    max_damage = int(floor(double(max_damage) * 1.5));
                }
            }
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

    vector<int> get_damage_range_from_spec_pkmn(spec_pkmn::sptr attacker, spec_pkmn::sptr defender, base_move::sptr move)
    {
        vector<int> vec;
        get_damage_range(attacker, defender, move, vec);
        return vec;
    }

    //Abilities not taken into account
    void get_damage_range(base_pkmn::sptr attacker, base_pkmn::sptr defender, base_move::sptr move,
                          int attacker_level, int defender_level, vector<int>& damage_range_vec)
    {
        damage_range_vec.clear();
        int attacker_min_ATK, attacker_max_ATK;
        int defender_min_DEF, defender_max_DEF;
        int min_damage, max_damage;

        int move_damage_class = move->get_move_damage_class();
        int gen = attacker->get_generation();
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
        min_damage = int(floor(get_base_damage(attacker_level, attacker_min_ATK, defender_max_DEF, move->get_base_power()) * 0.85));
        max_damage = get_base_damage(attacker_level, attacker_max_ATK, defender_min_DEF, move->get_base_power());

        double type_mod = double(get_type_damage_mod(move->get_type(), defender->get_types()[0], (gen == 1))) *
                          double(get_type_damage_mod(move->get_type(), defender->get_types()[1], (gen == 1)));

        min_damage = int(floor(double(min_damage) * type_mod));
        max_damage = int(floor(double(max_damage) * type_mod));

        if(type_mod != 0)
        {
            if(defender->get_species_name() == "Shedinja")
            {
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
                    min_damage = int(floor(double(min_damage) * 1.5));
                    max_damage = int(floor(double(max_damage) * 1.5));
                }
            }
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

    vector<int> get_damage_range_from_base_pkmn(base_pkmn::sptr attacker, base_pkmn::sptr defender, base_move::sptr move,
                                                int attacker_level, int defender_level)
    {
        vector<int> vec;
        get_damage_range(attacker, defender, move, attacker_level, defender_level, vec);
        return vec;
    }
}
