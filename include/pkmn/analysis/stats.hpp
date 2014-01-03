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

#include <pkmn/base_pokemon.hpp>
#include <pkmn/config.hpp>

namespace pkmnsim
{
    namespace analysis
    {
        unsigned int PKMNSIM_API get_min_possible_stat(base_pokemon::sptr b_pkmn,
                                                       unsigned int stat,
                                                       unsigned int level,
                                                       unsigned int gen);

        unsigned int PKMNSIM_API get_max_possible_stat(base_pokemon::sptr b_pkmn,
                                                       unsigned int stat,
                                                       unsigned int level,
                                                       unsigned int gen);

        bool PKMNSIM_API is_stat_possible(base_pokemon::sptr b_pkmn,
                                          unsigned int stat_value,
                                          unsigned int stat,
                                          unsigned int level,
                                          unsigned int gen);

        void PKMNSIM_API get_stat_range(base_pokemon::sptr b_pkmn,
                                        unsigned int stat,
                                        unsigned int level,
                                        unsigned int gen,
                                        std::vector<unsigned int> &stat_vec);
    }
}

#endif /* INCLUDED_PKMNSIM_ANALYSIS_STATS_HPP */
