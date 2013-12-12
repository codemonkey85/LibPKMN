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
#include <pkmnsim/lists.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

#include "base_pokemon_modernimpl.hpp"

namespace fs = boost::filesystem;
using namespace std;

namespace pkmnsim
{
    base_pokemon_modernimpl::base_pokemon_modernimpl(unsigned int id, unsigned int game):
                                           base_pokemon_impl(id, game)
    {
        //Get final part of images path
        switch(_generation)
        {
            case Games::RUBY:
            case Games::SAPPHIRE:
                _images_game_string = "ruby-sapphire";
                break;
            case Games::EMERALD:
                _images_game_string = "emerald";
                break;
            case Games::FIRE_RED:
            case Games::LEAF_GREEN:
                _images_game_string = "firered-leafgreen";
                break;
            case Games::DIAMOND:
            case Games::PEARL:
                _images_game_string = "diamond-pearl";
                break;
            case Games::PLATINUM:
                _images_game_string = "platinum";
                break;
            case Games::HEART_GOLD:
            case Games::SOUL_SILVER:
                _images_game_string = "heartgold-soulsilver";
                break;
            default: //Gen 5 all uses black-white
                _images_game_string = "black-white";
                break;
        }
        
        boost::format png_format("%d.png");
        std::string gen_string = "generation-" + to_string(_generation);
        boost::format gen_format("generation-%d");
        std::string icon_directory = fs::path(fs::path(get_images_dir()) / "pokemon-icons").string();
        std::string sprite_directory = fs::path(fs::path(get_images_dir()) / gen_string
                                     / _images_game_string.c_str()).string();
        std::string shiny_sprite_directory = fs::path(fs::path(sprite_directory) / "shiny").string();
        
        
        switch(id)
        {
            case Species::NONE: //None, should only be used for empty slots at end of party
                _male_icon_path = fs::path(fs::path(get_images_dir())
                                / "misc" / "pokeball.png").string();
                _female_icon_path = _male_icon_path;
                _male_sprite_path = fs::path(fs::path(get_images_dir())
                                  / "misc" / "pokeball.png").string();
                _female_sprite_path = _female_icon_path;
                _male_shiny_sprite_path = _male_sprite_path;
                _female_shiny_sprite_path = _female_sprite_path;
                break;

            case Species::INVALID: //Invalid, aka Bad Egg
                _male_icon_path = fs::path(fs::path(sprite_directory)
                                / (png_format % "substitute.png").str()).string();
                _female_icon_path = _male_icon_path;
                _male_sprite_path = fs::path(fs::path(sprite_directory)
                                / (png_format % "substitute.png").str()).string();
                _female_sprite_path = _female_icon_path;
                _male_shiny_sprite_path = _male_sprite_path;
                _female_shiny_sprite_path = _female_sprite_path;
                break;

            default:
                //Unfezant, Frillish and Jellicent have different icons for each gender
                _male_icon_path = fs::path(fs::path(icon_directory)
                                / (png_format % _species_id).str()).string();
                if(_species_id == Species::UNFEZANT or _species_id == Species::FRILLISH or _species_id == Species::JELLICENT)
                    _female_icon_path = fs::path(fs::path(icon_directory)
                                      / "female" / (png_format % _species_id).str()).string();
                else _female_icon_path = _male_icon_path;
                
                _male_sprite_path = fs::path(fs::path(sprite_directory)
                                  / (png_format % _species_id).str()).string();
                _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory)
                                        / (png_format % _species_id).str()).string();
                if(_generation > 3 and has_gender_differences())
                {
                    _female_sprite_path = fs::path(fs::path(sprite_directory)
                                        / "female" / (png_format % _species_id).str()).string();
                    _female_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory)
                                              / "female" / (png_format % _species_id).str()).string();
                }
                else
                {
                    _female_sprite_path = _male_sprite_path;
                    _female_shiny_sprite_path = _male_shiny_sprite_path;
                }
                
                //Even though most attributes are queried from the database when called, stats take a long time when
                //doing a lot at once, so grab these upon instantiation
                SQLite::Database db(get_database_path().c_str());
                string query_string = "SELECT base_stat FROM pokemon_stats WHERE _pokemon_id=" + to_string(_pokemon_id)
                                    + " AND stat_id IN (3,5)";
                SQLite::Statement query(db, query_string.c_str());
                query.executeStep();
                _special_attack = int(query.getColumn(0));
                query.executeStep();
                _special_defense = int(query.getColumn(0));
                
                repair(_pokemon_id);
                break;
        }
    }

    dict<unsigned int, unsigned int> base_pokemon_modernimpl::get_base_stats() const
    {
        dict<unsigned int, unsigned int> stats;
        stats[Stats::HP] = _hp;
        stats[Stats::ATTACK] = _attack;
        stats[Stats::DEFENSE] = _defense;
        stats[Stats::SPECIAL_ATTACK] = _special_attack;
        stats[Stats::SPECIAL_DEFENSE] = _special_defense;
        stats[Stats::SPEED] = _speed;
        
        return stats;
    }

    dict<unsigned int, unsigned int> base_pokemon_modernimpl::get_ev_yields() const
    {
        dict<unsigned int, unsigned int> ev_yields;
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                ev_yields[Stats::HP] = 0;
                ev_yields[Stats::ATTACK] = 0;
                ev_yields[Stats::DEFENSE] = 0;
                ev_yields[Stats::SPECIAL_ATTACK] = 0;
                ev_yields[Stats::SPECIAL_DEFENSE] = 0;
                ev_yields[Stats::SPEED] = 0;

            default:
                SQLite::Database db(get_database_path().c_str());
                string query_string = "SELECT effort FROM pokemon_stats WHERE _pokemon_id=" + to_string(_pokemon_id) +
                               " AND stat_id IN (1,2,3,4,5,6)";
                SQLite::Statement stats_query(db, query_string.c_str());

                stats_query.executeStep();
                ev_yields[Stats::HP] = int(stats_query.getColumn(0));
                stats_query.executeStep();
                ev_yields[Stats::ATTACK] = int(stats_query.getColumn(0));
                stats_query.executeStep();
                ev_yields[Stats::DEFENSE] = int(stats_query.getColumn(0));
                stats_query.executeStep();
                ev_yields[Stats::SPECIAL_ATTACK] = int(stats_query.getColumn(0));
                stats_query.executeStep();
                ev_yields[Stats::SPECIAL_DEFENSE] = int(stats_query.getColumn(0));
                stats_query.executeStep();
                ev_yields[Stats::SPEED] = int(stats_query.getColumn(0));

                return ev_yields;
        }
    }

    double base_pokemon_modernimpl::get_chance_male() const
    {
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return 0.0;

            default:
                SQLite::Database db(get_database_path().c_str());
                
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
                int gender_val = db.execAndGet(query_string.c_str());

                if(gender_val == -1) return 0.0;
                else return gender_val_map[gender_val];
        }
    }

    double base_pokemon_modernimpl::get_chance_female() const
    {
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return 0.0;

            default:
                SQLite::Database db(get_database_path().c_str());

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
                int gender_val = db.execAndGet(query_string.c_str());

                if(gender_val == -1) return 0.0;
                else return (1.0 - gender_val_map[gender_val]);
        }
    }

    bool base_pokemon_modernimpl::has_gender_differences(void) const
    {
        if(_generation == 3 or _species_id == Species::NONE or _species_id == Species::INVALID) return false;
        else
        {
            SQLite::Database db(get_database_path().c_str());
            string query_string = "SELECT has_gender_differences FROM pokemon_species WHERE id=" + to_string(_pokemon_id);
            return bool(int(db.execAndGet(query_string.c_str())));
        }
    }
    
    dict<unsigned int, unsigned int> base_pokemon_modernimpl::get_abilities() const
    {
        dict<unsigned int, unsigned int> abilities;
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return abilities;

            default:
                SQLite::Database db(get_database_path().c_str());

                //Ability 1 (guaranteed)
                string query_string = "SELECT ability_id FROM pokemon_abilities WHERE _pokemon_id=" + to_string(_pokemon_id)
                             + " AND slot=1";
                abilities[0] = int(db.execAndGet(query_string.c_str()));

                //Ability 2 (not guaranteed, and if exists, might not exist in specified generation)
                query_string = "SELECT ability_id FROM pokemon_abilities WHERE _pokemon_id=" + to_string(_pokemon_id)
                             + " AND slot=2";
                SQLite::Statement ability2_query(db, query_string.c_str());
                if(ability2_query.executeStep()) //Will be false if no entry exists
                {
                    unsigned int ability2_id = int(ability2_query.getColumn(0)); //ability_id
                    query_string = "SELECT generation_id FROM abilities WHERE id=" + to_string(ability2_id);
                    unsigned int generation_id = int(db.execAndGet(query_string.c_str()));
                    
                    if(generation_id > _generation) abilities[1] = Abilities::NONE;
                    else abilities[1] = ability2_id;
                }
                else abilities[1] = Abilities::NONE;

                //Ability 3 (hidden ability, only in Gen 5, even then not guaranteed)
                if(_generation == 5)
                {
                    query_string = "SELECT ability_id FROM pokemon_abilities WHERE _pokemon_id=" + to_string(_pokemon_id)
                                 + " AND slot=3";
                    SQLite::Statement ability3_query(db, query_string.c_str());
                    if(ability3_query.executeStep()) //Will be false if no entry exists
                    {
                        abilities[2] = int(db.execAndGet(query_string.c_str()));
                    }
                    else abilities[2] = Abilities::NONE;
                }
                
                return abilities;
        }
    }

    string base_pokemon_modernimpl::get_icon_path(bool is_male) const
    {
        if(_generation > 3 and not is_male) return _female_icon_path;
        else return _male_icon_path;
    }
    
    string base_pokemon_modernimpl::get_sprite_path(bool is_male, bool is_shiny) const
    {
        if(is_male)
        {
            if(is_shiny) return _male_shiny_sprite_path;
            else return _male_sprite_path;
        }
        else
        {
            if(is_shiny) return _female_shiny_sprite_path;
            else return _female_sprite_path;
        }
    }
	
    //Manually set Pokemon form
    void base_pokemon_modernimpl::set_form(unsigned int form)
    {
        boost::format png_format("%d.png");
        string gen_string = "generation-" + to_string(_generation);
        std::string icon_directory = fs::path(fs::path(get_images_dir()) / "pokemon-icons").string();
        std::string sprite_directory = fs::path(fs::path(get_images_dir()) / gen_string
                                     / _images_game_string.c_str()).string();
        std::string shiny_sprite_directory = fs::path(fs::path(sprite_directory) / "shiny").string();
        
        switch(_species_id)
        {
            case 201:
                if(form >= 1 and form <= 26)
                {
                    char letter = form + 96;
                    string basename = str(boost::format("201-%c.png") % letter);
                    _male_icon_path = fs::path(fs::path(icon_directory) / basename).string();
                    _female_icon_path = _male_icon_path;
                    _male_sprite_path = fs::path(fs::path(sprite_directory) / basename).string();
                    _female_sprite_path = _male_sprite_path;
                    _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / basename).string();
                    _female_shiny_sprite_path = _male_sprite_path;
                }
                else if(form == Forms::Unown::QUESTION)
                {
                    _male_icon_path = fs::path(fs::path(icon_directory) / "201-question.png").string();
                    _female_icon_path = _male_icon_path;
                    _male_sprite_path = fs::path(fs::path(sprite_directory) / "201-question.png").string();
                    _female_sprite_path = _male_sprite_path;
                    _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "201-question.png").string();
                    _female_shiny_sprite_path = _male_sprite_path;
                }
                else if(form == Forms::Unown::EXCLAMATION)
                {
                    _male_icon_path = fs::path(fs::path(icon_directory) / "201-exclamation.png").string();
                    _female_icon_path = _male_icon_path;
                    _male_sprite_path = fs::path(fs::path(sprite_directory) / "201-exclamation.png").string();
                    _female_sprite_path = _male_sprite_path;
                    _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "201-exclamation.png").string();
                    _female_shiny_sprite_path = _male_sprite_path;
                }
                break;

            case 351:
                switch(form)
                {
                    case Forms::Castform::NORMAL:
                        _type1_id = Types::NORMAL;
                        _type2_id = Types::NONE;
                        _pokemon_id = 351;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "351.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "351.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "351.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Castform::SUNNY:
                        _type1_id = Types::FIRE;
                        _type2_id = Types::NONE;
                        _pokemon_id = 662;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "351-sunny.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "351-sunny.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "351-sunny.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Castform::RAINY:
                        _type1_id = Types::WATER;
                        _type2_id = Types::NONE;
                        _pokemon_id = 663;

                        _male_icon_path = fs::path(fs::path(get_images_dir().c_str()) / "pokemon-icons" / "351-rainy.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(get_images_dir()) / gen_string.c_str() / _images_game_string.c_str() / "351-rainy.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_sprite_path = fs::path(fs::path(get_images_dir()) / gen_string.c_str() / _images_game_string.c_str() / "shiny" / "351-rainy.png").string();
                        _female_shiny_sprite_path = _female_shiny_sprite_path;
                        break;

                    case Forms::Castform::SNOWY:
                        _type1_id = Types::ICE;
                        _type2_id = Types::NONE;
                        _pokemon_id = 664;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "351-snowy.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "351-snowy.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "351-snowy.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 386:
                switch(form)
                {
                    case Forms::Deoxys::NORMAL:
                        _pokemon_id = 386;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "386.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "386.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "386.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Deoxys::ATTACK:
                        _pokemon_id = 650;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "386-attack.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "386-attack.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "386-attack.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Deoxys::DEFENSE:
                        _pokemon_id = 651;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "386-defense.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "386-defense.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "386-defense.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Deoxys::SPEED:
                        _pokemon_id = 652;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "386-speed.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "386-speed.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "386-speed.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 412:
                switch(form)
                {
                    case Forms::Burmy::PLANT_CLOAK:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "412-plant.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "412-plant.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "412-plant.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Burmy::SANDY_CLOAK:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "412-sandy.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "412-sandy.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "412-sandy.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Burmy::TRASH_CLOAK:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "412-trash.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "412-trash.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "412-trash.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 413:
                switch(form)
                {
                    case Forms::Wormadam::PLANT_CLOAK:
                        _type1_id = Types::BUG;
                        _type2_id = Types::GRASS;
                        _pokemon_id = 413;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "413-plant.png").string();
                        _female_sprite_path = fs::path(fs::path(sprite_directory)
                                           / "female" / "413-plant.png").string();
                        _male_sprite_path = _female_sprite_path; //Will never be used
                        _female_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory)
                                                 / "female" / "413-plant.png").string();
                        _male_shiny_sprite_path = _female_shiny_sprite_path; //Will never be used
                        break;

                    case Forms::Wormadam::SANDY_CLOAK:
                        _type1_id = Types::BUG;
                        _type2_id = Types::GROUND;
                        _pokemon_id = 653;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "413-sandy.png").string();
                        _female_sprite_path = fs::path(fs::path(sprite_directory)
                                           / "female" / "413-sandy.png").string();
                        _male_sprite_path = _female_sprite_path; //Will never be used
                        _female_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory)
                                                 / "female" / "413-sandy.png").string();
                        _male_shiny_sprite_path = _female_shiny_sprite_path; //Will never be used
                        break;

                    case Forms::Wormadam::TRASH_CLOAK:
                        _type1_id = Types::BUG;
                        _type2_id = Types::STEEL;
                        _pokemon_id = 654;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "413-trash.png").string();
                        _male_sprite_path = fs::path(fs::path(sprite_directory)
                                         / "female" / "413-trash.png").string();
                        _male_sprite_path = _female_sprite_path; //Will never be used
                        _male_sprite_path = fs::path(fs::path(shiny_sprite_directory)
                                         / "female" / "413-trash.png").string();
                        _male_shiny_sprite_path = _female_shiny_sprite_path; //Will never be used
                        break;
                }
                break;

            case 421:
                switch(form)
                {
                    case Forms::Cherrim::OVERCAST:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "421-overcast.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "421-overcast.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "421-overcast.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Cherrim::SUNSHINE:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "421-sunshine.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "421-sunshine.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "421-sunshine.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 422:
                switch(form)
                {
                    case Forms::Shellos::WEST_SEA:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "422-west.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "422-west.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "422-west.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Shellos::EAST_SEA:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "422-east.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "422-east.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "422-east.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 423:
                switch(form)
                {
                    case Forms::Gastrodon::WEST_SEA:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "423-west.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "423-west.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "423-west.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Gastrodon::EAST_SEA:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "423-east.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "423-east.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "423-east.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 479:
                switch(form)
                {
                    case Forms::Rotom::NORMAL:
                        _type1_id = Types::ELECTRIC;
                        _type2_id = Types::GHOST;
                        _pokemon_id = 479;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "479.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "479.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "479.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Rotom::HEAT:
                        _type1_id = Types::ELECTRIC;
                        _type2_id = Types::FIRE;
                        _pokemon_id = 657;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "479-heat.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "479-heat.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "479-heat.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Rotom::WASH:
                        _type1_id = Types::ELECTRIC;
                        _type2_id = Types::WATER;
                        _pokemon_id = 658;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "479-wash.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "479-wash.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "479-wash.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Rotom::FROST:
                        _type1_id = Types::ELECTRIC;
                        _type2_id = Types::ICE;
                        _pokemon_id = 659;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "479-frost.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "479-frost.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "479-frost.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Rotom::FAN:
                        _type1_id = Types::ELECTRIC;
                        _type2_id = Types::FLYING;
                        _pokemon_id = 660;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "479-fan.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "479-fan.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "479-fan.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Rotom::MOW:
                        _type1_id = Types::ELECTRIC;
                        _type2_id = Types::GRASS;
                        _pokemon_id = 661;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "479-mow.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "479-mow.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "479-mow.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 487:
                switch(form)
                {
                    case Forms::Giratina::ALTERED:
                        _pokemon_id = 487;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "487-altered.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "487-altered.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "487-altered.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Giratina::ORIGIN:
                        _pokemon_id = 656;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "487-origin.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "487-origin.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "487-origin.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 492:
                switch(form)
                {
                    case Forms::Shaymin::LAND:
                        _type1_id = Types::GRASS;
                        _type2_id = Types::NONE;
                        _pokemon_id = 492;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "492-land.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "492-land.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "492-;amd.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Shaymin::SKY:
                        _type1_id = Types::GRASS;
                        _type2_id = Types::FLYING;
                        _pokemon_id = 655;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "492-sky.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "492-sky.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "492-sky.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 493:
                switch(form)
                {
                    case Forms::Arceus::NORMAL:
                        set_form("Normal");
                        break;

                    case Forms::Arceus::FIRE:
                        set_form("Fire");
                        break;

                    case Forms::Arceus::WATER:
                        set_form("Water");
                        break;

                    case Forms::Arceus::ELECTRIC:
                        set_form("Electric");
                        break;

                    case Forms::Arceus::GRASS:
                        set_form("Grass");
                        break;

                    case Forms::Arceus::ICE:
                        set_form("Ice");
                        break;

                    case Forms::Arceus::FIGHTING:
                        set_form("Fighting");
                        break;

                    case Forms::Arceus::POISON:
                        set_form("Poison");
                        break;

                    case Forms::Arceus::GROUND:
                        set_form("Ground");
                        break;

                    case Forms::Arceus::FLYING:
                        set_form("Flying");
                        break;

                    case Forms::Arceus::PSYCHIC:
                        set_form("Psychic");
                        break;

                    case Forms::Arceus::BUG:
                        set_form("Bug");
                        break;

                    case Forms::Arceus::ROCK:
                        set_form("Rock");
                        break;

                    case Forms::Arceus::GHOST:
                        set_form("Ghost");
                        break;

                    case Forms::Arceus::DRAGON:
                        set_form("Dragon");
                        break;

                    case Forms::Arceus::DARK:
                        set_form("Dark");
                        break;

                    case Forms::Arceus::STEEL:
                        set_form("Steel");
                        break;
                }
                break;

            case 550:
                switch(form)
                {
                    case Forms::Basculin::RED_STRIPED:
                        _pokemon_id = 550;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "550-red-striped.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "550-red-striped.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "550-red-striped.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Basculin::BLUE_STRIPED:
                        _pokemon_id = 665;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "550-blue-striped.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "550-blue-striped.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "550-blue-striped.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
            }
            break;

            case 555:
                switch(form)
                {
                    case Forms::Darmanitan::STANDARD:
                        _type1_id = Types::FIRE;
                        _type2_id = Types::NONE;
                        _pokemon_id = 555;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "555-standard.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "555-standard.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "555-standard.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Darmanitan::ZEN:
                        _type1_id = Types::FIRE;
                        _type2_id = Types::PSYCHIC;
                        _pokemon_id = 666;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "555-zen.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "555-zen.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "555-zen.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 585:
                switch(form)
                {
                    case Forms::Deerling::SPRING:
                        set_form("Spring");
                        break;

                    case Forms::Deerling::SUMMER:
                        set_form("Summer");
                        break;

                    case Forms::Deerling::AUTUMN:
                        set_form("Autumn");
                        break;

                    case Forms::Deerling::WINTER:
                        set_form("Winter");
                        break;
                }
                break;

            case 586:
                switch(form)
                {
                    case Forms::Sawsbuck::SPRING:
                        set_form("Spring");
                        break;

                    case Forms::Sawsbuck::SUMMER:
                        set_form("Summer");
                        break;

                    case Forms::Sawsbuck::AUTUMN:
                        set_form("Autumn");
                        break;

                    case Forms::Sawsbuck::WINTER:
                        set_form("Winter");
                        break;
                }

            case 641:
                switch(form)
                {
                    case Forms::Tornadus::INCARNATE:
                        _pokemon_id = 641;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "641-incarnate.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "641-incarnate.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "641-incarnate.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Tornadus::THERIAN:
                        _pokemon_id = 668;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "641-therian.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "641-therian.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "641-therian.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 642:
                switch(form)
                {
                    case Forms::Thundurus::INCARNATE:
                        _pokemon_id = 642;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "642-incarnate.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "642-incarnate.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "642-incarnate.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Thundurus::THERIAN:
                        _pokemon_id = 669;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "642-therian.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "642-therian.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "642-therian.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 645:
                switch(form)
                {
                    case Forms::Landorus::INCARNATE:
                        _pokemon_id = 645;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "645-incarnate.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "645-incarnate.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "645-incarnate.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Landorus::THERIAN:
                        _pokemon_id = 670;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "645-therian.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "645-therian.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "645-therian.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 646:
                switch(form)
                {
                    case Forms::Kyurem::NORMAL:
                        _pokemon_id = 646;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "646.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "646.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "646.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Kyurem::BLACK:
                        _pokemon_id = 671;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "646-black.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "646-black.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "646-black.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Kyurem::WHITE:
                        _pokemon_id = 672;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "646-white.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "646-white.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "646-white.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                    break;
                }
                break;

            case 647:
                switch(form)
                {
                    case Forms::Keldeo::ORDINARY:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "647-ordinary.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "647-ordinary.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "647-ordinary.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Keldeo::RESOLUTE:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "647-resolute.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "647-resolute.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "647-resolute.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 648:
                switch(form)
                {
                    case Forms::Meloetta::ARIA:
                        _type1_id = Types::NORMAL;
                        _type2_id = Types::PSYCHIC;
                        _pokemon_id = 648;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "648-aria.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "648-aria.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "648-aria.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Meloetta::PIROUETTE:
                        _type1_id = Types::NORMAL;
                        _type2_id = Types::FIGHTING;
                        _pokemon_id = 673;

                        _male_icon_path = fs::path(fs::path(icon_directory) / "648-pirouette.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "648-pirouette.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "648-pirouette.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            case 649:
                switch(form)
                {
                    case Forms::Genesect::NORMAL:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "649.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "649.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "649.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Genesect::SHOCK_DRIVE:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "649-shock.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "649-shock.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "649-shock.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Genesect::BURN_DRIVE:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "649-burn.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "649-burn.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "649-burn.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Genesect::CHILL_DRIVE:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "649-chill.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "649-chill.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "649-chill.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;

                    case Forms::Genesect::DOUSE_DRIVE:
                        _male_icon_path = fs::path(fs::path(icon_directory) / "649-douse.png").string();
                        _female_icon_path = _male_icon_path;
                        _male_sprite_path = fs::path(fs::path(sprite_directory) / "649-douse.png").string();
                        _female_sprite_path = _male_sprite_path;
                        _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / "649-douse.png").string();
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                        break;
                }
                break;

            default:
                break;
        }
        
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT base_stat FROM pokemon_stats WHERE _pokemon_id=" + to_string(_pokemon_id) +
                              " AND stat_id IN (1,2,3,4,5,6)";
        SQLite::Statement stats_query(db, query_string.c_str());

        stats_query.executeStep();
        _hp = int(stats_query.getColumn(0));
        stats_query.executeStep();
        _attack = int(stats_query.getColumn(0));
        stats_query.executeStep();
        _defense = int(stats_query.getColumn(0));
        stats_query.executeStep();
        _special_attack = int(stats_query.getColumn(0));
        stats_query.executeStep();
        _special_defense = int(stats_query.getColumn(0));
        stats_query.executeStep();
        _speed = int(stats_query.getColumn(0));
    }

    void base_pokemon_modernimpl::set_form(string form)
    {
        boost::format png_format("%d.png");
        string gen_string = "generation-" + to_string(_generation);
        std::string icon_directory = fs::path(fs::path(get_images_dir()) / "pokemon-icons").string();
        std::string sprite_directory = fs::path(fs::path(get_images_dir()) / gen_string
                                     / _images_game_string.c_str()).string();
        std::string shiny_sprite_directory = fs::path(fs::path(sprite_directory) / "shiny").string();
        
        switch(_species_id)
        {
            case 201:
                if(form.size() != 1) break;
                else
                {
                    char letter = form.c_str()[0];
                    if(tolower(letter) >= 'a' and tolower(letter) <= 'z') set_form(int(letter)-96);
                    else if(letter == '?') set_form(Forms::Unown::QUESTION);
                    else if(letter == '!') set_form(Forms::Unown::EXCLAMATION);
                }
                break;

            case 351:
                if(form == "Normal") set_form(Forms::Castform::NORMAL);
                else if(form == "Sunny") set_form(Forms::Castform::SUNNY);
                else if(form == "Rainy") set_form(Forms::Castform::RAINY);
                else if(form == "Snowy") set_form(Forms::Castform::SNOWY);
                break;

            case 386:
                if(form == "Normal") set_form(Forms::Deoxys::NORMAL);
                else if(form == "Attack") set_form(Forms::Deoxys::ATTACK);
                else if(form == "Defense") set_form(Forms::Deoxys::DEFENSE);
                else if(form == "Speed") set_form(Forms::Deoxys::SPEED);
                break;

            case 412:
                if(form == "Plant Cloak") set_form(Forms::Burmy::PLANT_CLOAK);
                else if(form == "Sandy Cloak") set_form(Forms::Burmy::SANDY_CLOAK);
                else if(form == "Trash Cloak") set_form(Forms::Burmy::TRASH_CLOAK);
                break;

            case 413:
                if(form == "Plant Cloak") set_form(Forms::Wormadam::PLANT_CLOAK);
                else if(form == "Sandy Cloak") set_form(Forms::Wormadam::SANDY_CLOAK);
                else if(form == "Trash Cloak") set_form(Forms::Wormadam::TRASH_CLOAK);
                break;

            case 421:
                if(form == "Overcast") set_form(Forms::Cherrim::OVERCAST);
                else if(form == "Sunshine") set_form(Forms::Cherrim::SUNSHINE);
                break;

            case 422:
                if(form == "West Sea") set_form(Forms::Shellos::WEST_SEA);
                else if(form == "East Sea") set_form(Forms::Shellos::EAST_SEA);
                break;

            case 423:
                if(form == "West Sea") set_form(Forms::Gastrodon::WEST_SEA);
                else if(form == "East Sea") set_form(Forms::Gastrodon::EAST_SEA);
                break;

            case 479:
                if(form == "Normal") set_form(Forms::Rotom::NORMAL);
                else if(form == "Heat") set_form(Forms::Rotom::HEAT);
                else if(form == "Frost") set_form(Forms::Rotom::FROST);
                else if(form == "Wash") set_form(Forms::Rotom::WASH);
                else if(form == "Fan") set_form(Forms::Rotom::FAN);
                else if(form == "Mow") set_form(Forms::Rotom::MOW);
                break;

            case 487:
                if(form == "Altered") set_form(Forms::Giratina::ALTERED);
                else if(form == "Origin") set_form(Forms::Giratina::ORIGIN);
                break;

            case 492:
                if(form == "Land") set_form(Forms::Shaymin::LAND);
                else if(form == "Sky") set_form(Forms::Shaymin::SKY);
                break;

            case 493:
            {
                vector<string> type_vec;
                get_type_list(type_vec, 4);

                if(find(type_vec.begin(), type_vec.end(), form) != type_vec.end())
                {
                    _type1_id = database::get_type_id(form);
                    _type2_id = Types::NONE;
                    transform(form.begin(), form.end(), form.begin(), ::tolower);
                    string basename = (boost::format("493-%s.png") % form).str();

                    _male_icon_path = fs::path(fs::path(icon_directory) / basename.c_str()).string();
                    _female_icon_path = _male_icon_path;
                    _male_sprite_path = fs::path(fs::path(sprite_directory) / basename.c_str()).string();
                    _female_sprite_path = _male_sprite_path;
                    _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / basename.c_str()).string();
                    _female_shiny_sprite_path = _male_shiny_sprite_path;
                }
                break;
            }

            case 550:
                if(form == "Red-Striped") set_form(Forms::Basculin::RED_STRIPED);
                if(form == "Blue-Striped") set_form(Forms::Basculin::BLUE_STRIPED);
                break;

            case 555:
                if(form == "Standard") set_form(Forms::Darmanitan::STANDARD);
                if(form == "Zen") set_form(Forms::Darmanitan::ZEN);
                break;            

            case 585:
                if(form == "Spring" or form == "Summer" or form == "Autumn" or form == "Winter")
                {
                    transform(form.begin(), form.end(), form.begin(), ::tolower);
                    string basename = (boost::format("585-%s.png") % form).str();
                    
                    _male_icon_path = fs::path(fs::path(icon_directory) / basename.c_str()).string();
                    _female_icon_path = _male_icon_path;
                    _male_sprite_path = fs::path(fs::path(sprite_directory) / basename.c_str()).string();
                    _female_sprite_path = _male_sprite_path;
                    _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / basename.c_str()).string();
                    _female_shiny_sprite_path = _male_shiny_sprite_path;
                }
                break;

            case 586:
                if(form != "Spring" and form != "Summer" and form != "Autumn" and form != "Winter")
                {
                    transform(form.begin(), form.end(), form.begin(), ::tolower);
                    string basename = (boost::format("586-%s.png") % form).str();
                    
                    _male_icon_path = fs::path(fs::path(icon_directory) / basename.c_str()).string();
                    _female_icon_path = _male_icon_path;
                    _male_sprite_path = fs::path(fs::path(sprite_directory) / basename.c_str()).string();
                    _female_sprite_path = _male_sprite_path;
                    _male_shiny_sprite_path = fs::path(fs::path(shiny_sprite_directory) / basename.c_str()).string();
                    _female_shiny_sprite_path = _male_shiny_sprite_path;
                }
                break;

            case 641:
                if(form == "Incarnate") set_form(Forms::Tornadus::INCARNATE);
                if(form == "Therian") set_form(Forms::Tornadus::THERIAN);
                break;

            case 642:
                if(form == "Incarnate") set_form(Forms::Thundurus::INCARNATE);
                if(form == "Therian") set_form(Forms::Thundurus::THERIAN);
                break;

            case 645:
                if(form == "Incarnate") set_form(Forms::Landorus::INCARNATE);
                if(form == "Therian") set_form(Forms::Landorus::THERIAN);
                break;

            case 646:
                if(form == "Normal") set_form(Forms::Kyurem::NORMAL);
                else if(form == "Black") set_form(Forms::Kyurem::BLACK);
                else if(form == "White") set_form(Forms::Kyurem::WHITE);
                break;

            case 647:
                if(form == "Ordinary") set_form(Forms::Keldeo::ORDINARY);
                else if(form == "Resolute") set_form(Forms::Keldeo::RESOLUTE);
                break;

            case 648:
                if(form == "Aria") set_form(Forms::Meloetta::ARIA);
                else if(form == "Pirouette") set_form(Forms::Meloetta::PIROUETTE);
                break;

            case 649:
                if(form == "Normal") set_form(Forms::Genesect::NORMAL);
                if(form == "Shock Drive") set_form(Forms::Genesect::SHOCK_DRIVE);
                if(form == "Burn Drive") set_form(Forms::Genesect::BURN_DRIVE);
                if(form == "Chill Drive") set_form(Forms::Genesect::CHILL_DRIVE);
                if(form == "Douse Drive") set_form(Forms::Genesect::DOUSE_DRIVE);
                break;
        }
        
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT base_stat FROM pokemon_stats WHERE _pokemon_id=" + to_string(_pokemon_id) +
                              " AND stat_id IN (1,2,3,4,5,6)";
        SQLite::Statement stats_query(db, query_string.c_str());

        stats_query.executeStep();
        _hp = int(stats_query.getColumn(0));
        stats_query.executeStep();
        _attack = int(stats_query.getColumn(0));
        stats_query.executeStep();
        _defense = int(stats_query.getColumn(0));
        stats_query.executeStep();
        _special_attack = int(stats_query.getColumn(0));
        stats_query.executeStep();
        _special_defense = int(stats_query.getColumn(0));
        stats_query.executeStep();
        _speed = int(stats_query.getColumn(0));
    }

    void base_pokemon_modernimpl::repair(unsigned int id)
    {
        switch(id)
        {
            case 650: //Deoxys - Attack Forme
                set_form(Forms::Deoxys::ATTACK);
                break;

            case 651: //Deoxys - Defense Forme
                set_form(Forms::Deoxys::DEFENSE);
                break;

            case 652: //Deoxys - Speed Forme
                set_form(Forms::Deoxys::SPEED);
                break;

            case 653: //Wormadam - Sandy Cloak
                set_form(Forms::Wormadam::SANDY_CLOAK);
                break;

            case 654: //Wormadam - Trash Cloak
                set_form(Forms::Wormadam::TRASH_CLOAK);
                break;

            case 655: //Shaymin - Sky Forme
                set_form(Forms::Shaymin::SKY);
                break;

            case 656: //Giratina - Origin Forme
                set_form(Forms::Giratina::ORIGIN);
                break;

            case 657: //Rotom - Heat
                set_form(Forms::Rotom::HEAT);
                break;

            case 658: //Rotom - Wash
                set_form(Forms::Rotom::WASH);
                break;

            case 659: //Rotom - Frost
                set_form(Forms::Rotom::FROST);
                break;

            case 660: //Rotom - Fan
                set_form(Forms::Rotom::FAN);
                break;

            case 661: //Rotom - Mow
                set_form(Forms::Rotom::MOW);
                break;

            case 662: //Castform - Sunny
                set_form(Forms::Castform::SUNNY);
                break;

            case 663: //Castform - Rainy
                set_form(Forms::Castform::RAINY);
                break;

            case 664: //Castform - Snowy
                set_form(Forms::Castform::SNOWY);
                break;

            case 665: //Basculin - Blue-Striped
                set_form(Forms::Basculin::BLUE_STRIPED);
                break;

            case 666: //Darmanitan - Zen
                set_form(Forms::Darmanitan::ZEN);
                break;

            case 667: //Meloetta - Pirouette
                set_form(Forms::Meloetta::PIROUETTE);
                break;

            case 668: //Tornadus - Therian
                set_form(Forms::Tornadus::THERIAN);
                break;

            case 669: //Thundurus - Therian
                set_form(Forms::Thundurus::THERIAN);
                break;

            case 670: //Landorus - Therian
                set_form(Forms::Landorus::THERIAN);
                break;

            case 671: //Kyurem - Black
                set_form(Forms::Kyurem::BLACK);
                break;

            case 672: //Kyurem - White
                set_form(Forms::Kyurem::WHITE);
                break;

            case 673: //Keldeo - Resolute
                set_form(Forms::Keldeo::RESOLUTE);
                break;

            default: //Nothing to do
                break;
        }
    }
    
    void base_pokemon_modernimpl::get_egg_group_names(std::vector<std::string>
                                                      &egg_group_name_vec) const
    {
        egg_group_name_vec.clear();
    
        vector<unsigned int> egg_group_id_vec;
        get_egg_group_ids(egg_group_id_vec);
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return;

            default:
                for(unsigned int i = 0; i < egg_group_id_vec.size(); i++)
                    egg_group_name_vec.push_back(database::get_egg_group_name(egg_group_id_vec[i]));
        }
    }
    
    string base_pokemon_modernimpl::get_form_name() const {return get_species_name();}
    
    void base_pokemon_modernimpl::get_egg_group_ids(std::vector<unsigned int>
                                                    &egg_group_id_vec) const
    {
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return;

            default:
                SQLite::Database db(get_database_path().c_str());
                string query_string = "SELECT egg_group_id FROM pokemon_egg_groups WHERE _species_id="
                                    + to_string(_species_id);
                SQLite::Statement query(db, query_string.c_str());
                
                while(query.executeStep()) egg_group_id_vec.push_back(int(query.getColumn(0)));
        }
    }
    
    //TODO: Give Pokemon-sim its own internal way of distinguishing forms
    unsigned int base_pokemon_modernimpl::get_form_id() const {return _pokemon_id;}
} /* namespace pkmnsim */
