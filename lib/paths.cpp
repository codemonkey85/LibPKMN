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

#include <pkmnsim/paths.hpp>

namespace fs = boost::filesystem;
using namespace std;

namespace pkmnsim
{
    //Transform identifier strings into proper format for database
    string to_database_format(std::string identifier)
    {
        //Some manual changes
        if(identifier == "Farfetch'd") identifier = "farfetchd";
        else if(identifier == "Mr. Mime") identifier = "mr-mime";
        else if(identifier == "Mime Jr.") identifier = "mime-jr";

        transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);
        replace(identifier.begin(), identifier.end(), ' ', '-');

        return identifier;
    }

    //Get location of database
    string get_database_path(void)
    {
        const char* database_dir = getenv("PKMNSIM_DATABASE_DIR"); //Environment variable
        if(database_dir == NULL) database_dir = "@PKMNSIM_DATABASE_DIR@"; //CMake variable

        fs::path database_path = fs::path(database_dir) / "pkmnsim.db";

        ifstream ifile(database_path.string().c_str());
        if(not ifile) throw runtime_error("Could not find database!");
        else ifile.close();

        return database_path.string();
    }
    
    //Get images directory
    string get_images_dir(void)
    {
        const char* images_dir = getenv("PKMNSIM_IMAGES_DIR"); //Environment variable
        if(images_dir == NULL) images_dir = "@PKMNSIM_IMAGES_DIR@"; //CMake variable
        
        if(not fs::exists(fs::path(images_dir))) throw runtime_error("Could not find images directory!");
        
        return string(images_dir);
    }

    //Get path for system's temporary directory
    string get_tmp_dir(void)
    {
        const char* tmp_dir = getenv("TMP");

        fs::path tmp_dir_path = fs::path(tmp_dir);

        if(not fs::exists(fs::path(tmp_dir))) return "/tmp"; //Fallback
        else return tmp_dir_path.string();
    }
}
