/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CALCULATIONS_HPP
#define INCLUDED_PKMN_CALCULATIONS_HPP

#include <cstdint>
#include <utility>

#include <pkmn/config.hpp>
#include <pkmn/base_pokemon.hpp>
#include <pkmn/team_pokemon.hpp>

namespace pkmn
{
    namespace calculations
    {
        uint8_t PKMN_API get_ability_num(uint32_t personality);

        uint8_t PKMN_API get_gen2_gender(uint16_t species_id, uint8_t ivATK);

        uint8_t PKMN_API get_modern_gender(uint16_t species_id, uint32_t personality);

        std::pair<uint8_t, uint8_t> PKMN_API get_hidden_power(uint8_t ivHP, uint8_t ivATK, uint8_t ivDEF,
                                                                 uint8_t ivSATK, uint8_t ivSDEF, uint8_t ivSPD);

        uint8_t PKMN_API get_nature(uint32_t personality);

        bool PKMN_API get_gen2_shiny(uint8_t ivATK, uint8_t ivDEF,
                                        uint8_t ivSPD, uint8_t ivSPCL);

        bool PKMN_API get_modern_shiny(uint32_t personality, uint16_t secret_tid, uint16_t public_tid);

        uint32_t PKMN_API get_gen2_unown_form(uint8_t ivATK, uint8_t ivDEF,
                                                 uint8_t ivSPD, uint8_t ivSPCL);

        uint32_t PKMN_API get_gen3_unown_form(uint32_t personality);

        uint16_t PKMN_API get_wurmple_evolution(uint32_t personality);

        unsigned int PKMN_API get_min_possible_stat(base_pokemon::sptr b_pkmn,
                                                    std::string stat_name,
                                                    unsigned int level,
                                                    unsigned int gen);

        unsigned int PKMN_API get_max_possible_stat(base_pokemon::sptr b_pkmn,
                                                    std::string stat_name,
                                                    unsigned int level,
                                                    unsigned int gen);

        bool PKMN_API is_stat_possible(base_pokemon::sptr b_pkmn,
                                       unsigned int stat_value,
                                       std::string stat_name,
                                       unsigned int level,
                                       unsigned int gen);

        std::pair<unsigned int, unsigned int> PKMN_API get_stat_range(base_pokemon::sptr b_pkmn,
                                                                      std::string stat_name,
                                                                      unsigned int level,
                                                                      unsigned int gen);

        double PKMN_API get_type_damage_mod(std::string type1, std::string type2, unsigned int gen);

        unsigned int PKMN_API get_base_damage(unsigned int level, unsigned int attack,
                                              unsigned int defense, unsigned int base_power);

        unsigned int PKMN_API get_base_damage(team_pokemon::sptr attacker, team_pokemon::sptr defender, move::sptr attack);

        std::pair<unsigned int, unsigned int> PKMN_API get_damage_range(base_pokemon::sptr attacker, base_pokemon::sptr defender,
                                                                        move::sptr attack, unsigned int attacker_level,
                                                                        unsigned int defender_level);

        std::pair<unsigned int, unsigned int> PKMN_API get_damage_range(team_pokemon::sptr attacker, team_pokemon::sptr defender,
                                                                        move::sptr attack);
    }
}

#endif /* INCLUDED_PKMN_CALCULATIONS_HPP */
