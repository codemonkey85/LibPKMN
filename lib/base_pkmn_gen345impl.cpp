/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <map>

#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "base_pkmn_gen345impl.hpp"
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    base_pkmn_gen345impl::base_pkmn_gen345impl(string identifier, int gen):
                                           base_pkmn(identifier, gen) {}

    string base_pkmn_gen345impl::get_info()
    {
        string types_str;
        if(type2_id == -1) types_str = database::get_type_name_from_id(type1_id);
        else types_str = database::get_type_name_from_id(type1_id) + "/"
                       + database::get_type_name_from_id(type2_id);

        dict<string, int> stats = get_base_stats();

        string stats_str = to_string(stats["HP"]) + ", " + to_string(stats["ATK"]) + ", "
                         + to_string(stats["DEF"]) + ", " + to_string(stats["SATK"]) + ", "
                         + to_string(stats["SDEF"]) + ", " + to_string(stats["SPD"]);

        string output_string;
        output_string = get_species_name() + " (#" + to_string(species_id) + ")\n"
                      + "Type: " + types_str + "\n"
                      + "Stats: " + stats_str;

        return output_string;
    }

    string base_pkmn_gen345impl::get_info_verbose()
    {
        string types_str;
        if(type2_id == -1) types_str = database::get_type_name_from_id(type1_id);
        else types_str = database::get_type_name_from_id(type1_id) + "/"
                       + database::get_type_name_from_id(type2_id);

        dict<string, int> stats = get_base_stats();
        dict<int, string> abilities = get_abilities();

        string abilities_str;
        if(abilities[1] == "None") abilities_str = abilities[0]; //1 ability
        else if(abilities[2] == "None") abilities_str = abilities[0] + ", " + abilities[1]; //2 abilities
        else abilities_str = abilities[0] + ", " + abilities[1] + ", " + abilities[2];

        string output_string;
        output_string = get_species_name() + " (#" + to_string(species_id) + ")\n"
                      + "Type: " + types_str + "\n"
                      + to_string(get_height()) + " m, " + to_string(get_weight()) + " kg\n"
                      + "Abilities: " + abilities_str + "\n"
                      + "Base Stats:\n"
                      + " - HP: " + to_string(stats["HP"]) + "\n"
                      + " - Attack: " + to_string(stats["ATK"]) + "\n"
                      + " - Defense: " + to_string(stats["DEF"]) + "\n"
                      + " - Special Attack: " + to_string(stats["SATK"]) + "\n"
                      + " - Special Defense: " + to_string(stats["SDEF"]) + "\n"
                      + " - Speed: " + to_string(stats["SPD"]);

        return output_string;
    }

    dict<string, int> base_pkmn_gen345impl::get_base_stats()
    {
        dict<string,int> stats;

        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(pkmn_id) +
                       " AND stat_id IN (1,2,3,4,5,6)";
        SQLite::Statement stats_query(db, query_string.c_str());

        stats_query.executeStep();
        stats["HP"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["ATK"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["DEF"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["SATK"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["SDEF"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["SPD"] = stats_query.getColumn(0);
        return stats;
    }

    dict<string, int> base_pkmn_gen345impl::get_ev_yields()
    {
        dict<string, int> stats;

        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT effort FROM pokemon_stats WHERE pokemon_id=" + to_string(pkmn_id) +
                       " AND stat_id IN (1,2,3,4,6)";
        SQLite::Statement stats_query(db, query_string.c_str());

        stats_query.executeStep();
        stats["HP"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["ATK"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["DEF"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["SPCL"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["SPD"] = stats_query.getColumn(0);
        stats_query.executeStep();

        return stats;
    }

    double base_pkmn_gen345impl::get_chance_male()
    {
        SQLite::Database db(get_database_path().c_str());
        
        //Gender rates
        map<int, double> gender_val_map; //Double is percentage male
        gender_val_map[0] = 1.0;
        gender_val_map[1] = 0.875;
        gender_val_map[2] = 0.75;
        gender_val_map[4] = 0.5;
        gender_val_map[6] = 0.25;
        gender_val_map[8] = 0.0;

        string query_string = "SELECT gender_rate FROM pokemon_species WHERE id=" + to_string(species_id);
        int gender_val = db.execAndGet(query_string.c_str(), "gender_rate");

        if(gender_val == -1) return 0.0;
        else return gender_val_map[gender_val];
    }

    double base_pkmn_gen345impl::get_chance_female()
    {
        SQLite::Database db(get_database_path().c_str());

        //Gender rates
        map<int, double> gender_val_map; //Double is percentage male
        gender_val_map[0] = 1.0;
        gender_val_map[1] = 0.875;
        gender_val_map[2] = 0.75;
        gender_val_map[4] = 0.5;
        gender_val_map[6] = 0.25;
        gender_val_map[8] = 0.0;

        string query_string = "SELECT gender_rate FROM pokemon_species WHERE id=" + to_string(species_id);
        int gender_val = db.execAndGet(query_string.c_str(), "gender_rate");

        if(gender_val == -1) return 0.0;
        else return (1.0 - gender_val_map[gender_val]);
    }

    dict<int, string> base_pkmn_gen345impl::get_abilities()
    {
        SQLite::Database db(get_database_path().c_str());
        string ability1, ability2, ability3;

        //Ability 1 (guaranteed)
        string query_string = "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" + to_string(pkmn_id)
                     + " AND slot=1";
        int ability1_id = db.execAndGet(query_string.c_str(), "ability_id");
        query_string = "SELECT name FROM ability_names WHERE ability_id=" + to_string(ability1_id)
                     + " AND local_language_id=9";
        ability1 = db.execAndGetStr(query_string.c_str(), "name");

        //Ability 2 (not guaranteed, and if exists, might not exist in specified generation
        query_string = "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" + to_string(pkmn_id)
                     + " AND slot=2";
        SQLite::Statement ability2_query(db, query_string.c_str());
        if(ability2_query.executeStep()) //Will be false if no entry exists
        {
            int ability2_id = ability2_query.getColumn(0); //ability_id
            query_string = "SELECT generation_id FROM abilities WHERE id=" + to_string(ability2_id);
            int generation_id = db.execAndGet(query_string.c_str(), "generation_id");
            if(generation_id > from_gen) ability2 = "None";
            else
            {
                query_string = "SELECT name FROM ability_names WHERE ability_id=" + to_string(ability2_id)
                             + " AND local_language_id=9";
                ability2 = db.execAndGetStr(query_string.c_str(), "name");
            }
        }
        else ability2 = "None";

        //Ability 3 (hidden ability, only in Gen 5, even then not guaranteed)
        if(from_gen == 5)
        {
            query_string = "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" + to_string(pkmn_id)
                         + " AND slot=3";
            SQLite::Statement ability3_query(db, query_string.c_str());
            if(ability3_query.executeStep()) //Will be false if no entry exists
            {
                int ability3_id = db.execAndGet(query_string.c_str(), "ability_id");
                query_string = "SELECT name FROM ability_names WHERE ability_id=" + to_string(ability3_id)
                             + " AND local_language_id=9";
                ability3 = db.execAndGetStr(query_string.c_str(), "name");
            }
            else ability3 = "None";
        }
        else ability3 = "None";

        dict<int, string> abilities;
        abilities[0] = ability1;
        abilities[1] = ability2;
        abilities[2] = ability3;
        return abilities;
    }

    bool base_pkmn_gen345impl::has_gender_differences(void)
    {
        if(from_gen == 3) return false;
        else
        {
            SQLite::Database db(get_database_path().c_str());
            string query_string = "SELECT has_gender_differences FROM pokemon_species WHERE id=" + to_string(pkmn_id);
            return int(db.execAndGet(query_string.c_str()));
        }
    }

    string base_pkmn_gen345impl::get_sprite_path(bool is_male, bool is_shiny)
    {
        if(is_male)
        {
            if(is_shiny) return male_shiny_sprite_path;
            else return male_sprite_path;
        }
        else
        {
            if(is_shiny) return female_shiny_sprite_path;
            else return female_sprite_path;
        }
    }
}
