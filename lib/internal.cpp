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

    //Transform identifier strings into proper format for database
    void to_database_format(std::string *identifier)
    {
        transform(identifier->begin(), identifier->end(), identifier->begin(), ::tolower);
        replace(identifier->begin(), identifier->end(), ' ', '-');
        //TODO: Remove instances of ' (e.g. Farfetch'd)
    }

    //Get location of database
    string get_database_path(void)
    {
        const char* database_dir = getenv("PKMNSIM_DATABASE_DIR"); //Environment variable
        if(database_dir == NULL) database_dir = "@DATABASE_DIR@"; //CMake variable

        fs::path database_path = fs::path(database_dir) / "pkmnsim.db";

        ifstream ifile(database_path.string().c_str());
        if(not ifile) throw runtime_error("Could not find database!");

        return database_path.string();
    }
}
