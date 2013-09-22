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
#include <sqlitecpp/SQLiteCPP.h>

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

        //Even though most attributes are queried from the database when called, stats take a long time when
        //doing a lot at once, so grab these upon instantiation
        SQLite::Database db(get_database_path.c_str());
        query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(pokemon_id) +
                       " AND stat_id=9";
        special = db.execAndGet(query_string.c_str(), "base_stat");
        
        repair(pkmn_id);
    }

    string base_pokemon_gen1impl::get_info()
    {
        string types_str;
        if(type2_id == -1) types_str = database::get_type_name_from_id(type1_id);
        else types_str = database::get_type_name_from_id(type1_id) + "/"
                       + database::get_type_name_from_id(type2_id);

        dict<unsigned int, unsigned int> stats = get_base_stats();

        string stats_str = to_string(stats[Stats::HP]) + ", " + to_string(stats[Stats::ATTACK]) + ", "
                         + to_string(stats[Stats::DEFENSE]) + ", " + to_string(stats[Stats::SPEED]) + ", "
                         + to_string(stats[Stats::SPECIAL]);

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

        dict<unsigned int, unsigned int> stats = get_base_stats();

        string output_string;
        output_string = get_species_name() + " (#" + to_string(species_id) + ")\n"
                      //+ species + " Pokémon\n"
                      + "Type: " + types_str + "\n"
                      + to_string(get_height()) + " m, " + to_string(get_weight()) + " kg\n"
                      + "Base Stats:\n"
                      + " - HP: " + to_string(stats[Stats::HP]) + "\n"
                      + " - Attack: " + to_string(stats[Stats::ATTACK]) + "\n"
                      + " - Defense: " + to_string(stats[Stats::DEFENSE]) + "\n"
                      + " - Speed: " + to_string(stats[Stats::SPEED]) + "\n"
                      + " - Special: " + to_string(stats[Stats::SPECIAL]);

        return output_string;
    }

    dict<unsigned int, unsigned int> base_pokemon_gen1impl::get_base_stats()
    {
        dict<unsigned int, unsigned int> stats;
        stats[Stats::HP] = hp;
        stats[Stats::ATTACK] = attack;
        stats[Stats::DEFENSE] = defense;
        stats[Stats::SPEED] = speed;
        stats[Stats::SPECIAL] = special;
        
        return stats;
    }

    dict<string,int> base_pokemon_gen1impl::get_ev_yields()
    {
        //In Generation 1, EV yields were the same as the corresponding base stat
        return get_base_stats();
    }

    //No genders in Gen 1
    double base_pokemon_gen1impl::get_chance_male() {return 0.0;}
    double base_pokemon_gen1impl::get_chance_female() {return 0.0;}
    
    bool base_pokemon_gen1impl::has_gender_differences() {return false;}

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
    
    void base_pokemon_gen1impl::set_form(unsigned int form) {};
    void base_pokemon_gen1impl::repair(unsigned int id) {};
    string base_pokemon_gen1impl::get_egg_group_name() {return "N/A";}
    string base_pokemon_gen1impl::get_form_name() {return get_species_name();}
    unsigned int base_pokemon_gen1impl::get_egg_group_id() {return Egg_Groups::NONE;}
    unsigned int base_pokemon_gen1impl::get_form_id() {return species_id;}
}
