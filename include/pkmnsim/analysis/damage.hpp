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
#include <pkmnsim/spec_pokemon.hpp>

namespace pkmnsim
{
    namespace analysis
    {
        double PKMNSIM_API get_type_damage_mod(Types::types type1, Types::types type2, bool gen1);
        int PKMNSIM_API get_base_damage(spec_pokemon::sptr attacker, spec_pokemon::sptr defender, base_move::sptr move);
        int PKMNSIM_API get_base_damage(int level, int attack, int defense, int base_power);
        void PKMNSIM_API get_damage_range(spec_pokemon::sptr attacker, spec_pokemon::sptr defender,
                                          base_move::sptr move, std::vector<int> &damage_range_vec);
        void PKMNSIM_API get_damage_range(base_pokemon::sptr attacker, base_pokemon::sptr defender,
                                          base_move::sptr move, int attacker_level,
                                          int defender_level, std::vector<int> &damage_range_vec);
    }
}

#endif /* INCLUDED_PKMNSIM_ANALYSIS_DAMAGE_HPP */
