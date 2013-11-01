/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmnsim/analysis/damage.hpp>
#include <pkmnsim/analysis/stats.hpp>

#include "pkmnsim_swig_functions.hpp"

using namespace std;

namespace pkmnsim
{
    vector<unsigned int> get_stat_range_vec(base_pokemon::sptr b_pkmn, unsigned int stat, unsigned int level, unsigned int gen)
    {
        vector<unsigned int> stat_vec;
        analysis::get_stat_range(b_pkmn, stat, level, gen, stat_vec);
        return stat_vec;
    }
    
    vector<unsigned int> get_damage_range_vec(base_pokemon::sptr attacker, base_pokemon::sptr defender,
                              move::sptr attack, unsigned int attacker_level, unsigned int defender_level)
    {
        vector<unsigned int> damage_range_vec;
        analysis::get_damage_range(attacker, defender, attack, attacker_level, defender_level, damage_range_vec);
        return damage_range_vec;
    }
    
    std::vector<unsigned int> get_damage_range_vec(team_pokemon::sptr attacker, team_pokemon::sptr defender, move::sptr attack)
    {
        vector<unsigned int> damage_range_vec;
        analysis::get_damage_range(attacker, defender, attack, damage_range_vec);
        return damage_range_vec;
    }
}