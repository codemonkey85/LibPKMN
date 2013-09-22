/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_ANALYSIS_STATS_HPP
#define INCLUDED_PKMNSIM_ANALYSIS_STATS_HPP

#include <string>
#include <vector>

#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/config.hpp>
#include <pkmnsim/spec_pokemon.hpp>

namespace pkmnsim
{
    int PKMNSIM_API get_min_possible_stat(base_pokemon::sptr b_pkmn, int stat, int level, int gen);
    int PKMNSIM_API get_max_possible_stat(base_pokemon::sptr b_pkmn, int stat, int level, int gen);
    bool PKMNSIM_API is_stat_possible(base_pokemon::sptr b_pkmn, int, int stat, int level, int gen);
    void PKMNSIM_API get_stat_range(base_pokemon::sptr b_pkmn, int int stat, int level, int gen, std::vector<int> &stat_vec);
    std::vector<int> PKMNSIM_API get_stat_range_vec(base_pokemon::sptr b_pkmn, int stat, int level, int gen);
}

#endif /* INCLUDED_PKMNSIM_ANALYSIS_STATS_HPP */
