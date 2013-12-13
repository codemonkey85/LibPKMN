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
        
        std::string basename = to_string(_species_id) + ".png";
        std::string gen_string = "generation-" + to_string(_generation);
        std::string icons = fs::path(fs::path(get_images_dir()) / "pokemon-icons").string();

        std::string sprites = fs::path(fs::path(get_images_dir()) / gen_string
                                     / _images_game_string.c_str()).string();
        std::string s_sprites = fs::path(fs::path(sprites) / "shiny").string();
        
        switch(_species_id)
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
                _male_icon_path = SPRITE_PATH("substitute.png");
                _female_icon_path = _male_icon_path;
                _male_sprite_path = SPRITE_PATH("substitute.png");
                _female_sprite_path = _female_icon_path;
                _male_shiny_sprite_path = _male_sprite_path;
                _female_shiny_sprite_path = _female_sprite_path;
                break;

            default:
                //_male_icon_path = fs::path(fs::path(icons) / basename).string();
                _male_icon_path = ICON_PATH(basename);

                //Unfezant, Frillish and Jellicent have different icons for each gender
                if(HAS_DIFFERENT_FEMALE_ICON) _female_icon_path = FEMALE_ICON_PATH(basename);
                else _female_icon_path = _male_icon_path;
                
                _male_sprite_path = SPRITE_PATH(basename);
                _male_shiny_sprite_path = SHINY_SPRITE_PATH(basename);
                if(_generation > 3 and has_gender_differences())
                {
                    _female_sprite_path = FEMALE_SPRITE_PATH(basename);
                    _female_shiny_sprite_path = FEMALE_SHINY_SPRITE_PATH(basename);
                }
                else
                {
                    _female_sprite_path = _male_sprite_path;
                    _female_shiny_sprite_path = _male_shiny_sprite_path;
                }
                
                //Even though most attributes are queried from the database when called, stats take a long time when
                //doing a lot at once, so grab these upon instantiation
                SQLite::Database db(get_database_path().c_str());
                string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(_pokemon_id)
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
                string query_string = "SELECT effort FROM pokemon_stats WHERE pokemon_id=" + to_string(_pokemon_id) +
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
                string query_string = "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" + to_string(_pokemon_id)
                             + " AND slot=1";
                abilities[0] = int(db.execAndGet(query_string.c_str()));

                //Ability 2 (not guaranteed, and if exists, might not exist in specified generation)
                query_string = "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" + to_string(_pokemon_id)
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
                    query_string = "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" + to_string(_pokemon_id)
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
        std::string icons = fs::path(fs::path(get_images_dir()) / "pokemon-icons").string();
        std::string sprites = fs::path(fs::path(get_images_dir()) / gen_string
                                     / _images_game_string.c_str()).string();
        std::string s_sprites = fs::path(fs::path(sprites) / "shiny").string();
        
        switch(_species_id)
        {
            case Species::UNOWN:
            {
                string basename;
                if(form >= 1 and form <= 26) basename = str(boost::format("201-%c.png") % (form + 96));
                else if(form == Forms::Unown::QUESTION) basename = "201-question.png";
                else if(form == Forms::Unown::EXCLAMATION) basename = "201-exclamation.png";

                SET_IMAGES_PATHS(basename);
                break;
            }

            case Species::CASTFORM:
                switch(form)
                {
                    case Forms::Castform::NORMAL:
                        _type1_id = Types::NORMAL;
                        _type2_id = Types::NONE;
                        _pokemon_id = 351;

                        SET_IMAGES_PATHS("351.png");
                        break;

                    case Forms::Castform::SUNNY:
                        _type1_id = Types::FIRE;
                        _type2_id = Types::NONE;
                        _pokemon_id = 10013;

                        SET_IMAGES_PATHS("351-sunny.png");
                        break;

                    case Forms::Castform::RAINY:
                        _type1_id = Types::WATER;
                        _type2_id = Types::NONE;
                        _pokemon_id = 10014;

                        SET_IMAGES_PATHS("351-rainy.png");
                        break;

                    case Forms::Castform::SNOWY:
                        _type1_id = Types::ICE;
                        _type2_id = Types::NONE;
                        _pokemon_id = 10015;

                        SET_IMAGES_PATHS("351-snowy.png");
                        break;
                }
                break;

            case Species::DEOXYS:
                switch(form)
                {
                    case Forms::Deoxys::NORMAL:
                        _pokemon_id = 386;

                        SET_IMAGES_PATHS("386.png");
                        break;

                    case Forms::Deoxys::ATTACK:
                        _pokemon_id = 10001;

                        SET_IMAGES_PATHS("386-attack.png");
                        break;

                    case Forms::Deoxys::DEFENSE:
                        _pokemon_id = 10002;

                        SET_IMAGES_PATHS("386-defense.png");
                        break;

                    case Forms::Deoxys::SPEED:
                        _pokemon_id = 10003;

                        SET_IMAGES_PATHS("386-speed.png");
                        break;
                }
                break;

            case Species::BURMY:
                switch(form)
                {
                    case Forms::Burmy::PLANT_CLOAK:
                        SET_IMAGES_PATHS("412-plant.png");
                        break;

                    case Forms::Burmy::SANDY_CLOAK:
                        SET_IMAGES_PATHS("412-sandy.png");
                        break;

                    case Forms::Burmy::TRASH_CLOAK:
                        SET_IMAGES_PATHS("412-trash.png");
                        break;
                }
                break;

            case Species::WORMADAM:
                switch(form)
                {
                    case Forms::Wormadam::PLANT_CLOAK:
                        _type1_id = Types::BUG;
                        _type2_id = Types::GRASS;
                        _pokemon_id = 413;

                        _male_icon_path = ICON_PATH("413-plant.png");
                        _female_icon_path = _male_icon_path;
                        _female_sprite_path = FEMALE_SPRITE_PATH("413-plant.png");
                        _male_sprite_path = _female_sprite_path; //Will never be used
                        _female_shiny_sprite_path = FEMALE_SHINY_SPRITE_PATH("413-plant.png");
                        _male_shiny_sprite_path = _female_shiny_sprite_path; //Will never be used
                        break;

                    case Forms::Wormadam::SANDY_CLOAK:
                        _type1_id = Types::BUG;
                        _type2_id = Types::GROUND;
                        _pokemon_id = 10004;

                        _male_icon_path = ICON_PATH("413-sandy.png");
                        _female_icon_path = _male_icon_path;
                        _female_sprite_path = FEMALE_SPRITE_PATH("413-sandy.png");
                        _male_sprite_path = _female_sprite_path; //Will never be used
                        _female_shiny_sprite_path = FEMALE_SHINY_SPRITE_PATH("413-sandy.png");
                        _male_shiny_sprite_path = _female_shiny_sprite_path; //Will never be used
                        break;

                    case Forms::Wormadam::TRASH_CLOAK:
                        _type1_id = Types::BUG;
                        _type2_id = Types::STEEL;
                        _pokemon_id = 10005;

                        _male_icon_path = ICON_PATH("413-trash.png");
                        _female_icon_path = _male_icon_path;
                        _female_sprite_path = FEMALE_SPRITE_PATH("413-trash.png");
                        _male_sprite_path = _female_sprite_path; //Will never be used
                        _female_shiny_sprite_path = FEMALE_SHINY_SPRITE_PATH("413-trash.png");
                        _male_shiny_sprite_path = _female_shiny_sprite_path; //Will never be used
                        break;
                }
                break;

            case Species::CHERRIM:
                switch(form)
                {
                    case Forms::Cherrim::OVERCAST:
                        SET_IMAGES_PATHS("421-overcast.png");
                        break;

                    case Forms::Cherrim::SUNSHINE:
                        SET_IMAGES_PATHS("421-sunshine.png");
                        break;
                }
                break;

            case Species::SHELLOS:
                switch(form)
                {
                    case Forms::Shellos::WEST_SEA:
                        SET_IMAGES_PATHS("422-west.png");
                        break;

                    case Forms::Shellos::EAST_SEA:
                        SET_IMAGES_PATHS("422-east.png");
                        break;
                }
                break;

            case Species::GASTRODON:
                switch(form)
                {
                    case Forms::Gastrodon::WEST_SEA:
                        SET_IMAGES_PATHS("423-west.png");
                        break;

                    case Forms::Gastrodon::EAST_SEA:
                        SET_IMAGES_PATHS("423-east.png");
                        break;
                }
                break;

            case Species::ROTOM:
                switch(form)
                {
                    case Forms::Rotom::NORMAL:
                        _type1_id = Types::ELECTRIC;
                        _type2_id = Types::GHOST;
                        _pokemon_id = 479;

                        SET_IMAGES_PATHS("479.png");
                        break;

                    case Forms::Rotom::HEAT:
                        _type1_id = Types::ELECTRIC;
                        _type2_id = Types::FIRE;
                        _pokemon_id = 10008;

                        SET_IMAGES_PATHS("479-heat.png");
                        break;

                    case Forms::Rotom::WASH:
                        _type1_id = Types::ELECTRIC;
                        _type2_id = Types::WATER;
                        _pokemon_id = 10009;

                        SET_IMAGES_PATHS("479-wash.png");
                        break;

                    case Forms::Rotom::FROST:
                        _type1_id = Types::ELECTRIC;
                        _type2_id = Types::ICE;
                        _pokemon_id = 10010;

                        SET_IMAGES_PATHS("479-frost.png");
                        break;

                    case Forms::Rotom::FAN:
                        _type1_id = Types::ELECTRIC;
                        _type2_id = Types::FLYING;
                        _pokemon_id = 10011;

                        SET_IMAGES_PATHS("479-fan.png");
                        break;

                    case Forms::Rotom::MOW:
                        _type1_id = Types::ELECTRIC;
                        _type2_id = Types::GRASS;
                        _pokemon_id = 10012;

                        SET_IMAGES_PATHS("479-mow.png");
                        break;
                }
                break;

            case Species::GIRATINA:
                switch(form)
                {
                    case Forms::Giratina::ALTERED:
                        _pokemon_id = 487;

                        SET_IMAGES_PATHS("487-altered.png");
                        break;

                    case Forms::Giratina::ORIGIN:
                        _pokemon_id = 10007;

                        SET_IMAGES_PATHS("487-origin.png");
                        break;
                }
                break;

            case Species::SHAYMIN:
                switch(form)
                {
                    case Forms::Shaymin::LAND:
                        _type1_id = Types::GRASS;
                        _type2_id = Types::NONE;
                        _pokemon_id = 492;

                        SET_IMAGES_PATHS("492-land.png");
                        break;

                    case Forms::Shaymin::SKY:
                        _type1_id = Types::GRASS;
                        _type2_id = Types::FLYING;
                        _pokemon_id = 10006;

                        SET_IMAGES_PATHS("492-sky.png");
                        break;
                }
                break;

            case Species::ARCEUS:
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

            case Species::BASCULIN:
                switch(form)
                {
                    case Forms::Basculin::RED_STRIPED:
                        _pokemon_id = 550;

                        SET_IMAGES_PATHS("500-red-striped.png");
                        break;

                    case Forms::Basculin::BLUE_STRIPED:
                        _pokemon_id = 10016;

                        SET_IMAGES_PATHS("550-blue-striped.png");
                        break;
            }
            break;

            case Species::DARMANITAN:
                switch(form)
                {
                    case Forms::Darmanitan::STANDARD:
                        _type1_id = Types::FIRE;
                        _type2_id = Types::NONE;
                        _pokemon_id = 555;

                        SET_IMAGES_PATHS("555-standard.png");
                        break;

                    case Forms::Darmanitan::ZEN:
                        _type1_id = Types::FIRE;
                        _type2_id = Types::PSYCHIC;
                        _pokemon_id = 10017;

                        SET_IMAGES_PATHS("555-zen.png");
                        break;
                }
                break;

            case Species::DEERLING:
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

            case Species::SAWSBUCK:
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

            case Species::TORNADUS:
                switch(form)
                {
                    case Forms::Tornadus::INCARNATE:
                        _pokemon_id = 641;

                        SET_IMAGES_PATHS("641-incarnate.png");
                        break;

                    case Forms::Tornadus::THERIAN:
                        _pokemon_id = 10019;

                        SET_IMAGES_PATHS("641-therian.png");
                        break;
                }
                break;

            case Species::THUNDURUS:
                switch(form)
                {
                    case Forms::Thundurus::INCARNATE:
                        _pokemon_id = 642;

                        SET_IMAGES_PATHS("642-incarnate.png");
                        break;

                    case Forms::Thundurus::THERIAN:
                        _pokemon_id = 10020;

                        SET_IMAGES_PATHS("642-therian.png");
                        break;
                }
                break;

            case Species::LANDORUS:
                switch(form)
                {
                    case Forms::Landorus::INCARNATE:
                        _pokemon_id = 645;

                        SET_IMAGES_PATHS("645-incarnate.png");
                        break;

                    case Forms::Landorus::THERIAN:
                        _pokemon_id = 10021;

                        SET_IMAGES_PATHS("645-therian.png");
                        break;
                }
                break;

            case Species::KYUREM:
                switch(form)
                {
                    case Forms::Kyurem::NORMAL:
                        _pokemon_id = 646;

                        SET_IMAGES_PATHS("646.png");
                        break;

                    case Forms::Kyurem::BLACK:
                        _pokemon_id = 10022;

                        SET_IMAGES_PATHS("646-black.png");
                        break;

                    case Forms::Kyurem::WHITE:
                        _pokemon_id = 10023;

                        SET_IMAGES_PATHS("646-white.png");
                    break;
                }
                break;

            case Species::KELDEO:
                switch(form)
                {
                    case Forms::Keldeo::ORDINARY:
                        _pokemon_id = 647;

                        SET_IMAGES_PATHS("647-ordinary.png");
                        break;

                    case Forms::Keldeo::RESOLUTE:
                        _pokemon_id = 10024;

                        SET_IMAGES_PATHS("647-resolute.png");
                        break;
                }
                break;

            case Species::MELOETTA:
                switch(form)
                {
                    case Forms::Meloetta::ARIA:
                        _type1_id = Types::NORMAL;
                        _type2_id = Types::PSYCHIC;
                        _pokemon_id = 648;

                        SET_IMAGES_PATHS("648-aria.png");
                        break;

                    case Forms::Meloetta::PIROUETTE:
                        _type1_id = Types::NORMAL;
                        _type2_id = Types::FIGHTING;
                        _pokemon_id = 10018;

                        SET_IMAGES_PATHS("648-pirouette.png");
                        break;
                }
                break;

            case Species::GENESECT:
                switch(form)
                {
                    case Forms::Genesect::NORMAL:
                        SET_IMAGES_PATHS("649.png");
                        break;

                    case Forms::Genesect::SHOCK_DRIVE:
                        SET_IMAGES_PATHS("649-shock.png");
                        break;

                    case Forms::Genesect::BURN_DRIVE:
                        SET_IMAGES_PATHS("649-burn.png");
                        break;

                    case Forms::Genesect::CHILL_DRIVE:
                        SET_IMAGES_PATHS("649-chill.png");
                        break;

                    case Forms::Genesect::DOUSE_DRIVE:
                        SET_IMAGES_PATHS("649-douse.png");
                        break;
                }
                break;

            default:
                break;
        }
        
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(_pokemon_id) +
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
        std::string icons = fs::path(fs::path(get_images_dir()) / "pokemon-icons").string();
        std::string sprites = fs::path(fs::path(get_images_dir()) / gen_string
                                     / _images_game_string.c_str()).string();
        std::string s_sprites = fs::path(fs::path(sprites) / "shiny").string();
        
        switch(_species_id)
        {
            case Species::UNOWN:
                if(form.size() != 1) break;
                else
                {
                    char letter = form.c_str()[0];
                    if(tolower(letter) >= 'a' and tolower(letter) <= 'z') set_form(int(letter)-96);
                    else if(letter == '?') set_form(Forms::Unown::QUESTION);
                    else if(letter == '!') set_form(Forms::Unown::EXCLAMATION);
                }
                break;

            case Species::CASTFORM:
                if(form == "Normal") set_form(Forms::Castform::NORMAL);
                else if(form == "Sunny") set_form(Forms::Castform::SUNNY);
                else if(form == "Rainy") set_form(Forms::Castform::RAINY);
                else if(form == "Snowy") set_form(Forms::Castform::SNOWY);
                break;

            case Species::DEOXYS:
                if(form == "Normal") set_form(Forms::Deoxys::NORMAL);
                else if(form == "Attack") set_form(Forms::Deoxys::ATTACK);
                else if(form == "Defense") set_form(Forms::Deoxys::DEFENSE);
                else if(form == "Speed") set_form(Forms::Deoxys::SPEED);
                break;

            case Species::BURMY:
                if(form == "Plant Cloak") set_form(Forms::Burmy::PLANT_CLOAK);
                else if(form == "Sandy Cloak") set_form(Forms::Burmy::SANDY_CLOAK);
                else if(form == "Trash Cloak") set_form(Forms::Burmy::TRASH_CLOAK);
                break;

            case Species::WORMADAM:
                if(form == "Plant Cloak") set_form(Forms::Wormadam::PLANT_CLOAK);
                else if(form == "Sandy Cloak") set_form(Forms::Wormadam::SANDY_CLOAK);
                else if(form == "Trash Cloak") set_form(Forms::Wormadam::TRASH_CLOAK);
                break;

            case Species::CHERRIM:
                if(form == "Overcast") set_form(Forms::Cherrim::OVERCAST);
                else if(form == "Sunshine") set_form(Forms::Cherrim::SUNSHINE);
                break;

            case Species::SHELLOS:
                if(form == "West Sea") set_form(Forms::Shellos::WEST_SEA);
                else if(form == "East Sea") set_form(Forms::Shellos::EAST_SEA);
                break;

            case Species::GASTRODON:
                if(form == "West Sea") set_form(Forms::Gastrodon::WEST_SEA);
                else if(form == "East Sea") set_form(Forms::Gastrodon::EAST_SEA);
                break;

            case Species::ROTOM:
                if(form == "Normal") set_form(Forms::Rotom::NORMAL);
                else if(form == "Heat") set_form(Forms::Rotom::HEAT);
                else if(form == "Frost") set_form(Forms::Rotom::FROST);
                else if(form == "Wash") set_form(Forms::Rotom::WASH);
                else if(form == "Fan") set_form(Forms::Rotom::FAN);
                else if(form == "Mow") set_form(Forms::Rotom::MOW);
                break;

            case Species::GIRATINA:
                if(form == "Altered") set_form(Forms::Giratina::ALTERED);
                else if(form == "Origin") set_form(Forms::Giratina::ORIGIN);
                break;

            case Species::SHAYMIN:
                if(form == "Land") set_form(Forms::Shaymin::LAND);
                else if(form == "Sky") set_form(Forms::Shaymin::SKY);
                break;

            case Species::ARCEUS:
            {
                vector<string> type_vec;
                get_type_list(type_vec, 4);

                if(find(type_vec.begin(), type_vec.end(), form) != type_vec.end())
                {
                    _type1_id = database::get_type_id(form);
                    _type2_id = Types::NONE;
                    transform(form.begin(), form.end(), form.begin(), ::tolower);
                    string basename = (boost::format("493-%s.png") % form).str();

                    SET_IMAGES_PATHS(basename);
                }
                break;
            }

            case Species::BASCULIN:
                if(form == "Red-Striped") set_form(Forms::Basculin::RED_STRIPED);
                if(form == "Blue-Striped") set_form(Forms::Basculin::BLUE_STRIPED);
                break;

            case Species::DARMANITAN:
                if(form == "Standard") set_form(Forms::Darmanitan::STANDARD);
                if(form == "Zen") set_form(Forms::Darmanitan::ZEN);
                break;            

            case Species::DEERLING:
            case Species::SAWSBUCK:
                if(form == "Spring" or form == "Summer" or form == "Autumn" or form == "Winter")
                {
                    transform(form.begin(), form.end(), form.begin(), ::tolower);
                    string basename = (boost::format("%d-%s.png") % _species_id % form).str();
                    
                    SET_IMAGES_PATHS(basename);
                }
                break;

            case Species::TORNADUS:
                if(form == "Incarnate") set_form(Forms::Tornadus::INCARNATE);
                if(form == "Therian") set_form(Forms::Tornadus::THERIAN);
                break;

            case Species::THUNDURUS:
                if(form == "Incarnate") set_form(Forms::Thundurus::INCARNATE);
                if(form == "Therian") set_form(Forms::Thundurus::THERIAN);
                break;

            case Species::LANDORUS:
                if(form == "Incarnate") set_form(Forms::Landorus::INCARNATE);
                if(form == "Therian") set_form(Forms::Landorus::THERIAN);
                break;

            case Species::KYUREM:
                if(form == "Normal") set_form(Forms::Kyurem::NORMAL);
                else if(form == "Black") set_form(Forms::Kyurem::BLACK);
                else if(form == "White") set_form(Forms::Kyurem::WHITE);
                break;

            case Species::KELDEO:
                if(form == "Ordinary") set_form(Forms::Keldeo::ORDINARY);
                else if(form == "Resolute") set_form(Forms::Keldeo::RESOLUTE);
                break;

            case Species::MELOETTA:
                if(form == "Aria") set_form(Forms::Meloetta::ARIA);
                else if(form == "Pirouette") set_form(Forms::Meloetta::PIROUETTE);
                break;

            case Species::GENESECT:
                if(form == "Normal") set_form(Forms::Genesect::NORMAL);
                if(form == "Shock Drive") set_form(Forms::Genesect::SHOCK_DRIVE);
                if(form == "Burn Drive") set_form(Forms::Genesect::BURN_DRIVE);
                if(form == "Chill Drive") set_form(Forms::Genesect::CHILL_DRIVE);
                if(form == "Douse Drive") set_form(Forms::Genesect::DOUSE_DRIVE);
                break;
        }
        
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(_pokemon_id) +
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
        //TODO: standard forms
        switch(id)
        {
            case 10001: //Deoxys - Attack Forme
                set_form(Forms::Deoxys::ATTACK);
                break;

            case 10002: //Deoxys - Defense Forme
                set_form(Forms::Deoxys::DEFENSE);
                break;

            case 10003: //Deoxys - Speed Forme
                set_form(Forms::Deoxys::SPEED);
                break;

            case 10004: //Wormadam - Sandy Cloak
                set_form(Forms::Wormadam::SANDY_CLOAK);
                break;

            case 10005: //Wormadam - Trash Cloak
                set_form(Forms::Wormadam::TRASH_CLOAK);
                break;

            case 10006: //Shaymin - Sky Forme
                set_form(Forms::Shaymin::SKY);
                break;

            case 10007: //Giratina - Origin Forme
                set_form(Forms::Giratina::ORIGIN);
                break;

            case 10008: //Rotom - Heat
                set_form(Forms::Rotom::HEAT);
                break;

            case 10009: //Rotom - Wash
                set_form(Forms::Rotom::WASH);
                break;

            case 10010: //Rotom - Frost
                set_form(Forms::Rotom::FROST);
                break;

            case 10011: //Rotom - Fan
                set_form(Forms::Rotom::FAN);
                break;

            case 10012: //Rotom - Mow
                set_form(Forms::Rotom::MOW);
                break;

            case 10013: //Castform - Sunny
                set_form(Forms::Castform::SUNNY);
                break;

            case 10014: //Castform - Rainy
                set_form(Forms::Castform::RAINY);
                break;

            case 10015: //Castform - Snowy
                set_form(Forms::Castform::SNOWY);
                break;

            case 10016: //Basculin - Blue-Striped
                set_form(Forms::Basculin::BLUE_STRIPED);
                break;

            case 10017: //Darmanitan - Zen
                set_form(Forms::Darmanitan::ZEN);
                break;

            case 10018: //Meloetta - Pirouette
                set_form(Forms::Meloetta::PIROUETTE);
                break;

            case 10019: //Tornadus - Therian
                set_form(Forms::Tornadus::THERIAN);
                break;

            case 10020: //Thundurus - Therian
                set_form(Forms::Thundurus::THERIAN);
                break;

            case 10021: //Landorus - Therian
                set_form(Forms::Landorus::THERIAN);
                break;

            case 10022: //Kyurem - Black
                set_form(Forms::Kyurem::BLACK);
                break;

            case 10023: //Kyurem - White
                set_form(Forms::Kyurem::WHITE);
                break;

            case 10024: //Keldeo - Resolute
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
                string query_string = "SELECT egg_group_id FROM pokemon_egg_groups WHERE species_id="
                                    + to_string(_species_id);
                SQLite::Statement query(db, query_string.c_str());
                
                while(query.executeStep()) egg_group_id_vec.push_back(int(query.getColumn(0)));
        }
    }
    
    //TODO: Give Pokemon-sim its own internal way of distinguishing forms
    unsigned int base_pokemon_modernimpl::get_form_id() const {return _pokemon_id;}
} /* namespace pkmnsim */
