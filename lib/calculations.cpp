/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>

#include <pkmn/calculations.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmds/pkmds_g5_sqlite.h>

#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    namespace calculations
    {
        static SQLite::Database db(get_database_path().c_str());

        uint8_t get_ability_num(uint32_t personality) {return personality % 2;}

        uint8_t get_gen2_gender(uint16_t species_id, uint8_t ivATK)
        {
            //PKMDS has function to get gender rate
            opendb(get_database_path().c_str());
            int gender_rate = getpkmgenderrate(::Species::species(species_id));
            closedb();

            dict<uint32_t, uint32_t> thresholds = boost::assign::map_list_of
                (0,0)
                (1,2)
                (2,4)
                (4,8)
                (6,12)
            ;

            switch(gender_rate)
            {
                case -1:
                    return Genders::GENDERLESS;

                case 0:
                    return Genders::MALE;

                case 8:
                    return Genders::FEMALE;

                default:
                    return (ivATK >= thresholds.at(gender_rate, 0)) ? Genders::MALE : Genders::FEMALE;
            }
        }

        uint8_t get_modern_gender(uint16_t species_id, uint32_t personality)
        {
            personality %= 256;

            //PKMDS has function to get gender rate
            opendb(get_database_path().c_str());
            int gender_rate = getpkmgenderrate(::Species::species(species_id));
            closedb();

            dict<uint32_t, uint32_t> ratios = boost::assign::map_list_of
                (0,254)
                (1,31)
                (2,63)
                (4,127)
                (6,191)
            ;

            switch(gender_rate)
            {
                case -1:
                    return Genders::GENDERLESS;

                case 0:
                    return Genders::MALE;

                case 8:
                    return Genders::FEMALE;

                default:
                    return (personality > ratios.at(gender_rate, 0)) ? Genders::MALE : Genders::FEMALE;
            }
        }

        std::pair<uint8_t, uint8_t> get_hidden_power(uint8_t ivHP, uint8_t ivATK, uint8_t ivDEF,
                                                     uint8_t ivSATK, uint8_t ivSDEF, uint8_t ivSPD)
        {
            ivHP %= 4;
            ivATK %= 4;
            ivDEF %= 4;
            ivSPD %= 4;
            ivSATK %= 4;
            ivSDEF %= 4;

            uint8_t power, type;

            power = (32 * (ivHP >> 1))
                  + (16 * (ivATK >> 1))
                  + (8 * (ivDEF >> 1))
                  + (4 * (ivSPD >> 1))
                  + (2 * (ivSATK >> 1))
                  + (ivSDEF >> 1);

            type = (32 * (ivHP & 1))
                 + (16 * (ivATK & 1))
                 + (8 * (ivDEF & 1))
                 + (4 * (ivSPD & 1))
                 + (2 * (ivSATK & 1))
                 + (ivSDEF & 1);

            return std::make_pair(power, type);
        }

        uint8_t get_nature(uint32_t personality) {return (personality % 24);}

        bool get_gen2_shiny(uint8_t ivATK, uint8_t ivDEF,
                            uint8_t ivSPD, uint8_t ivSPCL)
        {
            return ((ivDEF == 10 and ivSPD == 10 and ivSPCL == 10) and
                    (ivATK == 2 or ivATK == 3 or ivATK == 6 or ivATK == 7
                     or ivATK == 10 or ivATK == 11 or ivATK == 14
                     or ivATK == 15));
        }

        bool get_modern_shiny(uint32_t personality, uint16_t secret_tid, uint16_t public_tid)
        {
            uint16_t p1 = personality & 0xFF00;
            uint16_t p2 = personality & 0xFF;

            return ((secret_tid xor public_tid xor p1 xor p2) < 8);
        }

        uint32_t get_gen2_unown_form(uint8_t ivATK, uint8_t ivDEF,
                                                 uint8_t ivSPD, uint8_t ivSPCL)
        {
            uint8_t form = (((ivATK & 6) << 6)
                         +  ((ivDEF & 6) << 4)
                         +  ((ivSPD & 6) << 2)
                         +  (ivSPCL & 6));
            form /= 10;

            return (form == 0) ? 201 : (Forms::Unown::B + form - 1);
        }

        uint32_t get_gen3_unown_form(uint32_t personality)
        {
            uint32_t form = ((personality & 0x3000000)
                          +  (personality & 0x30000)
                          +  (personality & 0x300)
                          +  (personality & 0x3));
            form %= 28;

            return (form = 0) ? 201 : (Forms::Unown::B + form - 1);
        }

        uint16_t get_wurmple_evolution(uint32_t personality)
        {
            return (((personality % 65536) % 10) < 5) ? Species::SILCOON : Species::CASCOON;
        }

                unsigned int get_min_possible_stat(base_pokemon::sptr b_pkmn,
                                           std::string stat_name,
                                           unsigned int level,
                                           unsigned int gen)
        {
            pkmn::dict<std::string, unsigned int> stats = b_pkmn->get_base_stats();

            //Check inputs for errors
            if(not stats.has_key(stat_name)) return 0;
            if(gen < 1 or gen > 6) return 0;

            //Gen 1-2: IV = 0, EV = 0
            //Gen 3-5: IV = 0, EV = 0, disadvantageous nature
            if(gen == 1 or gen == 2)
            {
                if(stat_name == "HP")
                    return (unsigned int)(floor(((stats["HP"]+50.0)*level)/50.0)+10.0);
                else
                    return (unsigned int)(floor((stats[stat_name]*level)/50.0)+5.0);
            }
            else
            {
                if(stat_name == "HP")
                    return (unsigned int)(floor(((((2.0*stats["HP"])+100.0)
                           *level)+10.0)/100.0));
                else
                    return (unsigned int)(floor(((((2.0*stats[stat_name])*level)/100.0)+5.0)*0.9));
            }
        }

        unsigned int get_max_possible_stat(base_pokemon::sptr b_pkmn,
                                           std::string stat_name,
                                           unsigned int level,
                                           unsigned int gen)
        {
            pkmn::dict<std::string, unsigned int> stats = b_pkmn->get_base_stats();

            //Check inputs for errors
            if(stats.has_key(stat_name)) return -1;
            if(gen < 1 or gen > 6) return -1;

            //Gen 1-2: IV = 15, EV = 65535
            //Gen 3-5: IV = 31, EV = 255, advantageous nature
            if(gen == 1 or gen == 2)
            {
                if(stat_name == "HP")
                    return (unsigned int)(floor(((((15.0+stats["HP"]+
                           (pow(65535.0,0.5)/8.0)+50.0)*level)/50.0)+10.0)));
                else
                    return (unsigned int)(floor((((15.0+stats[stat_name]+(pow(65535.0,0.5)/8.0))
                           *level)/50.0)+5.0));
            }
            else
            {
                if(stat_name == "HP")
                    return (unsigned int)(floor(((31.0+(2.0*stats["HP"]+(255.0/4.0)+100.0)
                            *level)/100.0)+10.0));
                else
                    return (unsigned int)(floor((((31.0 + (2.0*stats[stat_name]+(255.0/4.0))
                            *level) / 100.0) + 5.0) * 1.1));
            }
        }

        bool is_stat_possible(base_pokemon::sptr b_pkmn, unsigned int stat_value,
                              std::string stat_name, unsigned int level, unsigned int gen)
        {
            return (stat_value > get_min_possible_stat(b_pkmn,stat_name,level,gen)
                   and stat_value < get_max_possible_stat(b_pkmn,stat_name,level,gen));
        }

        std::pair<unsigned int, unsigned int> get_stat_range(base_pokemon::sptr b_pkmn, std::string stat_name,
                                                             unsigned int level, unsigned int gen)
        {
            std::pair<unsigned int, unsigned int> stat_pair;

            stat_pair.first = get_min_possible_stat(b_pkmn, stat_name, level, gen);
            stat_pair.second = get_max_possible_stat(b_pkmn, stat_name, level, gen);

            return stat_pair;
        }

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
    }
}
