/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

#include "../SQLiteCpp/src/SQLiteC++.h"

using namespace std;

namespace pkmnsim
{
    namespace database
    {
        static SQLite::Database db(get_database_path().c_str());
    
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

        unsigned int get_ability_id(string ability_name)
        {
            string query_string = str(boost::format("SELECT ability_id FROM ability_names WHERE name='%s'") % ability_name);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        string get_ability_name(unsigned int ability_id)
        {
            string query_string = str(boost::format("SELECT name FROM ability_names WHERE ability_id=%d AND local_language_id=9")
                                                    % ability_id);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }

        unsigned int get_damage_class(unsigned int type_id)
        {
            string query_string = str(boost::format("SELECT damage_class_id FROM types WHERE id=%d") % type_id);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }
        
        unsigned int get_egg_group_id(string egg_group_name)
        {
            string query_string = str(boost::format("SELECT egg_group_id FROM egg_group_prose WHERE name='%s'")
                                                    % egg_group_name);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }
        
        string get_egg_group_name(unsigned int egg_group_id)
        {
            string query_string = str(boost::format("SELECT name FROM egg_group_prose WHERE egg_group_id=%d AND local_language_id=9")
                                                    % egg_group_id);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }
        
        unsigned int get_game_id(string game_name)
        {
            string query_string = str(boost::format("SELECT version_id FROM version_names WHERE name='%s'")
                                                    % game_name);
            SQLite::Statement query(db, query_string.c_str());
            
            if(query.executeStep()) return int(query.getColumn(0));
            else return 0;
        }
        
        string get_game_name(unsigned int game_id)
        {
            string query_string = str(boost::format("SELECT name FROM version_names WHERE version_id=%d AND local_language_id=9")
                                                    % game_id);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }
        
        unsigned int get_generation(unsigned int game_id)
        {
            string query_string = "SELECT version_group_id FROM versions WHERE id=" + to_string(game_id);
            SQLite::Statement query(db, query_string.c_str());
            int version_group_id = (query.executeStep()) ? int(query.getColumn(0)) : Games::X;
            
            query_string = "SELECT generation_id FROM version_groups WHERE id=" + to_string(version_group_id);
            SQLite::Statement query2(db, query_string.c_str());
            
            return (query2.executeStep()) ? int(query2.getColumn(0)) : 0;
        }

        unsigned int get_item_index(unsigned int item_id, unsigned int version)
        {
            int gen = get_generation(version);
            
            string query_string = "SELECT game_index FROM item_game_indices WHERE item_id=" + to_string(item_id);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }
        
        unsigned int get_item_index(string item_name, unsigned int version)
        {
            return get_item_index(get_item_id(item_name), version);
        }
        
        string get_item_description(unsigned int item_id, unsigned int version)
        {
            //Main Gen 1 games have no item descriptions, so convert version ID to Stadium 2
            //Convert enums to version_group_id
            if(version < Games::GOLD) version = Games::STADIUM2;
            unsigned int version_group_id = get_version_group(version);
            
            string query_string = str(boost::format("SELECT flavor_text FROM item_flavor_text WHERE item_id=%d AND version_group_id=%d AND language_id=9")
                                                    % item_id % version_group_id);
            SQLite::Statement query(db, query_string.c_str());
                                                    
            string entry = (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
            
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
        
        string get_item_description(string item_name, unsigned int version)
        {
            return get_item_description(get_item_id(item_name), version);
        }
        
        unsigned int get_item_id(string item_name)
        {
            string query_string = str(boost::format("SELECT item_id FROM item_names WHERE name='%s'") % item_name);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        unsigned int get_item_id(unsigned int item_index, unsigned int game)
        {
            string query_string = str(boost::format("SELECT item_id FROM item_game_indices WHERE generation_id=%d AND game_index=%d")
                                      % get_generation(game) % item_index);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }
        
        string get_item_name(unsigned int item_id)
        {
            string query_string = str(boost::format("SELECT name FROM item_names WHERE item_id=%d AND local_language_id=9")
                                                    % item_id);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }
        
        unsigned int get_item_category(unsigned int item_id)
        {
            string query_string = str(boost::format("SELECT category_id FROM items WHERE id=%d") % item_id);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }
        
        unsigned int get_item_category(string item_name)
        {
            return get_item_category(get_item_id(item_name));
        }
        
        string get_move_description(unsigned int move_id, unsigned int version)
        {
            //Main Gen 1 games have no move descriptions, so convert version ID to Stadium 2
            //Convert enums to version_group_id
            if(version < Games::GOLD) version = Games::STADIUM2;
            unsigned int version_group_id = get_version_group(version);
            
            SQLite::Database db(get_database_path().c_str());
            string query_string = str(boost::format("SELECT flavor_text FROM move_flavor_text WHERE move_id=%d AND version_group_id=%d AND language_id=9")
                                                    % move_id % version_group_id);
            SQLite::Statement query(db, query_string.c_str());
                                                    
            string entry = (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
            
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
        
        string get_move_description(std::string move_name, unsigned int version)
        {
            return get_move_description(get_move_id(move_name), version);
        }
        
        unsigned int get_move_id(string move_name)
        {
            string query_string = str(boost::format("SELECT move_id FROM move_names WHERE name='%s'") % move_name);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        string get_move_name(unsigned int move_id)
        {
            string query_string = str(boost::format("SELECT name FROM move_names WHERE move_id=%d AND local_language_id=9")
                                                    % move_id);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }

        double get_nature_stat_effect(unsigned int nature_id, unsigned int stat)
        {
            string query_string = str(boost::format("SELECT * FROM natures WHERE id=%d") % nature_id);
            SQLite::Statement query(db, query_string.c_str());
            
            query.executeStep();
            int increased_id = query.getColumn(2);
            int decreased_id = query.getColumn(3);
            
            if(stat == increased_id) return 1.1;
            else if(stat == decreased_id) return 0.9;
            else return 1.0;
        }
        
        double get_nature_stat_effect(std::string nature_name, unsigned int stat)
        {
            return get_nature_stat_effect(get_nature_id(nature_name), stat);
        }
        
        unsigned int get_nature_id(string nature_name)
        {
            string query_string = str(boost::format("SELECT nature_id FROM nature_names WHERE name='%s'") % nature_name);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        string get_nature_name(unsigned int nature_id)
        {
            string query_string = str(boost::format("SELECT name FROM nature_names WHERE nature_id=%d AND local_language_id=9")
                                                    % nature_id);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }

        string get_pokedex_entry(unsigned int species_id, unsigned int version)
        {
            //Some enum values need to be translated over to database ID's
            int to_database_id[] = {1,2,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,21,22};

            string query_string = str(boost::format("SELECT flavor_text FROM pokemon_species_flavor_text WHERE species_id=%d AND version_id=%d AND language_id=9")
                                                    % species_id % to_database_id[version]);

            SQLite::Statement query(db, query_string.c_str());
                                                    
            string entry = (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
            
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

        string get_pokedex_entry(string species_name, unsigned int version)
        {
            string query_string = str(boost::format("SELECT pokemon_species_id FROM pokemon_species_names WHERE name='%s'")
                                                    % species_name);
            SQLite::Statement query(db, query_string.c_str());
            return (query.executeStep()) ? get_pokedex_entry(int(query.getColumn(0)), version) : "None";
        }

        unsigned int get_pokemon_id(unsigned int species_index, unsigned int version)
        {
            string query_string = str(boost::format("SELECT pokemon_id FROM pokemon_game_indices WHERE version_id=%d AND game_index=%d")
                                                    % version
                                                    % species_index);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : Species::INVALID;
        }
        
        unsigned int get_move_pp(unsigned int move_id)
        {
            string query_string = str(boost::format("SELECT pp FROM moves WHERE id=%d")
                                                    % move_id);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }
        
        unsigned int get_species_id(unsigned int pkmn_id)
        {
            string query_string = str(boost::format("SELECT species_id FROM pokemon WHERE id=%d") % pkmn_id);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        unsigned int get_species_id(string species_name)
        {
            string query_string = str(boost::format("SELECT pokemon_species_id FROM pokemon_species_names WHERE name='%s'") % species_name);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        unsigned int get_species_id(unsigned int species_index, unsigned int version)
        {
            return get_species_id(get_pokemon_id(species_index, version));
        }

        unsigned int get_species_index(unsigned int pokemon_id, unsigned int version)
        {
            string query_string = str(boost::format("SELECT game_index FROM pokemon_game_indices WHERE pokemon_id=%d AND version_id=%d")
                                                    % pokemon_id
                                                    % version);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        string get_species_name(unsigned int species_id)
        {
            string query_string = str(boost::format("SELECT name FROM pokemon_species_names WHERE pokemon_species_id=%d AND local_language_id=9")
                                                    % species_id);
            SQLite::Statement query(db, query_string.c_str());

            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }

        unsigned int get_type_id(string type_name)
        {
            string query_string = str(boost::format("SELECT type_id FROM type_names WHERE name='%s'") % type_name);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        string get_type_name(unsigned int type_id)
        {
            string query_string = str(boost::format("SELECT name FROM type_names WHERE type_id=%d AND local_language_id=9") % type_id);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }
        
        unsigned int get_version_group(unsigned int version_id)
        {
            string query_string = str(boost::format("SELECT version_group_id FROM versions WHERE id=%d") % version_id);
            SQLite::Statement query(db, query_string.c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }
    } /* namespace database */
} /* namespace pkmnsim */
