/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <map>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include <sqlitecpp/SQLiteCPP.h>

#include "base_pokemon_gen2impl.hpp"

namespace fs = boost::filesystem;
using namespace std;

namespace pkmnsim
{
    base_pokemon_gen2impl::base_pokemon_gen2impl(unsigned int id, unsigned int game):
                                           base_pokemon_impl(id, game)
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
        
        male_icon_path = fs::path(fs::path(get_images_dir()) / "pokemon-icons" / (png_format % species_id).str()).string();
        female_icon_path = male_icon_path; //No gender differences in Generation 2
        male_sprite_path = fs::path(fs::path(get_images_dir()) / "generation-2" / images_game_string.c_str() / (png_format % species_id).str()).string();
        female_sprite_path = male_sprite_path; //No gender differences in Generation 2
        male_shiny_sprite_path = fs::path(fs::path(get_images_dir()) / "generation-2" / images_game_string.c_str() / "shiny" / (png_format % species_id).str()).string();
        female_shiny_sprite_path = male_shiny_sprite_path; //No gender differences in Generation 2
        
        //Even though most attributes are queried from the database when called, stats take a long time when
        //doing a lot at once, so grab these upon instantiation
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(pokemon_id)
                            + " AND stat_id IN (3,5)";
        SQLite::Statement query(db, query_string.c_str());
        query.executeStep();
        special_attack = query.getColumn(0);
        query.executeStep();
        special_defense = query.getColumn(0);
        
        repair(pokemon_id);
    }

    string base_pokemon_gen2impl::get_info() const
    {
        string types_str;
        if(type2_id == Types::NONE) types_str = database::get_type_name_from_id(type1_id);
        else types_str = database::get_type_name_from_id(type1_id) + "/"
                       + database::get_type_name_from_id(type2_id);

        dict<unsigned int, unsigned int> stats = get_base_stats();

        string stats_str = to_string(stats[Stats::HP]) + ", " + to_string(stats[Stats::ATTACK]) + ", "
                         + to_string(stats[Stats::DEFENSE]) + ", " + to_string(stats[Stats::SPECIAL_ATTACK]) + ", "
                         + to_string(stats[Stats::SPECIAL_DEFENSE]) + ", " + to_string(stats[Stats::SPEED]);

        string output_string;
        output_string = get_species_name() + " (#" + to_string(species_id) + ")\n"
                      + "Type: " + types_str + "\n"
                      + "Stats: " + stats_str;

        return output_string;
    }

    string base_pokemon_gen2impl::get_info_verbose() const
    {
        string types_str;
        if(type2_id == Types::NONE) types_str = database::get_type_name_from_id(type1_id);
        else types_str = database::get_type_name_from_id(type1_id) + "/"
                       + database::get_type_name_from_id(type2_id);

        dict<unsigned int, unsigned int> stats = get_base_stats();

        string output_string;
        output_string = get_species_name() + " (#" + to_string(species_id) + ")\n"
                      + "Type: " + types_str + "\n"
                      + to_string(get_height()) + " m, " + to_string(get_weight()) + " kg\n"
                      + "Base Stats:\n"
                      + " - HP: " + to_string(stats[Stats::HP]) + "\n"
                      + " - Attack: " + to_string(stats[Stats::ATTACK]) + "\n"
                      + " - Defense: " + to_string(stats[Stats::DEFENSE]) + "\n"
                      + " - Special Attack: " + to_string(stats[Stats::SPECIAL_ATTACK]) + "\n"
                      + " - Special Defense: " + to_string(stats[Stats::SPECIAL_DEFENSE]) + "\n"
                      + " - Speed: " + to_string(stats[Stats::SPEED]);
    
        return output_string;
    }

    dict<unsigned int, unsigned int> base_pokemon_gen2impl::get_base_stats() const
    {
        dict<unsigned int, unsigned int> stats;
        stats[Stats::HP] = hp;
        stats[Stats::ATTACK] = attack;
        stats[Stats::DEFENSE] = defense;
        stats[Stats::SPECIAL_ATTACK] = special_attack;
        stats[Stats::SPECIAL_DEFENSE] = special_defense;
        stats[Stats::SPEED] = speed;
        
        return stats;
    }

    dict<unsigned int, unsigned int> base_pokemon_gen2impl::get_ev_yields() const
    {
        dict<unsigned int, unsigned int> stats = get_base_stats();
        dict<unsigned int, unsigned int> ev_yields;
        ev_yields[Stats::HP] = stats[Stats::HP];
        ev_yields[Stats::ATTACK] = stats[Stats::ATTACK];
        ev_yields[Stats::DEFENSE] = stats[Stats::DEFENSE];
        ev_yields[Stats::SPEED] = stats[Stats::SPEED];
        ev_yields[Stats::SPECIAL] = stats[Stats::SPECIAL_ATTACK]; //For Gen 1 compatibility
        
        return ev_yields;
    }

    double base_pokemon_gen2impl::get_chance_male() const
    {
        SQLite::Database db(get_database_path().c_str());

        //Gender rates
        map<unsigned int, double> gender_val_map; //Double is percentage male
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

    double base_pokemon_gen2impl::get_chance_female() const
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

    bool base_pokemon_gen2impl::has_gender_differences() const {return false;}

    dict<unsigned int, unsigned int> base_pokemon_gen2impl::get_abilities() const
    {
        dict<unsigned int, unsigned int> abilities;
        return abilities;
    }
    
    string base_pokemon_gen2impl::get_icon_path(bool is_male) const
    {
        //Gender doesn't matter in Gen 2
        return male_icon_path;
    }
    
    string base_pokemon_gen2impl::get_sprite_path(bool is_male, bool is_shiny) const
    {
        //No separate male/female sprites
        if(is_shiny) return male_shiny_sprite_path;
        else return male_sprite_path;
    }
    
    void base_pokemon_gen2impl::set_form(unsigned int form) {};
    void base_pokemon_gen2impl::set_form(std::string form) {};
    void base_pokemon_gen2impl::repair(unsigned int id) {};
    
    vector<string> base_pokemon_gen2impl::get_egg_group_names() const
    {
        vector<string> egg_group_vec;
        return egg_group_vec;
    }
    
    string base_pokemon_gen2impl::get_form_name() const {return get_species_name();}

    vector<unsigned int> base_pokemon_gen2impl::get_egg_group_ids() const
    {
        vector<unsigned int> egg_group_vec;
        return egg_group_vec;
    }

    unsigned int base_pokemon_gen2impl::get_form_id() const {return species_id;}
}
