/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <iostream>
#include "internal.hpp"
#include <pkmnsim/lists.hpp>
#include "sqlitecpp/SQLiteCPP.h"
#include <stdexcept>

using namespace std;

namespace pkmnsim
{
    void get_game_list(vector<string>& game_vec)
    {
        game_vec.clear();

        SQLite::Database db(get_database_path().c_str());
        string query_str = "SELECT name FROM version_names";
        SQLite::Statement query(db, query_str.c_str());

        while(query.executeStep()) game_vec.push_back(query.getColumn(0));
    }

    void get_game_group_list(vector<string>& game_group_vec)
    {
        //Must be done manually, only really used for GamesComboBox
        game_group_vec.clear();

        game_group_vec.push_back("Red/Blue/Green");
        game_group_vec.push_back("Yellow");
        game_group_vec.push_back("Gold/Silver");
        game_group_vec.push_back("Crystal");
        game_group_vec.push_back("Ruby/Sapphire/Emerald");
        game_group_vec.push_back("Fire Red/Leaf Green");
        game_group_vec.push_back("Diamond/Pearl");
        game_group_vec.push_back("Platinum");
        game_group_vec.push_back("Heart Gold/Soul Silver");
        game_group_vec.push_back("Black/White");
        game_group_vec.push_back("Black 2/White 2");
    }

    void get_type_list(vector<string>& type_vec, int gen)
    {
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT name FROM type_names";

        SQLite::Statement type_names_query(db, query_string.c_str());
        while(type_names_query.executeStep())
        {
            string type = type_names_query.getColumn(0);
            if(not (gen == 1 and (type == "Steel" or type == "Dark")) and type != "???" and type != "Shadow")
            {
                type_vec.push_back(type);
            }
        }
    }
}
