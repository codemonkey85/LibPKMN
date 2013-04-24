/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmnsim/base_pkmn.hpp>
#include "internal.hpp"
#include <string>

using namespace std;

namespace pkmnsim
{
    //Allows spec_pkmn to get protected base_pkmn variables
    int get_pkmn_id(base_pkmn::sptr base) {return base->pkmn_id;}
    int get_species_id(base_pkmn::sptr base) {return base->species_id;}

    //Transform identifier strings into proper format for database
    void to_database_format(std::string *identifier)
    {
        transform(identifier->begin(), identifier->end(), identifier->begin(), ::tolower);
        replace(identifier->begin(), identifier->end(), ' ', '-');
        //TODO: Remove instances of ' (e.g. Farfetch'd)
    }
}
