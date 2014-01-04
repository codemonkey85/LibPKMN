/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_ANALYSIS_DAMAGE_HPP
#define INCLUDED_PKMN_ANALYSIS_DAMAGE_HPP

#include <string>
#include <vector>

#include <pkmn/base_pokemon.hpp>
#include <pkmn/config.hpp>
#include <pkmn/team_pokemon.hpp>

namespace pkmn
{
    namespace analysis
    {
        double PKMN_API get_type_damage_mod(unsigned int type1, unsigned int type2, bool gen1);
        
        unsigned int PKMN_API get_base_damage(team_pokemon::sptr attacker, team_pokemon::sptr defender, move::sptr attack);
        
        unsigned int PKMN_API get_base_damage(unsigned int level, unsigned int attack,
                                                 unsigned int defense, unsigned int base_power);
                                                 
        void PKMN_API get_damage_range(team_pokemon::sptr attacker, team_pokemon::sptr defender,
                                          move::sptr attack, std::vector<unsigned int> &damage_range_vec);
                                          
        void PKMN_API get_damage_range(base_pokemon::sptr attacker, base_pokemon::sptr defender,
                                          move::sptr attack, unsigned int attacker_level,
                                          unsigned int defender_level, std::vector<unsigned int> &damage_range_vec);
    }
}

#endif /* INCLUDED_PKMN_ANALYSIS_DAMAGE_HPP */
