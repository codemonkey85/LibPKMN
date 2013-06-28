/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <string>

#include <boost/filesystem.hpp>

#include <pkmnsim/base_pkmn.hpp>

#include "internal.hpp"

namespace fs = boost::filesystem;
using namespace std;

namespace pkmnsim
{
    //Allows spec_pkmn to get protected base_pkmn variables
    int get_pkmn_id(base_pkmn::sptr base) {return base->pkmn_id;}
    int get_species_id(base_pkmn::sptr base) {return base->species_id;}
}
