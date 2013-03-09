/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_TYPES_HPP
#define PKMN_TYPES_HPP

#include <pkmnsim/config.hpp>
#include <string>
#include <vector>

namespace pkmnsim
{
    /*
     * Return the damage mod an attack of type1 would have against a Pokemon of type2.
     * \param type1 Offensive type
     * \param type2 Defensive type
     * \param Double with type mod
     */
    double PKMNSIM_API get_type_damage_mod(std::string type1, std::string type2, bool gen1);

    /*
     * Get a list of type names present in specified generation.
     * \param gen Generation from which to grab types (Dark and Steel not present in Generation 1)
     * \return a vector of strings containing type list
     */
    std::vector<std::string> PKMNSIM_API get_type_names(int gen);
}

#endif /*PKMN_TYPES_HPP*/
