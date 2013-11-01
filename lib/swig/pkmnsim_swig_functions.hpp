/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_SWIG_FUNCTIONS_HPP
#define INCLUDED_PKMNSIM_SWIG_FUNCTIONS_HPP

#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/move.hpp>
#include <pkmnsim/team_pokemon.hpp>

namespace pkmnsim
{
    //These functions would be inefficient to use in C++, but because
    //some languages don't have user-facing pass-by-reference, pass-by-value
    //equivalents must be made
    std::vector<unsigned int> get_stat_range_vec(base_pokemon::sptr b_pkmn, unsigned int stat, unsigned int level, unsigned int gen);
    std::vector<unsigned int> get_damage_range_vec(base_pokemon::sptr attacker, base_pokemon::sptr defender,
                              move::sptr attack, unsigned int attacker_level, unsigned int defender_level);
    std::vector<unsigned int> get_damage_range_vec(team_pokemon::sptr attacker, team_pokemon::sptr defender, move::sptr attack);
}

#endif /* INCLUDED_PKMNSIM_SWIG_FUNCTIONS_HPP */
