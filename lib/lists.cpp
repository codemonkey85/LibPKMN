/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <stdexcept>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <pkmnsim/lists.hpp>
#include <pkmnsim/paths.hpp>

#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    void get_game_list(vector<string>& game_vec)
    {
        game_vec.clear();

        SQLite::Database db(get_database_path().c_str());
        string query_str = "SELECT name FROM version_names";
        SQLite::Statement query(db, query_str.c_str());

        while(query.executeStep())
        {
            string game = query.getColumn(0);
            game_vec.push_back(game);
        }
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

    void get_pokemon_list(vector<string>& pokemon_vec, int game)
    {
        pokemon_vec.clear();

        //Amount of Pokemon in generation correponding to game enum specified
        int bounds[] = {151,151,151,251,251,251,386,386,386,386,386,493,493,493,493,493,
                        649,649,386,386,649,649,151,251};
        bool multiple = false;

        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT id,species_id FROM pokemon WHERE id <= 649";// + to_string(bounds[game]);
        SQLite::Statement query(db, query_string.c_str());

        boost::format form_format("%s (%s)");
        while(query.executeStep())
        {

            int pokemon_id = query.getColumn(0);
            int species_id = query.getColumn(1);

            query_string = "SELECT name FROM pokemon_species_names WHERE local_language_id=9 AND pokemon_species_id=" + to_string(species_id);
            string normal_name = db.execAndGet(query_string.c_str());

            query_string = "SELECT id,form_identifier FROM pokemon_forms WHERE form_identifier!='NULL' AND pokemon_id=" + to_string(pokemon_id);
            SQLite::Statement inner_query(db, query_string.c_str());

            while(inner_query.executeStep())
            {
                multiple = true;

                int form_id = inner_query.getColumn(0);
                string form_identifier = inner_query.getColumn(1);

                query_string = "SELECT form_name FROM pokemon_form_names WHERE local_language_id=9 AND pokemon_form_id=" + to_string(form_id);
                string form_name = db.execAndGet(query_string.c_str());
                vector<string> form_halves;
                boost::split(form_halves, form_name, boost::is_any_of(" "));
                string full_form_name = (form_format % normal_name % form_halves[0]).str();
                pokemon_vec.push_back(full_form_name);

                //TODO: skip forms not in specific games (Arceus ??? isn't in Gen 5, for example)
            }
            if(multiple) multiple = false;
            else pokemon_vec.push_back(normal_name);
        }
    }

    void get_type_list(vector<string>& type_vec, int gen)
    {
        type_vec.clear();
    
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT name FROM type_names WHERE local_language_id=9";

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

    //List functions that pass by value, for SWIG's use
    vector<string> get_game_vec()
    {
        vector<string> game_list;
        get_game_list(game_list);
        return game_list;
    }

    vector<string> get_game_group_vec()
    {
        vector<string> game_group_list;
        get_game_group_list(game_group_list);
        return game_group_list;
    }

    vector<string> get_pokemon_vec(int game)
    {
        vector<string> pokemon_list;
        get_pokemon_list(pokemon_list, game);
        return pokemon_list;
    }

    vector<string> get_type_vec(int gen)
    {
        vector<string> type_list;
        get_type_list(type_list, gen);
        return type_list;
    }
}
