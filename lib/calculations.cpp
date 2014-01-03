/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/assign/list_of.hpp>

#include <pkmn/calculations.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/types/dict.hpp>

#include <pkmds/pkmds_g5_sqlite.h>

#include "SQLiteCpp/src/SQLiteC++.h"

namespace pkmnsim
{
    namespace calculations
    {
        uint8_t get_ability_num(uint32_t personality) {return personality % 2;}

        uint8_t get_gen2_gender(uint16_t species_id, uint8_t ivATK)
        {
            //PKMDS has function to get gender rate
            opendb(get_database_path().c_str());
            int gender_rate = getpkmgenderrate(::Species::pkmspecies(species_id));
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
                    return (ivATK >= thresholds.get(gender_rate, 0)) ? Genders::MALE : Genders::FEMALE;
            }
        }

        uint8_t get_modern_gender(uint16_t species_id, uint32_t personality)
        {
            personality %= 256;

            //PKMDS has function to get gender rate
            opendb(get_database_path().c_str());
            int gender_rate = getpkmgenderrate(::Species::pkmspecies(species_id));
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
                    return (personality > ratios.get(gender_rate, 0)) ? Genders::MALE : Genders::FEMALE;
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
    }
}
