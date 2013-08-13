/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <map>
#include <stdio.h>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "base_pkmn_gen2impl.hpp"
#include "sqlitecpp/SQLiteCPP.h"

namespace fs = boost::filesystem;
using namespace std;

namespace pkmnsim
{
    base_pkmn_gen2impl::base_pkmn_gen2impl(string identifier, int game):
                                           base_pkmn(identifier, game)
    {
        //Get final part of images path
        switch(from_game)
        {
            case Games::GOLD:
                images_game_string = "gold";
                break;
            case Games::SILVER:
                images_game_string = "silver";
                break;
            case Games::CRYSTAL:
                images_game_string = "crystal";
                break;
            default: //It should never get here
                images_game_string = "crystal";
                break;
        }
        
        boost::format png_format("%d.png");
        female_icon_path = male_icon_path; //No gender differences in Generation 2
        female_sprite_path = male_sprite_path; //No gender differences in Generation 2
        male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "generation-2" / images_game_string.c_str() / "shiny" / (png_format % species_id).str()).string();
        female_shiny_sprite_path = male_shiny_sprite_path; //No gender differences in Generation 2
    }

    string base_pkmn_gen2impl::get_info()
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

    string base_pkmn_gen2impl::get_info_verbose()
    {
        string types_str;
        if(type2_id == -1) types_str = database::get_type_name_from_id(type1_id);
        else types_str = database::get_type_name_from_id(type1_id) + "/"
                       + database::get_type_name_from_id(type2_id);

        dict<string, int> stats = get_base_stats();

        string output_string;
        output_string = get_species_name() + " (#" + to_string(species_id) + ")\n"
                      + "Type: " + types_str + "\n"
                      + to_string(get_height()) + " m, " + to_string(get_weight()) + " kg\n"
                      + "Base Stats:\n"
                      + " - HP: " + to_string(stats["HP"]) + "\n"
                      + " - Attack: " + to_string(stats["ATK"]) + "\n"
                      + " - Defense: " + to_string(stats["DEF"]) + "\n"
                      + " - Special Attack: " + to_string(stats["SATK"]) + "\n"
                      + " - Special Defense: " + to_string(stats["SDEF"]) + "\n"
                      + " - Speed: " + to_string(stats["SPD"]);
    
        return output_string;
    }

    dict<string,int> base_pkmn_gen2impl::get_base_stats()
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

    dict<string,int> base_pkmn_gen2impl::get_ev_yields()
    {
        dict<string,int> stats;

        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(pkmn_id) +
                       "AND stat_id IN (1,2,3,4,6)";
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
        return stats;
    }

    double base_pkmn_gen2impl::get_chance_male()
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

    double base_pkmn_gen2impl::get_chance_female()
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

    bool base_pkmn_gen2impl::has_gender_differences(void) {return false;}

    string base_pkmn_gen2impl::get_icon_path(bool is_male)
    {
        //Gender doesn't matter in Gen 2
        return male_icon_path;
    }
    
    string base_pkmn_gen2impl::get_sprite_path(bool is_male, bool is_shiny)
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
