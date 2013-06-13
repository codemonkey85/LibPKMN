/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ANALYSIS_HPP
#define INCLUDED_ANALYSIS_HPP

#include <string>
#include <vector>

#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    int PKMNSIM_API get_min_possible_stat(base_pkmn::sptr b_pkmn, std::string, int level, int gen);
    int PKMNSIM_API get_max_possible_stat(base_pkmn::sptr b_pkmn, std::string, int level, int gen);
    bool PKMNSIM_API is_stat_possible(base_pkmn::sptr b_pkmn, int, std::string, int leve, int gen);
    int PKMNSIM_API get_base_damage(int level, int attack, int defense, int base_power);
    void PKMNSIM_API get_damage_range(base_pkmn::sptr attacker, base_pkmn::sptr defender,
                                      int attacker_level, int defender_level, int gen,
                                      std::vector<int>& damage_range_vec);
}

#endif /* INCLUDED_ANALYSIS_HPP */
