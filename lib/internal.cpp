/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmnsim/base_pkmn.hpp>
#include "internal.hpp"

namespace pkmnsim
{
    int get_pkmn_id(base_pkmn::sptr base) {return base->pkmn_id;}
    int get_species_id(base_pkmn::sptr base) {return base->species_id;}
}
