/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lambda/lambda.hpp>

#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "../sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    namespace database
    {
        //Transform identifier strings into proper format for database
        string to_database_format(string identifier)
        {
            //Some manual changes
            if(identifier == "Farfetch'd") identifier = "farfetchd";
            else if(identifier == "Mr. Mime") identifier = "mr-mime";
            else if(identifier == "Mime Jr.") identifier = "mime-jr";
            else if(identifier == "Exp. Share") identifier = "exp-share";

            transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);
            replace(identifier.begin(), identifier.end(), ' ', '-');

            return identifier;
        }

        int get_ability_id_from_name(string ability_name)
        {
            SQLite::Database db(get_database_path().c_str());
            string query_string = str(boost::format("SELECT ability_id FROM ability_names WHERE name='%s'") % ability_name);
            return db.execAndGet(query_string.c_str());
        }

        string get_ability_name_from_id(int ability_id)
        {
            SQLite::Database db(get_database_path().c_str());
            string query_string = str(boost::format("SELECT name FROM ability_names WHERE ability_id=%d AND local_language_id=9")
                                                    % ability_id);
            return db.execAndGetStr(query_string.c_str(), "ability_id");
        }

        int get_move_id_from_name(string move_name)
        {
            SQLite::Database db(get_database_path().c_str());
            string query_string = str(boost::format("SELECT move_id FROM move_names WHERE name='%s'") % move_name);
            return db.execAndGet(query_string.c_str());
        }

        string get_move_name_from_id(int move_id)
        {
            SQLite::Database db(get_database_path().c_str());
            string query_string = str(boost::format("SELECT name FROM move_names WHERE move_id=%d AND local_language_id=9")
                                                    % move_id);
            return db.execAndGetStr(query_string.c_str(), "move_id");
        }

        int get_nature_id_from_name(string nature_name)
        {
            SQLite::Database db(get_database_path().c_str());
            string query_string = str(boost::format("SELECT nature_id FROM nature_names WHERE name='%s'") % nature_name);
            return db.execAndGet(query_string.c_str());
        }

        string get_nature_name_from_id(int nature_id)
        {
            SQLite::Database db(get_database_path().c_str());
            string query_string = str(boost::format("SELECT name FROM nature_names WHERE nature_id=%d AND local_language_id=9")
                                                    % nature_id);
            return db.execAndGetStr(query_string.c_str(), "nature_id");
        }

        string get_pokedex_entry(int species_id, int version)
        {
            //Some enum values need to be translated over to database ID's
            int to_database_id[] = {1,2,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,21,22};

            SQLite::Database db(get_database_path().c_str());
            string query_string = str(boost::format("SELECT flavor_text FROM pokemon_species_flavor_text WHERE species_id=%d AND version_id=%d AND language_id=9")
                                                    % species_id % to_database_id[version]);

            string entry = db.execAndGetStr(query_string.c_str(), "flavor_text");
            string s;
            istringstream iss(entry);
            entry = "";
            while(iss >> s)
            {
                if (entry != "") entry += " " + s;
                else entry = s;
            }

            return entry;
        }

        string get_species_name_from_id(int species_id)
        {
            SQLite::Database db(get_database_path().c_str());
            string query_string = str(boost::format("SELECT name FROM pokemon_species_names WHERE pokemon_species_id=%d AND local_language_id=9")
                                                    % species_id);
            return db.execAndGetStr(query_string.c_str(), "pokemon_species_id");
        }

        int get_type_id_from_name(string type_name)
        {
            SQLite::Database db(get_database_path().c_str());
            string query_string = str(boost::format("SELECT type_id FROM type_names WHERE name='%s'") % type_name);
            return db.execAndGet(query_string.c_str());
        }

        string get_type_name_from_id(int type_id)
        {
            SQLite::Database db(get_database_path().c_str());
            string query_string = str(boost::format("SELECT name FROM type_names WHERE type_id=%d AND local_language_id=9") % type_id);
            return db.execAndGetStr(query_string.c_str(), "type_id");
        }
    }
}
