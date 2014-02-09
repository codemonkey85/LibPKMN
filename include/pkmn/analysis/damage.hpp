/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_ANALYSIS_DAMAGE_HPP
#define INCLUDED_PKMN_ANALYSIS_DAMAGE_HPP

#include <string>
#include <utility>
#include <vector>

#include <pkmn/base_pokemon.hpp>
#include <pkmn/config.hpp>
#include <pkmn/team_pokemon.hpp>

namespace pkmn
{
    namespace analysis
    {
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

#endif /* INCLUDED_PKMN_ANALYSIS_DAMAGE_HPP */
