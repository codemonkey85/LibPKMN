/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifdef _MSC_VER
#pragma warning(disable:4800)
#endif

#include <string>
#include <vector>

#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/locale/encoding_utf.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/item.hpp>
#include <pkmn/move.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/team_pokemon.hpp>
#include <pkmn/database/queries.hpp>
#include <pkmn/types/pokemon_text.hpp>

#include <pkmds/pkmds_g5_sqlite.h>

#include "pokemon.hpp"
#include "../library_bridge.hpp"
#include "../SQLiteCpp/src/SQLiteC++.h"

using namespace std;

namespace pkmn
{
    namespace conversions
    {
        //Conversions from SQLite database values for growth ID's to LibSPEC's enums
        stat_growth_rate_t libspec_growth_rates[6] = {STAT_GROWTH_RATE_ERRATIC, STAT_GROWTH_RATE_FAST,
                                                      STAT_GROWTH_RATE_MEDIUM_FAST, STAT_GROWTH_RATE_MEDIUM_SLOW,
                                                      STAT_GROWTH_RATE_SLOW, STAT_GROWTH_RATE_FLUCTUATING};

        //Used with item index functions
        uint8_t gen3_game_ids[] = {Games::NONE, Games::RUBY, Games::EMERALD, Games::FIRE_RED};

        SQLite::Database _db(get_database_path().c_str());

        team_pokemon::sptr import_gen3_pokemon(pk3_box_t* pkmn, gba_savetype_t save_type)
        {
            pk3_decrypt(pkmn);
            //Avoiding bitfields and structs
            uint16_t* met_int = reinterpret_cast<uint16_t*>(&(pkmn->met_loc)+1);
            uint32_t* IV_int = reinterpret_cast<uint32_t*>(&(pkmn->iv));
            uint8_t* marking_int = reinterpret_cast<uint8_t*>(&(pkmn->markings));
            uint32_t* ribbon_int = reinterpret_cast<uint32_t*>(&(pkmn->ribbon));

            uint16_t game_id = hometown_to_libpkmn_game((*met_int & 0x780) >> 7);

            //Check for invalid values
            if(pkmn->species == 0 or
               (pkmn->species > 251 and pkmn->species < 277) or
               pkmn->species == 412) //TODO: investigate how egg works
            {
                return team_pokemon::make(Species::INVALID, game_id, 0,
                                          Moves::NONE, Moves::NONE, Moves::NONE, Moves::NONE);
            }

            uint16_t species_id = database::get_species_id(pkmn->species, game_id);

            //LibSPEC has no way to link Pokemon to growth rates, so use database
            std::string query_string(str(boost::format("SELECT growth_rate_id FROM pokemon_species WHERE id=%d")
                                         % pkmn->species));
            uint8_t growth_rate = int(_db.execAndGet(query_string.c_str())) - 1;

            uint8_t level = stat_get_level(stat_growth_rate_t(growth_rate), pkmn->exp);

            team_pokemon::sptr t_pkmn = team_pokemon::make(species_id, game_id, level,
                                                           pkmn->move[0], pkmn->move[1],
                                                           pkmn->move[2], pkmn->move[3]);

            //Convert nickname and trainer name
            uint16_t nickname_arr[10];
            gba_text_to_ucs2((char16_t*)nickname_arr, (char8_t*)pkmn->nickname, PK3_NICKNAME_LENGTH);
            t_pkmn->set_nickname(boost::locale::conv::utf_to_utf<wchar_t>(nickname_arr));

            uint16_t trainername_arr[10];
            gba_text_to_ucs2((char16_t*)trainername_arr, (char8_t*)pkmn->ot_name, PK3_OT_NAME_LENGTH);
            t_pkmn->set_trainer_name(boost::locale::conv::utf_to_utf<wchar_t>(trainername_arr));

            //Item
            uint16_t item_id = database::get_item_id(pkmn->held_item, gen3_game_ids[save_type]);
            t_pkmn->set_held_item(database::get_item_name(item_id));

            //ID's
            t_pkmn->set_personality(pkmn->pid);
            t_pkmn->set_trainer_id(pkmn->ot_fid);

            //Move PP's
            //TODO: take PP Ups into account
            t_pkmn->set_move_PP(pkmn->move_pp[0], 1);
            t_pkmn->set_move_PP(pkmn->move_pp[1], 2);
            t_pkmn->set_move_PP(pkmn->move_pp[2], 3);
            t_pkmn->set_move_PP(pkmn->move_pp[3], 4);

            //Effort values
            t_pkmn->set_EV("HP", pkmn->ev.hp);
            t_pkmn->set_EV("Attack", pkmn->ev.atk);
            t_pkmn->set_EV("Defense", pkmn->ev.def);
            t_pkmn->set_EV("Speed", pkmn->ev.spd);
            t_pkmn->set_EV("Special Attack", pkmn->ev.satk);
            t_pkmn->set_EV("Special Defense", pkmn->ev.sdef);

            //Individual values
            t_pkmn->set_IV("HP", modern_get_IV(IV_int, Stats::HP));
            t_pkmn->set_IV("Attack", modern_get_IV(IV_int, Stats::ATTACK));
            t_pkmn->set_IV("Defense", modern_get_IV(IV_int, Stats::DEFENSE));
            t_pkmn->set_IV("Speed", modern_get_IV(IV_int, Stats::SPEED));
            t_pkmn->set_IV("Special Attack", modern_get_IV(IV_int, Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV("Special Defense", modern_get_IV(IV_int, Stats::SPECIAL_DEFENSE));

            t_pkmn->set_met_level(get_gen3_met_level(met_int));
            t_pkmn->set_ball(ball_dict.at(game_ball_to_libpkmn_ball(get_gen3_ball(met_int)), "Poke Ball"));
            if(get_gen3_otgender(met_int)) t_pkmn->set_trainer_gender("Female");
            else t_pkmn->set_trainer_gender("Male");

            //Attributes
            t_pkmn->set_attribute("friendship", pkmn->friendship);
            t_pkmn->set_attribute("circle", get_marking(marking_int, Markings::CIRCLE));
            t_pkmn->set_attribute("triangle", get_marking(marking_int, Markings::TRIANGLE));
            t_pkmn->set_attribute("square", get_marking(marking_int, Markings::SQUARE));
            t_pkmn->set_attribute("heart", get_marking(marking_int, Markings::HEART));
            t_pkmn->set_attribute("country", pkmn->language);

            t_pkmn->set_attribute("cool", pkmn->contest.cool);
            t_pkmn->set_attribute("beauty", pkmn->contest.beauty);
            t_pkmn->set_attribute("cute", pkmn->contest.cute);
            t_pkmn->set_attribute("smart", pkmn->contest.smart);
            t_pkmn->set_attribute("tough", pkmn->contest.tough);
            t_pkmn->set_attribute("sheen", pkmn->contest.sheen);

            t_pkmn->set_attribute("hoenn_cool_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::COOL));
            t_pkmn->set_attribute("hoenn_cool_super_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::COOL_SUPER));
            t_pkmn->set_attribute("hoenn_cool_hyper_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::COOL_HYPER));
            t_pkmn->set_attribute("hoenn_cool_master_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::COOL_MASTER));
            t_pkmn->set_attribute("hoenn_beauty_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::BEAUTY));
            t_pkmn->set_attribute("hoenn_beauty_super_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::BEAUTY_SUPER));
            t_pkmn->set_attribute("hoenn_beauty_hyper_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::BEAUTY_HYPER));
            t_pkmn->set_attribute("hoenn_beauty_master_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::BEAUTY_MASTER));
            t_pkmn->set_attribute("hoenn_cute_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::CUTE));
            t_pkmn->set_attribute("hoenn_cute_super_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::CUTE_SUPER));
            t_pkmn->set_attribute("hoenn_cute_hyper_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::CUTE_HYPER));
            t_pkmn->set_attribute("hoenn_cute_master_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::CUTE_MASTER));
            t_pkmn->set_attribute("hoenn_smart_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::SMART));
            t_pkmn->set_attribute("hoenn_smart_super_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::SMART_SUPER));
            t_pkmn->set_attribute("hoenn_smart_hyper_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::SMART_HYPER));
            t_pkmn->set_attribute("hoenn_smart_master_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::SMART_MASTER));
            t_pkmn->set_attribute("hoenn_tough_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::TOUGH));
            t_pkmn->set_attribute("hoenn_tough_super_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::TOUGH_SUPER));
            t_pkmn->set_attribute("hoenn_tough_hyper_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::TOUGH_HYPER));
            t_pkmn->set_attribute("hoenn_tough_master_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::TOUGH_MASTER));
            
            t_pkmn->set_attribute("hoenn_champion_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::CHAMPION));
            t_pkmn->set_attribute("hoenn_winning_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::WINNING));
            t_pkmn->set_attribute("hoenn_victory_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::VICTORY));
            t_pkmn->set_attribute("hoenn_artist_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::ARTIST));
            t_pkmn->set_attribute("hoenn_effort_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::EFFORT));
            t_pkmn->set_attribute("hoenn_marine_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::MARINE));
            t_pkmn->set_attribute("hoenn_land_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::LAND));
            t_pkmn->set_attribute("hoenn_sky_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::SKY));
            t_pkmn->set_attribute("hoenn_country_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::COUNTRY));
            t_pkmn->set_attribute("hoenn_national_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::NATIONAL));
            t_pkmn->set_attribute("hoenn_earth_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::EARTH));
            t_pkmn->set_attribute("hoenn_world_ribbon", get_ribbon(ribbon_int, Ribbons::Hoenn::WORLD));
            pk3_encrypt(pkmn);
            
            return t_pkmn;
        }

        team_pokemon::sptr import_gen3_pokemon(pk3_t* pkmn, gba_savetype_t save_type)
        {
            return import_gen3_pokemon(&(pkmn->box), save_type);
        }

        void export_gen3_pokemon(team_pokemon::sptr t_pkmn, pk3_box_t* pkmn, gba_savetype_t save_type)
        {
            pk3_decrypt(pkmn);

            //Avoiding bitfields and structs
            uint8_t* ppup_int = reinterpret_cast<uint8_t*>(&(pkmn->pp_up));
            uint8_t* marking_int = reinterpret_cast<uint8_t*>(&(pkmn->markings));
            uint16_t* met_int = reinterpret_cast<uint16_t*>(&(pkmn->met_loc)+1);
            uint32_t* IV_int = reinterpret_cast<uint32_t*>(&(pkmn->iv));
            uint32_t* ribbon_int = reinterpret_cast<uint32_t*>(&(pkmn->ribbon));
            pkmn::dict<std::string, int> attributes = t_pkmn->get_attributes();
            pkmn::dict<std::string, unsigned int> EVs = t_pkmn->get_EVs();
            pkmn::dict<std::string, unsigned int> IVs = t_pkmn->get_IVs();

            //ID's
            pkmn->pid = t_pkmn->get_personality();
            pkmn->ot_fid = t_pkmn->get_trainer_id();

            //Nickname
            std::basic_string<uint16_t> nickname(boost::locale::conv::utf_to_utf<uint16_t>(t_pkmn->get_nickname().std_wstring()));
            memset(pkmn->nickname, 0xFF, PK3_NICKNAME_LENGTH);
            ucs2_to_gba_text((char8_t*)pkmn->nickname, (char16_t*)nickname.c_str(), nickname.size());

            pkmn->language = 0x202; //Default to English

            //Trainer Name
            std::basic_string<uint16_t> trainer_name(boost::locale::conv::utf_to_utf<uint16_t>(t_pkmn->get_trainer_name().std_wstring()));
            memset(pkmn->ot_name, 0xFF, PK3_OT_NAME_LENGTH);
            ucs2_to_gba_text((char8_t*)pkmn->ot_name, (char16_t*)trainer_name.c_str(), trainer_name.size());

            //Markings
            set_marking(marking_int, Markings::CIRCLE, attributes.at("circle", 0));
            set_marking(marking_int, Markings::TRIANGLE, attributes.at("triangle", 0));
            set_marking(marking_int, Markings::SQUARE, attributes.at("square", 0));
            set_marking(marking_int, Markings::HEART, attributes.at("heart", 0));

            pkmn->species = t_pkmn->get_species_id();
            pkmn->held_item = database::get_item_index(t_pkmn->get_held_item()->get_item_id(),
                                                       t_pkmn->get_game_id());
            //TODO: store actual experience, currently set it to what level needs
            //LibSPEC has no way to link Pokemon to growth rates, so use database
            std::string query_string(str(boost::format("SELECT growth_rate_id FROM pokemon_species WHERE id=%d")
                                         % pkmn->species));
            uint8_t growth_rate = int(_db.execAndGet(query_string.c_str())) - 1;
            switch(growth_rate)
            {
                case STAT_GROWTH_RATE_ERRATIC:
                    pkmn->exp = STAT_TOTAL_EXP_ERRATIC[t_pkmn->get_level() - 1];
                    break;

                case STAT_GROWTH_RATE_FAST:
                    pkmn->exp = STAT_TOTAL_EXP_FAST[t_pkmn->get_level() - 1];
                    break;

                case STAT_GROWTH_RATE_MEDIUM_FAST:
                    pkmn->exp = STAT_TOTAL_EXP_MEDIUM_FAST[t_pkmn->get_level() - 1];
                    break;

                case STAT_GROWTH_RATE_MEDIUM_SLOW:
                    pkmn->exp = STAT_TOTAL_EXP_MEDIUM_SLOW[t_pkmn->get_level() - 1];
                    break;

                case STAT_GROWTH_RATE_SLOW:
                    pkmn->exp = STAT_TOTAL_EXP_SLOW[t_pkmn->get_level() - 1];
                    break;

                default:
                    pkmn->exp = STAT_TOTAL_EXP_FLUCTUATING[t_pkmn->get_level() - 1];
                    break;
            }
            *ppup_int = 0;
            pkmn->friendship = attributes.at("friendship",70); //TODO: find base friendships for each species

            //Moves
            moveset_t moves;
            std::vector<unsigned int> move_PPs;
            t_pkmn->get_moves(moves);
            t_pkmn->get_move_PPs(move_PPs);
            pkmn->move[0] = moves[0]->get_move_id();
            pkmn->move[1] = moves[1]->get_move_id();
            pkmn->move[2] = moves[2]->get_move_id();
            pkmn->move[3] = moves[3]->get_move_id();
            pkmn->move_pp[0] = move_PPs[0];
            pkmn->move_pp[1] = move_PPs[1];
            pkmn->move_pp[2] = move_PPs[2];
            pkmn->move_pp[3] = move_PPs[3];

            //EVs
            pkmn->ev.hp = EVs["HP"];
            pkmn->ev.atk = EVs["Attack"];
            pkmn->ev.def = EVs["Defense"];
            pkmn->ev.spd = EVs["Speed"];
            pkmn->ev.satk = EVs["Special Attack"];
            pkmn->ev.sdef = EVs["Special Defense"];

            //Contest
            pkmn->contest.cool = attributes.at("cool",0);
            pkmn->contest.beauty = attributes.at("beauty",0);
            pkmn->contest.cute = attributes.at("cute",0);
            pkmn->contest.smart = attributes.at("smart",0);
            pkmn->contest.tough = attributes.at("tough",0);
            pkmn->contest.sheen = attributes.at("sheen",0);

            //Origin Info

            //TODO: Pokerus
            //TODO: Met location
            set_gen3_met_level(met_int, t_pkmn->get_met_level());
            //TODO: allow game_id to be different than met game
            set_gen3_ball(met_int, reverse_ball_dict[t_pkmn->get_ball()]);
            set_gen3_otgender(met_int, (t_pkmn->get_trainer_gender() == "Female"));

            //IVs
            modern_set_IV(IV_int, Stats::HP, IVs["HP"]);
            modern_set_IV(IV_int, Stats::ATTACK, IVs["Attack"]);
            modern_set_IV(IV_int, Stats::DEFENSE, IVs["Defense"]);
            modern_set_IV(IV_int, Stats::SPEED, IVs["Speed"]);
            modern_set_IV(IV_int, Stats::SPECIAL_ATTACK, IVs["Special Attack"]);
            modern_set_IV(IV_int, Stats::SPECIAL_DEFENSE, IVs["Special Defense"]);

            //Misc
            query_string = str(boost::format("SELECT slot FROM pokemon_abilities WHERE pokemon_id=%d AND ability_id=%d")
                               % t_pkmn->get_pokemon_id()
                               % t_pkmn->get_ability_id());
            *IV_int |= (int(_db.execAndGet(query_string.c_str())) % 2) ? 0 : 1;

            //Ribbons
            set_ribbon(ribbon_int, Ribbons::Hoenn::COOL, attributes.at("hoenn_cool_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::COOL_SUPER, attributes.at("hoenn_cool_super_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::COOL_HYPER, attributes.at("hoenn_cool_hyper_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::COOL_MASTER, attributes.at("hoenn_cool_master_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::BEAUTY, attributes.at("hoenn_beauty_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::BEAUTY_SUPER, attributes.at("hoenn_beauty_super_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::BEAUTY_HYPER, attributes.at("hoenn_beauty_hyper_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::BEAUTY_MASTER, attributes.at("hoenn_beauty_master_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::CUTE, attributes.at("hoenn_cute_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::CUTE_SUPER, attributes.at("hoenn_cute_super_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::CUTE_HYPER, attributes.at("hoenn_cute_hyper_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::CUTE_MASTER, attributes.at("hoenn_cute_master_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::SMART, attributes.at("hoenn_smart_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::SMART_SUPER, attributes.at("hoenn_smart_super_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::SMART_HYPER, attributes.at("hoenn_smart_hyper_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::SMART_MASTER, attributes.at("hoenn_smart_master_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::TOUGH, attributes.at("hoenn_tough_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::TOUGH_SUPER, attributes.at("hoenn_tough_super_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::TOUGH_HYPER, attributes.at("hoenn_tough_hyper_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::TOUGH_MASTER, attributes.at("hoenn_tough_master_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::CHAMPION, attributes.at("hoenn_champion_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::WINNING, attributes.at("hoenn_winning_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::VICTORY, attributes.at("hoenn_victory_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::ARTIST, attributes.at("hoenn_artist_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::EFFORT, attributes.at("hoenn_effort_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::MARINE, attributes.at("hoenn_marine_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::LAND, attributes.at("hoenn_land_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::SKY, attributes.at("hoenn_sky_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::COUNTRY, attributes.at("hoenn_country_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::NATIONAL, attributes.at("hoenn_national_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::EARTH, attributes.at("hoenn_earth_ribbon",false));
            set_ribbon(ribbon_int, Ribbons::Hoenn::WORLD, attributes.at("hoenn_world_ribbon",false));

            pk3_encrypt(pkmn);
        }

        void export_gen3_pokemon(team_pokemon::sptr t_pkmn, pk3_t* pkmn, gba_savetype_t save_type)
        {
            uint8_t* status_int = reinterpret_cast<uint8_t*>(&(pkmn->party));
            pkmn::dict<std::string, unsigned int> stats = t_pkmn->get_stats();

            export_gen3_pokemon(t_pkmn, &(pkmn->box), save_type);
            *status_int = 0;
            pkmn->party.level = t_pkmn->get_level();
            pkmn->party.stats.hp = stats["HP"];
            pkmn->party.stats.max_hp = stats["HP"];
            pkmn->party.stats.atk = stats["Attack"];
            pkmn->party.stats.def = stats["Defense"];
            pkmn->party.stats.spd = stats["Speed"];
            pkmn->party.stats.satk = stats["Special Attack"];
            pkmn->party.stats.sdef = stats["Special Defense"];
        }

        team_pokemon::sptr import_nds_pokemon(pkm_box_t* pkmn)
        {
            pkm_decrypt(pkmn);
            //Avoiding bitfields and structs
            uint8_t* met_int = reinterpret_cast<uint8_t*>(&(pkmn->pokeball)+1);
            uint32_t* IV_int = reinterpret_cast<uint32_t*>(&(pkmn->iv));
            uint8_t* marking_int = reinterpret_cast<uint8_t*>(&(pkmn->markings));
            uint32_t* hoenn_ribbon_int = reinterpret_cast<uint32_t*>(&(pkmn->ribbon_hoenn1));
            uint32_t* sinnoh_unova_ribbon12_int = reinterpret_cast<uint32_t*>(&(pkmn->ribbon_sinnoh1));
            uint32_t* sinnoh_ribbon34_int = reinterpret_cast<uint32_t*>(&(pkmn->ribbon_sinnoh3));

            uint16_t game_id = hometown_to_libpkmn_game(pkmn->hometown);
            uint16_t species_id = database::get_species_id(pkmn->species, game_id);

            if(species_id == 0)
            {
                return team_pokemon::make(Species::INVALID, game_id, 0,
                                          Moves::NONE, Moves::NONE, Moves::NONE, Moves::NONE);
            }

            //LibSPEC has no way to link Pokemon to growth rates, so use database
            std::string query_string(str(boost::format("SELECT growth_rate_id FROM pokemon_species WHERE id=%d")
                                         % pkmn->species));
            uint8_t growth_rate = int(_db.execAndGet(query_string.c_str())) - 1;

            uint8_t level = stat_get_level(stat_growth_rate_t(growth_rate), pkmn->exp);

            team_pokemon::sptr t_pkmn = team_pokemon::make(species_id, game_id, level,
                                                           pkmn->move[0], pkmn->move[1],
                                                           pkmn->move[2], pkmn->move[3]);

            //convert nickname and trainer name
            uint16_t nickname_arr[11];
            nds_text_to_ucs2((char16_t*)nickname_arr, (char16_t*)pkmn->nickname, 11);
            t_pkmn->set_nickname(boost::locale::conv::utf_to_utf<wchar_t>(nickname_arr));

            uint16_t trainername_arr[11];
            nds_text_to_ucs2((char16_t*)trainername_arr, (char16_t*)pkmn->ot_name, 11);
            t_pkmn->set_trainer_name(boost::locale::conv::utf_to_utf<wchar_t>(trainername_arr));

            //Item
            uint16_t item_id = database::get_item_id(pkmn->held_item, game_id);
            t_pkmn->set_held_item(database::get_item_name(item_id));

            //ID's
            t_pkmn->set_personality(pkmn->header.pid);
            t_pkmn->set_trainer_public_id(pkmn->ot_id);
            t_pkmn->set_trainer_secret_id(pkmn->ot_sid);

            //Move PP's
            //TODO: take PP Ups into account
            t_pkmn->set_move_PP(pkmn->move_pp[0], 1);
            t_pkmn->set_move_PP(pkmn->move_pp[1], 2);
            t_pkmn->set_move_PP(pkmn->move_pp[2], 3);
            t_pkmn->set_move_PP(pkmn->move_pp[3], 4);

            //Effort values
            t_pkmn->set_EV("HP", pkmn->ev.hp);
            t_pkmn->set_EV("Attack", pkmn->ev.atk);
            t_pkmn->set_EV("Defense", pkmn->ev.def);
            t_pkmn->set_EV("Speed", pkmn->ev.spd);
            t_pkmn->set_EV("Special Attack", pkmn->ev.satk);
            t_pkmn->set_EV("Special Defense", pkmn->ev.sdef);

            //Individual values
            t_pkmn->set_IV("HP", modern_get_IV(IV_int, Stats::HP));
            t_pkmn->set_IV("Attack", modern_get_IV(IV_int, Stats::ATTACK));
            t_pkmn->set_IV("Defense", modern_get_IV(IV_int, Stats::DEFENSE));
            t_pkmn->set_IV("Speed", modern_get_IV(IV_int, Stats::SPEED));
            t_pkmn->set_IV("Special Attack", modern_get_IV(IV_int, Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV("Special Defense", modern_get_IV(IV_int, Stats::SPECIAL_DEFENSE));

            t_pkmn->set_met_level(get_gen_456_met_level(met_int));
            if(game_id == Games::HEART_GOLD or game_id == Games::SOUL_SILVER)
                t_pkmn->set_ball(ball_dict.at(game_ball_to_libpkmn_ball(pkmn->pokeball_hgss), "Poke Ball"));
            else
                t_pkmn->set_ball(ball_dict.at(game_ball_to_libpkmn_ball(pkmn->pokeball), "Poke Ball"));
            if(*met_int & 1) t_pkmn->set_trainer_gender("Female");
            else t_pkmn->set_trainer_gender("Male");

            //Attributes
            t_pkmn->set_attribute("friendship", pkmn->friendship);
            t_pkmn->set_attribute("circle", get_marking(marking_int, Markings::CIRCLE));
            t_pkmn->set_attribute("triangle", get_marking(marking_int, Markings::TRIANGLE));
            t_pkmn->set_attribute("square", get_marking(marking_int, Markings::SQUARE));
            t_pkmn->set_attribute("heart", get_marking(marking_int, Markings::HEART));
            t_pkmn->set_attribute("country", pkmn->country);

            t_pkmn->set_attribute("cool", pkmn->contest.cool);
            t_pkmn->set_attribute("beauty", pkmn->contest.beauty);
            t_pkmn->set_attribute("cute", pkmn->contest.cute);
            t_pkmn->set_attribute("smart", pkmn->contest.smart);
            t_pkmn->set_attribute("tough", pkmn->contest.tough);
            t_pkmn->set_attribute("sheen", pkmn->contest.sheen);

            //Hoenn Ribbons
            t_pkmn->set_attribute("hoenn_cool_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::COOL));
            t_pkmn->set_attribute("hoenn_cool_super_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::COOL_SUPER));
            t_pkmn->set_attribute("hoenn_cool_hyper_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::COOL_HYPER));
            t_pkmn->set_attribute("hoenn_cool_master_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::COOL_MASTER));
            t_pkmn->set_attribute("hoenn_beauty_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::BEAUTY));
            t_pkmn->set_attribute("hoenn_beauty_super_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::BEAUTY_SUPER));
            t_pkmn->set_attribute("hoenn_beauty_hyper_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::BEAUTY_HYPER));
            t_pkmn->set_attribute("hoenn_beauty_master_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::BEAUTY_MASTER));
            t_pkmn->set_attribute("hoenn_cute_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::CUTE));
            t_pkmn->set_attribute("hoenn_cute_super_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::CUTE_SUPER));
            t_pkmn->set_attribute("hoenn_cute_hyper_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::CUTE_HYPER));
            t_pkmn->set_attribute("hoenn_cute_master_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::CUTE_MASTER));
            t_pkmn->set_attribute("hoenn_smart_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::SMART));
            t_pkmn->set_attribute("hoenn_smart_super_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::SMART_SUPER));
            t_pkmn->set_attribute("hoenn_smart_hyper_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::SMART_HYPER));
            t_pkmn->set_attribute("hoenn_smart_master_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::SMART_MASTER));
            t_pkmn->set_attribute("hoenn_tough_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::TOUGH));
            t_pkmn->set_attribute("hoenn_tough_super_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::TOUGH_SUPER));
            t_pkmn->set_attribute("hoenn_tough_hyper_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::TOUGH_HYPER));
            t_pkmn->set_attribute("hoenn_tough_master_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::TOUGH_MASTER));
            t_pkmn->set_attribute("hoenn_champion_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::CHAMPION));
            t_pkmn->set_attribute("hoenn_winning_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::WINNING));
            t_pkmn->set_attribute("hoenn_victory_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::VICTORY));
            t_pkmn->set_attribute("hoenn_artist_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::ARTIST));
            t_pkmn->set_attribute("hoenn_effort_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::EFFORT));
            t_pkmn->set_attribute("hoenn_marine_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::MARINE));
            t_pkmn->set_attribute("hoenn_land_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::LAND));
            t_pkmn->set_attribute("hoenn_sky_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::SKY));
            t_pkmn->set_attribute("hoenn_country_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::COUNTRY));
            t_pkmn->set_attribute("hoenn_national_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::NATIONAL));
            t_pkmn->set_attribute("hoenn_earth_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::EARTH));
            t_pkmn->set_attribute("hoenn_world_ribbon", get_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::WORLD));

            //Sinnoh Ribbons
            t_pkmn->set_attribute("sinnoh_champion_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::CHAMPION));
            t_pkmn->set_attribute("sinnoh_ability_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::ABILITY));
            t_pkmn->set_attribute("sinnoh_great_ability_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::GREAT_ABILITY));
            t_pkmn->set_attribute("sinnoh_double_ability_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::DOUBLE_ABILITY));
            t_pkmn->set_attribute("sinnoh_multi_ability_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::MULTI_ABILITY));
            t_pkmn->set_attribute("sinnoh_pair_ability_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::PAIR_ABILITY));
            t_pkmn->set_attribute("sinnoh_world_ability_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::WORLD_ABILITY));
            t_pkmn->set_attribute("sinnoh_alert_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::ALERT));
            t_pkmn->set_attribute("sinnoh_shock_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::SHOCK));
            t_pkmn->set_attribute("sinnoh_downcast_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::DOWNCAST));
            t_pkmn->set_attribute("sinnoh_careless_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::CARELESS));
            t_pkmn->set_attribute("sinnoh_relax_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::RELAX));
            t_pkmn->set_attribute("sinnoh_snooze_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::SNOOZE));
            t_pkmn->set_attribute("sinnoh_smile_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::SMILE));
            t_pkmn->set_attribute("sinnoh_gorgeous_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::GORGEOUS));
            t_pkmn->set_attribute("sinnoh_royal_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::ROYAL));
            t_pkmn->set_attribute("sinnoh_gorgeous_royal_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::GORGEOUS_ROYAL));
            t_pkmn->set_attribute("sinnoh_footprint_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::FOOTPRINT));
            t_pkmn->set_attribute("sinnoh_record_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::RECORD));
            t_pkmn->set_attribute("sinnoh_history_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::HISTORY));
            t_pkmn->set_attribute("sinnoh_legend_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::LEGEND));
            t_pkmn->set_attribute("sinnoh_red_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::RED));
            t_pkmn->set_attribute("sinnoh_green_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::GREEN));
            t_pkmn->set_attribute("sinnoh_blue_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::BLUE));
            t_pkmn->set_attribute("sinnoh_festival_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_carnival_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_classic_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::CLASSIC));
            t_pkmn->set_attribute("sinnoh_premier_ribbon", get_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::PREMIER));
            t_pkmn->set_attribute("sinnoh_cool_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::COOL-23));
            t_pkmn->set_attribute("sinnoh_cool_super_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::COOL_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cool_hyper_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::COOL_HYPER-23));
            t_pkmn->set_attribute("sinnoh_cool_master_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::COOL_MASTER-23));
            t_pkmn->set_attribute("sinnoh_beauty_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::BEAUTY-23));
            t_pkmn->set_attribute("sinnoh_beauty_super_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::BEAUTY_SUPER-23));
            t_pkmn->set_attribute("sinnoh_beauty_hyper_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::BEAUTY_HYPER-23));
            t_pkmn->set_attribute("sinnoh_beauty_master_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::BEAUTY_MASTER-23));
            t_pkmn->set_attribute("sinnoh_cute_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::CUTE-23));
            t_pkmn->set_attribute("sinnoh_cute_super_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::CUTE_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cute_hyper_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::CUTE_HYPER-23));
            t_pkmn->set_attribute("sinnoh_cute_master_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::CUTE_MASTER-23));
            t_pkmn->set_attribute("sinnoh_smart_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::SMART-23));
            t_pkmn->set_attribute("sinnoh_smart_super_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::SMART_SUPER-23));
            t_pkmn->set_attribute("sinnoh_smart_hyper_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::SMART_HYPER-23));
            t_pkmn->set_attribute("sinnoh_smart_master_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::SMART_MASTER-23));
            t_pkmn->set_attribute("sinnoh_tough_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::TOUGH-23));
            t_pkmn->set_attribute("sinnoh_tough_super_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::TOUGH_SUPER-23));
            t_pkmn->set_attribute("sinnoh_tough_hyper_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::TOUGH_HYPER-23));
            t_pkmn->set_attribute("sinnoh_tough_master_ribbon", get_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::TOUGH_MASTER-23));

            t_pkmn->set_attribute("eggmet_year", pkmn->egg_met_date.year);
            t_pkmn->set_attribute("eggmet_month", pkmn->egg_met_date.month);
            t_pkmn->set_attribute("eggmet_day", pkmn->egg_met_date.day);
            t_pkmn->set_attribute("met_year", pkmn->met_date.year);
            t_pkmn->set_attribute("met_month", pkmn->met_date.month);
            t_pkmn->set_attribute("met_day", pkmn->met_date.day);

            pkm_encrypt(pkmn);
            return t_pkmn;
        }

        team_pokemon::sptr import_nds_pokemon(pkm_nds_t* pkmn) {return import_nds_pokemon(&(pkmn->box));}

        void export_nds_pokemon(team_pokemon::sptr t_pkmn, pkm_box_t* pkmn)
        {
            pkm_decrypt(pkmn);

            //Avoiding bitfields and structs
            uint8_t* ppup_int = reinterpret_cast<uint8_t*>(&(pkmn->move_pp_up));
            uint8_t* marking_int = reinterpret_cast<uint8_t*>(&(pkmn->markings));
            uint8_t* met_int = reinterpret_cast<uint8_t*>(&(pkmn->pokeball)+1);
            uint32_t* IV_int = reinterpret_cast<uint32_t*>(&(pkmn->iv));
            uint32_t* hoenn_ribbon_int = reinterpret_cast<uint32_t*>(&(pkmn->ribbon_hoenn1));
            uint32_t* sinnoh_unova_ribbon12_int = reinterpret_cast<uint32_t*>(&(pkmn->ribbon_sinnoh1));
            uint32_t* sinnoh_ribbon34_int = reinterpret_cast<uint32_t*>(&(pkmn->ribbon_sinnoh3));
            pkmn::dict<std::string, int> attributes = t_pkmn->get_attributes();
            pkmn::dict<std::string, unsigned int> EVs = t_pkmn->get_EVs();
            pkmn::dict<std::string, unsigned int> IVs = t_pkmn->get_IVs();

            //ID's
            pkmn->header.pid = t_pkmn->get_personality();
            pkmn->ot_id = t_pkmn->get_trainer_public_id();
            pkmn->ot_sid = t_pkmn->get_trainer_secret_id();

            //Nickname
            std::basic_string<uint16_t> nickname(boost::locale::conv::utf_to_utf<uint16_t>(t_pkmn->get_nickname().std_wstring()));
            memset(pkmn->nickname, 0xFF, PKM_NICKNAME_LENGTH);
            ucs2_to_nds_text(pkmn->nickname, (char16_t*)nickname.c_str(), nickname.size());

            pkmn->country = 0x2; //Default to USA

            //Trainer name
            std::basic_string<uint16_t> trainer_name(boost::locale::conv::utf_to_utf<uint16_t>(t_pkmn->get_trainer_name().std_wstring()));
            memset(pkmn->ot_name, 0xFF, PKM_OT_NAME_LENGTH);
            ucs2_to_nds_text(pkmn->ot_name, (char16_t*)trainer_name.c_str(), nickname.size());

            //Markings
            set_marking(marking_int, Markings::CIRCLE, attributes.at("circle", 0));
            set_marking(marking_int, Markings::TRIANGLE, attributes.at("triangle", 0));
            set_marking(marking_int, Markings::SQUARE, attributes.at("square", 0));
            set_marking(marking_int, Markings::HEART, attributes.at("heart", 0));

            pkmn->species = t_pkmn->get_species_id();
            pkmn->held_item = database::get_item_index(t_pkmn->get_held_item()->get_item_id(),
                                                       t_pkmn->get_game_id());

            //TODO: store actual experience, currently set it to what level needs
            //LibSPEC has no way to link Pokemon to growth rates, so use database
            std::string query_string(str(boost::format("SELECT growth_rate_id FROM pokemon_species WHERE id=%d")
                                         % pkmn->species));
            uint8_t growth_rate = int(_db.execAndGet(query_string.c_str())) - 1;
            switch(growth_rate)
            {
                case STAT_GROWTH_RATE_ERRATIC:
                    pkmn->exp = STAT_TOTAL_EXP_ERRATIC[t_pkmn->get_level() - 1];
                    break;

                case STAT_GROWTH_RATE_FAST:
                    pkmn->exp = STAT_TOTAL_EXP_FAST[t_pkmn->get_level() - 1];
                    break;

                case STAT_GROWTH_RATE_MEDIUM_FAST:
                    pkmn->exp = STAT_TOTAL_EXP_MEDIUM_FAST[t_pkmn->get_level() - 1];
                    break;

                case STAT_GROWTH_RATE_MEDIUM_SLOW:
                    pkmn->exp = STAT_TOTAL_EXP_MEDIUM_SLOW[t_pkmn->get_level() - 1];
                    break;

                case STAT_GROWTH_RATE_SLOW:
                    pkmn->exp = STAT_TOTAL_EXP_SLOW[t_pkmn->get_level() - 1];
                    break;

                default:
                    pkmn->exp = STAT_TOTAL_EXP_FLUCTUATING[t_pkmn->get_level() - 1];
                    break;
            }
            *ppup_int = 0;
            pkmn->friendship = attributes.at("friendship",70); //TODO: find base friendships for each species

            //Moves
            moveset_t moves;
            std::vector<unsigned int> move_PPs;
            t_pkmn->get_moves(moves);
            t_pkmn->get_move_PPs(move_PPs);
            pkmn->move[0] = moves[0]->get_move_id();
            pkmn->move[1] = moves[1]->get_move_id();
            pkmn->move[2] = moves[2]->get_move_id();
            pkmn->move[3] = moves[3]->get_move_id();
            pkmn->move_pp[0] = move_PPs[0];
            pkmn->move_pp[1] = move_PPs[1];
            pkmn->move_pp[2] = move_PPs[2];
            pkmn->move_pp[3] = move_PPs[3];

            //EVs
            pkmn->ev.hp = EVs["HP"];
            pkmn->ev.atk = EVs["Attack"];
            pkmn->ev.def = EVs["Defense"];
            pkmn->ev.spd = EVs["Speed"];
            pkmn->ev.satk = EVs["Special Attack"];
            pkmn->ev.sdef = EVs["Special Defense"];

            //Contest
            pkmn->contest.cool = attributes.at("cool",0);
            pkmn->contest.beauty = attributes.at("beauty",0);
            pkmn->contest.cute = attributes.at("cute",0);
            pkmn->contest.smart = attributes.at("smart",0);
            pkmn->contest.tough = attributes.at("tough",0);
            pkmn->contest.sheen = attributes.at("sheen",0);

            //Origin Info

            //TODO: Pokerus
            //TODO: Met location
            set_gen_456_met_level(met_int, t_pkmn->get_met_level());
            //TODO: allow game_id to be different than met game
            pkmn->pokeball = reverse_ball_dict[t_pkmn->get_ball()];
            if(t_pkmn->get_trainer_gender() == "Female") *met_int |= 1;
            else *met_int &= 1;

            //IVs
            modern_set_IV(IV_int, Stats::HP, IVs["HP"]);
            modern_set_IV(IV_int, Stats::ATTACK, IVs["Attack"]);
            modern_set_IV(IV_int, Stats::DEFENSE, IVs["Defense"]);
            modern_set_IV(IV_int, Stats::SPEED, IVs["Speed"]);
            modern_set_IV(IV_int, Stats::SPECIAL_ATTACK, IVs["Special Attack"]);

            pkmn->ability = t_pkmn->get_ability_id();

            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::COOL, attributes.at("hoenn_cool_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::COOL_SUPER, attributes.at("hoenn_cool_super_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::COOL_HYPER, attributes.at("hoenn_cool_hyper_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::COOL_MASTER, attributes.at("hoenn_cool_master_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::BEAUTY, attributes.at("hoenn_beauty_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::BEAUTY_SUPER, attributes.at("hoenn_beauty_super_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::BEAUTY_HYPER, attributes.at("hoenn_beauty_hyper_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::BEAUTY_MASTER, attributes.at("hoenn_beauty_master_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::CUTE, attributes.at("hoenn_cute_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::CUTE_SUPER, attributes.at("hoenn_cute_super_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::CUTE_HYPER, attributes.at("hoenn_cute_hyper_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::CUTE_MASTER, attributes.at("hoenn_cute_master_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::SMART, attributes.at("hoenn_smart_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::SMART_SUPER, attributes.at("hoenn_smart_super_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::SMART_HYPER, attributes.at("hoenn_smart_hyper_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::SMART_MASTER, attributes.at("hoenn_smart_master_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::TOUGH, attributes.at("hoenn_tough_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::TOUGH_SUPER, attributes.at("hoenn_tough_super_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::TOUGH_HYPER, attributes.at("hoenn_tough_hyper_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::TOUGH_MASTER, attributes.at("hoenn_tough_master_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::CHAMPION, attributes.at("hoenn_champion_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::WINNING, attributes.at("hoenn_winning_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::VICTORY, attributes.at("hoenn_victory_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::ARTIST, attributes.at("hoenn_artist_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::EFFORT, attributes.at("hoenn_effort_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::MARINE, attributes.at("hoenn_marine_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::LAND, attributes.at("hoenn_land_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::SKY, attributes.at("hoenn_sky_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::COUNTRY, attributes.at("hoenn_country_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::NATIONAL, attributes.at("hoenn_national_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::EARTH, attributes.at("hoenn_earth_ribbon",false));
            set_ribbon(hoenn_ribbon_int, Ribbons::Hoenn::WORLD, attributes.at("hoenn_world_ribbon",false));

            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::CHAMPION, attributes.at("champion_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::ABILITY, attributes.at("ability_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::GREAT_ABILITY, attributes.at("great_ability_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::DOUBLE_ABILITY, attributes.at("double_ability_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::MULTI_ABILITY, attributes.at("multi_ability_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::PAIR_ABILITY, attributes.at("pair_ability_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::WORLD_ABILITY, attributes.at("world_ability_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::ALERT, attributes.at("alert_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::SHOCK, attributes.at("shock_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::DOWNCAST, attributes.at("downcast_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::CARELESS, attributes.at("careless_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::RELAX, attributes.at("relax_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::ALERT, attributes.at("alert_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::SHOCK, attributes.at("shock_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::DOWNCAST, attributes.at("downcast_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::CARELESS, attributes.at("careless_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::RELAX, attributes.at("relax_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::SNOOZE, attributes.at("snooze_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::SMILE, attributes.at("smile_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::GORGEOUS, attributes.at("gorgeous_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::ROYAL, attributes.at("royal_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::GORGEOUS_ROYAL, attributes.at("gorgeous_royal_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::RECORD, attributes.at("record_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::HISTORY, attributes.at("history_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::LEGEND, attributes.at("legend_ribbon",false));
            if(t_pkmn->get_generation() == 4)
            {
                set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::RED, attributes.at("red_ribbon",false));
                set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::GREEN, attributes.at("green_ribbon",false));
                set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::BLUE, attributes.at("blue_ribbon",false));
                set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::FESTIVAL, attributes.at("festival_ribbon",false));
                set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::CARNIVAL, attributes.at("carnival_ribbon",false));
            }
            else
            {
                set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Unova::WORLD_CHAMPION, attributes.at("world_champion_ribbon",false));
                set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Unova::BIRTHDAY, attributes.at("birthday_ribbon",false));
                set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Unova::SPECIAL, attributes.at("special_ribbon",false));
                set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Unova::SOUVENIR, attributes.at("souvenir_ribbon",false));
                set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Unova::WISHING, attributes.at("wishing_ribbon",false));
            }
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::CLASSIC, attributes.at("classic_ribbon",false));
            set_ribbon(sinnoh_unova_ribbon12_int, Ribbons::Sinnoh::PREMIER, attributes.at("premier_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::COOL-23, attributes.at("cool_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::COOL_SUPER-23, attributes.at("cool_great_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::COOL_HYPER-23, attributes.at("cool_ultra_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::COOL_MASTER-23, attributes.at("cool_master_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::BEAUTY-23, attributes.at("beauty_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::BEAUTY_SUPER-23, attributes.at("beauty_great_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::BEAUTY_HYPER-23, attributes.at("beauty_ultra_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::BEAUTY_MASTER-23, attributes.at("beauty_master_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::CUTE-23, attributes.at("cute_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::CUTE_SUPER-23, attributes.at("cute_great_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::CUTE_HYPER-23, attributes.at("cute_ultra_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::CUTE_MASTER-23, attributes.at("cute_master_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::SMART-23, attributes.at("smart_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::SMART_SUPER-23, attributes.at("smart_great_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::SMART_HYPER-23, attributes.at("smart_ultra_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::SMART_MASTER-23, attributes.at("smart_master_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::TOUGH-23, attributes.at("tough_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::TOUGH_SUPER-23, attributes.at("tough_great_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::TOUGH_HYPER-23, attributes.at("tough_ultra_ribbon",false));
            set_ribbon(sinnoh_ribbon34_int, Ribbons::Sinnoh::TOUGH_MASTER-23, attributes.at("tough_master_ribbon",false));

            pkmn->egg_met_date.year = attributes.at("eggmet_year",0);
            pkmn->egg_met_date.month = attributes.at("eggmet_month",0);
            pkmn->egg_met_date.day = attributes.at("eggmet_day",0);
            pkmn->met_date.year = attributes.at("met_year",0);
            pkmn->met_date.month = attributes.at("met_month",0);
            pkmn->met_date.day = attributes.at("met_day",0);

            pkm_encrypt(pkmn);
        }

        void export_nds_pokemon(team_pokemon::sptr t_pkmn, pkm_nds_t* pkmn)
        {
            uint8_t* status_int = reinterpret_cast<uint8_t*>(&(pkmn->party));
            pkmn::dict<std::string, unsigned int> stats = t_pkmn->get_stats();

            export_nds_pokemon(t_pkmn, &(pkmn->box));
            *status_int = 0;
            pkmn->party.level = t_pkmn->get_level();
            pkmn->party.hp = stats["HP"];
            pkmn->party.maxhp = stats["HP"];
            pkmn->party.atk = stats["Attack"];
            pkmn->party.def = stats["Defense"];
            pkmn->party.spd = stats["Speed"];
            pkmn->party.satk = stats["Special Attack"];
            pkmn->party.sdef = stats["Special Defense"];
        }

        //OLD below

        team_pokemon::sptr rpokesav_gen1_pokemon_to_team_pokemon(rpokesav::gen1_pokemon pkmn,
                                                                 pokemon_text trainer_name)
        {
            unsigned int species_id, move1, move2, move3, move4;
        
            uint8_t rpokesav_species = pkmn.get_species_index();
            if(rpokesav_species == 0 or rpokesav_species > 190)
            {
                species_id = Species::INVALID;
                move1 = Moves::NONE;
                move2 = Moves::NONE;
                move3 = Moves::NONE;
                move4 = Moves::NONE;
            }
            else
            {
                //In Gen 1, species_id always matches pokemon_id
                species_id = database::get_pokemon_id(rpokesav_species, Games::YELLOW);
                
                rpokesav::vla<uint8_t> moves = pkmn.get_moves();
                move1 = moves[0];
                move2 = moves[1];
                move3 = moves[2];
                move4 = moves[3];
            }
        
            unsigned int level = pkmn.get_level();
            
            //No way to determine specific game in Generation 1 games
            team_pokemon::sptr t_pkmn = team_pokemon::make(species_id, Games::YELLOW, level,
                                                           move1, move2, move3, move4);
            
            t_pkmn->set_nickname(pkmn.get_nickname());
            
            //rpokesav::gen1_pokemon doesn't have a trainer name stored, so it must be passed in
            t_pkmn->set_trainer_name(trainer_name);
            
            t_pkmn->set_EV("HP", pkmn.get_ev_hp());
            t_pkmn->set_EV("Attack", pkmn.get_ev_attack());
            t_pkmn->set_EV("Defense", pkmn.get_ev_defense());
            t_pkmn->set_EV("Speed", pkmn.get_ev_speed());
            t_pkmn->set_EV("Special", pkmn.get_ev_special());
            
            t_pkmn->set_IV("HP", pkmn.get_iv_hp());
            t_pkmn->set_IV("Attack", pkmn.get_iv_attack());
            t_pkmn->set_IV("Defense", pkmn.get_iv_defense());
            t_pkmn->set_IV("Speed", pkmn.get_iv_speed());
            t_pkmn->set_IV("Special", pkmn.get_iv_special());
            
            //Generation 1 didn't have separate genders
            t_pkmn->set_trainer_gender("Male");
            
            return t_pkmn;
        }
    
        team_pokemon::sptr pokelib_pokemon_to_team_pokemon(PokeLib::Pokemon pokelib_pkmn)
        {
            unsigned int level, from_game, species_id;

            level = pokelib_pkmn.getLevel();
            from_game = hometown_to_libpkmn_game(pokelib_pkmn.pkm->pkm.hometown);
            species_id = database::get_species_id(pokelib_pkmn.pkm->pkm.species, from_game);

            team_pokemon::sptr t_pkmn = team_pokemon::make(species_id, from_game, level,
                                        pokelib_pkmn.pkm->pkm.move[0], pokelib_pkmn.pkm->pkm.move[1],
                                        pokelib_pkmn.pkm->pkm.move[2], pokelib_pkmn.pkm->pkm.move[3]);

            t_pkmn->set_nickname(pokelib_pkmn.getNickname());
            t_pkmn->set_trainer_name(pokelib_pkmn.getTrainerName());

            t_pkmn->set_move_PP(pokelib_pkmn.pkm->pkm.movePP[0], 1);
            t_pkmn->set_move_PP(pokelib_pkmn.pkm->pkm.movePP[1], 2);
            t_pkmn->set_move_PP(pokelib_pkmn.pkm->pkm.movePP[2], 3);
            t_pkmn->set_move_PP(pokelib_pkmn.pkm->pkm.movePP[3], 4);

            uint8_t* metLevelInt = &(pokelib_pkmn.pkm->pkm.metLevelInt);
            t_pkmn->set_met_level(get_gen_456_met_level(metLevelInt));
            t_pkmn->set_ball(ball_dict.at(game_ball_to_libpkmn_ball(pokelib_pkmn.pkm->pkm.pokeball), "Poke Ball"));
            if(get_gen_456_otgender(metLevelInt)) t_pkmn->set_trainer_gender("Female");
            else t_pkmn->set_trainer_gender("Male");

            t_pkmn->set_held_item(database::get_item_name(
                                  database::get_item_id(pokelib_pkmn.pkm->pkm.held_item, from_game)
                                 ));
            t_pkmn->set_personality(pokelib_pkmn.pkm->pkm.pid);
            t_pkmn->set_trainer_public_id(pokelib_pkmn.pkm->pkm.ot_id);
            t_pkmn->set_trainer_secret_id(pokelib_pkmn.pkm->pkm.ot_sid);

            t_pkmn->set_EV("HP", pokelib_pkmn.pkm->pkm.ev_hp);
            t_pkmn->set_EV("Attack", pokelib_pkmn.pkm->pkm.ev_atk);
            t_pkmn->set_EV("Defense", pokelib_pkmn.pkm->pkm.ev_def);
            t_pkmn->set_EV("Special Attack", pokelib_pkmn.pkm->pkm.ev_satk);
            t_pkmn->set_EV("Special Defense", pokelib_pkmn.pkm->pkm.ev_sdef);
            t_pkmn->set_EV("Speed", pokelib_pkmn.pkm->pkm.ev_spd);

            uint32_t* IVint = &(pokelib_pkmn.pkm->pkm.IVint);
            t_pkmn->set_IV("HP", modern_get_IV(IVint, Stats::HP));
            t_pkmn->set_IV("Attack", modern_get_IV(IVint, Stats::ATTACK));
            t_pkmn->set_IV("Defense", modern_get_IV(IVint, Stats::DEFENSE));
            t_pkmn->set_IV("Special Attack", modern_get_IV(IVint, Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV("Special Defense", modern_get_IV(IVint, Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV("Speed", modern_get_IV(IVint, Stats::SPEED));

            //TODO: use form data to set LibPKMN form, is fateful encounter

            //Attributes
            uint8_t* markings = &(pokelib_pkmn.pkm->pkm.markings);
            t_pkmn->set_attribute("friendship", pokelib_pkmn.pkm->pkm.friendship);
            t_pkmn->set_attribute("circle", get_marking(markings, Markings::CIRCLE));
            t_pkmn->set_attribute("triangle", get_marking(markings, Markings::TRIANGLE));
            t_pkmn->set_attribute("square", get_marking(markings, Markings::SQUARE));
            t_pkmn->set_attribute("heart", get_marking(markings, Markings::HEART));
            t_pkmn->set_attribute("star", get_marking(markings, Markings::STAR));
            t_pkmn->set_attribute("diamond", get_marking(markings, Markings::DIAMOND));

            t_pkmn->set_attribute("country", pokelib_pkmn.pkm->pkm.country);
            t_pkmn->set_attribute("cool", pokelib_pkmn.pkm->pkm.contest_cool);
            t_pkmn->set_attribute("beauty", pokelib_pkmn.pkm->pkm.contest_beauty);
            t_pkmn->set_attribute("cute", pokelib_pkmn.pkm->pkm.contest_cute);
            t_pkmn->set_attribute("smart", pokelib_pkmn.pkm->pkm.contest_smart);
            t_pkmn->set_attribute("tough", pokelib_pkmn.pkm->pkm.contest_tough);
            t_pkmn->set_attribute("sheen", pokelib_pkmn.pkm->pkm.contest_sheen);

            uint32_t* ribbonHoenn = &(pokelib_pkmn.pkm->pkm.intRibbonHoenn);
            t_pkmn->set_attribute("hoenn_cool_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL));
            t_pkmn->set_attribute("hoenn_cool_super_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL_SUPER));
            t_pkmn->set_attribute("hoenn_cool_hyper_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL_HYPER));
            t_pkmn->set_attribute("hoenn_cool_master_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL_MASTER));
            t_pkmn->set_attribute("hoenn_beauty_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY));
            t_pkmn->set_attribute("hoenn_beauty_super_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY_SUPER));
            t_pkmn->set_attribute("hoenn_beauty_hyper_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY_HYPER));
            t_pkmn->set_attribute("hoenn_beauty_master_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY_MASTER));
            t_pkmn->set_attribute("hoenn_cute_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE));
            t_pkmn->set_attribute("hoenn_cute_super_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE_SUPER));
            t_pkmn->set_attribute("hoenn_cute_hyper_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE_HYPER));
            t_pkmn->set_attribute("hoenn_cute_master_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE_MASTER));
            t_pkmn->set_attribute("hoenn_smart_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART));
            t_pkmn->set_attribute("hoenn_smart_super_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART_SUPER));
            t_pkmn->set_attribute("hoenn_smart_hyper_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART_HYPER));
            t_pkmn->set_attribute("hoenn_smart_master_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART_MASTER));
            t_pkmn->set_attribute("hoenn_tough_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH));
            t_pkmn->set_attribute("hoenn_tough_super_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH_SUPER));
            t_pkmn->set_attribute("hoenn_tough_hyper_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH_HYPER));
            t_pkmn->set_attribute("hoenn_tough_master_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH_MASTER));
            t_pkmn->set_attribute("hoenn_champion_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::CHAMPION));
            t_pkmn->set_attribute("hoenn_winning_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::WINNING));
            t_pkmn->set_attribute("hoenn_victory_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::VICTORY));
            t_pkmn->set_attribute("hoenn_artist_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::ARTIST));
            t_pkmn->set_attribute("hoenn_effort_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::EFFORT));
            t_pkmn->set_attribute("hoenn_marine_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::MARINE));
            t_pkmn->set_attribute("hoenn_land_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::LAND));
            t_pkmn->set_attribute("hoenn_sky_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::SKY));
            t_pkmn->set_attribute("hoenn_country_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::COUNTRY));
            t_pkmn->set_attribute("hoenn_national_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::NATIONAL));
            t_pkmn->set_attribute("hoenn_earth_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::EARTH));
            t_pkmn->set_attribute("hoenn_world_ribbon", get_ribbon(ribbonHoenn, Ribbons::Hoenn::WORLD));

            uint32_t* ribbonSinnohA = &(pokelib_pkmn.pkm->pkm.intRibbonSinnohA);
            t_pkmn->set_attribute("sinnoh_champion_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::CHAMPION));
            t_pkmn->set_attribute("sinnoh_ability_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::ABILITY));
            t_pkmn->set_attribute("sinnoh_great_ability_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::GREAT_ABILITY));
            t_pkmn->set_attribute("sinnoh_double_ability_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::DOUBLE_ABILITY));
            t_pkmn->set_attribute("sinnoh_multi_ability_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::MULTI_ABILITY));
            t_pkmn->set_attribute("sinnoh_pair_ability_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::PAIR_ABILITY));
            t_pkmn->set_attribute("sinnoh_world_ability_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::WORLD_ABILITY));
            t_pkmn->set_attribute("sinnoh_alert_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::ALERT));
            t_pkmn->set_attribute("sinnoh_shock_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::SHOCK));
            t_pkmn->set_attribute("sinnoh_downcast_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::DOWNCAST));
            t_pkmn->set_attribute("sinnoh_careless_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::CARELESS));
            t_pkmn->set_attribute("sinnoh_relax_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::RELAX));
            t_pkmn->set_attribute("sinnoh_snooze_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::SNOOZE));
            t_pkmn->set_attribute("sinnoh_smile_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::SMILE));
            t_pkmn->set_attribute("sinnoh_gorgeous_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::GORGEOUS));
            t_pkmn->set_attribute("sinnoh_royal_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::ROYAL));
            t_pkmn->set_attribute("sinnoh_gorgeous_royal_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::GORGEOUS_ROYAL));
            t_pkmn->set_attribute("sinnoh_footprint_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::FOOTPRINT));
            t_pkmn->set_attribute("sinnoh_record_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::RECORD));
            t_pkmn->set_attribute("sinnoh_history_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::HISTORY));
            t_pkmn->set_attribute("sinnoh_legend_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::LEGEND));
            t_pkmn->set_attribute("sinnoh_red_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::RED));
            t_pkmn->set_attribute("sinnoh_green_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::GREEN));
            t_pkmn->set_attribute("sinnoh_blue_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::BLUE));
            t_pkmn->set_attribute("sinnoh_festival_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_carnival_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_classic_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::CLASSIC));
            t_pkmn->set_attribute("sinnoh_premier_ribbon", get_ribbon(ribbonSinnohA, Ribbons::Sinnoh::PREMIER));

            uint32_t* ribbonSinnohB = &(pokelib_pkmn.pkm->pkm.intRibbonSinnohB);
            t_pkmn->set_attribute("sinnoh_cool_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::COOL-23));
            t_pkmn->set_attribute("sinnoh_cool_great_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::COOL_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cool_ultra_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::COOL_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cool_master_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::COOL_MASTER-23));
            t_pkmn->set_attribute("sinnoh_beauty_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY-23));
            t_pkmn->set_attribute("sinnoh_beauty_great_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY_SUPER-23));
            t_pkmn->set_attribute("sinnoh_beauty_ultra_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY_SUPER-23));
            t_pkmn->set_attribute("sinnoh_beauty_master_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY_MASTER-23));
            t_pkmn->set_attribute("sinnoh_cute_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::CUTE-23));
            t_pkmn->set_attribute("sinnoh_cute_great_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::CUTE_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cute_ultra_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::CUTE_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cute_master_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::CUTE_MASTER-23));
            t_pkmn->set_attribute("sinnoh_smart_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::SMART-23));
            t_pkmn->set_attribute("sinnoh_smart_great_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::SMART_SUPER-23));
            t_pkmn->set_attribute("sinnoh_smart_ultra_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::SMART_SUPER-23));
            t_pkmn->set_attribute("sinnoh_smart_master_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::SMART_MASTER-23));
            t_pkmn->set_attribute("sinnoh_tough_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::TOUGH-23));
            t_pkmn->set_attribute("sinnoh_tough_great_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::TOUGH_SUPER-23));
            t_pkmn->set_attribute("sinnoh_tough_ultra_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::TOUGH_SUPER-23));
            t_pkmn->set_attribute("sinnoh_tough_master_ribbon", get_ribbon(ribbonSinnohB, Ribbons::Sinnoh::TOUGH_MASTER-23));

            t_pkmn->set_attribute("eggmet_year", pokelib_pkmn.pkm->pkm.eggDate[0]);
            t_pkmn->set_attribute("eggmet_month", pokelib_pkmn.pkm->pkm.eggDate[1]);
            t_pkmn->set_attribute("eggmet_day", pokelib_pkmn.pkm->pkm.eggDate[2]);
            t_pkmn->set_attribute("met_year", pokelib_pkmn.pkm->pkm.metDate[0]);
            t_pkmn->set_attribute("met_month", pokelib_pkmn.pkm->pkm.metDate[1]);
            t_pkmn->set_attribute("met_day", pokelib_pkmn.pkm->pkm.metDate[2]);

            return t_pkmn;
        }

        PokeLib::Pokemon team_pokemon_to_pokelib_pokemon(team_pokemon::sptr t_pkmn)
        {
            PokeLib::Pokemon pokelib_pkmn;

            pokelib_pkmn.pkm->pkm.species = database::get_species_index(t_pkmn->get_pokemon_id(), t_pkmn->get_game_id());
            pokelib_pkmn.setLevel(uint8_t(t_pkmn->get_level()));
            pokelib_pkmn.setNickname(t_pkmn->get_nickname());
            pokelib_pkmn.setTrainerName(t_pkmn->get_trainer_name());
            pokelib_pkmn.pkm->pkm.pid = t_pkmn->get_personality();
            pokelib_pkmn.pkm->pkm.ot_id = t_pkmn->get_trainer_public_id();
            pokelib_pkmn.pkm->pkm.ot_sid = t_pkmn->get_trainer_secret_id();
            
            unsigned int raw_held = t_pkmn->get_held_item()->get_item_id();
            pokelib_pkmn.pkm->pkm.held_item = database::get_item_index(raw_held, t_pkmn->get_game_id());

            uint8_t* metlevel_int = reinterpret_cast<uint8_t*>(&(pokelib_pkmn.pkm->pkm.pokeball)+1);
            set_gen_456_met_level(metlevel_int, t_pkmn->get_met_level());
            pokelib_pkmn.pkm->pkm.pokeball = libpkmn_ball_to_game_ball(reverse_ball_dict.at(t_pkmn->get_ball(), PokeBalls::POKE_BALL));
            set_gen_456_met_level(metlevel_int, (t_pkmn->get_trainer_gender().std_string() == "Female"));

            moveset_t moves;
            t_pkmn->get_moves(moves);
            pokelib_pkmn.pkm->pkm.move[0] = moves[0]->get_move_id();
            pokelib_pkmn.pkm->pkm.move[1] = moves[1]->get_move_id();
            pokelib_pkmn.pkm->pkm.move[2] = moves[2]->get_move_id();
            pokelib_pkmn.pkm->pkm.move[3] = moves[3]->get_move_id();

            std::vector<unsigned int> move_PPs;
            t_pkmn->get_move_PPs(move_PPs);
            pokelib_pkmn.pkm->pkm.move[0] = move_PPs[0];
            pokelib_pkmn.pkm->pkm.move[1] = move_PPs[1];
            pokelib_pkmn.pkm->pkm.move[2] = move_PPs[2];
            pokelib_pkmn.pkm->pkm.move[3] = move_PPs[3];

            dict<std::string, unsigned int> EVs = t_pkmn->get_EVs();
            pokelib_pkmn.pkm->pkm.ev_hp = EVs["HP"];
            pokelib_pkmn.pkm->pkm.ev_atk = EVs["Attack"];
            pokelib_pkmn.pkm->pkm.ev_def = EVs["Defense"];
            pokelib_pkmn.pkm->pkm.ev_satk = EVs["Special Attack"];
            pokelib_pkmn.pkm->pkm.ev_sdef = EVs["Special Defense"];
            pokelib_pkmn.pkm->pkm.ev_spd = EVs["Speed"];

            dict<std::string, unsigned int> IVs = t_pkmn->get_IVs();
            uint32_t* IVint = &(pokelib_pkmn.pkm->pkm.IVint);
            modern_set_IV(IVint, Stats::HP, IVs["HP"]);
            modern_set_IV(IVint, Stats::ATTACK, IVs["Attack"]);
            modern_set_IV(IVint, Stats::DEFENSE, IVs["Defense"]);
            modern_set_IV(IVint, Stats::SPECIAL_ATTACK, IVs["Special Attack"]);
            modern_set_IV(IVint, Stats::SPECIAL_DEFENSE, IVs["Special Defense"]);
            modern_set_IV(IVint, Stats::SPEED, IVs["Speed"]);
            
            pokelib_pkmn.updateBattleStats();

            pokelib_pkmn.pkm->pkm.hometown = libpkmn_game_to_hometown(t_pkmn->get_game_id());

            //Attributes
            dict<std::string, int> attributes = t_pkmn->get_attributes();

            uint8_t* markings = &(pokelib_pkmn.pkm->pkm.markings);
            pokelib_pkmn.pkm->pkm.friendship = attributes.at("friendship",0);
            set_marking(markings, Markings::CIRCLE, attributes.at("circle",false));
            set_marking(markings, Markings::TRIANGLE, attributes.at("triangle",false));
            set_marking(markings, Markings::SQUARE, attributes.at("square",false));
            set_marking(markings, Markings::HEART, attributes.at("heart",false));
            set_marking(markings, Markings::STAR, attributes.at("star",false));
            set_marking(markings, Markings::DIAMOND, attributes.at("diamond",false));

            pokelib_pkmn.pkm->pkm.country = attributes.at("country",2); //Default to English
            pokelib_pkmn.pkm->pkm.contest_cool = attributes.at("cool",false);
            pokelib_pkmn.pkm->pkm.contest_beauty = attributes.at("beauty",false);
            pokelib_pkmn.pkm->pkm.contest_smart = attributes.at("smart",false);
            pokelib_pkmn.pkm->pkm.contest_tough = attributes.at("tough",false);
            pokelib_pkmn.pkm->pkm.contest_sheen = attributes.at("sheen",false);

            uint32_t* ribbonSinnohA = &(pokelib_pkmn.pkm->pkm.intRibbonSinnohA);
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::CHAMPION, attributes.at("sinnoh_champion_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::ABILITY, attributes.at("sinnoh_ability_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::GREAT_ABILITY, attributes.at("sinnoh_great_ability_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::DOUBLE_ABILITY, attributes.at("sinnoh_double_ability_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::MULTI_ABILITY, attributes.at("sinnoh_multi_ability_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::PAIR_ABILITY, attributes.at("sinnoh_pair_ability_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::WORLD_ABILITY, attributes.at("sinnoh_world_ability_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::ALERT, attributes.at("sinnoh_alert_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::SHOCK, attributes.at("sinnoh_shock_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::DOWNCAST, attributes.at("sinnoh_downcast_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::CARELESS, attributes.at("sinnoh_careless_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::RELAX, attributes.at("sinnoh_relax_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::ALERT, attributes.at("sinnoh_alert_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::SHOCK, attributes.at("sinnoh_shock_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::DOWNCAST, attributes.at("sinnoh_downcast_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::CARELESS, attributes.at("sinnoh_careless_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::RELAX, attributes.at("sinnoh_relax_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::SNOOZE, attributes.at("sinnoh_snooze_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::SMILE, attributes.at("sinnoh_smile_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::GORGEOUS, attributes.at("sinnoh_gorgeous_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::ROYAL, attributes.at("sinnoh_royal_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::GORGEOUS_ROYAL, attributes.at("sinnoh_gorgeous_royal_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::RECORD, attributes.at("sinnoh_record_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::HISTORY, attributes.at("sinnoh_history_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::LEGEND, attributes.at("sinnoh_legend_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::RED, attributes.at("sinnoh_red_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::GREEN, attributes.at("sinnoh_green_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::BLUE, attributes.at("sinnoh_blue_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::FESTIVAL, attributes.at("sinnoh_festival_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::CARNIVAL, attributes.at("sinnoh_carnival_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::CLASSIC, attributes.at("sinnoh_classic_ribbon",false));
            set_ribbon(ribbonSinnohA, Ribbons::Sinnoh::PREMIER, attributes.at("sinnoh_premier_ribbon",false));

            uint32_t* ribbonHoenn = &(pokelib_pkmn.pkm->pkm.intRibbonHoenn);
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL, attributes.at("hoenn_cool_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL_SUPER, attributes.at("hoenn_cool_super_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL_HYPER, attributes.at("hoenn_cool_hyper_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL_MASTER, attributes.at("hoenn_cool_master_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY, attributes.at("hoenn_beauty_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY_SUPER, attributes.at("hoenn_beauty_super_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY_HYPER, attributes.at("hoenn_beauty_hyper_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY_MASTER, attributes.at("hoenn_beauty_master_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE, attributes.at("hoenn_cute_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE_SUPER, attributes.at("hoenn_cute_super_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE_HYPER, attributes.at("hoenn_cute_hyper_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE_MASTER, attributes.at("hoenn_cute_master_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART, attributes.at("hoenn_smart_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART_SUPER, attributes.at("hoenn_smart_super_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART_HYPER, attributes.at("hoenn_smart_hyper_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART_MASTER, attributes.at("hoenn_smart_master_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH, attributes.at("hoenn_tough_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH_SUPER, attributes.at("hoenn_tough_super_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH_HYPER, attributes.at("hoenn_tough_hyper_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH_MASTER, attributes.at("hoenn_tough_master_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::CHAMPION, attributes.at("hoenn_champion_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::WINNING, attributes.at("hoenn_winning_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::VICTORY, attributes.at("hoenn_victory_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::ARTIST, attributes.at("hoenn_artist_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::EFFORT, attributes.at("hoenn_effort_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::MARINE, attributes.at("hoenn_marine_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::LAND, attributes.at("hoenn_land_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::SKY, attributes.at("hoenn_sky_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::COUNTRY, attributes.at("hoenn_country_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::NATIONAL, attributes.at("hoenn_national_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::EARTH, attributes.at("hoenn_earth_ribbon",false));
            set_ribbon(ribbonHoenn, Ribbons::Hoenn::WORLD, attributes.at("hoenn_world_ribbon",false));

            uint32_t* ribbonSinnohB = &(pokelib_pkmn.pkm->pkm.intRibbonSinnohB);
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::COOL-23, attributes.at("sinnoh_cool_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::COOL_SUPER-23, attributes.at("sinnoh_cool_great_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::COOL_HYPER-23, attributes.at("sinnoh_cool_ultra_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::COOL_MASTER-23, attributes.at("sinnoh_cool_master_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY-23, attributes.at("sinnoh_beauty_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY_SUPER-23, attributes.at("sinnoh_beauty_great_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY_HYPER-23, attributes.at("sinnoh_beauty_ultra_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY_MASTER-23, attributes.at("sinnoh_beauty_master_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::CUTE-23, attributes.at("sinnoh_cute_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::CUTE_SUPER-23, attributes.at("sinnoh_cute_great_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::CUTE_HYPER-23, attributes.at("sinnoh_cute_ultra_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::CUTE_MASTER-23, attributes.at("sinnoh_cute_master_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::SMART-23, attributes.at("sinnoh_smart_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::SMART_SUPER-23, attributes.at("sinnoh_smart_great_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::SMART_HYPER-23, attributes.at("sinnoh_smart_ultra_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::SMART_MASTER-23, attributes.at("sinnoh_smart_master_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::TOUGH-23, attributes.at("sinnoh_tough_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::TOUGH_SUPER-23, attributes.at("sinnoh_tough_great_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::TOUGH_HYPER-23, attributes.at("sinnoh_tough_ultra_ribbon",false));
            set_ribbon(ribbonSinnohB, Ribbons::Sinnoh::TOUGH_MASTER-23, attributes.at("sinnoh_tough_master_ribbon",false));

            pokelib_pkmn.pkm->pkm.eggDate[0] = attributes.at("eggmet_year",0);
            pokelib_pkmn.pkm->pkm.eggDate[1] = attributes.at("eggmet_month",0);
            pokelib_pkmn.pkm->pkm.eggDate[2] = attributes.at("eggmet_day",0);
            pokelib_pkmn.pkm->pkm.metDate[0] = attributes.at("met_year",0);
            pokelib_pkmn.pkm->pkm.metDate[1] = attributes.at("met_month",0);
            pokelib_pkmn.pkm->pkm.metDate[2] = attributes.at("met_day",0);

            return pokelib_pkmn;
        }

        team_pokemon::sptr pkmds_g5_pokemon_to_team_pokemon(party_pkm* p_pkm)
        {
            ::opendb(get_database_path().c_str());

            unsigned int level, from_game, species_id;

            level = ::getpkmlevel(p_pkm->pkm_data);
            from_game = hometown_to_libpkmn_game(p_pkm->pkm_data.hometown);
            species_id = database::get_species_id(p_pkm->pkm_data.species, from_game);

            team_pokemon::sptr t_pkmn = team_pokemon::make(species_id, from_game, level,
                                        p_pkm->pkm_data.moves[0], p_pkm->pkm_data.moves[1],
                                        p_pkm->pkm_data.moves[2], p_pkm->pkm_data.moves[3]);

            #ifdef _MSC_VER
                t_pkmn->set_nickname(getpkmnickname(p_pkm->pkm_data));
                t_pkmn->set_trainer_name(getpkmotname(p_pkm->pkm_data));
            #else
                //Testing new pokemon_text class, need to get around PKMDS's use of chars for Linux
                wchar_t nickname[11];
                wchar_t otname[8];
                memcpy(nickname, p_pkm->pkm_data.nickname, 22);
                memcpy(otname, p_pkm->pkm_data.otname, 16);
                t_pkmn->set_nickname(nickname);
                t_pkmn->set_trainer_name(otname);
            #endif

            t_pkmn->set_move_PP(p_pkm->pkm_data.pp[0], 1);
            t_pkmn->set_move_PP(p_pkm->pkm_data.pp[1], 2);
            t_pkmn->set_move_PP(p_pkm->pkm_data.pp[2], 3);
            t_pkmn->set_move_PP(p_pkm->pkm_data.pp[3], 4);

            uint8_t* metlevel_int = reinterpret_cast<uint8_t*>(&(p_pkm->pkm_data.ball)+1);
            t_pkmn->set_met_level(get_gen_456_met_level(metlevel_int));
            t_pkmn->set_ball(ball_dict.at(game_ball_to_libpkmn_ball(p_pkm->pkm_data.ball), "Poke Ball"));
            if(get_gen_456_otgender(metlevel_int)) t_pkmn->set_trainer_gender("Female");
            else t_pkmn->set_trainer_gender("Male");
            
            t_pkmn->set_held_item(item::make(database::get_item_id(p_pkm->pkm_data.item, from_game), from_game));
            t_pkmn->set_personality(p_pkm->pkm_data.pid);
            t_pkmn->set_trainer_public_id(p_pkm->pkm_data.tid);
            t_pkmn->set_trainer_secret_id(p_pkm->pkm_data.sid);

            t_pkmn->set_EV("HP", p_pkm->pkm_data.evs.hp);
            t_pkmn->set_EV("Attack", p_pkm->pkm_data.evs.attack);
            t_pkmn->set_EV("Defense", p_pkm->pkm_data.evs.defense);
            t_pkmn->set_EV("Special Attack", p_pkm->pkm_data.evs.spatk);
            t_pkmn->set_EV("Special Defense", p_pkm->pkm_data.evs.spdef);
            t_pkmn->set_EV("Speed", p_pkm->pkm_data.evs.speed);

            uint32_t* ivs = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.ivs));
            t_pkmn->set_IV("HP", modern_get_IV(ivs, Stats::HP));
            t_pkmn->set_IV("Attack", modern_get_IV(ivs, Stats::ATTACK));
            t_pkmn->set_IV("Defense", modern_get_IV(ivs, Stats::DEFENSE));
            t_pkmn->set_IV("Special Attack", modern_get_IV(ivs, Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV("Special Defense", modern_get_IV(ivs, Stats::SPECIAL_DEFENSE));
            t_pkmn->set_IV("Speed", modern_get_IV(ivs, Stats::SPEED));

            //Attributes
            uint8_t* markings = reinterpret_cast<uint8_t*>(&(p_pkm->pkm_data.markings));
            t_pkmn->set_attribute("circle", get_marking(markings, Markings::CIRCLE));
            t_pkmn->set_attribute("triangle", get_marking(markings, Markings::TRIANGLE));
            t_pkmn->set_attribute("square", get_marking(markings, Markings::SQUARE));
            t_pkmn->set_attribute("heart", get_marking(markings, Markings::HEART));
            t_pkmn->set_attribute("star", get_marking(markings, Markings::STAR));
            t_pkmn->set_attribute("diamond", get_marking(markings, Markings::DIAMOND));

            t_pkmn->set_attribute("country", p_pkm->pkm_data.country);
            t_pkmn->set_attribute("cool", p_pkm->pkm_data.contest.cool);
            t_pkmn->set_attribute("beauty", p_pkm->pkm_data.contest.beauty);
            t_pkmn->set_attribute("cute", p_pkm->pkm_data.contest.cute);
            t_pkmn->set_attribute("smart", p_pkm->pkm_data.contest.smart);
            t_pkmn->set_attribute("tough", p_pkm->pkm_data.contest.tough);
            t_pkmn->set_attribute("sheen", p_pkm->pkm_data.contest.sheen);

            uint32_t* hribbon1 = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.hribbon1));
            t_pkmn->set_attribute("hoenn_cool_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::COOL));
            t_pkmn->set_attribute("hoenn_cool_super_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::COOL_SUPER));
            t_pkmn->set_attribute("hoenn_cool_hyper_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::COOL_HYPER));
            t_pkmn->set_attribute("hoenn_cool_master_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::COOL_MASTER));
            t_pkmn->set_attribute("hoenn_beauty_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY));
            t_pkmn->set_attribute("hoenn_beauty_super_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_SUPER));
            t_pkmn->set_attribute("hoenn_beauty_hyper_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_HYPER));
            t_pkmn->set_attribute("hoenn_beauty_master_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_MASTER));
            t_pkmn->set_attribute("hoenn_cute_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::CUTE));
            t_pkmn->set_attribute("hoenn_cute_super_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::CUTE_SUPER));
            t_pkmn->set_attribute("hoenn_cute_hyper_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::CUTE_HYPER));
            t_pkmn->set_attribute("hoenn_cute_master_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::CUTE_MASTER));
            t_pkmn->set_attribute("hoenn_smart_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::SMART));
            t_pkmn->set_attribute("hoenn_smart_super_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::SMART_SUPER));
            t_pkmn->set_attribute("hoenn_smart_hyper_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::SMART_HYPER));
            t_pkmn->set_attribute("hoenn_smart_master_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::SMART_MASTER));
            t_pkmn->set_attribute("hoenn_tough_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::TOUGH));
            t_pkmn->set_attribute("hoenn_tough_super_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_SUPER));
            t_pkmn->set_attribute("hoenn_tough_hyper_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_HYPER));
            t_pkmn->set_attribute("hoenn_tough_master_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_MASTER));
            t_pkmn->set_attribute("hoenn_champion_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::CHAMPION));
            t_pkmn->set_attribute("hoenn_winning_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::WINNING));
            t_pkmn->set_attribute("hoenn_victory_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::VICTORY));
            t_pkmn->set_attribute("hoenn_artist_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::ARTIST));
            t_pkmn->set_attribute("hoenn_effort_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::EFFORT));
            t_pkmn->set_attribute("hoenn_marine_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::MARINE));
            t_pkmn->set_attribute("hoenn_land_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::LAND));
            t_pkmn->set_attribute("hoenn_sky_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::SKY));
            t_pkmn->set_attribute("hoenn_country_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::COUNTRY));
            t_pkmn->set_attribute("hoenn_national_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::NATIONAL));
            t_pkmn->set_attribute("hoenn_earth_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::EARTH));
            t_pkmn->set_attribute("hoenn_world_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::WORLD));

            uint32_t* sribbon1 = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.sribbon1));
            t_pkmn->set_attribute("sinnoh_champion_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::CHAMPION));
            t_pkmn->set_attribute("sinnoh_ability_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::ABILITY));
            t_pkmn->set_attribute("sinnoh_great_ability_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::GREAT_ABILITY));
            t_pkmn->set_attribute("sinnoh_double_ability_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::DOUBLE_ABILITY));
            t_pkmn->set_attribute("sinnoh_multi_ability_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::MULTI_ABILITY));
            t_pkmn->set_attribute("sinnoh_pair_ability_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::PAIR_ABILITY));
            t_pkmn->set_attribute("sinnoh_world_ability_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::WORLD_ABILITY));
            t_pkmn->set_attribute("sinnoh_alert_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::ALERT));
            t_pkmn->set_attribute("sinnoh_shock_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::SHOCK));
            t_pkmn->set_attribute("sinnoh_downcast_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::DOWNCAST));
            t_pkmn->set_attribute("sinnoh_careless_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::CARELESS));
            t_pkmn->set_attribute("sinnoh_relax_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::RELAX));
            t_pkmn->set_attribute("sinnoh_snooze_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::SNOOZE));
            t_pkmn->set_attribute("sinnoh_smile_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::SMILE));
            t_pkmn->set_attribute("sinnoh_gorgeous_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::GORGEOUS));
            t_pkmn->set_attribute("sinnoh_royal_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::ROYAL));
            t_pkmn->set_attribute("sinnoh_gorgeous_royal_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::GORGEOUS_ROYAL));
            t_pkmn->set_attribute("sinnoh_footprint_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::FOOTPRINT));
            t_pkmn->set_attribute("sinnoh_record_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::RECORD));
            t_pkmn->set_attribute("sinnoh_history_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::HISTORY));
            t_pkmn->set_attribute("sinnoh_legend_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::LEGEND));
            t_pkmn->set_attribute("sinnoh_red_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::RED));
            t_pkmn->set_attribute("sinnoh_green_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::GREEN));
            t_pkmn->set_attribute("sinnoh_blue_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::BLUE));
            t_pkmn->set_attribute("sinnoh_festival_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_carnival_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_classic_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::CLASSIC));
            t_pkmn->set_attribute("sinnoh_premier_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::PREMIER));

            uint32_t* sribbon3 = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.sribbon3));
            t_pkmn->set_attribute("sinnoh_cool_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::COOL-23));
            t_pkmn->set_attribute("sinnoh_cool_great_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::COOL_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cool_ultra_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::COOL_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cool_master_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::COOL_MASTER-23));
            t_pkmn->set_attribute("sinnoh_beauty_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY-23));
            t_pkmn->set_attribute("sinnoh_beauty_great_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY_SUPER-23));
            t_pkmn->set_attribute("sinnoh_beauty_ultra_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY_SUPER-23));
            t_pkmn->set_attribute("sinnoh_beauty_master_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY_MASTER-23));
            t_pkmn->set_attribute("sinnoh_cute_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::CUTE-23));
            t_pkmn->set_attribute("sinnoh_cute_great_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::CUTE_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cute_ultra_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::CUTE_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cute_master_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::CUTE_MASTER-23));
            t_pkmn->set_attribute("sinnoh_smart_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::SMART-23));
            t_pkmn->set_attribute("sinnoh_smart_great_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::SMART_SUPER-23));
            t_pkmn->set_attribute("sinnoh_smart_ultra_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::SMART_SUPER-23));
            t_pkmn->set_attribute("sinnoh_smart_master_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::SMART_MASTER-23));
            t_pkmn->set_attribute("sinnoh_tough_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH-23));
            t_pkmn->set_attribute("sinnoh_tough_great_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH_SUPER-23));
            t_pkmn->set_attribute("sinnoh_tough_ultra_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH_SUPER-23));
            t_pkmn->set_attribute("sinnoh_tough_master_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH_MASTER-23));

            t_pkmn->set_attribute("eggmet_year", p_pkm->pkm_data.metdate.year);
            t_pkmn->set_attribute("eggmet_month", p_pkm->pkm_data.metdate.month);
            t_pkmn->set_attribute("eggmet_day", p_pkm->pkm_data.metdate.day);
            t_pkmn->set_attribute("met_year", p_pkm->pkm_data.metdate.year);
            t_pkmn->set_attribute("met_month", p_pkm->pkm_data.metdate.month);
            t_pkmn->set_attribute("met_day", p_pkm->pkm_data.metdate.day);

            closedb();

            return t_pkmn;
        }

        void team_pokemon_to_pkmds_g5_pokemon(team_pokemon::sptr t_pkmn, party_pkm* p_pkm)
        {
            p_pkm->pkm_data.species = ::Species::pkmspecies(
                                        database::get_species_index(t_pkmn->get_pokemon_id(),
                                        t_pkmn->get_game_id()));
            
            moveset_t moves;
            t_pkmn->get_moves(moves);
            p_pkm->pkm_data.moves[0] = ::Moves::moves(moves[0]->get_move_id());
            p_pkm->pkm_data.moves[1] = ::Moves::moves(moves[1]->get_move_id());
            p_pkm->pkm_data.moves[2] = ::Moves::moves(moves[2]->get_move_id());
            p_pkm->pkm_data.moves[3] = ::Moves::moves(moves[3]->get_move_id());

            std::vector<unsigned int> move_PPs;
            t_pkmn->get_move_PPs(move_PPs);
            p_pkm->pkm_data.pp[0] = move_PPs[0];
            p_pkm->pkm_data.pp[1] = move_PPs[1];
            p_pkm->pkm_data.pp[2] = move_PPs[2];
            p_pkm->pkm_data.pp[3] = move_PPs[3];

            ::setlevel(p_pkm->pkm_data, t_pkmn->get_level());

            wstring nickname_wide = t_pkmn->get_nickname();
            wstring trainer_name_wide = t_pkmn->get_trainer_name();
            #ifdef LIBPKMN_PLATFORM_LINUX
                ::setpkmnickname(p_pkm->pkm_data, (wchar_t*)(nickname_wide.c_str()), nickname_wide.size());
                ::setpkmotname(p_pkm->pkm_data, (wchar_t*)(trainer_name_wide.c_str()), trainer_name_wide.size());
            #else
                //TODO: clean up when using Windows
                ::setpkmnickname(p_pkm->pkm_data, (wchar_t*)(nickname_wide.c_str()), nickname_wide.size());
                ::setpkmotname(p_pkm->pkm_data, (wchar_t*)(trainer_name_wide.c_str()), trainer_name_wide.size());
            #endif

            unsigned int raw_held = t_pkmn->get_held_item()->get_item_id();
            p_pkm->pkm_data.item = ::Items::items(database::get_item_index(raw_held, t_pkmn->get_game_id()));

            uint8_t* metlevel_int = reinterpret_cast<uint8_t*>(&(p_pkm->pkm_data.ball)+1);
            set_gen_456_met_level(metlevel_int, t_pkmn->get_met_level());
            p_pkm->pkm_data.ball = ::Balls::balls(libpkmn_ball_to_game_ball(reverse_ball_dict.at(t_pkmn->get_ball(), PokeBalls::POKE_BALL)));
            set_gen_456_otgender(metlevel_int, (t_pkmn->get_trainer_gender().std_string() == "Female"));
            p_pkm->pkm_data.pid = t_pkmn->get_personality();
            p_pkm->pkm_data.tid = t_pkmn->get_trainer_public_id();
            p_pkm->pkm_data.sid = t_pkmn->get_trainer_secret_id();

            dict<std::string, unsigned int> stats = t_pkmn->get_stats();
            p_pkm->party_data.maxhp = stats["HP"];
            p_pkm->party_data.attack = stats["Attack"];
            p_pkm->party_data.defense = stats["Defense"];
            p_pkm->party_data.spatk = stats["Special Attack"];
            p_pkm->party_data.spdef = stats["Special Defense"];
            p_pkm->party_data.speed = stats["Speed"];

            dict<std::string, unsigned int> IVs = t_pkmn->get_IVs();
            uint32_t* IVint = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.ppup[3])+1);
            modern_set_IV(IVint, Stats::HP, IVs["HP"]);
            modern_set_IV(IVint, Stats::ATTACK, IVs["Attack"]);
            modern_set_IV(IVint, Stats::DEFENSE, IVs["Defense"]);
            modern_set_IV(IVint, Stats::SPECIAL_ATTACK, IVs["Special Attack"]);
            modern_set_IV(IVint, Stats::SPECIAL_DEFENSE, IVs["Special Defense"]);
            modern_set_IV(IVint, Stats::SPEED, IVs["Speed"]);

            dict<std::string, unsigned int> EVs = t_pkmn->get_EVs();
            p_pkm->pkm_data.evs.hp = EVs["HP"];
            p_pkm->pkm_data.evs.attack = EVs["Attack"];
            p_pkm->pkm_data.evs.defense = EVs["Defense"];
            p_pkm->pkm_data.evs.spatk = EVs["Special Attack"];
            p_pkm->pkm_data.evs.spdef = EVs["Special Defense"];
            p_pkm->pkm_data.evs.speed = EVs["Speed"];

            p_pkm->pkm_data.hometown = ::Hometowns::hometowns(libpkmn_game_to_hometown(t_pkmn->get_game_id()));

            //Attributes
            dict<std::string, int> attributes = t_pkmn->get_attributes();

            uint8_t* markings = reinterpret_cast<uint8_t*>(&(p_pkm->pkm_data.ability)+1);
            p_pkm->pkm_data.tameness = attributes.at("friendship",0);
            set_marking(markings, Markings::CIRCLE, attributes.at("circle",false));
            set_marking(markings, Markings::TRIANGLE, attributes.at("triangle",false));
            set_marking(markings, Markings::SQUARE, attributes.at("square",false));
            set_marking(markings, Markings::HEART, attributes.at("heart",false));
            set_marking(markings, Markings::STAR, attributes.at("star",false));
            set_marking(markings, Markings::DIAMOND, attributes.at("diamond",false));

            p_pkm->pkm_data.country = Countries::countries(attributes.at("country",2)); //Default to English
            p_pkm->pkm_data.contest.cool = attributes.at("cool",false);
            p_pkm->pkm_data.contest.beauty = attributes.at("beauty",false);
            p_pkm->pkm_data.contest.cute = attributes.at("cute",false);
            p_pkm->pkm_data.contest.smart = attributes.at("smart",false);
            p_pkm->pkm_data.contest.tough = attributes.at("tough",false);
            p_pkm->pkm_data.contest.sheen = attributes.at("sheen",false);

            uint32_t* hribbon1 = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.hribbon1));
            set_ribbon(hribbon1, Ribbons::Hoenn::COOL, attributes.at("hoenn_cool_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::COOL_SUPER, attributes.at("hoenn_cool_super_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::COOL_HYPER, attributes.at("hoenn_cool_hyper_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::COOL_MASTER, attributes.at("hoenn_cool_master_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY, attributes.at("hoenn_beauty_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_SUPER, attributes.at("hoenn_beauty_super_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_HYPER, attributes.at("hoenn_beauty_hyper_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_MASTER, attributes.at("hoenn_beauty_master_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::CUTE, attributes.at("hoenn_cute_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::CUTE_SUPER, attributes.at("hoenn_cute_super_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::CUTE_HYPER, attributes.at("hoenn_cute_hyper_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::CUTE_MASTER, attributes.at("hoenn_cute_master_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::SMART, attributes.at("hoenn_smart_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::SMART_SUPER, attributes.at("hoenn_smart_super_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::SMART_HYPER, attributes.at("hoenn_smart_hyper_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::SMART_MASTER, attributes.at("hoenn_smart_master_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::TOUGH, attributes.at("hoenn_tough_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_SUPER, attributes.at("hoenn_tough_super_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_HYPER, attributes.at("hoenn_tough_hyper_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_MASTER, attributes.at("hoenn_tough_master_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::CHAMPION, attributes.at("hoenn_champion_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::WINNING, attributes.at("hoenn_winning_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::VICTORY, attributes.at("hoenn_victory_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::ARTIST, attributes.at("hoenn_artist_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::EFFORT, attributes.at("hoenn_effort_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::MARINE, attributes.at("hoenn_marine_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::LAND, attributes.at("hoenn_land_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::SKY, attributes.at("hoenn_sky_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::COUNTRY, attributes.at("hoenn_country_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::NATIONAL, attributes.at("hoenn_national_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::EARTH, attributes.at("hoenn_earth_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::WORLD, attributes.at("hoenn_world_ribbon",false));

            uint32_t* sribbon1 = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.sribbon1));
            set_ribbon(sribbon1, Ribbons::Sinnoh::CHAMPION, attributes.at("sinnoh_champion_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::ABILITY, attributes.at("sinnoh_ability_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::GREAT_ABILITY, attributes.at("sinnoh_great_ability_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::DOUBLE_ABILITY, attributes.at("sinnoh_double_ability_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::MULTI_ABILITY, attributes.at("sinnoh_multi_ability_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::PAIR_ABILITY, attributes.at("sinnoh_pair_ability_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::WORLD_ABILITY, attributes.at("sinnoh_ability_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::ALERT, attributes.at("sinnoh_alert_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::SHOCK, attributes.at("sinnoh_shock_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::DOWNCAST, attributes.at("sinnoh_downcast_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::CARELESS, attributes.at("sinnoh_careless_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::RELAX, attributes.at("sinnoh_relax_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::SNOOZE, attributes.at("sinnoh_snooze_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::SMILE, attributes.at("sinnoh_smile_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::GORGEOUS, attributes.at("sinnoh_gorgeous_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::ROYAL, attributes.at("sinnoh_royal_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::GORGEOUS_ROYAL, attributes.at("sinnoh_gorgeous_royal_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::RECORD, attributes.at("sinnoh_record_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::HISTORY, attributes.at("sinnoh_history_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::LEGEND, attributes.at("sinnoh_legend_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::RED, attributes.at("sinnoh_red_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::GREEN, attributes.at("sinnoh_green_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::BLUE, attributes.at("sinnoh_blue_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::FESTIVAL, attributes.at("sinnoh_festival_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::CARNIVAL, attributes.at("sinnoh_carnival_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::CLASSIC, attributes.at("sinnoh_classic_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::PREMIER, attributes.at("sinnoh_premier_ribbon",false));

            uint32_t* sribbon3 = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.sribbon3));
            set_ribbon(sribbon3, Ribbons::Sinnoh::COOL-23, attributes.at("sinnoh_cool_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::COOL_SUPER-23, attributes.at("sinnoh_cool_great_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::COOL_HYPER-23, attributes.at("sinnoh_cool_ultra_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::COOL_MASTER-23, attributes.at("sinnoh_cool_master_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY-23, attributes.at("sinnoh_beauty_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY_SUPER-23, attributes.at("sinnoh_beauty_great_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY_HYPER-23, attributes.at("sinnoh_beauty_ultra_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY_MASTER-23, attributes.at("sinnoh_beauty_master_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::CUTE-23, attributes.at("sinnoh_cute_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::CUTE_SUPER-23, attributes.at("sinnoh_cute_great_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::CUTE_HYPER-23, attributes.at("sinnoh_cute_ultra_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::CUTE_MASTER-23, attributes.at("sinnoh_cute_master_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::SMART-23, attributes.at("sinnoh_smart_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::SMART_SUPER-23, attributes.at("sinnoh_smart_great_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::SMART_HYPER-23, attributes.at("sinnoh_smart_ultra_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::SMART_MASTER-23, attributes.at("sinnoh_smart_master_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH-23, attributes.at("sinnoh_tough_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH_SUPER-23, attributes.at("sinnoh_tough_great_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH_HYPER-23, attributes.at("sinnoh_tough_ultra_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH_MASTER-23, attributes.at("sinnoh_tough_master_ribbon",false));

            p_pkm->pkm_data.eggdate.year = attributes.at("eggmet_year",0);
            p_pkm->pkm_data.eggdate.month = attributes.at("eggmet_month",0);
            p_pkm->pkm_data.eggdate.day = attributes.at("eggmet_day",0);
            p_pkm->pkm_data.metdate.year = attributes.at("met_year",0);
            p_pkm->pkm_data.metdate.month = attributes.at("met_month",0);
            p_pkm->pkm_data.metdate.day = attributes.at("met_day",0);
        }

        team_pokemon::sptr pkmds_g6_pokemon_to_team_pokemon(party_pkx* p_pkx)
        {
            unsigned int level, from_game, species_id;

            level = p_pkx->partyx_data.level;
            from_game = hometown_to_libpkmn_game(p_pkx->pkx_data.hometown);
            species_id = database::get_species_id(p_pkx->pkx_data.species, from_game);

            team_pokemon::sptr t_pkmn = team_pokemon::make(species_id, from_game, level,
                                        p_pkx->pkx_data.moves[0], p_pkx->pkx_data.moves[1],
                                        p_pkx->pkx_data.moves[2], p_pkx->pkx_data.moves[3]);

            t_pkmn->set_nickname(p_pkx->pkx_data.nickname);
            t_pkmn->set_trainer_name(p_pkx->pkx_data.otname);

            t_pkmn->set_move_PP(p_pkx->pkx_data.pp[0], 1);
            t_pkmn->set_move_PP(p_pkx->pkx_data.pp[1], 2);
            t_pkmn->set_move_PP(p_pkx->pkx_data.pp[2], 3);
            t_pkmn->set_move_PP(p_pkx->pkx_data.pp[3], 4);
            
            uint8_t* metlevel_int = reinterpret_cast<uint8_t*>(&(p_pkx->pkx_data.ball)+1);
            t_pkmn->set_met_level(get_gen_456_met_level(metlevel_int));
            t_pkmn->set_ball(ball_dict.at(game_ball_to_libpkmn_ball(p_pkx->pkx_data.ball), "Poke Ball"));
            if(get_gen_456_otgender(metlevel_int)) t_pkmn->set_trainer_gender("Female");
            else t_pkmn->set_trainer_gender("Male");
            
            t_pkmn->set_held_item(item::make(database::get_item_id(p_pkx->pkx_data.item, from_game), from_game));
            t_pkmn->set_personality(p_pkx->pkx_data.pid);
            t_pkmn->set_trainer_public_id(p_pkx->pkx_data.tid);
            t_pkmn->set_trainer_secret_id(p_pkx->pkx_data.sid);
            
            t_pkmn->set_EV("HP", p_pkx->pkx_data.evs.hp);
            t_pkmn->set_EV("Attack", p_pkx->pkx_data.evs.attack);
            t_pkmn->set_EV("Defense", p_pkx->pkx_data.evs.defense);
            t_pkmn->set_EV("Special Attack", p_pkx->pkx_data.evs.spatk);
            t_pkmn->set_EV("Special Defense", p_pkx->pkx_data.evs.spdef);
            t_pkmn->set_EV("Speed", p_pkx->pkx_data.evs.speed);
            
            uint32_t* ivs = reinterpret_cast<uint32_t*>(&(p_pkx->pkx_data.ivs));
            t_pkmn->set_IV("HP", modern_get_IV(ivs, Stats::HP));
            t_pkmn->set_IV("Attack", modern_get_IV(ivs, Stats::ATTACK));
            t_pkmn->set_IV("Defense", modern_get_IV(ivs, Stats::DEFENSE));
            t_pkmn->set_IV("Special Attack", modern_get_IV(ivs, Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV("Special Defense", modern_get_IV(ivs, Stats::SPECIAL_DEFENSE));
            t_pkmn->set_IV("Speed", modern_get_IV(ivs, Stats::SPEED));
            
            return t_pkmn; 
        }
        
        void team_pokemon_to_pkmds_g6_pokemon(team_pokemon::sptr t_pkmn, party_pkx* p_pkx)
        {
            p_pkx->pkx_data.species = ::Species_g6::pkxspecies(
                                        database::get_species_index(t_pkmn->get_pokemon_id(),
                                        t_pkmn->get_game_id()));
            
            moveset_t moves;
            t_pkmn->get_moves(moves);
            p_pkx->pkx_data.moves[0] = ::Moves::moves(moves[0]->get_move_id());
            p_pkx->pkx_data.moves[1] = ::Moves::moves(moves[1]->get_move_id());
            p_pkx->pkx_data.moves[2] = ::Moves::moves(moves[2]->get_move_id());
            p_pkx->pkx_data.moves[3] = ::Moves::moves(moves[3]->get_move_id());

            std::vector<unsigned int> move_PPs;
            t_pkmn->get_move_PPs(move_PPs);
            p_pkx->pkx_data.pp[0] = move_PPs[0];
            p_pkx->pkx_data.pp[1] = move_PPs[1];
            p_pkx->pkx_data.pp[2] = move_PPs[2];
            p_pkx->pkx_data.pp[3] = move_PPs[3];
            
            p_pkx->partyx_data.level = t_pkmn->get_level();
            
            //TODO: setting nicknames and trainer names
            
            unsigned int raw_held = t_pkmn->get_held_item()->get_item_id();
            p_pkx->pkx_data.item = ::Items::items(database::get_item_index(raw_held, t_pkmn->get_game_id()));
            
            uint8_t* metlevel_int = reinterpret_cast<uint8_t*>(&(p_pkx->pkx_data.ball)+1);
            set_gen_456_met_level(metlevel_int, t_pkmn->get_met_level());
            p_pkx->pkx_data.ball = Balls::balls(libpkmn_ball_to_game_ball(reverse_ball_dict[t_pkmn->get_ball()]));
            set_gen_456_otgender(metlevel_int, (t_pkmn->get_trainer_gender().std_string() == "Female"));
            p_pkx->pkx_data.pid = t_pkmn->get_personality();
            p_pkx->pkx_data.tid = t_pkmn->get_trainer_public_id();
            p_pkx->pkx_data.sid = t_pkmn->get_trainer_secret_id();

            dict<std::string, unsigned int> stats = t_pkmn->get_stats();
            p_pkx->partyx_data.maxhp = stats["HP"];
            p_pkx->partyx_data.attack = stats["Attack"];
            p_pkx->partyx_data.defense = stats["Defense"];
            p_pkx->partyx_data.spatk = stats["Special Attack"];
            p_pkx->partyx_data.spdef = stats["Special Defense"];
            p_pkx->partyx_data.speed = stats["Speed"];

            dict<std::string, unsigned int> IVs = t_pkmn->get_IVs();
            uint32_t* IVint = reinterpret_cast<uint32_t*>(&(p_pkx->pkx_data.ppups[3])+1);
            modern_set_IV(IVint, Stats::HP, IVs["HP"]);
            modern_set_IV(IVint, Stats::ATTACK, IVs["Attack"]);
            modern_set_IV(IVint, Stats::DEFENSE, IVs["Defense"]);
            modern_set_IV(IVint, Stats::SPECIAL_ATTACK, IVs["Special Attack"]);
            modern_set_IV(IVint, Stats::SPECIAL_DEFENSE, IVs["Special Defense"]);
            modern_set_IV(IVint, Stats::SPEED, IVs["Speed"]);

            dict<std::string, unsigned int> EVs = t_pkmn->get_EVs();
            p_pkx->pkx_data.evs.hp = EVs["HP"];
            p_pkx->pkx_data.evs.attack = EVs["Attack"];
            p_pkx->pkx_data.evs.defense = EVs["Defense"];
            p_pkx->pkx_data.evs.spatk = EVs["Special Attack"];
            p_pkx->pkx_data.evs.spdef = EVs["Special Defense"];
            p_pkx->pkx_data.evs.speed = EVs["Speed"];

            p_pkx->pkx_data.hometown = ::Hometowns::hometowns(libpkmn_game_to_hometown(t_pkmn->get_game_id()));
        }
        
        void pokelib_pokemon_to_pkmds_g5_pokemon(PokeLib::Pokemon pokelib_pkmn, party_pkm* p_pkm)
        {
            team_pokemon_to_pkmds_g5_pokemon(pokelib_pokemon_to_team_pokemon(pokelib_pkmn), p_pkm);

            //Manually set egg met location to Faraway place and met location to Poke Transfer
            p_pkm->pkm_data.eggmet = Locations::unovafarawayplace;
            p_pkm->pkm_data.met = Locations::poketransfer;
        }

        PokeLib::Pokemon pkmds_g5_pokemon_to_pokelib_pokemon(party_pkm* p_pkm)
        {
            PokeLib::Pokemon pokelib_pkmn = team_pokemon_to_pokelib_pokemon(pkmds_g5_pokemon_to_team_pokemon(p_pkm));

            //Manually set egg met and met locations to Faraway Place
            pokelib_pkmn.pkm->pkm.eggLoc_DP = char(3002);
            pokelib_pkmn.pkm->pkm.eggLoc_Plat = char(3002);
            pokelib_pkmn.pkm->pkm.metLoc_DP = char(3002);
            pokelib_pkmn.pkm->pkm.metLoc_Plat = char(3002);

            //If Pokemon was caught in a Gen 5 ball, set it to standard Poke Ball
            if(pokelib_pkmn.pkm->pkm.pokeball > char(Balls::sportball)) pokelib_pkmn.pkm->pkm.pokeball = 0x4;

            //Manually set encounter type to Special Event
            pokelib_pkmn.pkm->pkm.encounterType = 0x0;

            return pokelib_pkmn;
        }
    } /* namespace conversions */
} /* namespace pkmn */
