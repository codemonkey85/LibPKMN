/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <stdio.h>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

#include "base_pokemon_gen1impl.hpp"

namespace fs = boost::filesystem;
using namespace std;

namespace pkmnsim
{
    base_pokemon_gen1impl::base_pokemon_gen1impl(unsigned int id, unsigned int game):
                                           base_pokemon_impl(id, game)
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
        std::string icon_directory = fs::path(fs::path(get_images_dir()) / "pokemon-icons").string();
        std::string sprite_directory = fs::path(fs::path(get_images_dir()) / "generation-1"
                                     / images_game_string).string();
        switch(id)
        {   
            case Species::NONE: //None, should only be used for empty slots at end of party
                male_icon_path = fs::path(fs::path(get_images_dir())
                               / "misc" / "pokeball.png").string();
                female_icon_path = male_icon_path;
                male_sprite_path = fs::path(fs::path(get_images_dir())
                                 / "misc" / "pokeball.png").string();
                female_sprite_path = female_icon_path;
                male_shiny_sprite_path = male_sprite_path;
                female_shiny_sprite_path = female_sprite_path;
                break;
            
            case Species::INVALID: //Invalid, aka Missingno.
                male_icon_path = fs::path(fs::path(sprite_directory)
                               / (png_format % "substitute.png").str()).string();
                female_icon_path = male_icon_path;
                male_sprite_path = fs::path(fs::path(sprite_directory)
                                 / (png_format % "substitute.png").str()).string();
                female_sprite_path = female_icon_path;
                male_shiny_sprite_path = male_sprite_path;
                female_shiny_sprite_path = female_sprite_path;
                break;
            
            default:
                //No genders in Generation 1
                male_icon_path = fs::path(fs::path(icon_directory)
                               / (png_format % species_id).str()).string();
                female_icon_path = male_icon_path;

                male_sprite_path = fs::path(fs::path(sprite_directory)
                                 / (png_format % species_id).str()).string();
                female_sprite_path = male_sprite_path;

                //No shininess in Generation 1
                male_shiny_sprite_path = male_sprite_path;
                female_shiny_sprite_path = male_sprite_path;

                /*
                 * Special is exclusive to Generation 1, so query it separately
                 * from the rest.
                 */
                SQLite::Database db(get_database_path().c_str());
                string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id="
                                    + to_string(pokemon_id) + " AND stat_id=9";
                special = int(db.execAndGet(query_string.c_str()));
                break;
        }
    }

    string base_pokemon_gen1impl::get_info() const
    {
        switch(species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return "No info";

            default:
                string types_str;
                if(type2_id == Types::NONE) types_str = database::get_type_name(type1_id);
                else types_str = database::get_type_name(type1_id) + "/"
                               + database::get_type_name(type2_id);

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
    }

    string base_pokemon_gen1impl::get_info_verbose() const
    {
        switch(species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return "No info";

            default:
                string types_str;
                if(type2_id == Types::NONE) types_str = database::get_type_name(type1_id);
                else types_str = database::get_type_name(type1_id) + "/"
                               + database::get_type_name(type2_id);

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
    }

    dict<unsigned int, unsigned int> base_pokemon_gen1impl::get_base_stats() const
    {
        dict<unsigned int, unsigned int> stats;
        stats[Stats::HP] = hp;
        stats[Stats::ATTACK] = attack;
        stats[Stats::DEFENSE] = defense;
        stats[Stats::SPEED] = speed;
        stats[Stats::SPECIAL] = special;
        
        return stats;
    }

    //In Generation 1, EV yields were the same as the corresponding base stat
    dict<unsigned int, unsigned int> base_pokemon_gen1impl::get_ev_yields() const
    {
        return get_base_stats();
    }

    //No genders in Generation 1
    double base_pokemon_gen1impl::get_chance_male() const {return 0.0;}
    double base_pokemon_gen1impl::get_chance_female() const {return 0.0;}
    bool base_pokemon_gen1impl::has_gender_differences() const {return false;}

    //No abilities in Generation 1
    dict<unsigned int, unsigned int> base_pokemon_gen1impl::get_abilities() const
    {
        dict<unsigned int,  unsigned int> abilities;
        return abilities;
    }    
    
    //No genders in Generation 1
    string base_pokemon_gen1impl::get_icon_path(bool is_male) const
    {
        return male_icon_path;
    }
    
    //No genders or shininess in Generation 1
    string base_pokemon_gen1impl::get_sprite_path(bool is_male, bool is_shiny) const
    {
        return male_sprite_path;
    }

    //No forms in Generation 1    
    void base_pokemon_gen1impl::set_form(unsigned int form) {};
    void base_pokemon_gen1impl::set_form(std::string form) {};
    void base_pokemon_gen1impl::repair(unsigned int id) {};
    string base_pokemon_gen1impl::get_form_name() const {return get_species_name();}
    unsigned int base_pokemon_gen1impl::get_form_id() const {return species_id;}

    //No eggs in Generation 1
    vector<string> base_pokemon_gen1impl::get_egg_group_names() const
    {
        vector<string> egg_group_vec;
        return egg_group_vec;
    }

    //No eggs in Generation 1
    vector<unsigned int> base_pokemon_gen1impl::get_egg_group_ids() const
    {
        vector<unsigned int> egg_group_vec;
        return egg_group_vec;
    }

} /* namespace pkmnsim */
