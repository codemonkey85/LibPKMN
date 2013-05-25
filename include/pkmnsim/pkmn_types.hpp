/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_HPP
#define INCLUDED_PKMN_TYPES_HPP

#include <pkmnsim/config.hpp>
#include <string>
#include <vector>

namespace pkmnsim
{
    /*
     * Returns the damage mod an attack of type1 would have against a Pokémon of type2.
     */
    double PKMNSIM_API get_type_damage_mod(std::string type1, std::string type2, bool gen1);
}

#endif /* INCLUDED_PKMN_TYPES_HPP */
