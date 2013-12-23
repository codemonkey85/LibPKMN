/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdio>

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
    base_pokemon_gen1impl::base_pokemon_gen1impl(unsigned int id, unsigned int game_id):
                                           base_pokemon_impl(id, game_id)
    {
        //Get final part of images path
        switch(_game_id)
        {
            case Games::RED:
            case Games::BLUE:
                _images_game_string = "red-blue";
                break;
            case Games::YELLOW:
                _images_game_string = "yellow";
                break;
            default: //It should never get here
                _images_game_string = "yellow";
                break;
        }

        std::string basename = to_string(_species_id) + ".png";
        std::string icons = fs::path(fs::path(get_images_dir()) / "pokemon-icons").string();
        std::string sprites = fs::path(fs::path(get_images_dir()) / "generation-1"
                                     / _images_game_string).string();
        std::string s_sprites = sprites;

        fs::path images_path = fs::path(get_images_dir());

        switch(id)
        {   
            case Species::NONE: //None, should only be used for empty slots at end of party
                SET_POKEBALL_IMAGE();
                break;
            
            case Species::INVALID: //Invalid, aka Missingno.
                SET_SUBSTITUTE_IMAGE();
                break;
            
            default:
                SET_IMAGES_PATHS(basename);

                /*
                 * Special is exclusive to Generation 1, so query it separately
                 * from the rest.
                 */
                string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id="
                                    + to_string(_pokemon_id) + " AND stat_id=9";
                _special = int(_db.execAndGet(query_string.c_str()));
                break;
        }
    }

    dict<unsigned int, unsigned int> base_pokemon_gen1impl::get_base_stats() const
    {
        dict<unsigned int, unsigned int> stats;
        stats[Stats::HP] = _hp;
        stats[Stats::ATTACK] = _attack;
        stats[Stats::DEFENSE] = _defense;
        stats[Stats::SPEED] = _speed;
        stats[Stats::SPECIAL] = _special;
        
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
        return _male_icon_path;
    }
    
    //No genders or shininess in Generation 1
    string base_pokemon_gen1impl::get_sprite_path(bool is_male, bool is_shiny) const
    {
        return _male_sprite_path;
    }

    //No forms in Generation 1    
    void base_pokemon_gen1impl::set_form(unsigned int form) {};
    void base_pokemon_gen1impl::set_form(std::string form) {};
    void base_pokemon_gen1impl::repair(unsigned int id) {};
    string base_pokemon_gen1impl::get_form_name() const {return get_species_name();}
    unsigned int base_pokemon_gen1impl::get_form_id() const {return _species_id;}

    //No eggs in Generation 1
    void base_pokemon_gen1impl::get_egg_group_names(std::vector<std::string>
                                                    &egg_group_name_vec) const {};
    void base_pokemon_gen1impl::get_egg_group_ids(std::vector<unsigned int>
                                                  &egg_group_id_vec) const {};

} /* namespace pkmnsim */
