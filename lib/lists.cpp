/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <stdexcept>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/lists.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

#include <pokelib/data_tables.h>

#include "SQLiteCpp/src/SQLiteC++.h"

namespace pkmn
{
    SQLite::Database db(get_database_path().c_str());

    void get_game_list(std::vector<std::string> &game_vec)
    {
        game_vec.clear();

        std::string query_str = "SELECT name FROM version_names";
        SQLite::Statement query(db, query_str.c_str());

        while(query.executeStep())
        {
            std::string game = query.getColumn(0);
            game_vec.push_back(game);
        }
    }

    void get_game_group_list(std::vector<std::string> &game_group_vec)
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

    void get_item_list(std::vector<std::string> &item_vec, unsigned int game)
    {
    }

    void PKMN_API get_pokedex_order(std::vector<std::pair<unsigned int, unsigned int> >& entry_list, unsigned int pokedex_id)
    {
        entry_list.clear();
        std::string query_string(str(boost::format("SELECT species_id,pokedex_number FROM pokemon_dex_numbers WHERE pokedex_id=%d")
                                     % pokedex_id));
        SQLite::Statement query(db, query_string.c_str());

        while(query.executeStep()) entry_list.push_back(std::make_pair(int(query.getColumn(0)), int(query.getColumn(1))));
    }

    void get_pokemon_list(std::vector<std::string> &pokemon_vec, unsigned int game)
    {
    }

    void get_type_list(std::vector<std::string> &type_vec, unsigned int gen)
    {
        type_vec.clear();
    
        std::string query_string = "SELECT name FROM type_names WHERE local_language_id=9";

        SQLite::Statement type_names_query(db, query_string.c_str());
        while(type_names_query.executeStep())
        {
            std::string type = std::string((const char*)type_names_query.getColumn(0));
            if(not (gen == 1 and (type == "Steel" or type == "Dark")) and type != "???" and type != "Shadow")
            {
                type_vec.push_back(type);
            }
        }
    }

    void get_ability_list(std::vector<std::string> &ability_vec, unsigned int gen)
    {
        ability_vec.clear();
    
        std::string query_string = "SELECT id FROM abilities WHERE generation_id<=" + to_string(gen);
        
        SQLite::Statement query(db, query_string.c_str());
        while(query.executeStep())
        {
            query_string = "SELECT name FROM ability_names WHERE local_language_id=9 ability_id=" + to_string(query.getColumn(0));
            ability_vec.push_back(std::string((const char*)db.execAndGet(query_string.c_str())));
        }
    }

    void get_nature_list(std::vector<std::string> &nature_vec)
    {
        nature_vec.clear();

        std::string query_str = "SELECT name FROM nature_names";
        SQLite::Statement query(db, query_str.c_str());

        while(query.executeStep())
        {
            std::string nature = std::string((const char*)query.getColumn(0));
            nature_vec.push_back(nature);
        }
    }
    
    void get_pokemon_of_type(base_pokemon_vector &pkmn_vector, std::string type1, std::string type2, unsigned int gen, bool lax)
    {
        pkmn_vector.clear();

        std::string query_string;
        std::vector<int> applicable_ids;
        int pkmn_id, type1_id, type2_id;

        //Get type IDs
        query_string = "SELECT type_id FROM type_names WHERE name='" + type1 + "'";
        type1_id = int(db.execAndGet(query_string.c_str()));
        if(type2 != "None" and type2 != "Any")
        {
            query_string = "SELECT type_id FROM type_names WHERE name='" + type2 + "'";
            type2_id = int(db.execAndGet(query_string.c_str()));
        }

        if((type2 == "None" or type2 == "Any") and lax)
        {
            //Get IDs of Pokémon
            query_string = "SELECT pokemon_id FROM pokemon_types WHERE type_id=" + to_string(type1_id);
            SQLite::Statement pokemon_types_query(db, query_string.c_str());

            //Get any Pokémon of specified type (by itself or paired with any other)
            while(pokemon_types_query.executeStep())
            {
                pkmn_id = pokemon_types_query.getColumn(0); //pokemon_id

                query_string = "SELECT species_id FROM pokemon WHERE id=" + to_string(pkmn_id);
                int species_id = db.execAndGet(query_string.c_str());

                //Get generation ID to restrict list
                query_string = "SELECT generation_id FROM pokemon_species WHERE id=" + to_string(species_id);
                int generation_id = db.execAndGet(query_string.c_str());
                if(generation_id <= gen)
                {
                    applicable_ids.push_back(pkmn_id);
                }
            }
        }
        else
        {

            //Get IDs of Pokémon matching first type
            std::vector<int> pkmn_ids;
            query_string = "SELECT pokemon_id FROM pokemon_types WHERE type_id=" + to_string(type1_id);
            SQLite::Statement pokemon_types_id_query(db, query_string.c_str());

            while(pokemon_types_id_query.executeStep()) pkmn_ids.push_back(pokemon_types_id_query.getColumn(0));

            std::vector<int> to_erase;
            if(type2 == "None")
            {
                //If only one type is specified, find number of entries with that ID and remove duplicates
                for(unsigned int i = 0; i < pkmn_ids.size(); i++)
                {
                    int pkmn_count = 0; //Number of types Pokémon appears in pokemon_moves
                    query_string = "SELECT type_id FROM pokemon_types WHERE pokemon_id=" + to_string(pkmn_ids[i]);
                    SQLite::Statement inner_query(db, query_string.c_str());
                    while(inner_query.executeStep()) pkmn_count++;

                    if(pkmn_count > 1) to_erase.push_back(i);
                }
            }
            else
            {
                //See if entry exists for other type, add to to_erase if not
                for(unsigned int i = 0; i < pkmn_ids.size(); i++)
                {
                    query_string = "SELECT type_id FROM pokemon_types WHERE pokemon_id=" + to_string(pkmn_ids[i])
                                 + " AND type_id=" + to_string(type2_id);
                    SQLite::Statement inner_query(db, query_string.c_str());
                    if(not inner_query.executeStep()) to_erase.push_back(i);
                }
            }

            //Erase invalid entries
            for(unsigned int i = to_erase.size()-1; i > 0; i--) pkmn_ids.erase(pkmn_ids.begin() + to_erase[i]);
            pkmn_ids.erase(pkmn_ids.begin() + to_erase[0]);

            //Get identifiers for remaining entries
            for(unsigned int i = 0; i < pkmn_ids.size(); i++)
            {
                query_string = "SELECT species_id FROM pokemon WHERE id=" + to_string(pkmn_ids[i]);
                int species_id = db.execAndGet(query_string.c_str());

                query_string = "SELECT identifier FROM pokemon_species WHERE id=" + to_string(species_id);
                std::string pkmn_name = db.execAndGet(query_string.c_str());

                //Get generation ID to restrict list
                query_string = "SELECT generation_id FROM pokemon_species WHERE id=" + to_string(species_id);
                int generation_id = db.execAndGet(query_string.c_str());
                if(generation_id <= gen) applicable_ids.push_back(pkmn_ids[i]); //ID's that apply to final Pokemon
            }
        }

        //base_pokemon now takes a game ID in its constructor instead of a generation, but this
        //function doesn't discriminate between games in the same generation, so this array
        //guarantees that the given generation will use a game in that generation
        int game_id_from_gen[] = {0,1,4,7,13,17};
        
        for(unsigned int i = 0; i < applicable_ids.size(); i++)
        {
            //Manually correct for Magnemite and Magneton in Gen 1
            int final_species_id = database::get_species_id(applicable_ids[i]);
            if(not ((database::get_species_name(final_species_id) == "Magnemite" or
                     database::get_species_name(final_species_id) == "Magneton") and gen == 1))
            {
                base_pokemon::sptr b_pkmn = base_pokemon::make(database::get_species_id(applicable_ids[i]), game_id_from_gen[gen]);
                b_pkmn->repair(applicable_ids[i]);
                pkmn_vector.push_back(b_pkmn);
            }
        }
    }
} /* namespace pkmn */
