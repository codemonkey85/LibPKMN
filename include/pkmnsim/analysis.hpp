/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ANALYSIS_HPP
#define INCLUDED_ANALYSIS_HPP

#include <pkmnsim/base_pkmn.hpp>
#include <string>

namespace pkmnsim
{
    int return_min_possible_stat(pkmnsim::base_pkmn::sptr, std::string, int);
    int return_max_possible_stat(pkmnsim::base_pkmn::sptr, std::string, int);
    bool is_stat_possible(pkmnsim::base_pkmn::sptr, int, std::string, int);
}

#endif /*INCLUDED_ANALYSIS_HPP*/
