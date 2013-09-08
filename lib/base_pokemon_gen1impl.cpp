/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <stdio.h>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "base_pokemon_gen1impl.hpp"
#include "sqlitecpp/SQLiteCPP.h"

namespace fs = boost::filesystem;
using namespace std;

namespace pkmnsim
{
    base_pokemon_gen1impl::base_pokemon_gen1impl(int id, int game):
                                           base_pokemon(id, game)
    {
        //Get final part of images path
        switch(from_game)
        {
            case Games::RED:
            case Games::BLUE:
                images_game_string = "red-blue";
                break;
            case Games::YELLOW:
                images_game_string = "yellow";
                break;
            default: //It should never get here
                images_game_string = "yellow";
                break;
        }
    
        boost::format png_format("%d.png");
        
        male_icon_path = fs::path(fs::path(get_images_dir()) / "pokemon-icons" / (png_format % species_id).str()).string();
        female_icon_path = male_icon_path; //No genders in Generation 1
        male_sprite_path = fs::path(fs::path(get_images_dir()) / "generation-1" / images_game_string.c_str() / (png_format % species_id).str()).string();
        female_sprite_path = male_sprite_path; //No genders in Generation 1
        male_shiny_sprite_path = male_sprite_path; //No shininess in Generation 1
        female_shiny_sprite_path = male_sprite_path; //No shininess in Generation 1
        
        repair(pkmn_id);
    }

    string base_pokemon_gen1impl::get_info()
    {
        string types_str;
        if(type2_id == -1) types_str = database::get_type_name_from_id(type1_id);
        else types_str = database::get_type_name_from_id(type1_id) + "/"
                       + database::get_type_name_from_id(type2_id);

        dict<string, int> stats = get_base_stats();

        string stats_str = to_string(stats["HP"]) + ", " + to_string(stats["ATK"]) + ", "
                         + to_string(stats["DEF"]) + ", " + to_string(stats["SPD"]) + ", "
                         + to_string(stats["SPCL"]);

        string output_string;
        output_string = get_species_name() + " (#" + to_string(species_id) + ")\n"
                      + "Type: " + types_str + "\n"
                      + "Stats: " + stats_str;

        return output_string;
    }

    string base_pokemon_gen1impl::get_info_verbose()
    {
        string types_str;
        if(type2_id == -1) types_str = database::get_type_name_from_id(type1_id);
        else types_str = database::get_type_name_from_id(type1_id) + "/"
                       + database::get_type_name_from_id(type2_id);

        dict<string, int> stats = get_base_stats();

        string output_string;
        output_string = get_species_name() + " (#" + to_string(species_id) + ")\n"
                      //+ species + " Pokémon\n"
                      + "Type: " + types_str + "\n"
                      + to_string(get_height()) + " m, " + to_string(get_weight()) + " kg\n"
                      + "Base Stats:\n"
                      + " - HP: " + to_string(stats["HP"]) + "\n"
                      + " - Attack: " + to_string(stats["ATK"]) + "\n"
                      + " - Defense: " + to_string(stats["DEF"]) + "\n"
                      + " - Speed: " + to_string(stats["SPD"]) + "\n"
                      + " - Special: " + to_string(stats["SPCL"]);

        return output_string;
    }

    dict<string,int> base_pokemon_gen1impl::get_base_stats()
    {
        dict<string,int> stats;

        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(pkmn_id) +
                       " AND stat_id IN (1,2,3,6,9)";
        SQLite::Statement stats_query(db, query_string.c_str());

        stats_query.executeStep();
        stats["HP"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["ATK"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["DEF"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["SPD"] = stats_query.getColumn(0);
        stats_query.executeStep();
        stats["SPCL"] = stats_query.getColumn(0);
        return stats;
    }

    dict<string,int> base_pokemon_gen1impl::get_ev_yields()
    {
        //In Generation 1, EV yields were the same as the corresponding base stat
        return get_base_stats();
    }

    bool base_pokemon_gen1impl::has_gender_differences(void) {return false;}

    string base_pokemon_gen1impl::get_icon_path(bool is_male)
    {
        //Gender doesn't matter in Gen 1
        return male_icon_path;
    }
    
    string base_pokemon_gen1impl::get_sprite_path(bool is_male, bool is_shiny)
    {
        //Gender and shininess don't matter in Gen 1
        return male_sprite_path;
    }
}
