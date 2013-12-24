/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <string>

#include <boost/filesystem.hpp>

#include <pkmnsim/paths.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

namespace fs = boost::filesystem;

namespace pkmnsim
{
    //Get location of APPDATA
    std::string get_appdata_path()
    {
        //Windows
        #ifdef _MSC_VER
        char* appdata_dir;
        size_t len;
        _dupenv_s(&appdata_dir, &len, "APPDATA");
        #else
        const char* appdata_dir = getenv("APPDATA");
        #endif
        if(appdata_dir and fs::exists(fs::path(appdata_dir))) return std::string(appdata_dir);
        
        //Linux
        #ifdef _MSC_VER
        _dupenv_s(&appdata_dir, &len, "APPDATA");
        #else
        appdata_dir = getenv("HOME");
        #endif
        if(appdata_dir and fs::exists(fs::path(appdata_dir))) return std::string(appdata_dir);
        
        //If it's gotten to this point, fall back to the temp directory
        return get_tmp_dir();
    }

    //Get location of database
    std::string get_database_path()
    {
        #ifdef _MSC_VER
        char* database_dir;
        size_t len;
        _dupenv_s(&database_dir, &len, "PKMNSIM_DATABASE_DIR");
        #else
        const char* database_dir = getenv("PKMNSIM_DATABASE_DIR"); //Environment variable
        #endif
        if(database_dir == NULL) database_dir = "@PKMNSIM_DATABASE_DIR@"; //CMake variable

        fs::path database_path = fs::path(database_dir) / "pkmnsim.db";
        if(not fs::exists(database_path)) throw std::runtime_error("Could not find database!");
        else
        {
            /*
             * Use SQLiteCpp to check to see if database is valid.
             * The SQLite::Database constructor will throw an exception if
             * the given filename is not a valid SQLite database.
             */
            try {SQLite::Database db(database_path.string().c_str());}
            catch(std::exception &e) {return ":memory:";}
        }

        return database_path.string();
    }
    
    //Get images directory
    std::string get_images_dir()
    {
        #ifdef _MSC_VER
        char* images_dir;
        size_t len;
        _dupenv_s(&images_dir, &len, "PKMNSIM_IMAGES_DIR");
        #else
        const char* images_dir = getenv("PKMNSIM_IMAGES_DIR"); //Environment variable
        #endif
        if(images_dir == NULL) images_dir = "@PKMNSIM_IMAGES_DIR@"; //CMake variable
        
        if(not fs::exists(fs::path(images_dir))) throw std::runtime_error("Could not find images directory!");
        
        return std::string(images_dir);
    }

    //Get path for system's temporary directory
    std::string get_tmp_dir()
    {
        #ifdef _MSC_VER
        char* tmp_dir;
        size_t len;
        _dupenv_s(&tmp_dir, &len, "TMP");
        #else
        const char* tmp_dir = getenv("TMP");
        #endif

        if(tmp_dir and fs::exists(fs::path(tmp_dir))) return std::string(tmp_dir);
        else return "/tmp";
    }
} /* namespace pkmnsim */
