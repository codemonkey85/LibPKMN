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

namespace pkmn
{
    namespace database
    {
        static SQLite::Database db(get_database_path().c_str());

        //Transform identifier strings into proper format for database
        std::string to_database_format(std::string identifier)
        {
            //Some manual changes
            if(identifier == "Farfetch'd") identifier = "farfetchd";
            else if(identifier == "Mr. Mime") identifier = "mr-mime";
            else if(identifier == "Mime Jr.") identifier = "mime-jr";
            else if(identifier == "Exp. Share") identifier = "exp-share";

            std::transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);
            std::replace(identifier.begin(), identifier.end(), ' ', '-');

            return identifier;
        }

        unsigned int get_ability_id(std::string ability_name)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT ability_id FROM ability_names WHERE name='" << ability_name << "'";
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        std::string get_ability_name(unsigned int ability_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT name FROM ability_names WHERE local_language_id=9 AND ability_id=" << ability_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }

        unsigned int get_damage_class(unsigned int type_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT damage_class_id FROM types WHERE id=" << type_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        unsigned int get_egg_group_id(std::string egg_group_name)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT egg_group_id FROM egg_group_prose WHERE name='" << egg_group_name << "'";
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        std::string get_egg_group_name(unsigned int egg_group_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT name FROM egg_group_prose WHERE local_language_id=9 AND egg_group_id=" << egg_group_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }

        unsigned int get_game_id(std::string game_name)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT version_id FROM version_names WHERE name='" << game_name << "'";
            SQLite::Statement query(db, query_stream.str().c_str());
            
            if(query.executeStep()) return int(query.getColumn(0));
            else return 0;
        }

        std::string get_game_name(unsigned int game_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT name FROM version_names WHERE local_language_id=9 AND version_id=" << game_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }

        unsigned int get_generation(unsigned int game_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT generation_id FROM version_groups WHERE id="
                         << "    (SELECT version_group_id FROM versions WHERE id=" << game_id << ")";
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        unsigned int get_item_index(unsigned int item_id, unsigned int version)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT game_index FROM item_game_indices WHERE item_id=" << item_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        unsigned int get_item_index(std::string item_name, unsigned int version)
        {
            return get_item_index(get_item_id(item_name), version);
        }

        std::string get_item_description(unsigned int item_id, unsigned int version)
        {
            //Main Gen 1 games have no item descriptions, so convert version ID to Stadium 2
            //Convert enums to version_group_id
            if(version < Games::GOLD) version = Games::STADIUM2;
            unsigned int version_group_id = get_version_group(version);

            std::ostringstream query_stream;
            query_stream << "SELECT flavor_text FROM item_flavor_text WHERE language_id=9 AND "
                         << "item_id=" << item_id << " AND version_group_id=" << version_group_id;
            SQLite::Statement query(db, query_stream.str().c_str());

            if(query.executeStep())
            {
                std::string entry = (const char*)query.getColumn(0);

                std::string s;
                std::istringstream iss(entry);
                entry = "";
                while(iss >> s)
                {
                    if (entry != "") entry += " " + s;
                    else entry = s;
                }
                return entry;
            }
            else return "None";
        }

        std::string get_item_description(std::string item_name, unsigned int version)
        {
            return get_item_description(get_item_id(item_name), version);
        }

        unsigned int get_item_id(std::string item_name)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT item_id FROM item_names WHERE name='" << item_name << "'";
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        unsigned int get_item_id(unsigned int item_index, unsigned int game)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT item_id FROM item_game_indices WHERE generation_id="
                         << get_generation(game) << " AND game_index=" << item_index;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        std::string get_item_name(unsigned int item_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT name FROM item_names WHERE local_language_id-9 AND item_id=" << item_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }

        unsigned int get_item_category(unsigned int item_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT category_id FROM items WHERE id=" << item_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        unsigned int get_item_category(std::string item_name)
        {
            return get_item_category(get_item_id(item_name));
        }

        std::string get_move_description(unsigned int move_id, unsigned int version)
        {
            //Main Gen 1 games have no move descriptions, so convert version ID to Stadium 2
            //Convert enums to version_group_id
            if(version < Games::GOLD) version = Games::STADIUM2;
            unsigned int version_group_id = get_version_group(version);

            std::ostringstream query_stream;
            query_stream << "SELECT flavor_text FROM move_flavor_text WHERE language_id=9 AND "
                         << "move_id=" << move_id << " AND version_group_id=" << version_group_id;
            SQLite::Statement query(db, query_stream.str().c_str());

            if(query.executeStep())
            {
                std::string entry = (const char*)query.getColumn(0);
                std::string s;
                std::istringstream iss(entry);
                entry = "";
                while(iss >> s)
                {
                    if (entry != "") entry += " " + s;
                    else entry = s;
                }
                return entry;
            }
            else return "None";
        }

        std::string get_move_description(std::string move_name, unsigned int version)
        {
            return get_move_description(get_move_id(move_name), version);
        }

        unsigned int get_move_id(std::string move_name)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT move_id FROM move_names WHERE name='" << move_name << "'";
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        std::string get_move_name(unsigned int move_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT name FROM move_names WHERE local_language_id=9 AND move_id=" << move_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }

        double get_nature_stat_effect(unsigned int nature_id, unsigned int stat)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT decreased_stat_id,increased_stat_id FROM natures WHERE id=" << nature_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            query.executeStep();
            int decreased_id = query.getColumn(0);
            int increased_id = query.getColumn(1);
            
            if(stat == decreased_id) return 0.9;
            else if(stat == increased_id) return 1.1;
            else return 1.0;
        }

        double get_nature_stat_effect(std::string nature_name, unsigned int stat)
        {
            return get_nature_stat_effect(get_nature_id(nature_name), stat);
        }

        unsigned int get_nature_id(std::string nature_name)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT nature_id FROM nature_names WHERE name='" << nature_name << "'";
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        std::string get_nature_name(unsigned int nature_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT name FROM nature_names WHERE local_language_id=9 AND nature_id=" << nature_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }

        std::string get_pokedex_entry(unsigned int species_id, unsigned int version)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT flavor_text FROM pokemon_species_flavor_text WHERE language_id=9 AND species_id="
                         << species_id << " AND version_id=" << version;
            SQLite::Statement query(db, query_stream.str().c_str());

            if(query.executeStep())
            {
                std::string entry = (const char*)query.getColumn(0);
                std::string s;
                std::istringstream iss(entry);
                entry = "";
                while(iss >> s)
                {
                    if (entry != "") entry += " " + s;
                    else entry = s;
                }
                return entry;
            }
            else return "None";
        }

        std::string get_pokedex_entry(std::string species_name, unsigned int version)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT pokemon_species_id FROM pokemon_species_names WHERE name='" << species_name << "'";
            SQLite::Statement query(db, query_stream.str().c_str());
            return (query.executeStep()) ? get_pokedex_entry(int(query.getColumn(0)), version) : "None";
        }

        unsigned int get_pokemon_id(unsigned int species_index, unsigned int version)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT pokemon_id FROM pokemon_game_indices WHERE version_id=" << version
                         << " AND game_index=" << species_index;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : Species::INVALID;
        }

        unsigned int get_move_pp(unsigned int move_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT pp FROM moves WHERE id=" << move_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        unsigned int get_species_id(unsigned int pokemon_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT species_id FROM pokemon WHERE id=" << pokemon_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        unsigned int get_species_id(std::string species_name)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT pokemon_species_id FROM pokemon_species_names WHERE name='" << species_name << "'";
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        unsigned int get_species_id(unsigned int species_index, unsigned int version)
        {
            return get_species_id(get_pokemon_id(species_index, version));
        }

        unsigned int get_species_index(unsigned int pokemon_id, unsigned int version)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT game_index FROM pokemon_game_indices WHERE pokemon_id=" << pokemon_id
                         << " AND version_id=" << version;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        std::string get_species_name(unsigned int species_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT name FROM pokemon_species_names WHERE local_language_id=9 AND pokemon_species_id=" << species_id;
            SQLite::Statement query(db, query_stream.str().c_str());

            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }

        unsigned int get_type_id(std::string type_name)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT type_id FROM type_names WHERE name='" << type_name << "'";
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }

        std::string get_type_name(unsigned int type_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT name FROM type_names WHERE local_language_id=9 AND type_id=" << type_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";
        }
        
        unsigned int get_version_group(unsigned int version_id)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT version_group_id FROM versions WHERE id=" << version_id;
            SQLite::Statement query(db, query_stream.str().c_str());
            
            return (query.executeStep()) ? int(query.getColumn(0)) : 0;
        }
    } /* namespace database */
} /* namespace pkmn */
