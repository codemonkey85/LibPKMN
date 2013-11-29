/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <stdlib.h>
#include <string>

#include <boost/filesystem.hpp>

#include <pkmnsim/paths.hpp>

namespace fs = boost::filesystem;
using namespace std;

namespace pkmnsim
{
    //Get location of APPDATA
    string get_appdata_path()
    {
        //Windows
        #ifdef _MSC_VER
        char* appdata_dir;
        size_t len;
        _dupenv_s(&appdata_dir, &len, "APPDATA");
        #else
        const char* appdata_dir = getenv("APPDATA");
        #endif
        if(appdata_dir and fs::exists(fs::path(appdata_dir))) return string(appdata_dir);
        
        //Linux
        #ifdef _MSC_VER
        _dupenv_s(&appdata_dir, &len, "APPDATA");
        #else
        appdata_dir = getenv("HOME");
        #endif
        if(appdata_dir and fs::exists(fs::path(appdata_dir))) return string(appdata_dir);
        
        //If it's gotten to this point, fall back to the temp directory
        return get_tmp_dir();
    }

    //Get location of database
    string get_database_path()
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

        ifstream ifile(database_path.string().c_str());
        if(not fs::exists(database_path)) throw runtime_error("Could not find database!");
        else ifile.close();

        return database_path.string();
    }
    
    //Get images directory
    string get_images_dir()
    {
        #ifdef _MSC_VER
        char* images_dir;
        size_t len;
        _dupenv_s(&images_dir, &len, "PKMNSIM_IMAGES_DIR");
        #else
        const char* images_dir = getenv("PKMNSIM_IMAGES_DIR"); //Environment variable
        #endif
        if(images_dir == NULL) images_dir = "@PKMNSIM_IMAGES_DIR@"; //CMake variable
        
        if(not fs::exists(fs::path(images_dir))) throw runtime_error("Could not find images directory!");
        
        return string(images_dir);
    }

    //Get path for system's temporary directory
    string get_tmp_dir()
    {
        #ifdef _MSC_VER
        char* tmp_dir;
        size_t len;
        _dupenv_s(&tmp_dir, &len, "TMP");
        #else
        const char* tmp_dir = getenv("TMP");
        #endif

        fs::path tmp_dir_path = fs::path(tmp_dir);

        if(not fs::exists(fs::path(tmp_dir))) return "/tmp"; //Fallback
        else return tmp_dir_path.string();
    }
} /* namespace pkmnsim */