/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_ANALYSIS_DAMAGE_HPP
#define INCLUDED_PKMNSIM_ANALYSIS_DAMAGE_HPP

#include <string>
#include <vector>

#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/config.hpp>
#include <pkmnsim/team_pokemon.hpp>

namespace pkmnsim
{
    namespace analysis
    {
        double PKMNSIM_API get_type_damage_mod(unsigned int type1, unsigned int type2, bool gen1);
        
        unsigned int PKMNSIM_API get_base_damage(team_pokemon::sptr attacker, team_pokemon::sptr defender, move::sptr attack);
        
        unsigned int PKMNSIM_API get_base_damage(unsigned int level, unsigned int attack,
                                                 unsigned int defense, unsigned int base_power);
                                                 
        void PKMNSIM_API get_damage_range(team_pokemon::sptr attacker, team_pokemon::sptr defender,
                                          move::sptr attack, std::vector<unsigned int> &damage_range_vec);
                                          
        void PKMNSIM_API get_damage_range(base_pokemon::sptr attacker, base_pokemon::sptr defender,
                                          move::sptr attack, unsigned int attacker_level,
                                          unsigned int defender_level, std::vector<unsigned int> &damage_range_vec);
    }
}

#endif /* INCLUDED_PKMNSIM_ANALYSIS_DAMAGE_HPP */
