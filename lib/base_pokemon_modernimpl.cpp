/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <map>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/lists.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

#include "base_pokemon_modernimpl.hpp"

namespace fs = boost::filesystem;
using namespace std;

namespace pkmn
{
    base_pokemon_modernimpl::base_pokemon_modernimpl(unsigned int id, unsigned int game):
                                           base_pokemon_impl(id, game)
    {
        //Get final part of images path
        switch(_generation)
        {
            case Versions::RUBY:
            case Versions::SAPPHIRE:
                _images_game_string = "ruby-sapphire";
                break;
            case Versions::EMERALD:
                _images_game_string = "emerald";
                break;
            case Versions::FIRERED:
            case Versions::LEAFGREEN:
                _images_game_string = "firered-leafgreen";
                break;
            case Versions::DIAMOND:
            case Versions::PEARL:
                _images_game_string = "diamond-pearl";
                break;
            case Versions::PLATINUM:
                _images_game_string = "platinum";
                break;
            case Versions::HEARTGOLD:
            case Versions::SOULSILVER:
                _images_game_string = "heartgold-soulsilver";
                break;
            case Versions::BLACK:
            case Versions::WHITE:
            case Versions::BLACK_2:
            case Versions::WHITE_2:
                _images_game_string = "black-white";
            default:
                _images_game_string = "x-y";
                break;
        }

        _sprite_dir = fs::path(_images_dir / _images_gen_string / _images_game_string);
        _shiny_sprite_dir = fs::path(_sprite_dir / "shiny");

        switch(_species_id)
        {
            case Species::NONE: //None, should only be used for empty slots at end of party
                SET_POKEBALL_IMAGE();
                break;

            case Species::INVALID: //Invalid, aka Bad Egg
                SET_SUBSTITUTE_IMAGE();
                break;

            default:
                if(_generation == 6)
                {
                    SET_POKEBALL_IMAGE();
                }
                else
                {
                    _male_icon_path = ICON_PATH(_images_default_basename);

                    //Unfezant, Frillish and Jellicent have different icons for each gender
                    if(HAS_DIFFERENT_FEMALE_ICON) _female_icon_path = FEMALE_ICON_PATH(_images_default_basename);
                    else _female_icon_path = _male_icon_path;

                    _male_sprite_path = SPRITE_PATH(_images_default_basename);
                    _male_shiny_sprite_path = SHINY_SPRITE_PATH(_images_default_basename);
                    if(_generation > 3 and has_gender_differences())
                    {
                        _female_sprite_path = FEMALE_SPRITE_PATH(_images_default_basename);
                        _female_shiny_sprite_path = FEMALE_SHINY_SPRITE_PATH(_images_default_basename);
                    }
                    else
                    {
                        _female_sprite_path = _male_sprite_path;
                        _female_shiny_sprite_path = _male_shiny_sprite_path;
                    }
                }

                //Even though most attributes are queried from the database when called, stats take a long time when
                //doing a lot at once, so grab these upon instantiation
                std::string query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(_pokemon_id)
                                         + " AND stat_id IN (3,5)";
                SQLite::Statement query(*_db, query_string.c_str());
                query.executeStep();
                _special_attack = int(query.getColumn(0));
                query.executeStep();
                _special_defense = int(query.getColumn(0));
                if(_generation < 6) _set_old_values();

                break;
        }
    }

    base_pokemon_modernimpl::base_pokemon_modernimpl(const base_pokemon_modernimpl &other): base_pokemon_impl(other)
    {
        //Disconnect any signals that point at old base_pokemon_modernimpl's slots
        form_signal1.Clear();
        form_signal2.Clear();
    }

    base_pokemon_modernimpl& base_pokemon_modernimpl::operator=(const base_pokemon_modernimpl &other)
    {
        base_pokemon_impl::operator=(other);

        //Disconnect any signals that point at old base_pokemon_modernimpl's slots
        form_signal1.Clear();
        form_signal2.Clear();

        return (*this);
    }

    void base_pokemon_modernimpl::get_egg_groups(std::vector<std::string>& egg_group_vec) const
    {
        std::vector<unsigned int> egg_group_ids;
        get_egg_group_ids(egg_group_ids);

        for(size_t i = 0; i < egg_group_ids.size(); i++) egg_group_vec.push_back(database::get_egg_group_name(egg_group_ids[i]));
    }

    bool base_pokemon_modernimpl::has_gender_differences() const
    {
        if(_generation == 3 or _species_id == Species::NONE
           or _species_id == Species::INVALID) return false;
        else
        {
            string query_string = "SELECT has_gender_differences FROM pokemon_species WHERE id=" + to_string(_pokemon_id);
            return bool(int(_db->execAndGet(query_string.c_str())));
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
                int gender_val = _db->execAndGet(query_string.c_str());

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
                int gender_val = _db->execAndGet(query_string.c_str());

                if(gender_val == -1) return 0.0;
                else return (1.0 - gender_val_map[gender_val]);
        }
    }

    string_pair_t base_pokemon_modernimpl::get_abilities() const
    {
        string_pair_t abilities;
        abilities.first = "None";
        abilities.second = "None";

        if(_species_id == Species::NONE or _species_id == Species::INVALID) return abilities;
        else
        {
            //All Pokemon are guaranteed to have an ability in slot 1
            std::string query_string = "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" + to_string(_pokemon_id)
                                     + " AND slot=1";
            abilities.first = database::get_ability_name(int(_db->execAndGet(query_string.c_str())));

            //Second ability not guaranteed (if it exists, it may not be in the current generation)
            query_string = "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" + to_string(_pokemon_id)
                         + " AND slot=2";
            SQLite::Statement ability2_query(*_db, query_string.c_str());
            if(ability2_query.executeStep()) //Will be false if no entry exists
            {
                unsigned int ability2_id = int(ability2_query.getColumn(0)); //ability_id
                query_string = "SELECT generation_id FROM abilities WHERE id=" + to_string(ability2_id);
                unsigned int generation_id = int(_db->execAndGet(query_string.c_str()));

                if(generation_id <= _generation) abilities.second = database::get_ability_name(ability2_id);
                else abilities.second = "None";
            }
            else abilities.second = "None";
        }

        return abilities;
    }

    std::string base_pokemon_modernimpl::get_hidden_ability() const
    {
        std::string query_string = "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" + to_string(_pokemon_id)
                                 + " AND is_hidden=1 AND slot=3";
        SQLite::Statement query(*_db, query_string.c_str());
        if(query.executeStep()) //Will be false if no entry exists
        {
            unsigned int ability_id = int(query.getColumn(0)); //ability_id
            query_string = "SELECT generation_id FROM abilities WHERE id=" + to_string(ability_id);
            unsigned int generation_id = int(_db->execAndGet(query_string.c_str()));

            if(generation_id <= _generation) return database::get_ability_name(ability_id);
            else return "None";
        }
        else return "None";
    }

    dict<std::string, unsigned int> base_pokemon_modernimpl::get_base_stats() const
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

    dict<std::string, unsigned int> base_pokemon_modernimpl::get_ev_yields() const
    {
        dict<std::string, unsigned int> ev_yields;
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                ev_yields["HP"] = 0;
                ev_yields["Attack"] = 0;
                ev_yields["Defense"] = 0;
                ev_yields["Special Attack"] = 0;
                ev_yields["Special Defense"] = 0;
                ev_yields["Speed"] = 0;

            default:
                string query_string = "SELECT effort FROM pokemon_stats WHERE pokemon_id=" + to_string(_pokemon_id) +
                               " AND stat_id IN (1,2,3,4,5,6)";
                SQLite::Statement stats_query(*_db, query_string.c_str());

                stats_query.executeStep();
                ev_yields["HP"] = int(stats_query.getColumn(0));
                stats_query.executeStep();
                ev_yields["Attack"] = int(stats_query.getColumn(0));
                stats_query.executeStep();
                ev_yields["Defense"] = int(stats_query.getColumn(0));
                stats_query.executeStep();
                ev_yields["Special Attack"] = int(stats_query.getColumn(0));
                stats_query.executeStep();
                ev_yields["Special Defense"] = int(stats_query.getColumn(0));
                stats_query.executeStep();
                ev_yields["Speed"] = int(stats_query.getColumn(0));

                return ev_yields;
        }
    }

    //TODO: switch to actual Mega images when Gen VI images are in images repo
    #define SET_STANDARD_MEGA_FORMS(num, name) if(form == Forms::name::STANDARD) \
                                               { \
                                                   _form_id = form; \
                                                   SET_IMAGES_PATHS(str(_png_format % num)); \
                                               } \
                                               else if(form == Forms::name::MEGA and _generation == 6) \
                                               { \
                                                   _form_id = form; \
                                                   SET_POKEBALL_IMAGE(); \
                                               } \
                                               else throw std::runtime_error("Invalid form.");

    //Manually set Pokemon form
    void base_pokemon_modernimpl::set_form(unsigned int form)
    {
        unsigned int old_form_id = _form_id;

        switch(_species_id)
        {
            case Species::VENUSAUR:
                SET_STANDARD_MEGA_FORMS(3, Venusaur);
                break;

            case Species::CHARIZARD:
            {
                if(form == Forms::Charizard::STANDARD)
                {
                    _form_id = form;
                    SET_IMAGES_PATHS("6.png");
                }
                else if((form == Forms::Charizard::MEGA_X or form == Forms::Charizard::MEGA_Y) and
                        _generation == 6)
                {
                    _form_id = form;
                    SET_POKEBALL_IMAGE(); //TODO: fix when Gen VI images are in images repo
                }
                else throw std::runtime_error("Invalid form.");

                break;
            }

            case Species::BLASTOISE:
                SET_STANDARD_MEGA_FORMS(9, Blastoise);
                break;

            case Species::ALAKAZAM:
                SET_STANDARD_MEGA_FORMS(65, Alakazam);
                break;

            case Species::GENGAR:
                SET_STANDARD_MEGA_FORMS(94, Gengar);
                break;

            case Species::KANGASKHAN:
                SET_STANDARD_MEGA_FORMS(115, Kangaskhan);
                break;

            case Species::PINSIR:
                SET_STANDARD_MEGA_FORMS(127, Pinsir);
                break;

            case Species::GYARADOS:
                SET_STANDARD_MEGA_FORMS(130, Gyarados);
                break;

            case Species::AERODACTYL:
                SET_STANDARD_MEGA_FORMS(115, Aerodactyl);
                break;

            case Species::MEWTWO:
            {
                if(form == Forms::Mewtwo::STANDARD)
                {
                    _form_id = form;
                    SET_IMAGES_PATHS("150.png");
                }
                else if((form == Forms::Mewtwo::MEGA_X or form == Forms::Mewtwo::MEGA_Y) and
                        _generation == 6)
                {
                    _form_id = form;
                    SET_POKEBALL_IMAGE(); //TODO: fix when Gen VI images are in images repo
                }
                else throw std::runtime_error("Invalid form.");

                break;
            }

            case Species::PICHU:
            {
                if(form == Forms::Pichu::STANDARD)
                {
                    _form_id = form;
                    SET_IMAGES_PATHS("172.png");
                }
                else if(form == Forms::Pichu::SPIKY_EARED and
                        (_game_id == Versions::HEARTGOLD or _game_id == Versions::SOULSILVER))
                {
                    //TODO: force Spiky-eared Pichu to be female
                    _form_id = form;
                    _male_icon_path = ICON_PATH("172.png");
                    _female_icon_path = ICON_PATH("172.png");
                    _male_sprite_path = FEMALE_SPRITE_PATH("172-spiky.png");
                    _female_sprite_path = FEMALE_SPRITE_PATH("172-spiky.png");
                    _male_shiny_sprite_path = FEMALE_SHINY_SPRITE_PATH("172-spiky.png");
                    _female_shiny_sprite_path = FEMALE_SHINY_SPRITE_PATH("172-spiky.png");
                }
                else throw std::runtime_error("Invalid form.");

                break;
            }

            case Species::AMPHAROS:
                SET_STANDARD_MEGA_FORMS(181, Ampharos);
                break;

            case Species::UNOWN:
            {
                if(form == Forms::Unown::A)
                {
                    _form_id = form;
                    set_form("A");
                }
                else if(form >= Forms::Unown::B and form <= Forms::Unown::Z)
                {
                    _form_id = form;
                    std::string letter;
                    letter += (form - 9935); //Will become ASCII value for letter
                    set_form(letter);
                }
                else if(form == Forms::Unown::QUESTION_MARK)
                {
                    _form_id = form;
                    set_form("?");
                }
                else if(form == Forms::Unown::EXCLAMATION_MARK)
                {
                    _form_id = form;
                    set_form("!");
                }
                else throw std::runtime_error("Invalid form.");

                return;
            }

            case Species::SCIZOR:
                SET_STANDARD_MEGA_FORMS(212, Scizor);
                break;

            case Species::HERACROSS:
                SET_STANDARD_MEGA_FORMS(214, Heracross);
                break;

            case Species::HOUNDOOM:
                SET_STANDARD_MEGA_FORMS(229, Houndoom);
                break;

            case Species::TYRANITAR:
                SET_STANDARD_MEGA_FORMS(248, Tyranitar);
                break;

            case Species::BLAZIKEN:
                SET_STANDARD_MEGA_FORMS(357, Blaziken);
                break;

            case Species::GARDEVOIR:
                SET_STANDARD_MEGA_FORMS(282, Gardevoir);
                break;

            case Species::MAWILE:
                SET_STANDARD_MEGA_FORMS(303, Mawile);
                break;

            case Species::AGGRON:
                SET_STANDARD_MEGA_FORMS(306, Aggron);
                break;

            case Species::MEDICHAM:
                SET_STANDARD_MEGA_FORMS(308, Medicham);
                break;

            case Species::MANECTRIC:
                SET_STANDARD_MEGA_FORMS(310, Manectric);
                break;

            case Species::CASTFORM:
                switch(form)
                {
                    case Forms::Castform::STANDARD:
                        _form_id = form;
                        SET_IMAGES_PATHS("351.png");
                        break;

                    case Forms::Castform::SUNNY:
                        _form_id = form;
                        SET_IMAGES_PATHS("351-sunny.png");
                        break;

                    case Forms::Castform::RAINY:
                        _form_id = form;
                        SET_IMAGES_PATHS("351-rainy.png");
                        break;

                    case Forms::Castform::SNOWY:
                        _form_id = form;
                        SET_IMAGES_PATHS("351-snowy.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::BANETTE:
                SET_STANDARD_MEGA_FORMS(354, Banette);
                break;

            case Species::ABSOL:
                SET_STANDARD_MEGA_FORMS(359, Absol);
                break;

            case Species::DEOXYS:
                switch(form)
                {
                    case Forms::Deoxys::NORMAL:
                        _form_id = form;
                        SET_IMAGES_PATHS("386.png");
                        break;

                    case Forms::Deoxys::ATTACK:
                        _form_id = form;
                        SET_IMAGES_PATHS("386-attack.png");
                        break;

                    case Forms::Deoxys::DEFENSE:
                        _form_id = form;
                        SET_IMAGES_PATHS("386-defense.png");
                        break;

                    case Forms::Deoxys::SPEED:
                        _form_id = form;
                        SET_IMAGES_PATHS("386-speed.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::BURMY:
                switch(form)
                {
                    case Forms::Burmy::PLANT_CLOAK:
                        _form_id = form;
                        SET_IMAGES_PATHS("412-plant.png");
                        break;

                    case Forms::Burmy::SANDY_CLOAK:
                        _form_id = form;
                        SET_IMAGES_PATHS("412-sandy.png");
                        break;

                    case Forms::Burmy::TRASH_CLOAK:
                        _form_id = form;
                        SET_IMAGES_PATHS("412-trash.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            //Wormadam are only female, so manually set everything to female paths
            case Species::WORMADAM:
                switch(form)
                {
                    case Forms::Wormadam::PLANT_CLOAK:
                        _form_id = form;
                        _male_icon_path = ICON_PATH("413-plant.png");
                        _female_icon_path = _male_icon_path;
                        _female_sprite_path = FEMALE_SPRITE_PATH("413-plant.png");
                        _male_sprite_path = _female_sprite_path; //Will never be used
                        _female_shiny_sprite_path = FEMALE_SHINY_SPRITE_PATH("413-plant.png");
                        _male_shiny_sprite_path = _female_shiny_sprite_path; //Will never be used
                        break;

                    case Forms::Wormadam::SANDY_CLOAK:
                        _form_id = form;
                        _male_icon_path = ICON_PATH("413-sandy.png");
                        _female_icon_path = _male_icon_path;
                        _female_sprite_path = FEMALE_SPRITE_PATH("413-sandy.png");
                        _male_sprite_path = _female_sprite_path; //Will never be used
                        _female_shiny_sprite_path = FEMALE_SHINY_SPRITE_PATH("413-sandy.png");
                        _male_shiny_sprite_path = _female_shiny_sprite_path; //Will never be used
                        break;

                    case Forms::Wormadam::TRASH_CLOAK:
                        _form_id = form;
                        _male_icon_path = ICON_PATH("413-trash.png");
                        _female_icon_path = _male_icon_path;
                        _female_sprite_path = FEMALE_SPRITE_PATH("413-trash.png");
                        _male_sprite_path = _female_sprite_path; //Will never be used
                        _female_shiny_sprite_path = FEMALE_SHINY_SPRITE_PATH("413-trash.png");
                        _male_shiny_sprite_path = _female_shiny_sprite_path; //Will never be used
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::CHERRIM:
                switch(form)
                {
                    case Forms::Cherrim::OVERCAST:
                        _form_id = form;
                        SET_IMAGES_PATHS("421-overcast.png");
                        break;

                    case Forms::Cherrim::SUNSHINE:
                        _form_id = form;
                        SET_IMAGES_PATHS("421-sunshine.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::SHELLOS:
                switch(form)
                {
                    case Forms::Shellos::WEST_SEA:
                        _form_id = form;
                        SET_IMAGES_PATHS("422-west.png");
                        break;

                    case Forms::Shellos::EAST_SEA:
                        _form_id = form;
                        SET_IMAGES_PATHS("422-east.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::GASTRODON:
                switch(form)
                {
                    case Forms::Gastrodon::WEST_SEA:
                        _form_id = form;
                        SET_IMAGES_PATHS("423-west.png");
                        break;

                    case Forms::Gastrodon::EAST_SEA:
                        _form_id = form;
                        SET_IMAGES_PATHS("423-east.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::GARCHOMP:
                SET_STANDARD_MEGA_FORMS(445, Garchomp);
                break;

            case Species::LUCARIO:
                SET_STANDARD_MEGA_FORMS(448, Lucario);
                break;

            case Species::ABOMASNOW:
                SET_STANDARD_MEGA_FORMS(460, Abomasnow);
                break;

            case Species::ROTOM:
                switch(form)
                {
                    case Forms::Rotom::STANDARD:
                        _form_id = form;
                        SET_IMAGES_PATHS("479.png");
                        break;

                    case Forms::Rotom::HEAT:
                        _form_id = form;
                        SET_IMAGES_PATHS("479-heat.png");
                        break;

                    case Forms::Rotom::WASH:
                        _form_id = form;
                        SET_IMAGES_PATHS("479-wash.png");
                        break;

                    case Forms::Rotom::FROST:
                        _form_id = form;
                        SET_IMAGES_PATHS("479-frost.png");
                        break;

                    case Forms::Rotom::FAN:
                        _form_id = form;
                        SET_IMAGES_PATHS("479-fan.png");
                        break;

                    case Forms::Rotom::MOW:
                        _form_id = form;
                        SET_IMAGES_PATHS("479-mow.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::GIRATINA:
                switch(form)
                {
                    case Forms::Giratina::ALTERED:
                        _form_id = form;
                        SET_IMAGES_PATHS("487-altered.png");
                        break;

                    case Forms::Giratina::ORIGIN:
                        _form_id = form;
                        SET_IMAGES_PATHS("487-origin.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::SHAYMIN:
                switch(form)
                {
                    case Forms::Shaymin::LAND:
                        _form_id = form;
                        SET_IMAGES_PATHS("492-land.png");
                        break;

                    case Forms::Shaymin::SKY:
                        _form_id = form;
                        SET_IMAGES_PATHS("492-sky.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::ARCEUS:
                switch(form)
                {
                    case Forms::Arceus::NORMAL:
                        _form_id = form;
                        set_form("Normal");
                        return;

                    case Forms::Arceus::FIRE:
                        _form_id = form;
                        set_form("Fire");
                        return;

                    case Forms::Arceus::WATER:
                        _form_id = form;
                        set_form("Water");
                        return;

                    case Forms::Arceus::ELECTRIC:
                        _form_id = form;
                        set_form("Electric");
                        return;

                    case Forms::Arceus::GRASS:
                        _form_id = form;
                        set_form("Grass");
                        return;

                    case Forms::Arceus::ICE:
                        _form_id = form;
                        set_form("Ice");
                        return;

                    case Forms::Arceus::FIGHTING:
                        _form_id = form;
                        set_form("Fighting");
                        return;

                    case Forms::Arceus::POISON:
                        _form_id = form;
                        set_form("Poison");
                        return;

                    case Forms::Arceus::GROUND:
                        _form_id = form;
                        set_form("Ground");
                        return;

                    case Forms::Arceus::FLYING:
                        _form_id = form;
                        set_form("Flying");
                        return;

                    case Forms::Arceus::PSYCHIC:
                        _form_id = form;
                        set_form("Psychic");
                        return;

                    case Forms::Arceus::BUG:
                        _form_id = form;
                        set_form("Bug");
                        return;

                    case Forms::Arceus::ROCK:
                        _form_id = form;
                        set_form("Rock");
                        return;

                    case Forms::Arceus::GHOST:
                        _form_id = form;
                        set_form("Ghost");
                        return;

                    case Forms::Arceus::DRAGON:
                        _form_id = form;
                        set_form("Dragon");
                        return;

                    case Forms::Arceus::DARK:
                        _form_id = form;
                        set_form("Dark");
                        return;

                    case Forms::Arceus::STEEL:
                        _form_id = form;
                        set_form("Steel");
                        return;

                    case Forms::Arceus::QUESTION_MARK:
                        if(_generation == 4)
                        {
                            _form_id = form;
                            set_form("???");
                            return;
                        }
                        else throw std::runtime_error("Invalid form.");

                    case Forms::Arceus::FAIRY:
                        if(_generation == 6)
                        {
                            _form_id = form;
                            set_form("Fairy");
                            return;
                        }
                        else throw std::runtime_error("Invalid form.");

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::BASCULIN:
                switch(form)
                {
                    case Forms::Basculin::RED_STRIPED:
                        _form_id = form;
                        SET_IMAGES_PATHS("500-red-striped.png");
                        break;

                    case Forms::Basculin::BLUE_STRIPED:
                        _form_id = form;
                        SET_IMAGES_PATHS("550-blue-striped.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
            }
            break;

            case Species::DARMANITAN:
                switch(form)
                {
                    case Forms::Darmanitan::STANDARD:
                        _form_id = form;
                        SET_IMAGES_PATHS("555-standard.png");
                        break;

                    case Forms::Darmanitan::ZEN:
                        _form_id = form;
                        SET_IMAGES_PATHS("555-zen.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::DEERLING:
                switch(form)
                {
                    case Forms::Deerling::SPRING:
                        _form_id = form;
                        set_form("Spring");
                        return;

                    case Forms::Deerling::SUMMER:
                        _form_id = form;
                        set_form("Summer");
                        return;

                    case Forms::Deerling::AUTUMN:
                        _form_id = form;
                        set_form("Autumn");
                        return;

                    case Forms::Deerling::WINTER:
                        _form_id = form;
                        set_form("Winter");
                        return;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                return;

            case Species::SAWSBUCK:
                switch(form)
                {
                    case Forms::Sawsbuck::SPRING:
                        _form_id = form;
                        set_form("Spring");
                        return;

                    case Forms::Sawsbuck::SUMMER:
                        _form_id = form;
                        set_form("Summer");
                        return;

                    case Forms::Sawsbuck::AUTUMN:
                        _form_id = form;
                        set_form("Autumn");
                        return;

                    case Forms::Sawsbuck::WINTER:
                        _form_id = form;
                        set_form("Winter");
                        return;

                    default:
                        throw std::runtime_error("Invalid form.");
                }

            case Species::TORNADUS:
                switch(form)
                {
                    case Forms::Tornadus::INCARNATE:
                        _form_id = form;
                        SET_IMAGES_PATHS("641-incarnate.png");
                        break;

                    case Forms::Tornadus::THERIAN:
                        _form_id = form;
                        SET_IMAGES_PATHS("641-therian.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::THUNDURUS:
                switch(form)
                {
                    case Forms::Thundurus::INCARNATE:
                        _form_id = form;
                        SET_IMAGES_PATHS("642-incarnate.png");
                        break;

                    case Forms::Thundurus::THERIAN:
                        _form_id = form;
                        SET_IMAGES_PATHS("642-therian.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::LANDORUS:
                switch(form)
                {
                    case Forms::Landorus::INCARNATE:
                        _form_id = form;
                        SET_IMAGES_PATHS("645-incarnate.png");
                        break;

                    case Forms::Landorus::THERIAN:
                        _form_id = form;
                        SET_IMAGES_PATHS("645-therian.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::KYUREM:
                switch(form)
                {
                    case Forms::Kyurem::STANDARD:
                        _form_id = form;
                        SET_IMAGES_PATHS("646.png");
                        break;

                    case Forms::Kyurem::BLACK:
                        _form_id = form;
                        SET_IMAGES_PATHS("646-black.png");
                        break;

                    case Forms::Kyurem::WHITE:
                        _form_id = form;
                        SET_IMAGES_PATHS("646-white.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::KELDEO:
                switch(form)
                {
                    case Forms::Keldeo::ORDINARY:
                        _form_id = form;
                        SET_IMAGES_PATHS("647-ordinary.png");
                        break;

                    case Forms::Keldeo::RESOLUTE:
                        _form_id = form;
                        SET_IMAGES_PATHS("647-resolute.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::MELOETTA:
                switch(form)
                {
                    case Forms::Meloetta::ARIA:
                        _form_id = form;
                        SET_IMAGES_PATHS("648-aria.png");
                        break;

                    case Forms::Meloetta::PIROUETTE:
                        _form_id = form;
                        SET_IMAGES_PATHS("648-pirouette.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::GENESECT:
                switch(form)
                {
                    case Forms::Genesect::STANDARD:
                        _form_id = form;
                        SET_IMAGES_PATHS("649.png");
                        break;

                    case Forms::Genesect::DOUSE_DRIVE:
                        _form_id = form;
                        SET_IMAGES_PATHS("649-douse.png");
                        break;

                    case Forms::Genesect::SHOCK_DRIVE:
                        _form_id = form;
                        SET_IMAGES_PATHS("649-shock.png");
                        break;

                    case Forms::Genesect::BURN_DRIVE:
                        _form_id = form;
                        SET_IMAGES_PATHS("649-burn.png");
                        break;

                    case Forms::Genesect::CHILL_DRIVE:
                        _form_id = form;
                        SET_IMAGES_PATHS("649-chill.png");
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            //TODO: Change to real pictures once they're in the images repo
            case Species::VIVILLON:
                if(form == Forms::Vivillon::MEADOW or (form >= Forms::Vivillon::ICY_SNOW and
                                                       form <= Forms::Vivillon::JUNGLE))
                {
                    _form_id = form;
                    SET_POKEBALL_IMAGE();
                }
                else throw std::runtime_error("Invalid form.");
                break;

            case Species::FLABEBE:
                if(form == Forms::Flabebe::RED_FLOWER or (form >= Forms::Flabebe::YELLOW_FLOWER and
                                                          form <= Forms::Flabebe::WHITE_FLOWER))
                {
                    _form_id = form;
                    SET_POKEBALL_IMAGE();
                }
                else throw std::runtime_error("Invalid form.");
                break;

            case Species::FLOETTE:
                if(form == Forms::Floette::RED_FLOWER or (form >= Forms::Floette::YELLOW_FLOWER and
                                                          form <= Forms::Floette::WHITE_FLOWER))
                {
                    _form_id = form;
                    SET_POKEBALL_IMAGE();
                }
                else throw std::runtime_error("Invalid form.");
                break;

            case Species::FLORGES:
                if(form == Forms::Florges::RED_FLOWER or (form >= Forms::Florges::YELLOW_FLOWER and
                                                          form <= Forms::Florges::WHITE_FLOWER))
                {
                    _form_id = form;
                    SET_POKEBALL_IMAGE();
                }
                else throw std::runtime_error("Invalid form.");
                break;

            case Species::FURFROU:
                if(form == Forms::Furfrou::STANDARD or (form >= Forms::Furfrou::HEART_TRIM and
                                                        form <= Forms::Furfrou::PHARAOH_TRIM))
                {
                    _form_id = form;
                    SET_POKEBALL_IMAGE();
                }
                else throw std::runtime_error("Invalid form.");
                break;

            case Species::MEOWSTIC:
                switch(form)
                {
                    case Forms::Meowstic::MALE:
                        //TODO: force male
                        _form_id = form;
                        SET_POKEBALL_IMAGE();
                        break;

                    case Forms::Meowstic::FEMALE:
                        //TODO: force female
                        _form_id = form;
                        SET_POKEBALL_IMAGE();
                        break;

                    default:
                        throw std::runtime_error("Invalid form.");
                }
                break;

            case Species::AEGISLASH:
                if(form == Forms::Aegislash::SHIELD or form == Forms::Aegislash::BLADE)
                {
                    _form_id = form;
                    SET_POKEBALL_IMAGE();
                }
                else throw std::runtime_error("Invalid form.");
                break;

            case Species::PUMPKABOO:
                if(form == Forms::Pumpkaboo::AVERAGE_SIZE or (form >= Forms::Pumpkaboo::SMALL_SIZE and
                                                              form <= Forms::Pumpkaboo::SUPER_SIZE))
                {
                    _form_id = form;
                    SET_POKEBALL_IMAGE();
                }
                else throw std::runtime_error("Invalid form.");
                break;

            case Species::GOURGEIST:
                if(form == Forms::Gourgeist::AVERAGE_SIZE or (form >= Forms::Gourgeist::SMALL_SIZE and
                                                              form <= Forms::Gourgeist::SUPER_SIZE))
                {
                    _form_id = form;
                    SET_POKEBALL_IMAGE();
                }
                else throw std::runtime_error("Invalid form.");
                break;

            case Species::XERNEAS:
                if(form == Forms::Xerneas::ACTIVE or form == Forms::Xerneas::NEUTRAL)
                {
                    _form_id = form;
                    SET_POKEBALL_IMAGE();
                }
                else throw std::runtime_error("Invalid form.");
                break;

            default:
                if(form != _species_id) throw std::runtime_error("Invalid form.");
        }

        std::ostringstream query_stream;
        query_stream << "SELECT pokemon_id FROM pokemon_forms WHERE id=" << _form_id;
        SQLite::Statement pokemon_id_query(*_db, query_stream.str().c_str());
        _pokemon_id = _db->execAndGet(query_stream.str().c_str());

        query_stream.str("");
        query_stream << "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" << _pokemon_id
                     << " AND stat_id IN (1,2,3,4,5,6)";
        SQLite::Statement stats_query(*_db, query_stream.str().c_str());

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
        if(_generation < 6) _set_old_values();

        _type1_id = stats_query.getColumn(0);
        if(stats_query.executeStep()) _type2_id = stats_query.getColumn(0);

        form_signal1();
        form_signal2();
    }

    #define SET_STANDARD_MEGA_FORMS_FROM_STR(name) if(form == "Standard") set_form(Forms::name::STANDARD); \
                                                   else if(form == "Mega") set_form(Forms::name::MEGA); \
                                                   else throw std::runtime_error("Invalid form.");

    void base_pokemon_modernimpl::set_form(std::string form)
    {
        boost::format png_format("%d.png");
        std::string gen_string = "generation-" + to_string(_generation);
        std::string icons = fs::path(fs::path(get_images_dir()) / "pokemon-icons").string();
        std::string sprites = fs::path(fs::path(get_images_dir()) / gen_string
                                     / _images_game_string.c_str()).string();
        std::string s_sprites = fs::path(fs::path(sprites) / "shiny").string();

        switch(_species_id)
        {
            case Species::VENUSAUR:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Venusaur);
                return;

            case Species::CHARIZARD:
                if(form == "Standard") set_form(Forms::Charizard::STANDARD);
                if(form == "Mega X") set_form(Forms::Charizard::MEGA_X);
                if(form == "Mega Y") set_form(Forms::Charizard::MEGA_Y);
                return;

            case Species::BLASTOISE:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Blastoise);
                return;

            case Species::ALAKAZAM:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Alakazam);
                return;

            case Species::GENGAR:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Gengar);
                return;

            case Species::KANGASKHAN:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Kangaskhan);
                return;

            case Species::PINSIR:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Pinsir);
                return;

            case Species::GYARADOS:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Gyarados);
                return;

            case Species::AERODACTYL:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Aerodactyl);
                return;

            case Species::MEWTWO:
                if(form == "Standard") set_form(Forms::Mewtwo::STANDARD);
                if(form == "Mega X") set_form(Forms::Mewtwo::MEGA_X);
                if(form == "Mega Y") set_form(Forms::Mewtwo::MEGA_Y);
                return;

            case Species::PICHU:
                if(form == "Standard") set_form(Forms::Pichu::STANDARD);
                else if(form == "Spiky-eared") set_form(Forms::Pichu::SPIKY_EARED);
                return;

            case Species::AMPHAROS:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Ampharos);
                return;

            case Species::UNOWN:
            {
                char letter = boost::algorithm::to_lower_copy(form)[0];
                if(letter >= 'a' and letter <= 'z')
                {
                    SET_IMAGES_PATHS(str(boost::format("201-%c.png") % letter))
                }
                else if(letter == '?')
                {
                    SET_IMAGES_PATHS("201-question.png")
                }
                else if(letter == '!')
                {
                    SET_IMAGES_PATHS("201-exclamation.png")
                }
                else throw std::runtime_error("Invalid form.");
                break;
            }

            case Species::SCIZOR:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Scizor);
                return;

            case Species::HERACROSS:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Heracross);
                return;

            case Species::HOUNDOOM:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Houndoom);
                return;

            case Species::TYRANITAR:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Tyranitar);
                return;

            case Species::BLAZIKEN:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Blaziken);
                return;

            case Species::GARDEVOIR:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Gardevoir);
                return;

            case Species::MAWILE:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Mawile);
                return;

            case Species::AGGRON:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Aggron);
                return;

            case Species::MEDICHAM:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Medicham);
                return;

            case Species::MANECTRIC:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Manectric);
                return;

            case Species::CASTFORM:
                if(form == "Normal") set_form(Forms::Castform::STANDARD);
                else if(form == "Sunny") set_form(Forms::Castform::SUNNY);
                else if(form == "Rainy") set_form(Forms::Castform::RAINY);
                else if(form == "Snowy") set_form(Forms::Castform::SNOWY);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::BANETTE:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Banette);
                return;

            case Species::ABSOL:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Absol);
                return;

            case Species::DEOXYS:
                if(form == "Normal") set_form(Forms::Deoxys::NORMAL);
                else if(form == "Attack") set_form(Forms::Deoxys::ATTACK);
                else if(form == "Defense") set_form(Forms::Deoxys::DEFENSE);
                else if(form == "Speed") set_form(Forms::Deoxys::SPEED);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::BURMY:
                if(form == "Plant Cloak") set_form(Forms::Burmy::PLANT_CLOAK);
                else if(form == "Sandy Cloak") set_form(Forms::Burmy::SANDY_CLOAK);
                else if(form == "Trash Cloak") set_form(Forms::Burmy::TRASH_CLOAK);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::WORMADAM:
                if(form == "Plant Cloak") set_form(Forms::Wormadam::PLANT_CLOAK);
                else if(form == "Sandy Cloak") set_form(Forms::Wormadam::SANDY_CLOAK);
                else if(form == "Trash Cloak") set_form(Forms::Wormadam::TRASH_CLOAK);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::CHERRIM:
                if(form == "Overcast") set_form(Forms::Cherrim::OVERCAST);
                else if(form == "Sunshine") set_form(Forms::Cherrim::SUNSHINE);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::SHELLOS:
                if(form == "West Sea") set_form(Forms::Shellos::WEST_SEA);
                else if(form == "East Sea") set_form(Forms::Shellos::EAST_SEA);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::GASTRODON:
                if(form == "West Sea") set_form(Forms::Gastrodon::WEST_SEA);
                else if(form == "East Sea") set_form(Forms::Gastrodon::EAST_SEA);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::GARCHOMP:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Garchomp);
                return;

            case Species::LUCARIO:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Lucario);
                return;

            case Species::ABOMASNOW:
                SET_STANDARD_MEGA_FORMS_FROM_STR(Abomasnow);
                return;

            case Species::ROTOM:
                if(form == "Normal") set_form(Forms::Rotom::STANDARD);
                else if(form == "Heat") set_form(Forms::Rotom::HEAT);
                else if(form == "Frost") set_form(Forms::Rotom::FROST);
                else if(form == "Wash") set_form(Forms::Rotom::WASH);
                else if(form == "Fan") set_form(Forms::Rotom::FAN);
                else if(form == "Mow") set_form(Forms::Rotom::MOW);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::GIRATINA:
                if(form == "Altered") set_form(Forms::Giratina::ALTERED);
                else if(form == "Origin") set_form(Forms::Giratina::ORIGIN);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::SHAYMIN:
                if(form == "Land") set_form(Forms::Shaymin::LAND);
                else if(form == "Sky") set_form(Forms::Shaymin::SKY);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::ARCEUS:
            {
                std::vector<std::string> type_vec;
                get_type_list(type_vec, 4);

                if(find(type_vec.begin(), type_vec.end(), form) != type_vec.end())
                {
                    _type1_id = database::get_type_id(form);
                    _type2_id = Types::NONE;
                    transform(form.begin(), form.end(), form.begin(), ::tolower);
                    string basename = (boost::format("493-%s.png") % form).str();

                    SET_IMAGES_PATHS(basename);
                }
                else throw std::runtime_error("Invalid form.");
                break;
            }

            case Species::BASCULIN:
                if(form == "Red-Striped") set_form(Forms::Basculin::RED_STRIPED);
                else if(form == "Blue-Striped") set_form(Forms::Basculin::BLUE_STRIPED);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::DARMANITAN:
                if(form == "Standard") set_form(Forms::Darmanitan::STANDARD);
                else if(form == "Zen") set_form(Forms::Darmanitan::ZEN);
                else throw std::runtime_error("Invalid form.");
                return; 

            case Species::DEERLING:
            case Species::SAWSBUCK:
                if(form == "Spring" or form == "Summer" or form == "Autumn" or form == "Winter")
                {
                    std::transform(form.begin(), form.end(), form.begin(), ::tolower);
                    std::string basename = (boost::format("%d-%s.png") % _species_id % form).str();

                    SET_IMAGES_PATHS(basename);
                }
                else throw std::runtime_error("Invalid form.");
                break;

            case Species::TORNADUS:
                if(form == "Incarnate") set_form(Forms::Tornadus::INCARNATE);
                else if(form == "Therian") set_form(Forms::Tornadus::THERIAN);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::THUNDURUS:
                if(form == "Incarnate") set_form(Forms::Thundurus::INCARNATE);
                else if(form == "Therian") set_form(Forms::Thundurus::THERIAN);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::LANDORUS:
                if(form == "Incarnate") set_form(Forms::Landorus::INCARNATE);
                else if(form == "Therian") set_form(Forms::Landorus::THERIAN);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::KYUREM:
                if(form == "Normal") set_form(Forms::Kyurem::STANDARD);
                else if(form == "Black") set_form(Forms::Kyurem::BLACK);
                else if(form == "White") set_form(Forms::Kyurem::WHITE);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::KELDEO:
                if(form == "Ordinary") set_form(Forms::Keldeo::ORDINARY);
                else if(form == "Resolute") set_form(Forms::Keldeo::RESOLUTE);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::MELOETTA:
                if(form == "Aria") set_form(Forms::Meloetta::ARIA);
                else if(form == "Pirouette") set_form(Forms::Meloetta::PIROUETTE);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::GENESECT:
                if(form == "Normal") set_form(Forms::Genesect::STANDARD);
                else if(form == "Shock Drive") set_form(Forms::Genesect::SHOCK_DRIVE);
                else if(form == "Burn Drive") set_form(Forms::Genesect::BURN_DRIVE);
                else if(form == "Chill Drive") set_form(Forms::Genesect::CHILL_DRIVE);
                else if(form == "Douse Drive") set_form(Forms::Genesect::DOUSE_DRIVE);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::VIVILLON:
                if(form == "Meadow") set_form(Forms::Vivillon::MEADOW);
                else if(form == "Icy Snow") set_form(Forms::Vivillon::ICY_SNOW);
                else if(form == "Polar") set_form(Forms::Vivillon::POLAR);
                else if(form == "Tundra") set_form(Forms::Vivillon::TUNDRA);
                else if(form == "Continental") set_form(Forms::Vivillon::CONTINENTAL);
                else if(form == "Garden") set_form(Forms::Vivillon::GARDEN);
                else if(form == "Elegant") set_form(Forms::Vivillon::ELEGANT);
                else if(form == "Modern") set_form(Forms::Vivillon::MODERN);
                else if(form == "Marine") set_form(Forms::Vivillon::MARINE);
                else if(form == "Archipelago") set_form(Forms::Vivillon::ARCHIPELAGO);
                else if(form == "High Plains") set_form(Forms::Vivillon::HIGH_PLAINS);
                else if(form == "Sandstorm") set_form(Forms::Vivillon::SANDSTORM);
                else if(form == "River") set_form(Forms::Vivillon::RIVER);
                else if(form == "Monsoon") set_form(Forms::Vivillon::MONSOON);
                else if(form == "Savanna") set_form(Forms::Vivillon::SAVANNA);
                else if(form == "Sun") set_form(Forms::Vivillon::SUN);
                else if(form == "Ocean") set_form(Forms::Vivillon::OCEAN);
                else if(form == "Jungle") set_form(Forms::Vivillon::JUNGLE);
                else throw std::runtime_error("Invalid form.");
                return;

                //TODO: Setting Flabebe, Floette, and Florges images will likely be done here in same case statement
            case Species::FLABEBE:
                if(form == "Red Flower") set_form(Forms::Flabebe::RED_FLOWER);
                else if(form == "Yellow Flower") set_form(Forms::Flabebe::YELLOW_FLOWER);
                else if(form == "Orange Flower") set_form(Forms::Flabebe::ORANGE_FLOWER);
                else if(form == "Blue Flower") set_form(Forms::Flabebe::BLUE_FLOWER);
                else if(form == "White Flower") set_form(Forms::Flabebe::WHITE_FLOWER);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::FLOETTE:
                if(form == "Red Flower") set_form(Forms::Floette::RED_FLOWER);
                else if(form == "Yellow Flower") set_form(Forms::Floette::YELLOW_FLOWER);
                else if(form == "Orange Flower") set_form(Forms::Floette::ORANGE_FLOWER);
                else if(form == "Blue Flower") set_form(Forms::Floette::BLUE_FLOWER);
                else if(form == "White Flower") set_form(Forms::Floette::WHITE_FLOWER);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::FLORGES:
                if(form == "Red Flower") set_form(Forms::Florges::RED_FLOWER);
                else if(form == "Yellow Flower") set_form(Forms::Florges::YELLOW_FLOWER);
                else if(form == "Orange Flower") set_form(Forms::Florges::ORANGE_FLOWER);
                else if(form == "Blue Flower") set_form(Forms::Florges::BLUE_FLOWER);
                else if(form == "White Flower") set_form(Forms::Florges::WHITE_FLOWER);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::FURFROU:
                if(form == "Standard") set_form(Forms::Furfrou::STANDARD);
                else if(form == "Heart Trim") set_form(Forms::Furfrou::HEART_TRIM);
                else if(form == "Star Trim") set_form(Forms::Furfrou::STAR_TRIM);
                else if(form == "Diamond Trim") set_form(Forms::Furfrou::DIAMOND_TRIM);
                else if(form == "Debutante Trim") set_form(Forms::Furfrou::DEBUTANTE_TRIM);
                else if(form == "Matron Trim") set_form(Forms::Furfrou::MATRON_TRIM);
                else if(form == "Dandy Trim") set_form(Forms::Furfrou::DANDY_TRIM);
                else if(form == "La Reine Trim") set_form(Forms::Furfrou::LA_REINE_TRIM);
                else if(form == "Kabuki Trim") set_form(Forms::Furfrou::KABUKI_TRIM);
                else if(form == "Pharaoh Trim") set_form(Forms::Furfrou::PHARAOH_TRIM);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::MEOWSTIC:
                if(form == "Male") set_form(Forms::Meowstic::MALE);
                else if(form == "Female") set_form(Forms::Meowstic::FEMALE);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::AEGISLASH:
                if(form == "Shield") set_form(Forms::Aegislash::SHIELD);
                else if(form == "Blade") set_form(Forms::Aegislash::BLADE);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::PUMPKABOO:
                if(form == "Average Size") set_form(Forms::Pumpkaboo::AVERAGE_SIZE);
                else if(form == "Small Size") set_form(Forms::Pumpkaboo::SMALL_SIZE);
                else if(form == "Large Size") set_form(Forms::Pumpkaboo::LARGE_SIZE);
                else if(form == "Super Size") set_form(Forms::Pumpkaboo::SUPER_SIZE);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::GOURGEIST:
                if(form == "Average Size") set_form(Forms::Gourgeist::AVERAGE_SIZE);
                else if(form == "Small Size") set_form(Forms::Gourgeist::SMALL_SIZE);
                else if(form == "Large Size") set_form(Forms::Gourgeist::LARGE_SIZE);
                else if(form == "Super Size") set_form(Forms::Gourgeist::SUPER_SIZE);
                else throw std::runtime_error("Invalid form.");
                return;

            case Species::XERNEAS:
                if(form == "Active") set_form(Forms::Xerneas::ACTIVE);
                else if(form == "Neutral") set_form(Forms::Xerneas::NEUTRAL);
                else throw std::runtime_error("Invalid form.");
                return;

            default:
                if(form != "Standard") throw std::runtime_error("Invalid form.");
        }

        std::ostringstream query_stream;
        query_stream << "SELECT pokemon_id FROM pokemon_forms WHERE id=" << _form_id;
        _pokemon_id = _db->execAndGet(query_stream.str().c_str());

        query_stream.str("");
        query_stream << "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" << _pokemon_id
                     << " AND stat_id IN (1,2,3,4,5,6)";
        SQLite::Statement stats_query(*_db, query_stream.str().c_str());

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
        if(_generation < 6) _set_old_values();

        form_signal1();
        form_signal2();
    }

    string base_pokemon_modernimpl::get_icon_path(bool is_male) const
    {
        if(_generation > 3 and not is_male) return _female_icon_path.string();
        else return _male_icon_path.string();
    }

    string base_pokemon_modernimpl::get_sprite_path(bool is_male, bool is_shiny) const
    {
        if(is_male)
        {
            if(is_shiny) return _male_shiny_sprite_path.string();
            else return _male_sprite_path.string();
        }
        else
        {
            if(is_shiny) return _female_shiny_sprite_path.string();
            else return _female_sprite_path.string();
        }
    }
	
    void base_pokemon_modernimpl::get_egg_group_ids(std::vector<unsigned int>& egg_group_id_vec) const
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
                SQLite::Statement query(*_db, query_string.c_str());

                while(query.executeStep()) egg_group_id_vec.push_back(int(query.getColumn(0)));
                break;
        }
    }
} /* namespace pkmn */
