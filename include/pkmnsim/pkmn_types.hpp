/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_PKMN_TYPES_HPP
#define INCLUDED_PKMNSIM_PKMN_TYPES_HPP

#include <string>
#include <vector>

#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    /*
     * Returns the damage mod an attack of type1 would have against a Pokémon of type2.
     *
     * Parameters:
     *  - type1: Type of attack
     *  - type2: Type of receiving Pokemon
     *  - gen1: Whether or not analysis should be done with Gen 1 types
     */
    double PKMNSIM_API get_type_damage_mod(std::string type1, std::string type2, bool gen1);
}

#endif /* INCLUDED_PKMNSIM_PKMN_TYPES_HPP */
