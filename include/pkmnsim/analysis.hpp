/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ANALYSIS_HPP
#define INCLUDED_ANALYSIS_HPP

#include <string>

#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    int PKMNSIM_API return_min_possible_stat(pkmnsim::base_pkmn::sptr b_pkmn, std::string, int level, int gen);
    int PKMNSIM_API return_max_possible_stat(pkmnsim::base_pkmn::sptr b_pkmn, std::string, int level, int gen);
    bool PKMNSIM_API is_stat_possible(pkmnsim::base_pkmn::sptr b_pkmn, int, std::string, int leve, int gen);
}

#endif /* INCLUDED_ANALYSIS_HPP */
