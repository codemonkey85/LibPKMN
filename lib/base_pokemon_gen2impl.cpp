/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <map>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

#include "base_pokemon_gen2impl.hpp"

namespace fs = boost::filesystem;
using namespace std;

namespace pkmn
{
    base_pokemon_gen2impl::base_pokemon_gen2impl(unsigned int id, unsigned int game):
                                           base_pokemon_impl(id, game)
    {
        //Get final part of images path
        switch(_game_id)
        {
            case Games::GOLD:
                _images_game_string = "gold";
                break;
            case Games::SILVER:
                _images_game_string = "silver";
                break;
            case Games::CRYSTAL:
                _images_game_string = "crystal";
                break;
            default: //It should never get here
                _images_game_string = "crystal";
                break;
        }

        _sprite_dir = fs::path(_images_dir / "generation-2" / _images_game_string);
        _shiny_sprite_dir = _sprite_dir;

        switch(id)
        {
            case Species::NONE: //None, should only be used for empty slots at end of party
                SET_POKEBALL_IMAGE();
                break;

            case Species::INVALID: //Invalid, aka Missingno. equivalents
                SET_SUBSTITUTE_IMAGE();
                break;

            default:
                SET_IMAGES_PATHS(_images_default_basename);

                //Even though most attributes are queried from the database when called, stats take a long time when
                //doing a lot at once, so grab these upon instantiation
                string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(_pokemon_id)
                                    + " AND stat_id IN (3,5)";
                SQLite::Statement query(_db, query_string.c_str());
                query.executeStep();
                _special_attack = int(query.getColumn(0));
                query.executeStep();
                _special_defense = int(query.getColumn(0));
                _use_old_stats();

                repair(_pokemon_id);
                break;
        }

        _form_id = _species_id;
    }

    void base_pokemon_gen2impl::get_egg_groups(std::vector<std::string>& egg_group_vec) const
    {
        std::vector<unsigned int> egg_group_ids;
        get_egg_group_ids(egg_group_ids);

        egg_group_vec.clear();
        for(size_t i = 0; i < egg_group_ids.size(); i++) egg_group_vec.push_back(database::get_egg_group_name(egg_group_ids[i]));
    }

    //No gender differences in Generation 2
    bool base_pokemon_gen2impl::has_gender_differences() const {return false;}

    double base_pokemon_gen2impl::get_chance_male() const
    {
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return 0.0;

            default:
                /*
                 * gender_val_map's keys correspond to how the different
                 * gender rates are represented in the database. The values
                 * are the actual decimal representations of the percentages.
                 */
                map<unsigned int, double> gender_val_map; //Double is percentage male
                gender_val_map[0] = 1.0;
                gender_val_map[1] = 0.875;
                gender_val_map[2] = 0.75;
                gender_val_map[4] = 0.5;
                gender_val_map[6] = 0.25;
                gender_val_map[8] = 0.0;

                string query_string = "SELECT gender_rate FROM pokemon_species WHERE id=" + to_string(_species_id);
                int gender_val = _db.execAndGet(query_string.c_str());

                if(gender_val == -1) return 0.0;
                else return gender_val_map[gender_val];
        }
    }

    double base_pokemon_gen2impl::get_chance_female() const
    {
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return 0.0;

            default:
                /*
                 * gender_val_map's keys correspond to how the different
                 * gender rates are represented in the database. The values
                 * are the actual decimal representations of the percentages.
                 */
                map<int, double> gender_val_map; //Double is percentage male
                gender_val_map[0] = 1.0;
                gender_val_map[1] = 0.875;
                gender_val_map[2] = 0.75;
                gender_val_map[4] = 0.5;
                gender_val_map[6] = 0.25;
                gender_val_map[8] = 0.0;

                string query_string = "SELECT gender_rate FROM pokemon_species WHERE id=" + to_string(_species_id);
                int gender_val = _db.execAndGet(query_string.c_str());

                if(gender_val == -1) return 0.0;
                else return (1.0 - gender_val_map[gender_val]);
        }
    }

    //No abilities in Generation 2
    string_pair_t base_pokemon_gen2impl::get_abilities() const
    {
        string_pair_t abilities;
        abilities.first = "None";
        abilities.second = "None";

        return abilities;
    }

    std::string base_pokemon_gen2impl::get_hidden_ability() const {return "None";}

    dict<std::string, unsigned int> base_pokemon_gen2impl::get_base_stats() const
    {
        dict<std::string, unsigned int> stats;
        stats["HP"] = _hp;
        stats["Attack"] = _attack;
        stats["Defense"] = _defense;
        stats["Special Attack"] = _special_attack;
        stats["Special Defense"] = _special_defense;
        stats["Speed"] = _speed;

        return stats;
    }

    dict<std::string, unsigned int> base_pokemon_gen2impl::get_ev_yields() const
    {
        dict<std::string, unsigned int> stats = get_base_stats();
        dict<std::string, unsigned int> ev_yields;
        ev_yields["HP"] = stats["HP"];
        ev_yields["Attack"] = stats["Attack"];
        ev_yields["Defense"] = stats["Defense"];
        ev_yields["Speed"] = stats["Speed"];

        /*
         * Special was a single stat in Generation 1, but it was separated into
         * Special Attack and Special Defense in Generation 2. For backwards
         * compatibility, Pokemon still have a Special EV, which just matches
         * Pokemon's Special Attack stat.
         */
        ev_yields["Special"] = stats["Special Attack"];

        return ev_yields;
    }

    //No forms in Generation 2 
    void base_pokemon_gen2impl::set_form(unsigned int form) {};
    void base_pokemon_gen2impl::set_form(std::string form) {};
    void base_pokemon_gen2impl::repair(unsigned int id) {};

    //Gender doesn't matter for icons in Generation 2
    string base_pokemon_gen2impl::get_icon_path(bool is_male) const
    {
        return _male_icon_path.string();
    }

    //Gender doesn't matter for sprites in Generation 2
    string base_pokemon_gen2impl::get_sprite_path(bool is_male, bool is_shiny) const
    {
        if(is_shiny) return _male_shiny_sprite_path.string();
        else return _male_sprite_path.string();
    }

    void base_pokemon_gen2impl::get_egg_group_ids(std::vector<unsigned int>& egg_group_id_vec) const
    {
        egg_group_id_vec.clear();

        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                egg_group_id_vec.push_back(pkmn::Species::NONE);
                break;

            default:
                std::string query_string = "SELECT egg_group_id FROM pokemon_egg_groups WHERE species_id="
                                         + to_string(_species_id);
                SQLite::Statement query(_db, query_string.c_str());

                while(query.executeStep()) egg_group_id_vec.push_back(int(query.getColumn(0)));
                break;
        }
    }
} /* namespace pkmn */
