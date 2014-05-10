/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdio>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

#include "base_pokemon_gen1impl.hpp"

namespace fs = boost::filesystem;
using namespace std;

namespace pkmn
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

        _sprite_dir = fs::path(_images_dir / "generation-1" / _images_game_string);
        _shiny_sprite_dir = _sprite_dir;

        switch(id)
        {   
            case Species::NONE: //None, should only be used for empty slots at end of party
                SET_POKEBALL_IMAGE();
                break;

            case Species::INVALID: //Invalid, aka Missingno.
                SET_SUBSTITUTE_IMAGE();
                break;

            default:
                SET_IMAGES_PATHS(_images_default_basename);

                /*
                 * Special is exclusive to Generation 1, so query it separately
                 * from the rest.
                 */
                string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id="
                                    + to_string(_pokemon_id) + " AND stat_id=9";
                _special = int(_db.execAndGet(query_string.c_str()));
                _use_old_stats();
                break;
        }

        _form_id = _species_id;
    }

    //No eggs in Generation 1
    void base_pokemon_gen1impl::get_egg_groups(std::vector<std::string>& egg_group_vec) const
    {
        egg_group_vec.clear();
        egg_group_vec.push_back("None");
    }

    //No genders in Generation 1
    bool base_pokemon_gen1impl::has_gender_differences() const {return false;}
    double base_pokemon_gen1impl::get_chance_male() const {return 0.0;}
    double base_pokemon_gen1impl::get_chance_female() const {return 0.0;}

    //No abilities in Generation 1
    string_pair_t base_pokemon_gen1impl::get_abilities() const
    {
        string_pair_t abilities;
        abilities.first = "None";
        abilities.second = "None";

        return abilities;
    }

    std::string base_pokemon_gen1impl::get_hidden_ability() const {return "None";}

    pkmn::dict<std::string, unsigned int> base_pokemon_gen1impl::get_base_stats() const
    {
        pkmn::dict<std::string, unsigned int> stats;
        stats["HP"] = _hp;
        stats["Attack"] = _attack;
        stats["Defense"] = _defense;
        stats["Speed"] = _speed;
        stats["Special"] = _special;

        return stats;
    }

    //In Generation 1, EV yields were the same as the corresponding base stat
    pkmn::dict<std::string, unsigned int> base_pokemon_gen1impl::get_ev_yields() const
    {
        return get_base_stats();
    }

    //No forms in Generation 1    
    void base_pokemon_gen1impl::set_form(unsigned int form) {};
    void base_pokemon_gen1impl::set_form(std::string form) {};
    void base_pokemon_gen1impl::repair(unsigned int id) {};

    //No genders in Generation 1
    string base_pokemon_gen1impl::get_icon_path(bool is_male) const
    {
        return _male_icon_path.string();
    }

    //No genders or shininess in Generation 1
    string base_pokemon_gen1impl::get_sprite_path(bool is_male, bool is_shiny) const
    {
        return _male_sprite_path.string();
    }

    //No eggs in Generation 1
    void base_pokemon_gen1impl::get_egg_group_ids(std::vector<unsigned int>& egg_group_id_vec) const
    {
        egg_group_id_vec.clear();
        egg_group_id_vec.push_back(pkmn::Egg_Groups::NONE);
    }
} /* namespace pkmn */
