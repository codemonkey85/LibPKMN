/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_ANALYSIS_STATS_HPP
#define INCLUDED_PKMN_ANALYSIS_STATS_HPP

#include <string>
#include <utility>

#include <pkmn/base_pokemon.hpp>
#include <pkmn/config.hpp>

namespace pkmn
{
    namespace analysis
    {
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
    }
}

#endif /* INCLUDED_PKMN_ANALYSIS_STATS_HPP */
