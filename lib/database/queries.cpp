/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
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
#include <pkmn/types/shared_ptr.hpp>

#include "../SQLiteCpp/SQLiteC++.h"

#define CONNECT_TO_DB() if(!db) db = pkmn::shared_ptr<SQLite::Database>(new SQLite::Database(get_database_path().c_str()));
#define THROW_QUERY_ERROR() throw std::runtime_error(str(boost::format("%s: Invalid query \"%s\"") \
                                                      % __FUNCTION__ % query_stream.str().c_str()));

#define GET_NUM(query) if(query.executeStep()) return query.getColumn(0); \
                       else THROW_QUERY_ERROR();

#define GET_STRING(query) if(query.executeStep()) \
                          { \
                              std::string entry = (const char*)query.getColumn(0); \
                              std::string s; \
                              std::istringstream iss(entry); \
                              entry = ""; \
                              while(iss >> s) \
                              { \
                                  if (entry != "") entry += " " + s; \
                                  else entry = s; \
                              } \
                              return entry; \
                          } \
                          else THROW_QUERY_ERROR();

namespace pkmn
{
    namespace database
    {
        static pkmn::shared_ptr<SQLite::Database> db;

        //Transform identifier strings into proper format for database
        static std::string to_database_format(std::string identifier)
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

        unsigned int get_ability_id(const std::string &ability_name)
        {
            CONNECT_TO_DB();
            if(ability_name == "None") return Abilities::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT ability_id FROM ability_names WHERE name='" << ability_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        std::string get_ability_name(const unsigned int ability_id)
        {
            CONNECT_TO_DB();
            if(ability_id == Abilities::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM ability_names WHERE local_language_id=9 AND ability_id=" << ability_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_STRING(query);
        }

        std::string get_ability_description(const unsigned int ability_id, const unsigned int version_id)
        {
            CONNECT_TO_DB();
            if(ability_id == Abilities::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT flavor_text FROM ability_flavor_text WHERE language=9 AND ability_id=" << ability_id
                         << " AND version_group_id=" << get_version_group_id(version_id);
            SQLite::Statement query(*db, query_stream.str().c_str());
        }

        std::string get_ability_description(const std::string &ability_name, const std::string &version_name)
        {
            return get_ability_description(get_ability_id(ability_name), get_version_id(version_name));
        }

        unsigned int get_egg_group_id(const std::string &egg_group_id)
        {
            CONNECT_TO_DB();
            if(egg_group_id == Egg_Groups::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT egg_group_id FROM egg_group_prose WHERE name='" << egg_group_id << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        std::string get_egg_group_name(const unsigned int egg_group_id)
        {
            CONNECT_TO_DB();
            if(egg_group_id == Egg_Groups::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM egg_group_prose WHERE egg_group_id=" << egg_group_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_STRING(query);
        }

        unsigned int get_generation(const unsigned int version_id)
        {
            CONNECT_TO_DB();
            if(version_id == Versions::NONE) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT generation_id FROM version_groups WHERE version_group_id="
                         << get_version_group_id(version_id);
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        unsigned int get_generation(const std::string &version_name)
        {
            return get_generation(get_version_id(version_name));
        }

        unsigned int get_item_id(const std::string &item_name)
        {
            CONNECT_TO_DB();
            if(item_name == "None") return Items::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT item_id FROM item_names WHERE name='" << item_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        std::string get_item_name(const unsigned int item_id)
        {
            CONNECT_TO_DB();
            if(item_id == Items::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM item_names WHERE local_language_id=9 AND item_id=" << item_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_STRING(query);
        }

        std::string get_item_description(const unsigned int item_id, const unsigned int version_id)
        {
            CONNECT_TO_DB();
            if(item_id == Abilities::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT flavor_text FROM item_flavor_text WHERE language=9 AND ability_id=" << item_id
                         << " AND version_group_id=" << get_version_group_id(version_id);
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_STRING(query);
        }

        std::string get_item_description(const std::string &item_name, const std::string &version_name)
        {
            return get_item_description(get_item_id(item_name), get_version_id(version_name));
        }

        unsigned int get_item_game_index(const unsigned int item_id, const unsigned int version_id)
        {
            CONNECT_TO_DB();
            item(item_id == Items::NONE or version_id == Versions::NONE) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT game_index FROM item_game_indices WHERE item_id=" << item_id <<
                         << " AND generation_id=" << get_generation(version_id);
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        unsigned int get_item_game_index(const std::string &item_name, const std::string &version_name)
        {
            return get_item_game_index(get_item_id(item_name), get_verson_id(version_name));
        }

        unsigned int get_move_id(const std::string &move_name)
        {
            CONNECT_TO_DB();
            if(move_name == "None") return Moves::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT move_id FROM move_names WHERE name='" << move_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        std::string get_move_name(const unsigned int move_id)
        {
            CONNECT_TO_DB();
            if(move_id == Moves::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM move_names WHERE local_language_id=9 AND move_id=" << move_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_STRING(query);
        }

        std::string get_move_description(const unsigned int move_id, const unsigned int version_id)
        {
            CONNECT_TO_DB();
            if(move_id == Abilities::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT flavor_text FROM move_flavor_text WHERE language=9 AND ability_id=" << move_id
                         << " AND version_group_id=" << get_version_group_id(version_id);
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_STRING(query);
        }

        std::string get_move_description(const std::string &move_name, const std::string &version_name)
        {
            return get_move_description(get_move_id(move_name), get_version_id(version_name));
        }

        unsigned int get_nature_id(const std::string &nature_name)
        {
            CONNECT_TO_DB();
            if(nature_name == "None") return Moves::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT nature_id FROM nature_names WHERE name='" << nature_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        std::string get_nature_name(const unsigned int nature_id)
        {
            CONNECT_TO_DB();
            if(nature_id == Natures::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM nature_names WHERE local_language_id=9 AND nature_id=" << nature_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_STRING(query);
        }

        double get_nature_stat_mod(const unsigned int nature_id, const unsigned int stat_id)
        {
            CONNECT_TO_DB();
            if(nature_id == Natures::NONE or stat_id == Stats::NONE) return 1.0;

            std::ostringstream query_stream;
            query_stream << "SELECT decreased_stat_id,increased_stat_id FROM natures WHERE nature_id=" << nature_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            if(not query.executeStep()) THROW_QUERY_ERROR();

            if(int(query.getColumn(0)) == stat_id) return 0.9;
            else if(int(query.getColumn(1)) == stat_id) return 1.1;
            else return 1.0;
        }

        double get_nature_stat_mod(const std::string &nature_name, const std::string &stat_name)
        {
            return get_nature_stat_mod(get_nature_id(nature_name), get_stat_id(stat_name));
        }

        std::string get_pokedex_entry(const unsigned int species_id, const unsigned int version_id)
        {
            CONNECT_TO_DB();
            if(species_id == Species::NONE or version_id == Versions::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT flavor_text FROM pokemon_species_flavor_text WHERE language_id=9 AND species_id=" << species_id
                         << " AND version_id=" << version_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_STRING(query);
        }

        std::string get_pokedex_entry(const std::string &species_name, const std::string &version_name)
        {
            return get_pokedex_entry(get_species_id(species_name), get_version_id(version_name));
        }

        unsigned int get_pokemon_id(const unsigned int game_index, const unsigned int version_id)
        {
            CONNECT_TO_DB();
            if(version_id == Versions::NONE) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT pokemon_id FROM pokemon_game_indices WHERE version_id=" << version_id
                         << " AND game_index=" << game_index;
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        unsigned int get_pokemon_id(const unsigned int game_index, const std::string &version_name)
        {
            return get_pokemon_id(game_index, get_version_id(version_name));
        }

        unsigned int get_pokemon_id(const unsigned int form_id)
        {
            CONNECT_TO_DB();
            if(form_id == 0) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT pokemon_id FROM pokemon_forms WHERE id=" << form_id;
            SQLite::Statement(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        unsigned int get_pokemon_game_index(const unsigned int pokemon_id, const unsigned int version_id)
        {
            CONNECT_TO_DB();
            if(version_id == Versions::NONE) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT game_index FROM pokemon_game_indices WHERE pokemon_id=" << pokemon_id
                         << " AND version_id=" << version_id;
            SQlite::Statement(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        unsigned int get_pokemon_game_index(const unsigned int pokemon_id, const std::string &version_name)
        {
            return get_pokemon_game_index(pokemon_id, get_version_id(version_name));
        }

        unsigned int get_species_id(const std::string &species_name)
        {
            CONNECT_TO_DB();
            if(species_name == "None") return Species::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT pokemon_species_id FROM pokemon_species_names WHERE name='" << species_name << "'";
            SQLite::Statement(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        unsigned int get_species_id(const unsigned int pokemon_id)
        {
            CONNECT_TO_DB();
            if(pokemon_id == 0) return Species::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT species_id FROM pokemon WHERE id=" << pokemon_id;
            SQLite::Statement(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        std::string get_species_name(const unsigned int species_id)
        {
            CONNECT_TO_DB();
            if(species_id == Versions::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM pokemon_species_names WHERE local_language_id=9 AND pokemon_species_id=" << species_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_STRING(query);
        }

        unsigned int get_type_id(const std::string &type_name)
        {
            CONNECT_TO_DB();
            if(type_name == "None") return Types::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT type_id FROM type_names WHERE local_language_id=9 AND name='" << type_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        std::string get_type_name(const unsigned int type_id)
        {
            CONNECT_TO_DB();
            if(type_id == Versions::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM type_names WHERE local_language_id=9 AND type_id=" << type_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_STRING(query);
        }

        unsigned int get_version_id(const std::string &version_name)
        {
            CONNECT_TO_DB();
            if(version_name == "None") return Versions::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT version_id FROM version_names WHERE name='" << version_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        std::string get_version_name(const unsigned int version_id)
        {
            CONNECT_TO_DB();
            if(version_id == Versions::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM version_names WHERE local_language_id=9 AND version_id=" << version_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_STRING(query);
        }

        unsigned int get_version_group_id(const unsigned int version_id)
        {
            CONNECT_TO_DB();
            if(version_id == Versions::NONE) return Version_Groups::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT version_group_id FROM versions WHERE version_id=" << version_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            GET_NUM(query);
        }

        unsigned int get_version_group_id(const std::string &version_name)
        {
            return get_version_group_id(get_version_id(version_name));
        }

        std::string get_version_group_name(const unsigned int version_group_id)
        {
            CONNECT_TO_DB();
            if(version_group_id == Version_Groups::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM version_names WHERE local_language_id=9 AND version_id IN "
                         << "(SELECT id FROM versions WHERE version_group_id=" << version_group_id << ")";
            SQLite::Statement query(*db, query_stream.str().c_str());
            std::string output;

            if(query.executeStep()) output = (const char*)(query.executeStep());
            else THROW_QUERY_ERROR();

            while(query.executeStep())
            {
                output += "/";
                output += (const char*)(query.executeStep());
            }

            return output;
        }
    }
}
