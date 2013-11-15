/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
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

#include <pkmnsim/enums.hpp>
#include <pkmnsim/item.hpp>
#include <pkmnsim/move.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/team_pokemon.hpp>
#include <pkmnsim/database/queries.hpp>
#include <pkmnsim/types/pokemon_text.hpp>

#include <pkmds/pkmds_g5_sqlite.h>

#include "../SQLiteCpp/src/SQLiteC++.h"

#include "pokemon.hpp"
#include "../library_bridge.hpp"

using namespace std;

namespace pkmnsim
{
    namespace conversions
    {
        team_pokemon::sptr pokehack_pokemon_to_team_pokemon(belt_pokemon_t* b_pkmn_t,
                                                            pokemon_attacks_t* pkmn_a_t,
                                                            pokemon_effort_t* pkmn_e_t,
                                                            pokemon_misc_t* pkmn_m_t,
                                                            pokemon_growth_t* pkmn_g_t)
        {
            unsigned int level, from_game;
            
            level = b_pkmn_t->level;
            from_game = hometown_to_pkmnsim_game(pkmn_m_t->game);
            
            team_pokemon::sptr t_pkmn = team_pokemon::make(pkmn_g_t->species, from_game, level,
                                                         pkmn_a_t->atk1, pkmn_a_t->atk2,
                                                         pkmn_a_t->atk3, pkmn_a_t->atk4);
                                                         
            t_pkmn->set_nickname(pokehack_get_text(b_pkmn_t->name, true));
            t_pkmn->set_trainer_name(pokehack_get_text(b_pkmn_t->otname, false));
            t_pkmn->set_held_item(item::make(database::get_item_id(pkmn_g_t->held, from_game), from_game));
            t_pkmn->set_personality(b_pkmn_t->personality);
            t_pkmn->set_trainer_id(b_pkmn_t->otid);
            
            t_pkmn->set_EV(Stats::HP, pkmn_e_t->hp);
            t_pkmn->set_EV(Stats::ATTACK, pkmn_e_t->attack);
            t_pkmn->set_EV(Stats::DEFENSE, pkmn_e_t->defense);
            t_pkmn->set_EV(Stats::SPECIAL_ATTACK, pkmn_e_t->spatk);
            t_pkmn->set_EV(Stats::SPECIAL_DEFENSE, pkmn_e_t->spdef);
            t_pkmn->set_EV(Stats::SPEED, pkmn_e_t->speed);

            uint32_t* IVint = &(pkmn_m_t->IVint);            
            t_pkmn->set_IV(Stats::HP, gen3_4_5_get_IV(IVint, Stats::HP));
            t_pkmn->set_IV(Stats::ATTACK, gen3_4_5_get_IV(IVint, Stats::ATTACK));
            t_pkmn->set_IV(Stats::DEFENSE, gen3_4_5_get_IV(IVint, Stats::DEFENSE));
            t_pkmn->set_IV(Stats::SPECIAL_ATTACK, gen3_4_5_get_IV(IVint, Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV(Stats::SPECIAL_DEFENSE, gen3_4_5_get_IV(IVint, Stats::SPECIAL_DEFENSE));
            t_pkmn->set_IV(Stats::SPEED, gen3_4_5_get_IV(IVint, Stats::SPEED));
          
            uint16_t* metlevel_int = reinterpret_cast<uint16_t*>(&(pkmn_m_t->locationcaught)+1); 
            t_pkmn->set_met_level(get_gen3_met_level(metlevel_int));
            t_pkmn->set_ball(game_ball_to_pkmnsim_ball(get_gen3_ball(metlevel_int)));
            if(get_gen3_otgender(metlevel_int)) t_pkmn->set_trainer_gender(Genders::FEMALE);
            else t_pkmn->set_trainer_gender(Genders::MALE);

            uint8_t* markint = &(b_pkmn_t->markint); 
            t_pkmn->set_attribute("friendship", pkmn_g_t->happiness);
            t_pkmn->set_attribute("circle", get_marking(markint, Markings::CIRCLE));
            t_pkmn->set_attribute("triangle", get_marking(markint, Markings::TRIANGLE));
            t_pkmn->set_attribute("square", get_marking(markint, Markings::SQUARE));
            t_pkmn->set_attribute("heart", get_marking(markint, Markings::HEART));
            t_pkmn->set_attribute("country", b_pkmn_t->language);
            
            t_pkmn->set_attribute("cool", pkmn_e_t->coolness);
            t_pkmn->set_attribute("beauty", pkmn_e_t->beauty);
            t_pkmn->set_attribute("cute", pkmn_e_t->cuteness);
            t_pkmn->set_attribute("smart", pkmn_e_t->smartness);
            t_pkmn->set_attribute("tough", pkmn_e_t->toughness);
            
            uint32_t* ribbonint = &(pkmn_m_t->ribbonint);
            t_pkmn->set_attribute("hoenn_cool_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::COOL));
            t_pkmn->set_attribute("hoenn_cool_super_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::COOL_SUPER));
            t_pkmn->set_attribute("hoenn_cool_hyper_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::COOL_HYPER));
            t_pkmn->set_attribute("hoenn_cool_master_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::COOL_MASTER));
            t_pkmn->set_attribute("hoenn_beauty_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::BEAUTY));
            t_pkmn->set_attribute("hoenn_beauty_super_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::BEAUTY_SUPER));
            t_pkmn->set_attribute("hoenn_beauty_hyper_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::BEAUTY_HYPER));
            t_pkmn->set_attribute("hoenn_beauty_master_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::BEAUTY_MASTER));
            t_pkmn->set_attribute("hoenn_cute_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::CUTE));
            t_pkmn->set_attribute("hoenn_cute_super_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::CUTE_SUPER));
            t_pkmn->set_attribute("hoenn_cute_hyper_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::CUTE_HYPER));
            t_pkmn->set_attribute("hoenn_cute_master_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::CUTE_MASTER));
            t_pkmn->set_attribute("hoenn_smart_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::SMART));
            t_pkmn->set_attribute("hoenn_smart_super_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::SMART_SUPER));
            t_pkmn->set_attribute("hoenn_smart_hyper_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::SMART_HYPER));
            t_pkmn->set_attribute("hoenn_smart_master_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::SMART_MASTER));
            t_pkmn->set_attribute("hoenn_tough_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::TOUGH));
            t_pkmn->set_attribute("hoenn_tough_super_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::TOUGH_SUPER));
            t_pkmn->set_attribute("hoenn_tough_hyper_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::TOUGH_HYPER));
            t_pkmn->set_attribute("hoenn_tough_master_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::TOUGH_MASTER));
            
            t_pkmn->set_attribute("hoenn_champion_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::CHAMPION));
            t_pkmn->set_attribute("hoenn_winning_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::WINNING));
            t_pkmn->set_attribute("hoenn_victory_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::VICTORY));
            t_pkmn->set_attribute("hoenn_artist_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::ARTIST));
            t_pkmn->set_attribute("hoenn_effort_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::EFFORT));
            t_pkmn->set_attribute("hoenn_marine_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::MARINE));
            t_pkmn->set_attribute("hoenn_land_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::LAND));
            t_pkmn->set_attribute("hoenn_sky_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::SKY));
            t_pkmn->set_attribute("hoenn_country_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::COUNTRY));
            t_pkmn->set_attribute("hoenn_national_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::NATIONAL));
            t_pkmn->set_attribute("hoenn_earth_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::EARTH));
            t_pkmn->set_attribute("hoenn_world_ribbon", get_hoenn_ribbon(ribbonint, Ribbons::Hoenn::WORLD));
            
            return t_pkmn;
        }

        void team_pokemon_to_pokehack_pokemon(team_pokemon::sptr t_pkmn,
                                              belt_pokemon_t* b_pkmn_t,
                                              pokemon_attacks_t* pkmn_a_t,
                                              pokemon_effort_t* pkmn_e_t,
                                              pokemon_misc_t* pkmn_m_t,
                                              pokemon_growth_t* pkmn_g_t)
        {
            pkmn_g_t->species = t_pkmn->get_species_id(); //TODO: account for different forms
            b_pkmn_t->personality = t_pkmn->get_personality();
            b_pkmn_t->otid = t_pkmn->get_trainer_id();
            b_pkmn_t->level = t_pkmn->get_level();

            dict<char, int> pokehack_reverse_char_map = get_pokehack_reverse_char_map();
            string nickname = t_pkmn->get_nickname();
            for(int i = 0; i < 10; i++)
            {
                if(i < nickname.size()) b_pkmn_t->name[i] = pokehack_reverse_char_map[nickname[i]];
                else b_pkmn_t->name[i] = 0xFF;
            }
            string trainer_name = t_pkmn->get_trainer_name();
            for(int i = 0; i < 7; i++)
            {
                if(i < trainer_name.size()) b_pkmn_t->otname[i] = pokehack_reverse_char_map[trainer_name[i]];
                else b_pkmn_t->otname[i] = 0xFF;
            }

            moveset_t moves = t_pkmn->get_moves();
            pkmn_a_t->atk1 = moves[0]->get_move_id();
            pkmn_a_t->atk2 = moves[1]->get_move_id();
            pkmn_a_t->atk3 = moves[2]->get_move_id();
            pkmn_a_t->atk4 = moves[3]->get_move_id();

            pkmn_m_t->game = pkmnsim_game_to_hometown(t_pkmn->get_game_id());
            
            unsigned int raw_held = t_pkmn->get_held_item()->get_item_id();
            pkmn_g_t->held = database::get_item_index(raw_held, t_pkmn->get_game_id());

            uint16_t* metlevel_int = reinterpret_cast<uint16_t*>(&(pkmn_m_t->locationcaught)+1);
            set_gen3_met_level(metlevel_int, t_pkmn->get_met_level());
            set_gen3_ball(metlevel_int, pkmnsim_ball_to_game_ball(t_pkmn->get_ball()));
            set_gen3_otgender(metlevel_int, (t_pkmn->get_trainer_gender() == Genders::FEMALE));

            dict<unsigned int, unsigned int> stats = t_pkmn->get_stats();
            b_pkmn_t->maxHP = stats[Stats::HP];
            b_pkmn_t->attack = stats[Stats::ATTACK];
            b_pkmn_t->defense = stats[Stats::DEFENSE];
            b_pkmn_t->spatk = stats[Stats::SPECIAL_ATTACK];
            b_pkmn_t->spdef = stats[Stats::SPECIAL_DEFENSE];
            b_pkmn_t->speed = stats[Stats::SPEED];

            dict<unsigned int, unsigned int> EVs = t_pkmn->get_EVs();
            pkmn_e_t->hp = EVs[Stats::HP];
            pkmn_e_t->attack = EVs[Stats::ATTACK];
            pkmn_e_t->defense = EVs[Stats::DEFENSE];
            pkmn_e_t->spatk = EVs[Stats::SPECIAL_ATTACK];
            pkmn_e_t->spdef = EVs[Stats::SPECIAL_DEFENSE];
            pkmn_e_t->speed = EVs[Stats::SPEED];

            uint32_t* IVint = &(pkmn_m_t->IVint);
            dict<unsigned int, unsigned int> IVs = t_pkmn->get_IVs();
            gen3_4_5_set_IV(IVint, Stats::HP, IVs[Stats::HP]);
            gen3_4_5_set_IV(IVint, Stats::ATTACK, IVs[Stats::ATTACK]);
            gen3_4_5_set_IV(IVint, Stats::DEFENSE, IVs[Stats::DEFENSE]);
            gen3_4_5_set_IV(IVint, Stats::SPECIAL_ATTACK, IVs[Stats::SPECIAL_ATTACK]);
            gen3_4_5_set_IV(IVint, Stats::SPECIAL_DEFENSE, IVs[Stats::SPECIAL_DEFENSE]);
            gen3_4_5_set_IV(IVint, Stats::SPEED, IVs[Stats::SPEED]);
            
            //Attributes

            uint8_t* markint = &(b_pkmn_t->markint);
            if(t_pkmn->has_attribute("friendship"))
                pkmn_g_t->happiness = t_pkmn->get_attribute("friendship");
            if(t_pkmn->has_attribute("circle"))
                set_marking(markint, Markings::CIRCLE, t_pkmn->get_attribute("circle"));
            if(t_pkmn->has_attribute("triangle"))
                set_marking(markint, Markings::TRIANGLE, t_pkmn->get_attribute("triangle"));
            if(t_pkmn->has_attribute("square"))
                set_marking(markint, Markings::SQUARE, t_pkmn->get_attribute("square"));
            if(t_pkmn->has_attribute("heart"))
                set_marking(markint, Markings::HEART, t_pkmn->get_attribute("heart"));
            if(t_pkmn->has_attribute("country"))
                b_pkmn_t->language = t_pkmn->get_attribute("country");

            if(t_pkmn->has_attribute("cool"))
                pkmn_e_t->coolness = t_pkmn->get_attribute("cool");
            if(t_pkmn->has_attribute("beauty"))
                pkmn_e_t->beauty = t_pkmn->get_attribute("beauty");
            if(t_pkmn->has_attribute("cute"))
                pkmn_e_t->cuteness = t_pkmn->get_attribute("cute");
            if(t_pkmn->has_attribute("smart"))
                pkmn_e_t->smartness = t_pkmn->get_attribute("smart");
            if(t_pkmn->has_attribute("tough"))
                pkmn_e_t->toughness = t_pkmn->get_attribute("tough");

            uint32_t* ribbonint = &(pkmn_m_t->ribbonint);
            if(t_pkmn->has_attribute("hoenn_cool_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::COOL, t_pkmn->get_attribute("hoenn_cool_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_super_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::COOL_SUPER, t_pkmn->get_attribute("hoenn_cool_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_hyper_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::COOL_HYPER, t_pkmn->get_attribute("hoenn_cool_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_master_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::COOL_MASTER, t_pkmn->get_attribute("hoenn_cool_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::BEAUTY, t_pkmn->get_attribute("hoenn_beauty_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_super_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::BEAUTY_SUPER, t_pkmn->get_attribute("hoenn_beauty_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_hyper_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::BEAUTY_HYPER, t_pkmn->get_attribute("hoenn_beauty_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_master_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::BEAUTY_MASTER, t_pkmn->get_attribute("hoenn_beauty_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::CUTE, t_pkmn->get_attribute("hoenn_cute_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_super_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::CUTE_SUPER, t_pkmn->get_attribute("hoenn_cute_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_hyper_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::CUTE_HYPER, t_pkmn->get_attribute("hoenn_cute_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_master_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::CUTE_MASTER, t_pkmn->get_attribute("hoenn_cute_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::SMART, t_pkmn->get_attribute("hoenn_smart_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_super_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::SMART_SUPER, t_pkmn->get_attribute("hoenn_smart_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_hyper_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::SMART_HYPER, t_pkmn->get_attribute("hoenn_smart_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_master_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::SMART_MASTER, t_pkmn->get_attribute("hoenn_smart_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::TOUGH, t_pkmn->get_attribute("hoenn_tough_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_super_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::TOUGH_SUPER, t_pkmn->get_attribute("hoenn_tough_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_hyper_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::TOUGH_HYPER, t_pkmn->get_attribute("hoenn_tough_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_master_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::TOUGH_MASTER, t_pkmn->get_attribute("hoenn_tough_master_ribbon"));

            if(t_pkmn->has_attribute("hoenn_champion_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::CHAMPION, t_pkmn->get_attribute("hoenn_champion_ribbon"));
            if(t_pkmn->has_attribute("hoenn_winning_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::WINNING, t_pkmn->get_attribute("hoenn_winning_ribbon"));
            if(t_pkmn->has_attribute("hoenn_victory_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::VICTORY, t_pkmn->get_attribute("hoenn_victory_ribbon"));
            if(t_pkmn->has_attribute("hoenn_artist_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::ARTIST, t_pkmn->get_attribute("hoenn_artist_ribbon"));
            if(t_pkmn->has_attribute("hoenn_effort_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::EFFORT, t_pkmn->get_attribute("hoenn_effort_ribbon"));
            if(t_pkmn->has_attribute("hoenn_marine_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::MARINE, t_pkmn->get_attribute("hoenn_marine_ribbon"));
            if(t_pkmn->has_attribute("hoenn_land_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::LAND, t_pkmn->get_attribute("hoenn_land_ribbon"));
            if(t_pkmn->has_attribute("hoenn_sky_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::SKY, t_pkmn->get_attribute("hoenn_sky_ribbon"));
            if(t_pkmn->has_attribute("hoenn_country_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::COUNTRY, t_pkmn->get_attribute("hoenn_country_ribbon"));
            if(t_pkmn->has_attribute("hoenn_national_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::NATIONAL, t_pkmn->get_attribute("hoenn_national_ribbon"));
            if(t_pkmn->has_attribute("hoenn_earth_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::EARTH, t_pkmn->get_attribute("hoenn_earth_ribbon"));
            if(t_pkmn->has_attribute("hoenn_world_ribbon"))
                set_hoenn_ribbon(ribbonint, Ribbons::Hoenn::WORLD, t_pkmn->get_attribute("hoenn_world_ribbon"));
        }

        team_pokemon::sptr pokelib_pokemon_to_team_pokemon(PokeLib::Pokemon pokelib_pkmn)
        {
            unsigned int level, from_game;

            level = pokelib_pkmn.getLevel();
            from_game = hometown_to_pkmnsim_game(pokelib_pkmn.pkm->pkm.hometown);

            team_pokemon::sptr t_pkmn = team_pokemon::make(pokelib_pkmn.pkm->pkm.species, from_game, level,
                                        pokelib_pkmn.pkm->pkm.move[0], pokelib_pkmn.pkm->pkm.move[1],
                                        pokelib_pkmn.pkm->pkm.move[2], pokelib_pkmn.pkm->pkm.move[3]);

            t_pkmn->set_nickname(pokelib_pkmn.getNickname());
            t_pkmn->set_trainer_name(pokelib_pkmn.getTrainerName());

            uint8_t* metlevel_int = reinterpret_cast<uint8_t*>(&(pokelib_pkmn.pkm->pkm.pokeball)+1);
            t_pkmn->set_met_level(get_gen4_5_met_level(metlevel_int));
            t_pkmn->set_ball(game_ball_to_pkmnsim_ball(pokelib_pkmn.pkm->pkm.pokeball));
            if(get_gen4_5_otgender(metlevel_int)) t_pkmn->set_trainer_gender(Genders::FEMALE);
            else t_pkmn->set_trainer_gender(Genders::MALE);

            t_pkmn->set_held_item(item::make(database::get_item_id(pokelib_pkmn.pkm->pkm.held_item, from_game), from_game));
            t_pkmn->set_personality(pokelib_pkmn.pkm->pkm.pid);
            t_pkmn->set_public_trainer_id(pokelib_pkmn.pkm->pkm.ot_id);
            t_pkmn->set_secret_trainer_id(pokelib_pkmn.pkm->pkm.ot_sid);

            t_pkmn->set_EV(Stats::HP, pokelib_pkmn.pkm->pkm.ev_hp);
            t_pkmn->set_EV(Stats::ATTACK, pokelib_pkmn.pkm->pkm.ev_atk);
            t_pkmn->set_EV(Stats::DEFENSE, pokelib_pkmn.pkm->pkm.ev_def);
            t_pkmn->set_EV(Stats::SPECIAL_ATTACK, pokelib_pkmn.pkm->pkm.ev_satk);
            t_pkmn->set_EV(Stats::SPECIAL_DEFENSE, pokelib_pkmn.pkm->pkm.ev_sdef);
            t_pkmn->set_EV(Stats::SPEED, pokelib_pkmn.pkm->pkm.ev_spd);

            uint32_t* IVint = reinterpret_cast<uint32_t*>(&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1);
            t_pkmn->set_IV(Stats::HP, gen3_4_5_get_IV(IVint, Stats::HP));
            t_pkmn->set_IV(Stats::ATTACK, gen3_4_5_get_IV(IVint, Stats::ATTACK));
            t_pkmn->set_IV(Stats::DEFENSE, gen3_4_5_get_IV(IVint, Stats::DEFENSE));
            t_pkmn->set_IV(Stats::SPECIAL_ATTACK, gen3_4_5_get_IV(IVint, Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV(Stats::SPECIAL_DEFENSE, gen3_4_5_get_IV(IVint, Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV(Stats::SPEED, gen3_4_5_get_IV(IVint, Stats::SPEED));

            //TODO: use form data to set Pokemon-sim form, is fateful encounter

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

            uint32_t* ribbonHoenn = reinterpret_cast<uint32_t*>(&(pokelib_pkmn.pkm->pkm.ribbonHoenn));
            t_pkmn->set_attribute("hoenn_cool_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL));
            t_pkmn->set_attribute("hoenn_cool_super_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL_SUPER));
            t_pkmn->set_attribute("hoenn_cool_hyper_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL_HYPER));
            t_pkmn->set_attribute("hoenn_cool_master_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL_MASTER));
            t_pkmn->set_attribute("hoenn_beauty_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY));
            t_pkmn->set_attribute("hoenn_beauty_super_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY_SUPER));
            t_pkmn->set_attribute("hoenn_beauty_hyper_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY_HYPER));
            t_pkmn->set_attribute("hoenn_beauty_master_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY_MASTER));
            t_pkmn->set_attribute("hoenn_cute_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE));
            t_pkmn->set_attribute("hoenn_cute_super_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE_SUPER));
            t_pkmn->set_attribute("hoenn_cute_hyper_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE_HYPER));
            t_pkmn->set_attribute("hoenn_cute_master_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE_MASTER));
            t_pkmn->set_attribute("hoenn_smart_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART));
            t_pkmn->set_attribute("hoenn_smart_super_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART_SUPER));
            t_pkmn->set_attribute("hoenn_smart_hyper_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART_HYPER));
            t_pkmn->set_attribute("hoenn_smart_master_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART_MASTER));
            t_pkmn->set_attribute("hoenn_tough_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH));
            t_pkmn->set_attribute("hoenn_tough_super_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH_SUPER));
            t_pkmn->set_attribute("hoenn_tough_hyper_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH_HYPER));
            t_pkmn->set_attribute("hoenn_tough_master_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH_MASTER));
            t_pkmn->set_attribute("hoenn_champion_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::CHAMPION));
            t_pkmn->set_attribute("hoenn_winning_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::WINNING));
            t_pkmn->set_attribute("hoenn_victory_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::VICTORY));
            t_pkmn->set_attribute("hoenn_artist_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::ARTIST));
            t_pkmn->set_attribute("hoenn_effort_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::EFFORT));
            t_pkmn->set_attribute("hoenn_marine_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::MARINE));
            t_pkmn->set_attribute("hoenn_land_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::LAND));
            t_pkmn->set_attribute("hoenn_sky_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::SKY));
            t_pkmn->set_attribute("hoenn_country_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::COUNTRY));
            t_pkmn->set_attribute("hoenn_national_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::NATIONAL));
            t_pkmn->set_attribute("hoenn_earth_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::EARTH));
            t_pkmn->set_attribute("hoenn_world_ribbon", get_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::WORLD));

            uint32_t* ribbonSinnohA = reinterpret_cast<uint32_t*>(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA));
            t_pkmn->set_attribute("sinnoh_champion_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::CHAMPION));
            t_pkmn->set_attribute("sinnoh_ability_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::ABILITY));
            t_pkmn->set_attribute("sinnoh_great_ability_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::GREAT_ABILITY));
            t_pkmn->set_attribute("sinnoh_double_ability_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::DOUBLE_ABILITY));
            t_pkmn->set_attribute("sinnoh_multi_ability_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::MULTI_ABILITY));
            t_pkmn->set_attribute("sinnoh_pair_ability_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::PAIR_ABILITY));
            t_pkmn->set_attribute("sinnoh_world_ability_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::WORLD_ABILITY));
            t_pkmn->set_attribute("sinnoh_alert_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::ALERT));
            t_pkmn->set_attribute("sinnoh_shock_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::SHOCK));
            t_pkmn->set_attribute("sinnoh_downcast_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::DOWNCAST));
            t_pkmn->set_attribute("sinnoh_careless_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::CARELESS));
            t_pkmn->set_attribute("sinnoh_relax_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::RELAX));
            t_pkmn->set_attribute("sinnoh_snooze_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::SNOOZE));
            t_pkmn->set_attribute("sinnoh_smile_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::SMILE));
            t_pkmn->set_attribute("sinnoh_gorgeous_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::GORGEOUS));
            t_pkmn->set_attribute("sinnoh_royal_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::ROYAL));
            t_pkmn->set_attribute("sinnoh_gorgeous_royal_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::GORGEOUS_ROYAL));
            t_pkmn->set_attribute("sinnoh_footprint_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::FOOTPRINT));
            t_pkmn->set_attribute("sinnoh_record_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::RECORD));
            t_pkmn->set_attribute("sinnoh_history_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::HISTORY));
            t_pkmn->set_attribute("sinnoh_legend_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::LEGEND));
            t_pkmn->set_attribute("sinnoh_red_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::RED));
            t_pkmn->set_attribute("sinnoh_green_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::GREEN));
            t_pkmn->set_attribute("sinnoh_blue_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::BLUE));
            t_pkmn->set_attribute("sinnoh_festival_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_carnival_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_classic_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::CLASSIC));
            t_pkmn->set_attribute("sinnoh_premier_ribbon", get_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::PREMIER));

            uint32_t* ribbonSinnohB = reinterpret_cast<uint32_t*>(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB));
            t_pkmn->set_attribute("sinnoh_cool_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::COOL-23));
            t_pkmn->set_attribute("sinnoh_cool_great_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::COOL_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cool_ultra_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::COOL_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cool_master_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::COOL_MASTER-23));
            t_pkmn->set_attribute("sinnoh_beauty_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY-23));
            t_pkmn->set_attribute("sinnoh_beauty_great_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY_GREAT-23));
            t_pkmn->set_attribute("sinnoh_beauty_ultra_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY_GREAT-23));
            t_pkmn->set_attribute("sinnoh_beauty_master_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY_MASTER-23));
            t_pkmn->set_attribute("sinnoh_cute_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::CUTE-23));
            t_pkmn->set_attribute("sinnoh_cute_great_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::CUTE_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cute_ultra_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::CUTE_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cute_master_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::CUTE_MASTER-23));
            t_pkmn->set_attribute("sinnoh_smart_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::SMART-23));
            t_pkmn->set_attribute("sinnoh_smart_great_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::SMART_GREAT-23));
            t_pkmn->set_attribute("sinnoh_smart_ultra_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::SMART_GREAT-23));
            t_pkmn->set_attribute("sinnoh_smart_master_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::SMART_MASTER-23));
            t_pkmn->set_attribute("sinnoh_tough_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::TOUGH-23));
            t_pkmn->set_attribute("sinnoh_tough_great_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::TOUGH_GREAT-23));
            t_pkmn->set_attribute("sinnoh_tough_ultra_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::TOUGH_GREAT-23));
            t_pkmn->set_attribute("sinnoh_tough_master_ribbon", get_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::TOUGH_MASTER-23));

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

            pokelib_pkmn.pkm->pkm.species = t_pkmn->get_species_id();
            pokelib_pkmn.setLevel(uint8_t(t_pkmn->get_level()));
            pokelib_pkmn.setNickname(t_pkmn->get_nickname());
            pokelib_pkmn.setTrainerName(t_pkmn->get_trainer_name());
            pokelib_pkmn.pkm->pkm.pid = t_pkmn->get_personality();
            pokelib_pkmn.pkm->pkm.ot_id = t_pkmn->get_public_trainer_id();
            pokelib_pkmn.pkm->pkm.ot_sid = t_pkmn->get_secret_trainer_id();
            
            unsigned int raw_held = t_pkmn->get_held_item()->get_item_id();
            pokelib_pkmn.pkm->pkm.held_item = database::get_item_index(raw_held, t_pkmn->get_game_id());

            uint8_t* metlevel_int = reinterpret_cast<uint8_t*>(&(pokelib_pkmn.pkm->pkm.pokeball)+1);
            set_gen4_5_met_level(metlevel_int, t_pkmn->get_met_level());
            pokelib_pkmn.pkm->pkm.pokeball = pkmnsim_ball_to_game_ball(t_pkmn->get_ball());
            set_gen4_5_met_level(metlevel_int, (t_pkmn->get_trainer_gender() == Genders::FEMALE));

            pokelib_pkmn.pkm->pkm.move[0] = t_pkmn->get_moves()[0]->get_move_id();
            pokelib_pkmn.pkm->pkm.move[1] = t_pkmn->get_moves()[1]->get_move_id();
            pokelib_pkmn.pkm->pkm.move[2] = t_pkmn->get_moves()[2]->get_move_id();
            pokelib_pkmn.pkm->pkm.move[3] = t_pkmn->get_moves()[3]->get_move_id();

            dict<unsigned int, unsigned int> stats = t_pkmn->get_stats();
            pokelib_pkmn.pkm->pkm.battle_max_hp = stats[Stats::HP];
            pokelib_pkmn.pkm->pkm.battle_atk = stats[Stats::ATTACK];
            pokelib_pkmn.pkm->pkm.battle_def = stats[Stats::DEFENSE];
            pokelib_pkmn.pkm->pkm.battle_satk = stats[Stats::SPECIAL_ATTACK];
            pokelib_pkmn.pkm->pkm.battle_sdef = stats[Stats::SPECIAL_DEFENSE];
            pokelib_pkmn.pkm->pkm.battle_spd = stats[Stats::SPEED];

            dict<unsigned int, unsigned int> EVs = t_pkmn->get_EVs();
            pokelib_pkmn.pkm->pkm.ev_hp = EVs[Stats::HP];
            pokelib_pkmn.pkm->pkm.ev_atk = EVs[Stats::ATTACK];
            pokelib_pkmn.pkm->pkm.ev_def = EVs[Stats::DEFENSE];
            pokelib_pkmn.pkm->pkm.ev_satk = EVs[Stats::SPECIAL_ATTACK];
            pokelib_pkmn.pkm->pkm.ev_sdef = EVs[Stats::SPECIAL_DEFENSE];
            pokelib_pkmn.pkm->pkm.ev_spd = EVs[Stats::SPEED];

            dict<unsigned int, unsigned int> IVs = t_pkmn->get_IVs();
            uint32_t* IVint = reinterpret_cast<uint32_t*>(&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1);
            gen3_4_5_set_IV(IVint, Stats::HP, IVs[Stats::HP]);
            gen3_4_5_set_IV(IVint, Stats::ATTACK, IVs[Stats::ATTACK]);
            gen3_4_5_set_IV(IVint, Stats::DEFENSE, IVs[Stats::DEFENSE]);
            gen3_4_5_set_IV(IVint, Stats::SPECIAL_ATTACK, IVs[Stats::SPECIAL_ATTACK]);
            gen3_4_5_set_IV(IVint, Stats::SPECIAL_DEFENSE, IVs[Stats::SPECIAL_DEFENSE]);
            gen3_4_5_set_IV(IVint, Stats::SPEED, IVs[Stats::SPEED]);

            pokelib_pkmn.pkm->pkm.hometown = pkmnsim_game_to_hometown(t_pkmn->get_game_id());

            //Attributes
            uint8_t* markings = &(pokelib_pkmn.pkm->pkm.markings);
            if(t_pkmn->has_attribute("friendship"))
                pokelib_pkmn.pkm->pkm.friendship = t_pkmn->get_attribute("friendship");
            if(t_pkmn->has_attribute("circle"))
                set_marking(markings, Markings::CIRCLE, t_pkmn->get_attribute("circle"));
            if(t_pkmn->has_attribute("triangle"))
                set_marking(markings, Markings::TRIANGLE, t_pkmn->get_attribute("triangle"));
            if(t_pkmn->has_attribute("square"))
                set_marking(markings, Markings::SQUARE, t_pkmn->get_attribute("square"));
            if(t_pkmn->has_attribute("heart"))
                set_marking(markings, Markings::HEART, t_pkmn->get_attribute("heart"));
            if(t_pkmn->has_attribute("star"))
                set_marking(markings, Markings::STAR, t_pkmn->get_attribute("star"));
            if(t_pkmn->has_attribute("diamond"))
                set_marking(markings, Markings::DIAMOND, t_pkmn->get_attribute("diamond"));

            if(t_pkmn->has_attribute("country"))
                pokelib_pkmn.pkm->pkm.country = t_pkmn->get_attribute("country");
            if(t_pkmn->has_attribute("cool"))
                pokelib_pkmn.pkm->pkm.contest_cool = t_pkmn->get_attribute("cool");
            if(t_pkmn->has_attribute("beauty"))
                pokelib_pkmn.pkm->pkm.contest_beauty = t_pkmn->get_attribute("beauty");
            if(t_pkmn->has_attribute("cute"))
                pokelib_pkmn.pkm->pkm.contest_cute = t_pkmn->get_attribute("cute");
            if(t_pkmn->has_attribute("smart"))
                pokelib_pkmn.pkm->pkm.contest_smart = t_pkmn->get_attribute("smart");
            if(t_pkmn->has_attribute("tough"))
                pokelib_pkmn.pkm->pkm.contest_tough = t_pkmn->get_attribute("tough");
            if(t_pkmn->has_attribute("sheen"))
                pokelib_pkmn.pkm->pkm.contest_sheen = t_pkmn->get_attribute("sheen");

            uint32_t* ribbonSinnohA = reinterpret_cast<uint32_t*>(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA));
            if(t_pkmn->has_attribute("sinnoh_champion_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::CHAMPION, t_pkmn->get_attribute("sinnoh_champion_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_ability_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::ABILITY, t_pkmn->get_attribute("sinnoh_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_great_ability_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::GREAT_ABILITY, t_pkmn->get_attribute("sinnoh_great_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_double_ability_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::DOUBLE_ABILITY, t_pkmn->get_attribute("sinnoh_double_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_multi_ability_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::MULTI_ABILITY, t_pkmn->get_attribute("sinnoh_multi_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_pair_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::PAIR_ABILITY, t_pkmn->get_attribute("sinnoh_pair_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_world_ability_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::WORLD_ABILITY, t_pkmn->get_attribute("sinnoh_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_alert_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::ALERT, t_pkmn->get_attribute("sinnoh_alert_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_shock_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::SHOCK, t_pkmn->get_attribute("sinnoh_shock_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_downcast_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::DOWNCAST, t_pkmn->get_attribute("sinnoh_downcast_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_careless_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::CARELESS, t_pkmn->get_attribute("sinnoh_careless_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_relax_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::RELAX, t_pkmn->get_attribute("sinnoh_relax_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_snooze_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::SNOOZE, t_pkmn->get_attribute("sinnoh_snooze_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smile_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::SMILE, t_pkmn->get_attribute("sinnoh_smile_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_gorgeous_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::GORGEOUS, t_pkmn->get_attribute("sinnoh_gorgeous_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_royal_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::ROYAL, t_pkmn->get_attribute("sinnoh_royal_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_gorgeous_royal_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::GORGEOUS_ROYAL, t_pkmn->get_attribute("sinnoh_gorgeous_royal_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_record_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::RECORD, t_pkmn->get_attribute("sinnoh_record_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_history_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::HISTORY, t_pkmn->get_attribute("sinnoh_history_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_legend_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::LEGEND, t_pkmn->get_attribute("sinnoh_legend_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_red_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::RED, t_pkmn->get_attribute("sinnoh_red_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_green_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::GREEN, t_pkmn->get_attribute("sinnoh_green_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_blue_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::BLUE, t_pkmn->get_attribute("sinnoh_blue_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_festival_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::FESTIVAL, t_pkmn->get_attribute("sinnoh_festival_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_carnival_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::CARNIVAL, t_pkmn->get_attribute("sinnoh_carnival_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_classic_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::CLASSIC, t_pkmn->get_attribute("sinnoh_classic_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_premier_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohA, Ribbons::Sinnoh::PREMIER, t_pkmn->get_attribute("sinnoh_premier_ribbon"));

            uint32_t* ribbonHoenn = reinterpret_cast<uint32_t*>(&(pokelib_pkmn.pkm->pkm.ribbonHoenn));
            if(t_pkmn->has_attribute("hoenn_cool_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL, t_pkmn->get_attribute("hoenn_cool_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_super_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL_SUPER, t_pkmn->get_attribute("hoenn_cool_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_hyper_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL_HYPER, t_pkmn->get_attribute("hoenn_cool_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_master_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::COOL_MASTER, t_pkmn->get_attribute("hoenn_cool_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY, t_pkmn->get_attribute("hoenn_beauty_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_super_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY_SUPER, t_pkmn->get_attribute("hoenn_beauty_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_hyper_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY_HYPER, t_pkmn->get_attribute("hoenn_beauty_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_master_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::BEAUTY_MASTER, t_pkmn->get_attribute("hoenn_beauty_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE, t_pkmn->get_attribute("hoenn_cute_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_super_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE_SUPER, t_pkmn->get_attribute("hoenn_cute_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_hyper_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE_HYPER, t_pkmn->get_attribute("hoenn_cute_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_master_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::CUTE_MASTER, t_pkmn->get_attribute("hoenn_cute_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART, t_pkmn->get_attribute("hoenn_smart_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_super_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART_SUPER, t_pkmn->get_attribute("hoenn_smart_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_hyper_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART_HYPER, t_pkmn->get_attribute("hoenn_smart_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_master_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::SMART_MASTER, t_pkmn->get_attribute("hoenn_smart_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH, t_pkmn->get_attribute("hoenn_tough_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_super_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH_SUPER, t_pkmn->get_attribute("hoenn_tough_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_hyper_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH_HYPER, t_pkmn->get_attribute("hoenn_tough_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_master_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::TOUGH_MASTER, t_pkmn->get_attribute("hoenn_tough_master_ribbon"));

            if(t_pkmn->has_attribute("hoenn_champion_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::CHAMPION, t_pkmn->get_attribute("hoenn_champion_ribbon"));
            if(t_pkmn->has_attribute("hoenn_winning_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::WINNING, t_pkmn->get_attribute("hoenn_winning_ribbon"));
            if(t_pkmn->has_attribute("hoenn_victory_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::VICTORY, t_pkmn->get_attribute("hoenn_victory_ribbon"));
            if(t_pkmn->has_attribute("hoenn_artist_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::ARTIST, t_pkmn->get_attribute("hoenn_artist_ribbon"));
            if(t_pkmn->has_attribute("hoenn_effort_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::EFFORT, t_pkmn->get_attribute("hoenn_effort_ribbon"));
            if(t_pkmn->has_attribute("hoenn_marine_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::MARINE, t_pkmn->get_attribute("hoenn_marine_ribbon"));
            if(t_pkmn->has_attribute("hoenn_land_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::LAND, t_pkmn->get_attribute("hoenn_land_ribbon"));
            if(t_pkmn->has_attribute("hoenn_sky_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::SKY, t_pkmn->get_attribute("hoenn_sky_ribbon"));
            if(t_pkmn->has_attribute("hoenn_country_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::COUNTRY, t_pkmn->get_attribute("hoenn_country_ribbon"));
            if(t_pkmn->has_attribute("hoenn_national_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::NATIONAL, t_pkmn->get_attribute("hoenn_national_ribbon"));
            if(t_pkmn->has_attribute("hoenn_earth_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::EARTH, t_pkmn->get_attribute("hoenn_earth_ribbon"));
            if(t_pkmn->has_attribute("hoenn_world_ribbon"))
                set_hoenn_ribbon(ribbonHoenn, Ribbons::Hoenn::WORLD, t_pkmn->get_attribute("hoenn_world_ribbon"));

            uint32_t* ribbonSinnohB = reinterpret_cast<uint32_t*>(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB));
            if(t_pkmn->has_attribute("sinnoh_cool_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::COOL-23, t_pkmn->get_attribute("sinnoh_cool_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cool_great_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::COOL_GREAT-23, t_pkmn->get_attribute("sinnoh_cool_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cool_ultra_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::COOL_ULTRA-23, t_pkmn->get_attribute("sinnoh_cool_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cool_master_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::COOL_MASTER-23, t_pkmn->get_attribute("sinnoh_cool_master_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_beauty_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY-23, t_pkmn->get_attribute("sinnoh_beauty_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_beauty_great_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY_GREAT-23, t_pkmn->get_attribute("sinnoh_beauty_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_beauty_ultra_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY_ULTRA-23, t_pkmn->get_attribute("sinnoh_beauty_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_beauty_master_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::BEAUTY_MASTER-23, t_pkmn->get_attribute("sinnoh_beauty_master_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cute_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::CUTE-23, t_pkmn->get_attribute("sinnoh_cute_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cute_great_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::CUTE_GREAT-23, t_pkmn->get_attribute("sinnoh_cute_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cute_ultra_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::CUTE_ULTRA-23, t_pkmn->get_attribute("sinnoh_cute_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cute_master_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::CUTE_MASTER-23, t_pkmn->get_attribute("sinnoh_cute_master_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smart_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::SMART-23, t_pkmn->get_attribute("sinnoh_smart_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smart_great_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::SMART_GREAT-23, t_pkmn->get_attribute("sinnoh_smart_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smart_ultra_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::SMART_ULTRA-23, t_pkmn->get_attribute("sinnoh_smart_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smart_master_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::SMART_MASTER-23, t_pkmn->get_attribute("sinnoh_smart_master_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_tough_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::TOUGH-23, t_pkmn->get_attribute("sinnoh_tough_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_tough_great_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::TOUGH_GREAT-23, t_pkmn->get_attribute("sinnoh_tough_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_tough_ultra_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::TOUGH_ULTRA-23, t_pkmn->get_attribute("sinnoh_tough_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_tough_master_ribbon"))
                set_sinnoh_ribbon32(ribbonSinnohB, Ribbons::Sinnoh::TOUGH_MASTER-23, t_pkmn->get_attribute("sinnoh_tough_master_ribbon"));

            if(t_pkmn->has_attribute("eggmet_year"))
                pokelib_pkmn.pkm->pkm.eggDate[0] = t_pkmn->get_attribute("eggmet_year");
            if(t_pkmn->has_attribute("eggmet_month"))
                pokelib_pkmn.pkm->pkm.eggDate[1] = t_pkmn->get_attribute("eggmet_month");
            if(t_pkmn->has_attribute("eggmet_day"))
                pokelib_pkmn.pkm->pkm.eggDate[2] = t_pkmn->get_attribute("eggmet_day");
            if(t_pkmn->has_attribute("met_year"))
                pokelib_pkmn.pkm->pkm.metDate[0] = t_pkmn->get_attribute("met_year");
            if(t_pkmn->has_attribute("met_month"))
                pokelib_pkmn.pkm->pkm.metDate[1] = t_pkmn->get_attribute("met_month");
            if(t_pkmn->has_attribute("met_day"))
                pokelib_pkmn.pkm->pkm.metDate[2] = t_pkmn->get_attribute("met_day");
            if(t_pkmn->has_attribute("ot_is_female"))
                set_gen4_5_otgender(metlevel_int, t_pkmn->get_attribute("ot_is_female"));

            return pokelib_pkmn;
        }

        team_pokemon::sptr pkmds_pokemon_to_team_pokemon(party_pkm* p_pkm)
        {
            ::opendb(get_database_path().c_str());

            unsigned int level, from_game;
            level = ::getpkmlevel(p_pkm->pkm_data);
            from_game = hometown_to_pkmnsim_game(p_pkm->pkm_data.hometown);

            team_pokemon::sptr t_pkmn = team_pokemon::make(p_pkm->pkm_data.species, from_game, level,
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

            uint8_t* metlevel_int = reinterpret_cast<uint8_t*>(&(p_pkm->pkm_data.ball)+1);
            t_pkmn->set_met_level(get_gen4_5_met_level(metlevel_int));
            t_pkmn->set_ball(game_ball_to_pkmnsim_ball(p_pkm->pkm_data.ball));
            if(get_gen4_5_otgender(metlevel_int)) t_pkmn->set_trainer_gender(Genders::FEMALE);
            else t_pkmn->set_trainer_gender(Genders::MALE);
            
            t_pkmn->set_held_item(item::make(database::get_item_id(p_pkm->pkm_data.item, from_game), from_game));
            t_pkmn->set_personality(p_pkm->pkm_data.pid);
            t_pkmn->set_public_trainer_id(p_pkm->pkm_data.tid);
            t_pkmn->set_secret_trainer_id(p_pkm->pkm_data.sid);

            t_pkmn->set_EV(Stats::HP, p_pkm->pkm_data.evs.hp);
            t_pkmn->set_EV(Stats::ATTACK, p_pkm->pkm_data.evs.attack);
            t_pkmn->set_EV(Stats::DEFENSE, p_pkm->pkm_data.evs.defense);
            t_pkmn->set_EV(Stats::SPECIAL_ATTACK, p_pkm->pkm_data.evs.spatk);
            t_pkmn->set_EV(Stats::SPECIAL_DEFENSE, p_pkm->pkm_data.evs.spdef);
            t_pkmn->set_EV(Stats::SPEED, p_pkm->pkm_data.evs.speed);

            uint32_t* ivs = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.ivs));
            t_pkmn->set_IV(Stats::HP, gen3_4_5_get_IV(ivs, Stats::HP));
            t_pkmn->set_IV(Stats::ATTACK, gen3_4_5_get_IV(ivs, Stats::ATTACK));
            t_pkmn->set_IV(Stats::DEFENSE, gen3_4_5_get_IV(ivs, Stats::DEFENSE));
            t_pkmn->set_IV(Stats::SPECIAL_ATTACK, gen3_4_5_get_IV(ivs, Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV(Stats::SPECIAL_DEFENSE, gen3_4_5_get_IV(ivs, Stats::SPECIAL_DEFENSE));
            t_pkmn->set_IV(Stats::SPEED, gen3_4_5_get_IV(ivs, Stats::SPEED));

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
            t_pkmn->set_attribute("hoenn_cool_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::COOL));
            t_pkmn->set_attribute("hoenn_cool_super_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::COOL_SUPER));
            t_pkmn->set_attribute("hoenn_cool_hyper_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::COOL_HYPER));
            t_pkmn->set_attribute("hoenn_cool_master_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::COOL_MASTER));
            t_pkmn->set_attribute("hoenn_beauty_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY));
            t_pkmn->set_attribute("hoenn_beauty_super_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_SUPER));
            t_pkmn->set_attribute("hoenn_beauty_hyper_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_HYPER));
            t_pkmn->set_attribute("hoenn_beauty_master_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_MASTER));
            t_pkmn->set_attribute("hoenn_cute_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::CUTE));
            t_pkmn->set_attribute("hoenn_cute_super_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::CUTE_SUPER));
            t_pkmn->set_attribute("hoenn_cute_hyper_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::CUTE_HYPER));
            t_pkmn->set_attribute("hoenn_cute_master_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::CUTE_MASTER));
            t_pkmn->set_attribute("hoenn_smart_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::SMART));
            t_pkmn->set_attribute("hoenn_smart_super_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::SMART_SUPER));
            t_pkmn->set_attribute("hoenn_smart_hyper_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::SMART_HYPER));
            t_pkmn->set_attribute("hoenn_smart_master_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::SMART_MASTER));
            t_pkmn->set_attribute("hoenn_tough_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::TOUGH));
            t_pkmn->set_attribute("hoenn_tough_super_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_SUPER));
            t_pkmn->set_attribute("hoenn_tough_hyper_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_HYPER));
            t_pkmn->set_attribute("hoenn_tough_master_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_MASTER));
            t_pkmn->set_attribute("hoenn_champion_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::CHAMPION));
            t_pkmn->set_attribute("hoenn_winning_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::WINNING));
            t_pkmn->set_attribute("hoenn_victory_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::VICTORY));
            t_pkmn->set_attribute("hoenn_artist_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::ARTIST));
            t_pkmn->set_attribute("hoenn_effort_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::EFFORT));
            t_pkmn->set_attribute("hoenn_marine_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::MARINE));
            t_pkmn->set_attribute("hoenn_land_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::LAND));
            t_pkmn->set_attribute("hoenn_sky_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::SKY));
            t_pkmn->set_attribute("hoenn_country_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::COUNTRY));
            t_pkmn->set_attribute("hoenn_national_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::NATIONAL));
            t_pkmn->set_attribute("hoenn_earth_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::EARTH));
            t_pkmn->set_attribute("hoenn_world_ribbon", get_hoenn_ribbon(hribbon1, Ribbons::Hoenn::WORLD));

            uint32_t* sribbon1 = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.sribbon1));
            t_pkmn->set_attribute("sinnoh_champion_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::CHAMPION));
            t_pkmn->set_attribute("sinnoh_ability_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::ABILITY));
            t_pkmn->set_attribute("sinnoh_great_ability_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::GREAT_ABILITY));
            t_pkmn->set_attribute("sinnoh_double_ability_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::DOUBLE_ABILITY));
            t_pkmn->set_attribute("sinnoh_multi_ability_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::MULTI_ABILITY));
            t_pkmn->set_attribute("sinnoh_pair_ability_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::PAIR_ABILITY));
            t_pkmn->set_attribute("sinnoh_world_ability_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::WORLD_ABILITY));
            t_pkmn->set_attribute("sinnoh_alert_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::ALERT));
            t_pkmn->set_attribute("sinnoh_shock_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::SHOCK));
            t_pkmn->set_attribute("sinnoh_downcast_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::DOWNCAST));
            t_pkmn->set_attribute("sinnoh_careless_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::CARELESS));
            t_pkmn->set_attribute("sinnoh_relax_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::RELAX));
            t_pkmn->set_attribute("sinnoh_snooze_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::SNOOZE));
            t_pkmn->set_attribute("sinnoh_smile_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::SMILE));
            t_pkmn->set_attribute("sinnoh_gorgeous_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::GORGEOUS));
            t_pkmn->set_attribute("sinnoh_royal_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::ROYAL));
            t_pkmn->set_attribute("sinnoh_gorgeous_royal_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::GORGEOUS_ROYAL));
            t_pkmn->set_attribute("sinnoh_footprint_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::FOOTPRINT));
            t_pkmn->set_attribute("sinnoh_record_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::RECORD));
            t_pkmn->set_attribute("sinnoh_history_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::HISTORY));
            t_pkmn->set_attribute("sinnoh_legend_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::LEGEND));
            t_pkmn->set_attribute("sinnoh_red_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::RED));
            t_pkmn->set_attribute("sinnoh_green_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::GREEN));
            t_pkmn->set_attribute("sinnoh_blue_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::BLUE));
            t_pkmn->set_attribute("sinnoh_festival_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_carnival_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_classic_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::CLASSIC));
            t_pkmn->set_attribute("sinnoh_premier_ribbon", get_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::PREMIER));

            uint32_t* sribbon3 = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.sribbon3));
            t_pkmn->set_attribute("sinnoh_cool_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::COOL-23));
            t_pkmn->set_attribute("sinnoh_cool_great_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::COOL_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cool_ultra_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::COOL_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cool_master_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::COOL_MASTER-23));
            t_pkmn->set_attribute("sinnoh_beauty_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::BEAUTY-23));
            t_pkmn->set_attribute("sinnoh_beauty_great_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::BEAUTY_GREAT-23));
            t_pkmn->set_attribute("sinnoh_beauty_ultra_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::BEAUTY_GREAT-23));
            t_pkmn->set_attribute("sinnoh_beauty_master_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::BEAUTY_MASTER-23));
            t_pkmn->set_attribute("sinnoh_cute_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::CUTE-23));
            t_pkmn->set_attribute("sinnoh_cute_great_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::CUTE_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cute_ultra_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::CUTE_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cute_master_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::CUTE_MASTER-23));
            t_pkmn->set_attribute("sinnoh_smart_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::SMART-23));
            t_pkmn->set_attribute("sinnoh_smart_great_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::SMART_GREAT-23));
            t_pkmn->set_attribute("sinnoh_smart_ultra_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::SMART_GREAT-23));
            t_pkmn->set_attribute("sinnoh_smart_master_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::SMART_MASTER-23));
            t_pkmn->set_attribute("sinnoh_tough_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::TOUGH-23));
            t_pkmn->set_attribute("sinnoh_tough_great_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::TOUGH_GREAT-23));
            t_pkmn->set_attribute("sinnoh_tough_ultra_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::TOUGH_GREAT-23));
            t_pkmn->set_attribute("sinnoh_tough_master_ribbon", get_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::TOUGH_MASTER-23));

            t_pkmn->set_attribute("eggmet_year", p_pkm->pkm_data.metdate.year);
            t_pkmn->set_attribute("eggmet_month", p_pkm->pkm_data.metdate.month);
            t_pkmn->set_attribute("eggmet_day", p_pkm->pkm_data.metdate.day);
            t_pkmn->set_attribute("met_year", p_pkm->pkm_data.metdate.year);
            t_pkmn->set_attribute("met_month", p_pkm->pkm_data.metdate.month);
            t_pkmn->set_attribute("met_day", p_pkm->pkm_data.metdate.day);

            closedb();

            return t_pkmn;
        }

        void team_pokemon_to_pkmds_pokemon(team_pokemon::sptr t_pkmn, party_pkm* p_pkm)
        {
            p_pkm->pkm_data.species = ::Species::pkmspecies(t_pkmn->get_species_id());
            
            moveset_t moves = t_pkmn->get_moves();
            p_pkm->pkm_data.moves[0] = ::Moves::moves(t_pkmn->get_moves()[0]->get_move_id());
            p_pkm->pkm_data.moves[1] = ::Moves::moves(t_pkmn->get_moves()[1]->get_move_id());
            p_pkm->pkm_data.moves[2] = ::Moves::moves(t_pkmn->get_moves()[2]->get_move_id());
            p_pkm->pkm_data.moves[3] = ::Moves::moves(t_pkmn->get_moves()[3]->get_move_id());

            ::setlevel(p_pkm->pkm_data, t_pkmn->get_level());

            wstring nickname_wide = t_pkmn->get_nickname();
            wstring trainer_name_wide = t_pkmn->get_trainer_name();
            #ifdef PKMNSIM_PLATFORM_LINUX
                ::setpkmnickname(p_pkm->pkm_data, (wchar_t*)(nickname_wide.c_str()), nickname_wide.size());
                ::setpkmotname(p_pkm->pkm_data, (wchar_t*)(trainer_name_wide.c_str()), trainer_name_wide.size());
            #else
                //TODO: clean up when using Windows
                ::setpkmnickname(p_pkm->pkm_data, (wchar_t*)(t_pkmn->get_nickname().const_wchar_t()), t_pkmn->get_nickname().std_wstring().size());
                ::setpkmotname(p_pkm->pkm_data, (wchar_t*)(t_pkmn->get_trainer_name().const_wchar_t()), t_pkmn->get_trainer_name().std_wstring().size());
            #endif

            unsigned int raw_held = t_pkmn->get_held_item()->get_item_id();
            p_pkm->pkm_data.item = ::Items::items(database::get_item_index(raw_held, t_pkmn->get_game_id()));

            uint8_t* metlevel_int = reinterpret_cast<uint8_t*>(&(p_pkm->pkm_data.ball)+1);
            set_gen4_5_met_level(metlevel_int, t_pkmn->get_met_level());
            p_pkm->pkm_data.ball = Balls::balls(pkmnsim_ball_to_game_ball(t_pkmn->get_ball()));
            set_gen4_5_otgender(metlevel_int, (t_pkmn->get_trainer_gender() == Genders::FEMALE));
            p_pkm->pkm_data.pid = t_pkmn->get_personality();
            p_pkm->pkm_data.tid = t_pkmn->get_public_trainer_id();
            p_pkm->pkm_data.sid = t_pkmn->get_secret_trainer_id();

            dict<unsigned int, unsigned int> stats = t_pkmn->get_stats();
            p_pkm->party_data.maxhp = stats[Stats::HP];
            p_pkm->party_data.attack = stats[Stats::ATTACK];
            p_pkm->party_data.defense = stats[Stats::DEFENSE];
            p_pkm->party_data.spatk = stats[Stats::SPECIAL_ATTACK];
            p_pkm->party_data.spdef = stats[Stats::SPECIAL_DEFENSE];
            p_pkm->party_data.speed = stats[Stats::SPEED];

            dict<unsigned int, unsigned int> IVs = t_pkmn->get_IVs();
            uint32_t* IVint = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.ppup[3])+1);
            gen3_4_5_set_IV(IVint, Stats::HP, IVs[Stats::HP]);
            gen3_4_5_set_IV(IVint, Stats::ATTACK, IVs[Stats::ATTACK]);
            gen3_4_5_set_IV(IVint, Stats::DEFENSE, IVs[Stats::DEFENSE]);
            gen3_4_5_set_IV(IVint, Stats::SPECIAL_ATTACK, IVs[Stats::SPECIAL_ATTACK]);
            gen3_4_5_set_IV(IVint, Stats::SPECIAL_DEFENSE, IVs[Stats::SPECIAL_DEFENSE]);
            gen3_4_5_set_IV(IVint, Stats::SPEED, IVs[Stats::SPEED]);

            dict<unsigned int, unsigned int> EVs = t_pkmn->get_EVs();
            p_pkm->pkm_data.evs.hp = EVs[Stats::HP];
            p_pkm->pkm_data.evs.attack = EVs[Stats::ATTACK];
            p_pkm->pkm_data.evs.defense = EVs[Stats::DEFENSE];
            p_pkm->pkm_data.evs.spatk = EVs[Stats::SPECIAL_ATTACK];
            p_pkm->pkm_data.evs.spdef = EVs[Stats::SPECIAL_DEFENSE];
            p_pkm->pkm_data.evs.speed = EVs[Stats::SPEED];

            p_pkm->pkm_data.hometown = ::Hometowns::hometowns(pkmnsim_game_to_hometown(t_pkmn->get_game_id()));

            //Attributes
            uint8_t* markings = reinterpret_cast<uint8_t*>(&(p_pkm->pkm_data.ability)+1);
            if(t_pkmn->has_attribute("friendship"))
                p_pkm->pkm_data.tameness = t_pkmn->get_attribute("friendship");
            if(t_pkmn->has_attribute("circle"))
                set_marking(markings, Markings::CIRCLE, t_pkmn->get_attribute("circle"));
            if(t_pkmn->has_attribute("triangle"))
                set_marking(markings, Markings::TRIANGLE, t_pkmn->get_attribute("triangle"));
            if(t_pkmn->has_attribute("square"))
                set_marking(markings, Markings::SQUARE, t_pkmn->get_attribute("square"));
            if(t_pkmn->has_attribute("heart"))
                set_marking(markings, Markings::HEART, t_pkmn->get_attribute("heart"));
            if(t_pkmn->has_attribute("star"))
                set_marking(markings, Markings::STAR, t_pkmn->get_attribute("star"));
            if(t_pkmn->has_attribute("diamond"))
                set_marking(markings, Markings::DIAMOND, t_pkmn->get_attribute("diamond"));

            if(t_pkmn->has_attribute("country"))
                p_pkm->pkm_data.country = Countries::countries(t_pkmn->get_attribute("country"));
            if(t_pkmn->has_attribute("cool"))
                p_pkm->pkm_data.contest.cool = t_pkmn->get_attribute("cool");
            if(t_pkmn->has_attribute("beauty"))
                p_pkm->pkm_data.contest.beauty = t_pkmn->get_attribute("beauty");
            if(t_pkmn->has_attribute("cute"))
                p_pkm->pkm_data.contest.cute = t_pkmn->get_attribute("cute");
            if(t_pkmn->has_attribute("smart"))
                p_pkm->pkm_data.contest.smart = t_pkmn->get_attribute("smart");
            if(t_pkmn->has_attribute("tough"))
                p_pkm->pkm_data.contest.tough = t_pkmn->get_attribute("tough");
            if(t_pkmn->has_attribute("sheen"))
                p_pkm->pkm_data.contest.sheen = t_pkmn->get_attribute("sheen");

            uint32_t* hribbon1 = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.hribbon1));
            if(t_pkmn->has_attribute("hoenn_cool_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::COOL, t_pkmn->get_attribute("hoenn_cool_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_super_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::COOL_SUPER, t_pkmn->get_attribute("hoenn_cool_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_hyper_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::COOL_HYPER, t_pkmn->get_attribute("hoenn_cool_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_master_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::COOL_MASTER, t_pkmn->get_attribute("hoenn_cool_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY, t_pkmn->get_attribute("hoenn_beauty_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_super_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_SUPER, t_pkmn->get_attribute("hoenn_beauty_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_hyper_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_HYPER, t_pkmn->get_attribute("hoenn_beauty_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_master_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_MASTER, t_pkmn->get_attribute("hoenn_beauty_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::CUTE, t_pkmn->get_attribute("hoenn_cute_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_super_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::CUTE_SUPER, t_pkmn->get_attribute("hoenn_cute_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_hyper_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::CUTE_HYPER, t_pkmn->get_attribute("hoenn_cute_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_master_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::CUTE_MASTER, t_pkmn->get_attribute("hoenn_cute_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::SMART, t_pkmn->get_attribute("hoenn_smart_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_super_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::SMART_SUPER, t_pkmn->get_attribute("hoenn_smart_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_hyper_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::SMART_HYPER, t_pkmn->get_attribute("hoenn_smart_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_master_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::SMART_MASTER, t_pkmn->get_attribute("hoenn_smart_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::TOUGH, t_pkmn->get_attribute("hoenn_tough_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_super_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_SUPER, t_pkmn->get_attribute("hoenn_tough_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_hyper_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_HYPER, t_pkmn->get_attribute("hoenn_tough_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_master_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_MASTER, t_pkmn->get_attribute("hoenn_tough_master_ribbon"));

            if(t_pkmn->has_attribute("hoenn_champion_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::CHAMPION, t_pkmn->get_attribute("hoenn_champion_ribbon"));
            if(t_pkmn->has_attribute("hoenn_winning_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::WINNING, t_pkmn->get_attribute("hoenn_winning_ribbon"));
            if(t_pkmn->has_attribute("hoenn_victory_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::VICTORY, t_pkmn->get_attribute("hoenn_victory_ribbon"));
            if(t_pkmn->has_attribute("hoenn_artist_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::ARTIST, t_pkmn->get_attribute("hoenn_artist_ribbon"));
            if(t_pkmn->has_attribute("hoenn_effort_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::EFFORT, t_pkmn->get_attribute("hoenn_effort_ribbon"));
            if(t_pkmn->has_attribute("hoenn_marine_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::MARINE, t_pkmn->get_attribute("hoenn_marine_ribbon"));
            if(t_pkmn->has_attribute("hoenn_land_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::LAND, t_pkmn->get_attribute("hoenn_land_ribbon"));
            if(t_pkmn->has_attribute("hoenn_sky_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::SKY, t_pkmn->get_attribute("hoenn_sky_ribbon"));
            if(t_pkmn->has_attribute("hoenn_country_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::COUNTRY, t_pkmn->get_attribute("hoenn_country_ribbon"));
            if(t_pkmn->has_attribute("hoenn_national_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::NATIONAL, t_pkmn->get_attribute("hoenn_national_ribbon"));
            if(t_pkmn->has_attribute("hoenn_earth_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::EARTH, t_pkmn->get_attribute("hoenn_earth_ribbon"));
            if(t_pkmn->has_attribute("hoenn_world_ribbon"))
                set_hoenn_ribbon(hribbon1, Ribbons::Hoenn::WORLD, t_pkmn->get_attribute("hoenn_world_ribbon"));

            uint32_t* sribbon1 = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.sribbon1));
            if(t_pkmn->has_attribute("sinnoh_champion_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::CHAMPION, t_pkmn->get_attribute("sinnoh_champion_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_ability_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::ABILITY, t_pkmn->get_attribute("sinnoh_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_great_ability_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::GREAT_ABILITY, t_pkmn->get_attribute("sinnoh_great_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_double_ability_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::DOUBLE_ABILITY, t_pkmn->get_attribute("sinnoh_double_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_multi_ability_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::MULTI_ABILITY, t_pkmn->get_attribute("sinnoh_multi_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_pair_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::PAIR_ABILITY, t_pkmn->get_attribute("sinnoh_pair_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_world_ability_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::WORLD_ABILITY, t_pkmn->get_attribute("sinnoh_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_alert_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::ALERT, t_pkmn->get_attribute("sinnoh_alert_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_shock_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::SHOCK, t_pkmn->get_attribute("sinnoh_shock_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_downcast_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::DOWNCAST, t_pkmn->get_attribute("sinnoh_downcast_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_careless_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::CARELESS, t_pkmn->get_attribute("sinnoh_careless_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_relax_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::RELAX, t_pkmn->get_attribute("sinnoh_relax_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_snooze_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::SNOOZE, t_pkmn->get_attribute("sinnoh_snooze_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smile_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::SMILE, t_pkmn->get_attribute("sinnoh_smile_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_gorgeous_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::GORGEOUS, t_pkmn->get_attribute("sinnoh_gorgeous_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_royal_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::ROYAL, t_pkmn->get_attribute("sinnoh_royal_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_gorgeous_royal_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::GORGEOUS_ROYAL, t_pkmn->get_attribute("sinnoh_gorgeous_royal_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_record_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::RECORD, t_pkmn->get_attribute("sinnoh_record_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_history_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::HISTORY, t_pkmn->get_attribute("sinnoh_history_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_legend_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::LEGEND, t_pkmn->get_attribute("sinnoh_legend_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_red_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::RED, t_pkmn->get_attribute("sinnoh_red_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_green_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::GREEN, t_pkmn->get_attribute("sinnoh_green_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_blue_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::BLUE, t_pkmn->get_attribute("sinnoh_blue_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_festival_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::FESTIVAL, t_pkmn->get_attribute("sinnoh_festival_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_carnival_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::CARNIVAL, t_pkmn->get_attribute("sinnoh_carnival_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_classic_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::CLASSIC, t_pkmn->get_attribute("sinnoh_classic_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_premier_ribbon"))
                set_sinnoh_ribbon32(sribbon1, Ribbons::Sinnoh::PREMIER, t_pkmn->get_attribute("sinnoh_premier_ribbon"));

            uint32_t* sribbon3 = reinterpret_cast<uint32_t*>(&(p_pkm->pkm_data.sribbon3));
            if(t_pkmn->has_attribute("sinnoh_cool_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::COOL-23, t_pkmn->get_attribute("sinnoh_cool_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cool_great_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::COOL_GREAT-23, t_pkmn->get_attribute("sinnoh_cool_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cool_ultra_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::COOL_ULTRA-23, t_pkmn->get_attribute("sinnoh_cool_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cool_master_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::COOL_MASTER-23, t_pkmn->get_attribute("sinnoh_cool_master_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_beauty_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::BEAUTY-23, t_pkmn->get_attribute("sinnoh_beauty_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_beauty_great_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::BEAUTY_GREAT-23, t_pkmn->get_attribute("sinnoh_beauty_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_beauty_ultra_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::BEAUTY_ULTRA-23, t_pkmn->get_attribute("sinnoh_beauty_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_beauty_master_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::BEAUTY_MASTER-23, t_pkmn->get_attribute("sinnoh_beauty_master_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cute_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::CUTE-23, t_pkmn->get_attribute("sinnoh_cute_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cute_great_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::CUTE_GREAT-23, t_pkmn->get_attribute("sinnoh_cute_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cute_ultra_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::CUTE_ULTRA-23, t_pkmn->get_attribute("sinnoh_cute_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cute_master_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::CUTE_MASTER-23, t_pkmn->get_attribute("sinnoh_cute_master_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smart_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::SMART-23, t_pkmn->get_attribute("sinnoh_smart_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smart_great_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::SMART_GREAT-23, t_pkmn->get_attribute("sinnoh_smart_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smart_ultra_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::SMART_ULTRA-23, t_pkmn->get_attribute("sinnoh_smart_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smart_master_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::SMART_MASTER-23, t_pkmn->get_attribute("sinnoh_smart_master_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_tough_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::TOUGH-23, t_pkmn->get_attribute("sinnoh_tough_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_tough_great_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::TOUGH_GREAT-23, t_pkmn->get_attribute("sinnoh_tough_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_tough_ultra_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::TOUGH_ULTRA-23, t_pkmn->get_attribute("sinnoh_tough_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_tough_master_ribbon"))
                set_sinnoh_ribbon32(sribbon3, Ribbons::Sinnoh::TOUGH_MASTER-23, t_pkmn->get_attribute("sinnoh_tough_master_ribbon"));

            if(t_pkmn->has_attribute("eggmet_year"))
                p_pkm->pkm_data.eggdate.year = t_pkmn->get_attribute("eggmet_year");
            if(t_pkmn->has_attribute("eggmet_month"))
                p_pkm->pkm_data.eggdate.month = t_pkmn->get_attribute("eggmet_month");
            if(t_pkmn->has_attribute("eggmet_day"))
                p_pkm->pkm_data.eggdate.day = t_pkmn->get_attribute("eggmet_day");
            if(t_pkmn->has_attribute("met_year"))
                p_pkm->pkm_data.metdate.year = t_pkmn->get_attribute("met_year");
            if(t_pkmn->has_attribute("met_month"))
                p_pkm->pkm_data.metdate.month = t_pkmn->get_attribute("met_month");
            if(t_pkmn->has_attribute("met_day"))
                p_pkm->pkm_data.metdate.day = t_pkmn->get_attribute("met_day");
        }

        PokeLib::Pokemon pokehack_pokemon_to_pokelib_pokemon(belt_pokemon_t* b_pkmn_t,
                                                             pokemon_attacks_t* pkmn_a_t,
                                                             pokemon_effort_t* pkmn_e_t,
                                                             pokemon_misc_t* pkmn_m_t,
                                                             pokemon_growth_t* pkmn_g_t)
        {
            PokeLib::Pokemon pokelib_pkmn = team_pokemon_to_pokelib_pokemon(pokehack_pokemon_to_team_pokemon(b_pkmn_t,
                                                                            pkmn_a_t, pkmn_e_t, pkmn_m_t, pkmn_g_t));

            //Manually set egg met location to Faraway Place and met location to Pal Park
            pokelib_pkmn.pkm->pkm.eggLoc_DP = char(3002);
            pokelib_pkmn.pkm->pkm.eggLoc_Plat = char(3002);
            pokelib_pkmn.pkm->pkm.metLoc_DP = char(55);
            pokelib_pkmn.pkm->pkm.metLoc_Plat = char(55);

            //Manually set encounter type to Pal Park
            pokelib_pkmn.pkm->pkm.encounterType = 0x0;

            return pokelib_pkmn;
        }

        void pokehack_pokemon_to_pkmds_pokemon(belt_pokemon_t* b_pkmn_t,
                                               pokemon_attacks_t* pkmn_a_t,
                                               pokemon_effort_t* pkmn_e_t,
                                               pokemon_misc_t* pkmn_m_t,
                                               pokemon_growth_t* pkmn_g_t,
                                               party_pkm* p_pkm)
        {
            team_pokemon_to_pkmds_pokemon(pokehack_pokemon_to_team_pokemon(b_pkmn_t, pkmn_a_t, pkmn_e_t, pkmn_m_t, pkmn_g_t), p_pkm);

            //Manually set egg met and met locations to Faraway Place
            p_pkm->pkm_data.eggmet = Locations::unovafarawayplace;
            p_pkm->pkm_data.met = Locations::unovafarawayplace;
        }

        void pokelib_pokemon_to_pokehack_pokemon(PokeLib::Pokemon pokelib_pkmn,
                                                 belt_pokemon_t* b_pkmn_t,
                                                 pokemon_attacks_t* pkmn_a_t,
                                                 pokemon_effort_t* pkmn_e_t,
                                                 pokemon_misc_t* pkmn_m_t,
                                                 pokemon_growth_t* pkmn_g_t)
        {
            team_pokemon_to_pokehack_pokemon(pokelib_pokemon_to_team_pokemon(pokelib_pkmn), b_pkmn_t, pkmn_a_t, pkmn_e_t, pkmn_m_t, pkmn_g_t);

            //Manually set met location to In-Game Trade
            pkmn_m_t->locationcaught = 255;

            //If Pokemon was caught in a Gen 4 ball, set it to standard Poke Ball
            if(pkmn_m_t->pokeball > 0xC) pkmn_m_t->pokeball = 0x4;
        }

        void pokelib_pokemon_to_pkmds_pokemon(PokeLib::Pokemon pokelib_pkmn, party_pkm* p_pkm)
        {
            team_pokemon_to_pkmds_pokemon(pokelib_pokemon_to_team_pokemon(pokelib_pkmn), p_pkm);

            //Manually set egg met location to Faraway place and met location to Poke Transfer
            p_pkm->pkm_data.eggmet = Locations::unovafarawayplace;
            p_pkm->pkm_data.met = Locations::poketransfer;
        }

        void pkmds_pokemon_to_pokehack_pokemon(party_pkm* p_pkm,
                                               belt_pokemon_t* b_pkmn_t,
                                               pokemon_attacks_t* pkmn_a_t,
                                               pokemon_effort_t* pkmn_e_t,
                                               pokemon_misc_t* pkmn_m_t,
                                               pokemon_growth_t* pkmn_g_t)
        {
            team_pokemon_to_pokehack_pokemon(pkmds_pokemon_to_team_pokemon(p_pkm), b_pkmn_t, pkmn_a_t, pkmn_e_t, pkmn_m_t, pkmn_g_t);
        }

        PokeLib::Pokemon pkmds_pokemon_to_pokelib_pokemon(party_pkm* p_pkm)
        {
            PokeLib::Pokemon pokelib_pkmn = team_pokemon_to_pokelib_pokemon(pkmds_pokemon_to_team_pokemon(p_pkm));

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
    }
}
