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
#include <pkmnsim/move.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/team_pokemon.hpp>
#include <pkmnsim/conversions/pokemon.hpp>
#include <pkmnsim/database/queries.hpp>
#include <pkmnsim/types/pokemon_text.hpp>

#include <pkmds/pkmds_g5_sqlite.h>

#include <sqlitecpp/SQLiteCPP.h>

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
            t_pkmn->set_held_item(pkmn_g_t->held);
            t_pkmn->set_personality(b_pkmn_t->personality);
            t_pkmn->set_trainer_id(b_pkmn_t->otid);
            
            t_pkmn->set_EV(Stats::HP, pkmn_e_t->hp);
            t_pkmn->set_EV(Stats::ATTACK, pkmn_e_t->attack);
            t_pkmn->set_EV(Stats::DEFENSE, pkmn_e_t->defense);
            t_pkmn->set_EV(Stats::SPECIAL_ATTACK, pkmn_e_t->spatk);
            t_pkmn->set_EV(Stats::SPECIAL_DEFENSE, pkmn_e_t->spdef);
            t_pkmn->set_EV(Stats::SPEED, pkmn_e_t->speed);
            
            t_pkmn->set_IV(Stats::HP, gen3_4_5_get_IV(&(pkmn_m_t->IVint), Stats::HP));
            t_pkmn->set_IV(Stats::ATTACK, gen3_4_5_get_IV(&(pkmn_m_t->IVint), Stats::ATTACK));
            t_pkmn->set_IV(Stats::DEFENSE, gen3_4_5_get_IV(&(pkmn_m_t->IVint), Stats::DEFENSE));
            t_pkmn->set_IV(Stats::SPECIAL_ATTACK, gen3_4_5_get_IV(&(pkmn_m_t->IVint), Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV(Stats::SPECIAL_DEFENSE, gen3_4_5_get_IV(&(pkmn_m_t->IVint), Stats::SPECIAL_DEFENSE));
            t_pkmn->set_IV(Stats::SPEED, gen3_4_5_get_IV(&(pkmn_m_t->IVint), Stats::SPEED));
           
            t_pkmn->set_met_level(get_gen3_met_level(((uint16_t*)&(pkmn_m_t->locationcaught)+1)));
            t_pkmn->set_ball(game_ball_to_pkmnsim_ball(get_gen3_ball(((uint16_t*)&(pkmn_m_t->locationcaught)+1))));
            if(get_gen3_otgender(((uint16_t*)&(pkmn_m_t->locationcaught)+1))) t_pkmn->set_trainer_gender(Genders::FEMALE);
            else t_pkmn->set_trainer_gender(Genders::MALE);
 
            t_pkmn->set_attribute("friendship", pkmn_g_t->happiness);
            t_pkmn->set_attribute("circle", get_marking(&(b_pkmn_t->markint), Markings::CIRCLE));
            t_pkmn->set_attribute("triangle", get_marking(&(b_pkmn_t->markint), Markings::TRIANGLE));
            t_pkmn->set_attribute("square", get_marking(&(b_pkmn_t->markint), Markings::SQUARE));
            t_pkmn->set_attribute("heart", get_marking(&(b_pkmn_t->markint), Markings::HEART));
            t_pkmn->set_attribute("country", b_pkmn_t->language);
            
            t_pkmn->set_attribute("cool", pkmn_e_t->coolness);
            t_pkmn->set_attribute("beauty", pkmn_e_t->beauty);
            t_pkmn->set_attribute("cute", pkmn_e_t->cuteness);
            t_pkmn->set_attribute("smart", pkmn_e_t->smartness);
            t_pkmn->set_attribute("tough", pkmn_e_t->toughness);
            
            t_pkmn->set_attribute("hoenn_cool_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::COOL));
            t_pkmn->set_attribute("hoenn_cool_super_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::COOL_SUPER));
            t_pkmn->set_attribute("hoenn_cool_hyper_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::COOL_HYPER));
            t_pkmn->set_attribute("hoenn_cool_master_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::COOL_MASTER));
            t_pkmn->set_attribute("hoenn_beauty_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::BEAUTY));
            t_pkmn->set_attribute("hoenn_beauty_super_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::BEAUTY_SUPER));
            t_pkmn->set_attribute("hoenn_beauty_hyper_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::BEAUTY_HYPER));
            t_pkmn->set_attribute("hoenn_beauty_master_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::BEAUTY_MASTER));
            t_pkmn->set_attribute("hoenn_cute_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::CUTE));
            t_pkmn->set_attribute("hoenn_cute_super_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::CUTE_SUPER));
            t_pkmn->set_attribute("hoenn_cute_hyper_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::CUTE_HYPER));
            t_pkmn->set_attribute("hoenn_cute_master_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::CUTE_MASTER));
            t_pkmn->set_attribute("hoenn_smart_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::SMART));
            t_pkmn->set_attribute("hoenn_smart_super_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::SMART_SUPER));
            t_pkmn->set_attribute("hoenn_smart_hyper_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::SMART_HYPER));
            t_pkmn->set_attribute("hoenn_smart_master_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::SMART_MASTER));
            t_pkmn->set_attribute("hoenn_tough_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::TOUGH));
            t_pkmn->set_attribute("hoenn_tough_super_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::TOUGH_SUPER));
            t_pkmn->set_attribute("hoenn_tough_hyper_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::TOUGH_HYPER));
            t_pkmn->set_attribute("hoenn_tough_master_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::TOUGH_MASTER));
            
            t_pkmn->set_attribute("hoenn_champion_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::CHAMPION));
            t_pkmn->set_attribute("hoenn_winning_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::WINNING));
            t_pkmn->set_attribute("hoenn_victory_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::VICTORY));
            t_pkmn->set_attribute("hoenn_artist_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::ARTIST));
            t_pkmn->set_attribute("hoenn_effort_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::EFFORT));
            t_pkmn->set_attribute("hoenn_marine_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::MARINE));
            t_pkmn->set_attribute("hoenn_land_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::LAND));
            t_pkmn->set_attribute("hoenn_sky_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::SKY));
            t_pkmn->set_attribute("hoenn_country_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::COUNTRY));
            t_pkmn->set_attribute("hoenn_national_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::NATIONAL));
            t_pkmn->set_attribute("hoenn_earth_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::EARTH));
            t_pkmn->set_attribute("hoenn_world_ribbon", get_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::WORLD));
            
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

            pkmn_g_t->held = t_pkmn->get_held_item();

            set_gen3_met_level(((uint16_t*)(&pkmn_m_t->locationcaught)+1), t_pkmn->get_met_level());
            set_gen3_ball(((uint16_t*)(&pkmn_m_t->locationcaught)+1), pkmnsim_ball_to_game_ball(t_pkmn->get_ball()));
            set_gen3_otgender(((uint16_t*)(&pkmn_m_t->locationcaught)+1), (t_pkmn->get_trainer_gender() == Genders::FEMALE));

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

            dict<unsigned int, unsigned int> IVs = t_pkmn->get_IVs();
            gen3_4_5_set_IV(&(pkmn_m_t->IVint), Stats::HP, IVs[Stats::HP]);
            gen3_4_5_set_IV(&(pkmn_m_t->IVint), Stats::ATTACK, IVs[Stats::ATTACK]);
            gen3_4_5_set_IV(&(pkmn_m_t->IVint), Stats::DEFENSE, IVs[Stats::DEFENSE]);
            gen3_4_5_set_IV(&(pkmn_m_t->IVint), Stats::SPECIAL_ATTACK, IVs[Stats::SPECIAL_ATTACK]);
            gen3_4_5_set_IV(&(pkmn_m_t->IVint), Stats::SPECIAL_DEFENSE, IVs[Stats::SPECIAL_DEFENSE]);
            gen3_4_5_set_IV(&(pkmn_m_t->IVint), Stats::SPEED, IVs[Stats::SPEED]);

            pkmn_m_t->game = pkmnsim_game_to_hometown(t_pkmn->get_game_id());
            
            //Attributes
            if(t_pkmn->has_attribute("friendship"))
                pkmn_g_t->happiness = t_pkmn->get_attribute("friendship");
            if(t_pkmn->has_attribute("circle"))
                set_marking(&(b_pkmn_t->markint), Markings::CIRCLE, t_pkmn->get_attribute("circle"));
            if(t_pkmn->has_attribute("triangle"))
                set_marking(&(b_pkmn_t->markint), Markings::TRIANGLE, t_pkmn->get_attribute("triangle"));
            if(t_pkmn->has_attribute("square"))
                set_marking(&(b_pkmn_t->markint), Markings::SQUARE, t_pkmn->get_attribute("square"));
            if(t_pkmn->has_attribute("heart"))
                set_marking(&(b_pkmn_t->markint), Markings::HEART, t_pkmn->get_attribute("heart"));
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

            if(t_pkmn->has_attribute("hoenn_cool_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::COOL, t_pkmn->get_attribute("hoenn_cool_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_super_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::COOL_SUPER, t_pkmn->get_attribute("hoenn_cool_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_hyper_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::COOL_HYPER, t_pkmn->get_attribute("hoenn_cool_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_master_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::COOL_MASTER, t_pkmn->get_attribute("hoenn_cool_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::BEAUTY, t_pkmn->get_attribute("hoenn_beauty_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_super_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::BEAUTY_SUPER, t_pkmn->get_attribute("hoenn_beauty_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_hyper_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::BEAUTY_HYPER, t_pkmn->get_attribute("hoenn_beauty_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_master_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::BEAUTY_MASTER, t_pkmn->get_attribute("hoenn_beauty_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::CUTE, t_pkmn->get_attribute("hoenn_cute_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_super_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::CUTE_SUPER, t_pkmn->get_attribute("hoenn_cute_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_hyper_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::CUTE_HYPER, t_pkmn->get_attribute("hoenn_cute_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_master_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::CUTE_MASTER, t_pkmn->get_attribute("hoenn_cute_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::SMART, t_pkmn->get_attribute("hoenn_smart_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_super_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::SMART_SUPER, t_pkmn->get_attribute("hoenn_smart_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_hyper_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::SMART_HYPER, t_pkmn->get_attribute("hoenn_smart_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_master_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::SMART_MASTER, t_pkmn->get_attribute("hoenn_smart_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::TOUGH, t_pkmn->get_attribute("hoenn_tough_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_super_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::TOUGH_SUPER, t_pkmn->get_attribute("hoenn_tough_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_hyper_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::TOUGH_HYPER, t_pkmn->get_attribute("hoenn_tough_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_master_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::TOUGH_MASTER, t_pkmn->get_attribute("hoenn_tough_master_ribbon"));

            if(t_pkmn->has_attribute("hoenn_champion_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::CHAMPION, t_pkmn->get_attribute("hoenn_champion_ribbon"));
            if(t_pkmn->has_attribute("hoenn_winning_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::WINNING, t_pkmn->get_attribute("hoenn_winning_ribbon"));
            if(t_pkmn->has_attribute("hoenn_victory_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::VICTORY, t_pkmn->get_attribute("hoenn_victory_ribbon"));
            if(t_pkmn->has_attribute("hoenn_artist_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::ARTIST, t_pkmn->get_attribute("hoenn_artist_ribbon"));
            if(t_pkmn->has_attribute("hoenn_effort_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::EFFORT, t_pkmn->get_attribute("hoenn_effort_ribbon"));
            if(t_pkmn->has_attribute("hoenn_marine_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::MARINE, t_pkmn->get_attribute("hoenn_marine_ribbon"));
            if(t_pkmn->has_attribute("hoenn_land_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::LAND, t_pkmn->get_attribute("hoenn_land_ribbon"));
            if(t_pkmn->has_attribute("hoenn_sky_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::SKY, t_pkmn->get_attribute("hoenn_sky_ribbon"));
            if(t_pkmn->has_attribute("hoenn_country_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::COUNTRY, t_pkmn->get_attribute("hoenn_country_ribbon"));
            if(t_pkmn->has_attribute("hoenn_national_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::NATIONAL, t_pkmn->get_attribute("hoenn_national_ribbon"));
            if(t_pkmn->has_attribute("hoenn_earth_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::EARTH, t_pkmn->get_attribute("hoenn_earth_ribbon"));
            if(t_pkmn->has_attribute("hoenn_world_ribbon"))
                set_hoenn_ribbon(&(pkmn_m_t->ribbonint), Ribbons::Hoenn::WORLD, t_pkmn->get_attribute("hoenn_world_ribbon"));
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

            t_pkmn->set_met_level(get_gen4_5_met_level(((uint8_t*)&(pokelib_pkmn.pkm->pkm.pokeball)+1)));
            t_pkmn->set_ball(game_ball_to_pkmnsim_ball(pokelib_pkmn.pkm->pkm.pokeball));
            if(get_gen4_5_otgender((uint8_t*)&(pokelib_pkmn.pkm->pkm.pokeball)+1)) t_pkmn->set_trainer_gender(Genders::FEMALE);
            else t_pkmn->set_trainer_gender(Genders::MALE);

            t_pkmn->set_held_item(pokelib_pkmn.pkm->pkm.held_item);
            t_pkmn->set_personality(pokelib_pkmn.pkm->pkm.pid);
            t_pkmn->set_public_trainer_id(pokelib_pkmn.pkm->pkm.ot_id);
            t_pkmn->set_secret_trainer_id(pokelib_pkmn.pkm->pkm.ot_sid);

            t_pkmn->set_EV(Stats::HP, pokelib_pkmn.pkm->pkm.ev_hp);
            t_pkmn->set_EV(Stats::ATTACK, pokelib_pkmn.pkm->pkm.ev_atk);
            t_pkmn->set_EV(Stats::DEFENSE, pokelib_pkmn.pkm->pkm.ev_def);
            t_pkmn->set_EV(Stats::SPECIAL_ATTACK, pokelib_pkmn.pkm->pkm.ev_satk);
            t_pkmn->set_EV(Stats::SPECIAL_DEFENSE, pokelib_pkmn.pkm->pkm.ev_sdef);
            t_pkmn->set_EV(Stats::SPEED, pokelib_pkmn.pkm->pkm.ev_spd);

            t_pkmn->set_IV(Stats::HP, gen3_4_5_get_IV(((uint32_t*)&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1), Stats::HP));
            t_pkmn->set_IV(Stats::ATTACK, gen3_4_5_get_IV(((uint32_t*)&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1), Stats::ATTACK));
            t_pkmn->set_IV(Stats::DEFENSE, gen3_4_5_get_IV(((uint32_t*)&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1), Stats::DEFENSE));
            t_pkmn->set_IV(Stats::SPECIAL_ATTACK, gen3_4_5_get_IV(((uint32_t*)&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1), Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV(Stats::SPECIAL_DEFENSE, gen3_4_5_get_IV(((uint32_t*)&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1), Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV(Stats::SPEED, gen3_4_5_get_IV(((uint32_t*)&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1), Stats::SPEED));

            //TODO: use form data to set Pokemon-sim form, is fateful encounter

            //Attributes
            t_pkmn->set_attribute("friendship", pokelib_pkmn.pkm->pkm.friendship);
            t_pkmn->set_attribute("circle", get_marking(&(pokelib_pkmn.pkm->pkm.markings), Markings::CIRCLE));
            t_pkmn->set_attribute("triangle", get_marking(&(pokelib_pkmn.pkm->pkm.markings), Markings::TRIANGLE));
            t_pkmn->set_attribute("square", get_marking(&(pokelib_pkmn.pkm->pkm.markings), Markings::SQUARE));
            t_pkmn->set_attribute("heart", get_marking(&(pokelib_pkmn.pkm->pkm.markings), Markings::HEART));
            t_pkmn->set_attribute("star", get_marking(&(pokelib_pkmn.pkm->pkm.markings), Markings::STAR));
            t_pkmn->set_attribute("diamond", get_marking(&(pokelib_pkmn.pkm->pkm.markings), Markings::DIAMOND));

            t_pkmn->set_attribute("country", pokelib_pkmn.pkm->pkm.country);
            t_pkmn->set_attribute("cool", pokelib_pkmn.pkm->pkm.contest_cool);
            t_pkmn->set_attribute("beauty", pokelib_pkmn.pkm->pkm.contest_beauty);
            t_pkmn->set_attribute("cute", pokelib_pkmn.pkm->pkm.contest_cute);
            t_pkmn->set_attribute("smart", pokelib_pkmn.pkm->pkm.contest_smart);
            t_pkmn->set_attribute("tough", pokelib_pkmn.pkm->pkm.contest_tough);
            t_pkmn->set_attribute("sheen", pokelib_pkmn.pkm->pkm.contest_sheen);

            t_pkmn->set_attribute("hoenn_cool_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::COOL));
            t_pkmn->set_attribute("hoenn_cool_super_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::COOL_SUPER));
            t_pkmn->set_attribute("hoenn_cool_hyper_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::COOL_HYPER));
            t_pkmn->set_attribute("hoenn_cool_master_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::COOL_MASTER));
            t_pkmn->set_attribute("hoenn_beauty_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::BEAUTY));
            t_pkmn->set_attribute("hoenn_beauty_super_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::BEAUTY_SUPER));
            t_pkmn->set_attribute("hoenn_beauty_hyper_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::BEAUTY_HYPER));
            t_pkmn->set_attribute("hoenn_beauty_master_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::BEAUTY_MASTER));
            t_pkmn->set_attribute("hoenn_cute_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::CUTE));
            t_pkmn->set_attribute("hoenn_cute_super_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::CUTE_SUPER));
            t_pkmn->set_attribute("hoenn_cute_hyper_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::CUTE_HYPER));
            t_pkmn->set_attribute("hoenn_cute_master_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::CUTE_MASTER));
            t_pkmn->set_attribute("hoenn_smart_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::SMART));
            t_pkmn->set_attribute("hoenn_smart_super_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::SMART_SUPER));
            t_pkmn->set_attribute("hoenn_smart_hyper_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::SMART_HYPER));
            t_pkmn->set_attribute("hoenn_smart_master_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::SMART_MASTER));
            t_pkmn->set_attribute("hoenn_tough_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::TOUGH));
            t_pkmn->set_attribute("hoenn_tough_super_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::TOUGH_SUPER));
            t_pkmn->set_attribute("hoenn_tough_hyper_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::TOUGH_HYPER));
            t_pkmn->set_attribute("hoenn_tough_master_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::TOUGH_MASTER));
            
            t_pkmn->set_attribute("hoenn_champion_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::CHAMPION));
            t_pkmn->set_attribute("hoenn_winning_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::WINNING));
            t_pkmn->set_attribute("hoenn_victory_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::VICTORY));
            t_pkmn->set_attribute("hoenn_artist_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::ARTIST));
            t_pkmn->set_attribute("hoenn_effort_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::EFFORT));
            t_pkmn->set_attribute("hoenn_marine_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::MARINE));
            t_pkmn->set_attribute("hoenn_land_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::LAND));
            t_pkmn->set_attribute("hoenn_sky_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::SKY));
            t_pkmn->set_attribute("hoenn_country_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::COUNTRY));
            t_pkmn->set_attribute("hoenn_national_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::NATIONAL));
            t_pkmn->set_attribute("hoenn_earth_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::EARTH));
            t_pkmn->set_attribute("hoenn_world_ribbon", get_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::WORLD));

            t_pkmn->set_attribute("sinnoh_champion_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::CHAMPION));
            t_pkmn->set_attribute("sinnoh_ability_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::ABILITY));
            t_pkmn->set_attribute("sinnoh_great_ability_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::GREAT_ABILITY));
            t_pkmn->set_attribute("sinnoh_double_ability_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::DOUBLE_ABILITY));
            t_pkmn->set_attribute("sinnoh_multi_ability_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::MULTI_ABILITY));
            t_pkmn->set_attribute("sinnoh_pair_ability_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::PAIR_ABILITY));
            t_pkmn->set_attribute("sinnoh_world_ability_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::WORLD_ABILITY));
            t_pkmn->set_attribute("sinnoh_alert_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::ALERT));
            t_pkmn->set_attribute("sinnoh_shock_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::SHOCK));
            t_pkmn->set_attribute("sinnoh_downcast_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::DOWNCAST));
            t_pkmn->set_attribute("sinnoh_careless_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::CARELESS));
            t_pkmn->set_attribute("sinnoh_relax_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::RELAX));
            t_pkmn->set_attribute("sinnoh_snooze_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::SNOOZE));
            t_pkmn->set_attribute("sinnoh_smile_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::SMILE));
            t_pkmn->set_attribute("sinnoh_gorgeous_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::GORGEOUS));
            t_pkmn->set_attribute("sinnoh_royal_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::ROYAL));
            t_pkmn->set_attribute("sinnoh_gorgeous_royal_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::GORGEOUS_ROYAL));
            t_pkmn->set_attribute("sinnoh_footprint_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::FOOTPRINT));
            t_pkmn->set_attribute("sinnoh_record_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::RECORD));
            t_pkmn->set_attribute("sinnoh_history_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::HISTORY));
            t_pkmn->set_attribute("sinnoh_legend_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::LEGEND));
            t_pkmn->set_attribute("sinnoh_red_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::RED));
            t_pkmn->set_attribute("sinnoh_green_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::GREEN));
            t_pkmn->set_attribute("sinnoh_blue_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::BLUE));
            t_pkmn->set_attribute("sinnoh_festival_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_carnival_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_classic_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::CLASSIC));
            t_pkmn->set_attribute("sinnoh_premier_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::PREMIER));

            t_pkmn->set_attribute("sinnoh_cool_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::COOL-23));
            t_pkmn->set_attribute("sinnoh_cool_great_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::COOL_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cool_ultra_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::COOL_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cool_master_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::COOL_MASTER-23));
            t_pkmn->set_attribute("sinnoh_beauty_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::BEAUTY-23));
            t_pkmn->set_attribute("sinnoh_beauty_great_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::BEAUTY_GREAT-23));
            t_pkmn->set_attribute("sinnoh_beauty_ultra_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::BEAUTY_GREAT-23));
            t_pkmn->set_attribute("sinnoh_beauty_master_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::BEAUTY_MASTER-23));
            t_pkmn->set_attribute("sinnoh_cute_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::CUTE-23));
            t_pkmn->set_attribute("sinnoh_cute_great_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::CUTE_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cute_ultra_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::CUTE_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cute_master_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::CUTE_MASTER-23));
            t_pkmn->set_attribute("sinnoh_smart_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::SMART-23));
            t_pkmn->set_attribute("sinnoh_smart_great_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::SMART_GREAT-23));
            t_pkmn->set_attribute("sinnoh_smart_ultra_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::SMART_GREAT-23));
            t_pkmn->set_attribute("sinnoh_smart_master_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::SMART_MASTER-23));
            t_pkmn->set_attribute("sinnoh_tough_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::TOUGH-23));
            t_pkmn->set_attribute("sinnoh_tough_great_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::TOUGH_GREAT-23));
            t_pkmn->set_attribute("sinnoh_tough_ultra_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::TOUGH_GREAT-23));
            t_pkmn->set_attribute("sinnoh_tough_master_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::TOUGH_MASTER-23));

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

            set_gen4_5_met_level(((uint8_t*)&(pokelib_pkmn.pkm->pkm.pokeball)+1), t_pkmn->get_met_level());
            pokelib_pkmn.pkm->pkm.pokeball = pkmnsim_ball_to_game_ball(t_pkmn->get_ball());
            set_gen4_5_met_level(((uint8_t*)&(pokelib_pkmn.pkm->pkm.pokeball)+1), (t_pkmn->get_trainer_gender() == Genders::FEMALE));

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
            gen3_4_5_set_IV(((uint32_t*)&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1), Stats::HP, IVs[Stats::HP]);
            gen3_4_5_set_IV(((uint32_t*)&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1), Stats::ATTACK, IVs[Stats::ATTACK]);
            gen3_4_5_set_IV(((uint32_t*)&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1), Stats::DEFENSE, IVs[Stats::DEFENSE]);
            gen3_4_5_set_IV(((uint32_t*)&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1), Stats::SPECIAL_ATTACK, IVs[Stats::SPECIAL_ATTACK]);
            gen3_4_5_set_IV(((uint32_t*)&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1), Stats::SPECIAL_DEFENSE, IVs[Stats::SPECIAL_DEFENSE]);
            gen3_4_5_set_IV(((uint32_t*)&(pokelib_pkmn.pkm->pkm.movePPUP[3])+1), Stats::SPEED, IVs[Stats::SPEED]);

            pokelib_pkmn.pkm->pkm.hometown = pkmnsim_game_to_hometown(t_pkmn->get_game_id());

            //Attributes
            if(t_pkmn->has_attribute("friendship"))
                pokelib_pkmn.pkm->pkm.friendship = t_pkmn->get_attribute("friendship");
            if(t_pkmn->has_attribute("circle"))
                set_marking(&(pokelib_pkmn.pkm->pkm.markings), Markings::CIRCLE, t_pkmn->get_attribute("circle"));
            if(t_pkmn->has_attribute("triangle"))
                set_marking(&(pokelib_pkmn.pkm->pkm.markings), Markings::TRIANGLE, t_pkmn->get_attribute("triangle"));
            if(t_pkmn->has_attribute("square"))
                set_marking(&(pokelib_pkmn.pkm->pkm.markings), Markings::SQUARE, t_pkmn->get_attribute("square"));
            if(t_pkmn->has_attribute("heart"))
                set_marking(&(pokelib_pkmn.pkm->pkm.markings), Markings::HEART, t_pkmn->get_attribute("heart"));
            if(t_pkmn->has_attribute("star"))
                set_marking(&(pokelib_pkmn.pkm->pkm.markings), Markings::STAR, t_pkmn->get_attribute("star"));
            if(t_pkmn->has_attribute("diamond"))
                set_marking(&(pokelib_pkmn.pkm->pkm.markings), Markings::DIAMOND, t_pkmn->get_attribute("diamond"));

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

            if(t_pkmn->has_attribute("sinnoh_champion_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::CHAMPION, t_pkmn->get_attribute("sinnoh_champion_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_ability_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::ABILITY, t_pkmn->get_attribute("sinnoh_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_great_ability_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::GREAT_ABILITY, t_pkmn->get_attribute("sinnoh_great_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_double_ability_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::DOUBLE_ABILITY, t_pkmn->get_attribute("sinnoh_double_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_multi_ability_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::MULTI_ABILITY, t_pkmn->get_attribute("sinnoh_multi_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_pair_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::PAIR_ABILITY, t_pkmn->get_attribute("sinnoh_pair_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_world_ability_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::WORLD_ABILITY, t_pkmn->get_attribute("sinnoh_ability_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_alert_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::ALERT, t_pkmn->get_attribute("sinnoh_alert_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_shock_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::SHOCK, t_pkmn->get_attribute("sinnoh_shock_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_downcast_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::DOWNCAST, t_pkmn->get_attribute("sinnoh_downcast_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_careless_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::CARELESS, t_pkmn->get_attribute("sinnoh_careless_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_relax_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::RELAX, t_pkmn->get_attribute("sinnoh_relax_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_snooze_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::SNOOZE, t_pkmn->get_attribute("sinnoh_snooze_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smile_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::SMILE, t_pkmn->get_attribute("sinnoh_smile_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_gorgeous_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::GORGEOUS, t_pkmn->get_attribute("sinnoh_gorgeous_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_royal_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::ROYAL, t_pkmn->get_attribute("sinnoh_royal_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_gorgeous_royal_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::GORGEOUS_ROYAL, t_pkmn->get_attribute("sinnoh_gorgeous_royal_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_record_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::RECORD, t_pkmn->get_attribute("sinnoh_record_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_history_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::HISTORY, t_pkmn->get_attribute("sinnoh_history_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_legend_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::LEGEND, t_pkmn->get_attribute("sinnoh_legend_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_red_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::RED, t_pkmn->get_attribute("sinnoh_red_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_green_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::GREEN, t_pkmn->get_attribute("sinnoh_green_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_blue_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::BLUE, t_pkmn->get_attribute("sinnoh_blue_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_festival_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::FESTIVAL, t_pkmn->get_attribute("sinnoh_festival_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_carnival_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::CARNIVAL, t_pkmn->get_attribute("sinnoh_carnival_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_classic_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::CLASSIC, t_pkmn->get_attribute("sinnoh_classic_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_premier_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohA)), Ribbons::Sinnoh::PREMIER, t_pkmn->get_attribute("sinnoh_premier_ribbon"));

            if(t_pkmn->has_attribute("hoenn_cool_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::COOL, t_pkmn->get_attribute("hoenn_cool_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_super_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::COOL_SUPER, t_pkmn->get_attribute("hoenn_cool_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_hyper_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::COOL_HYPER, t_pkmn->get_attribute("hoenn_cool_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cool_master_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::COOL_MASTER, t_pkmn->get_attribute("hoenn_cool_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::BEAUTY, t_pkmn->get_attribute("hoenn_beauty_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_super_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::BEAUTY_SUPER, t_pkmn->get_attribute("hoenn_beauty_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_hyper_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::BEAUTY_HYPER, t_pkmn->get_attribute("hoenn_beauty_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_beauty_master_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::BEAUTY_MASTER, t_pkmn->get_attribute("hoenn_beauty_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::CUTE, t_pkmn->get_attribute("hoenn_cute_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_super_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::CUTE_SUPER, t_pkmn->get_attribute("hoenn_cute_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_hyper_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::CUTE_HYPER, t_pkmn->get_attribute("hoenn_cute_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_cute_master_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::CUTE_MASTER, t_pkmn->get_attribute("hoenn_cute_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::SMART, t_pkmn->get_attribute("hoenn_smart_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_super_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::SMART_SUPER, t_pkmn->get_attribute("hoenn_smart_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_hyper_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::SMART_HYPER, t_pkmn->get_attribute("hoenn_smart_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_smart_master_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::SMART_MASTER, t_pkmn->get_attribute("hoenn_smart_master_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::TOUGH, t_pkmn->get_attribute("hoenn_tough_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_super_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::TOUGH_SUPER, t_pkmn->get_attribute("hoenn_tough_super_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_hyper_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::TOUGH_HYPER, t_pkmn->get_attribute("hoenn_tough_hyper_ribbon"));
            if(t_pkmn->has_attribute("hoenn_tough_master_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::TOUGH_MASTER, t_pkmn->get_attribute("hoenn_tough_master_ribbon"));

            if(t_pkmn->has_attribute("hoenn_champion_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::CHAMPION, t_pkmn->get_attribute("hoenn_champion_ribbon"));
            if(t_pkmn->has_attribute("hoenn_winning_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::WINNING, t_pkmn->get_attribute("hoenn_winning_ribbon"));
            if(t_pkmn->has_attribute("hoenn_victory_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::VICTORY, t_pkmn->get_attribute("hoenn_victory_ribbon"));
            if(t_pkmn->has_attribute("hoenn_artist_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::ARTIST, t_pkmn->get_attribute("hoenn_artist_ribbon"));
            if(t_pkmn->has_attribute("hoenn_effort_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::EFFORT, t_pkmn->get_attribute("hoenn_effort_ribbon"));
            if(t_pkmn->has_attribute("hoenn_marine_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::MARINE, t_pkmn->get_attribute("hoenn_marine_ribbon"));
            if(t_pkmn->has_attribute("hoenn_land_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::LAND, t_pkmn->get_attribute("hoenn_land_ribbon"));
            if(t_pkmn->has_attribute("hoenn_sky_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::SKY, t_pkmn->get_attribute("hoenn_sky_ribbon"));
            if(t_pkmn->has_attribute("hoenn_country_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::COUNTRY, t_pkmn->get_attribute("hoenn_country_ribbon"));
            if(t_pkmn->has_attribute("hoenn_national_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::NATIONAL, t_pkmn->get_attribute("hoenn_national_ribbon"));
            if(t_pkmn->has_attribute("hoenn_earth_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::EARTH, t_pkmn->get_attribute("hoenn_earth_ribbon"));
            if(t_pkmn->has_attribute("hoenn_world_ribbon"))
                set_hoenn_ribbon((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonHoenn)), Ribbons::Hoenn::WORLD, t_pkmn->get_attribute("hoenn_world_ribbon"));

            if(t_pkmn->has_attribute("sinnoh_cool_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::COOL-23, t_pkmn->get_attribute("sinnoh_cool_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cool_great_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::COOL_GREAT-23, t_pkmn->get_attribute("sinnoh_cool_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cool_ultra_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::COOL_ULTRA-23, t_pkmn->get_attribute("sinnoh_cool_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cool_master_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::COOL_MASTER-23, t_pkmn->get_attribute("sinnoh_cool_master_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_beauty_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::BEAUTY-23, t_pkmn->get_attribute("sinnoh_beauty_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_beauty_great_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::BEAUTY_GREAT-23, t_pkmn->get_attribute("sinnoh_beauty_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_beauty_ultra_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::BEAUTY_ULTRA-23, t_pkmn->get_attribute("sinnoh_beauty_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_beauty_master_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::BEAUTY_MASTER-23, t_pkmn->get_attribute("sinnoh_beauty_master_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cute_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::CUTE-23, t_pkmn->get_attribute("sinnoh_cute_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cute_great_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::CUTE_GREAT-23, t_pkmn->get_attribute("sinnoh_cute_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cute_ultra_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::CUTE_ULTRA-23, t_pkmn->get_attribute("sinnoh_cute_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_cute_master_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::CUTE_MASTER-23, t_pkmn->get_attribute("sinnoh_cute_master_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smart_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::SMART-23, t_pkmn->get_attribute("sinnoh_smart_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smart_great_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::SMART_GREAT-23, t_pkmn->get_attribute("sinnoh_smart_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smart_ultra_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::SMART_ULTRA-23, t_pkmn->get_attribute("sinnoh_smart_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_smart_master_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::SMART_MASTER-23, t_pkmn->get_attribute("sinnoh_smart_master_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_tough_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::TOUGH-23, t_pkmn->get_attribute("sinnoh_tough_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_tough_great_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::TOUGH_GREAT-23, t_pkmn->get_attribute("sinnoh_tough_great_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_tough_ultra_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::TOUGH_ULTRA-23, t_pkmn->get_attribute("sinnoh_tough_ultra_ribbon"));
            if(t_pkmn->has_attribute("sinnoh_tough_master_ribbon"))
                set_sinnoh_ribbon32((uint32_t*)(&(pokelib_pkmn.pkm->pkm.ribbonSinnohB)), Ribbons::Sinnoh::TOUGH_MASTER-23, t_pkmn->get_attribute("sinnoh_tough_master_ribbon"));

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
            if(t_pkmn->has_attribute("met_level"))
                set_gen4_5_met_level((&(pokelib_pkmn.pkm->pkm.pokeball))+1, t_pkmn->get_attribute("met_level"));
            if(t_pkmn->has_attribute("ot_is_female"))
                set_gen4_5_otgender((&(pokelib_pkmn.pkm->pkm.pokeball))+1, t_pkmn->get_attribute("ot_is_female"));

            return pokelib_pkmn;
        }

        team_pokemon::sptr pkmds_pokemon_to_team_pokemon(party_pkm* p_pkm)
        {
            opendb(get_database_path().c_str());

            unsigned int level, from_game;
            level = getpkmlevel(p_pkm->pkm_data);
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

            t_pkmn->set_met_level(get_gen4_5_met_level(((uint8_t*)&(p_pkm->pkm_data.ball)+1)));
            t_pkmn->set_ball(game_ball_to_pkmnsim_ball(p_pkm->pkm_data.ball));
            if(get_gen4_5_otgender(((uint8_t*)&(p_pkm->pkm_data.ball)+1))) t_pkmn->set_trainer_gender(Genders::FEMALE);
            else t_pkmn->set_trainer_gender(Genders::MALE);

            t_pkmn->set_held_item(p_pkm->pkm_data.item);
            t_pkmn->set_personality(p_pkm->pkm_data.pid);
            t_pkmn->set_public_trainer_id(p_pkm->pkm_data.tid);
            t_pkmn->set_secret_trainer_id(p_pkm->pkm_data.sid);

            t_pkmn->set_EV(Stats::HP, p_pkm->pkm_data.evs.hp);
            t_pkmn->set_EV(Stats::ATTACK, p_pkm->pkm_data.evs.attack);
            t_pkmn->set_EV(Stats::DEFENSE, p_pkm->pkm_data.evs.defense);
            t_pkmn->set_EV(Stats::SPECIAL_ATTACK, p_pkm->pkm_data.evs.spatk);
            t_pkmn->set_EV(Stats::SPECIAL_DEFENSE, p_pkm->pkm_data.evs.spdef);
            t_pkmn->set_EV(Stats::SPEED, p_pkm->pkm_data.evs.speed);

            t_pkmn->set_IV(Stats::HP, gen3_4_5_get_IV(((uint32_t*)&(p_pkm->pkm_data.ivs)), Stats::HP));
            t_pkmn->set_IV(Stats::ATTACK, gen3_4_5_get_IV(((uint32_t*)&(p_pkm->pkm_data.ivs)), Stats::ATTACK));
            t_pkmn->set_IV(Stats::DEFENSE, gen3_4_5_get_IV(((uint32_t*)&(p_pkm->pkm_data.ivs)), Stats::DEFENSE));
            t_pkmn->set_IV(Stats::SPECIAL_ATTACK, gen3_4_5_get_IV(((uint32_t*)&(p_pkm->pkm_data.ivs)), Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV(Stats::SPECIAL_DEFENSE, gen3_4_5_get_IV(((uint32_t*)&(p_pkm->pkm_data.ivs)), Stats::SPECIAL_DEFENSE));
            t_pkmn->set_IV(Stats::SPEED, gen3_4_5_get_IV(((uint32_t*)&(p_pkm->pkm_data.ivs)), Stats::SPEED));

            //Attributes
            t_pkmn->set_attribute("circle", get_marking((uint8_t*)&(p_pkm->pkm_data.markings), Markings::CIRCLE));
            t_pkmn->set_attribute("triangle", get_marking((uint8_t*)&(p_pkm->pkm_data.markings), Markings::TRIANGLE));
            t_pkmn->set_attribute("square", get_marking((uint8_t*)&(p_pkm->pkm_data.markings), Markings::SQUARE));
            t_pkmn->set_attribute("heart", get_marking((uint8_t*)&(p_pkm->pkm_data.markings), Markings::HEART));
            t_pkmn->set_attribute("star", get_marking((uint8_t*)&(p_pkm->pkm_data.markings), Markings::STAR));
            t_pkmn->set_attribute("diamond", get_marking((uint8_t*)&(p_pkm->pkm_data.markings), Markings::DIAMOND));

            t_pkmn->set_attribute("country", p_pkm->pkm_data.country);
            t_pkmn->set_attribute("cool", p_pkm->pkm_data.contest.cool);
            t_pkmn->set_attribute("beauty", p_pkm->pkm_data.contest.beauty);
            t_pkmn->set_attribute("cute", p_pkm->pkm_data.contest.cute);
            t_pkmn->set_attribute("smart", p_pkm->pkm_data.contest.smart);
            t_pkmn->set_attribute("tough", p_pkm->pkm_data.contest.tough);
            t_pkmn->set_attribute("sheen", p_pkm->pkm_data.contest.sheen);

            t_pkmn->set_attribute("hoenn_cool_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::COOL));
            t_pkmn->set_attribute("hoenn_cool_super_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::COOL_SUPER));
            t_pkmn->set_attribute("hoenn_cool_hyper_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::COOL_HYPER));
            t_pkmn->set_attribute("hoenn_cool_master_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::COOL_MASTER));
            t_pkmn->set_attribute("hoenn_beauty_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::BEAUTY));
            t_pkmn->set_attribute("hoenn_beauty_super_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::BEAUTY_SUPER));
            t_pkmn->set_attribute("hoenn_beauty_hyper_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::BEAUTY_HYPER));
            t_pkmn->set_attribute("hoenn_beauty_master_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::BEAUTY_MASTER));
            t_pkmn->set_attribute("hoenn_cute_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::CUTE));
            t_pkmn->set_attribute("hoenn_cute_super_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::CUTE_SUPER));
            t_pkmn->set_attribute("hoenn_cute_hyper_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::CUTE_HYPER));
            t_pkmn->set_attribute("hoenn_cute_master_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::CUTE_MASTER));
            t_pkmn->set_attribute("hoenn_smart_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::SMART));
            t_pkmn->set_attribute("hoenn_smart_super_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::SMART_SUPER));
            t_pkmn->set_attribute("hoenn_smart_hyper_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::SMART_HYPER));
            t_pkmn->set_attribute("hoenn_smart_master_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::SMART_MASTER));
            t_pkmn->set_attribute("hoenn_tough_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::TOUGH));
            t_pkmn->set_attribute("hoenn_tough_super_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::TOUGH_SUPER));
            t_pkmn->set_attribute("hoenn_tough_hyper_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::TOUGH_HYPER));
            t_pkmn->set_attribute("hoenn_tough_master_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::TOUGH_MASTER));
            t_pkmn->set_attribute("hoenn_champion_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::CHAMPION));
            t_pkmn->set_attribute("hoenn_winning_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::WINNING));
            t_pkmn->set_attribute("hoenn_victory_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::VICTORY));
            t_pkmn->set_attribute("hoenn_artist_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::ARTIST));
            t_pkmn->set_attribute("hoenn_effort_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::EFFORT));
            t_pkmn->set_attribute("hoenn_marine_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::MARINE));
            t_pkmn->set_attribute("hoenn_land_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::LAND));
            t_pkmn->set_attribute("hoenn_sky_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::SKY));
            t_pkmn->set_attribute("hoenn_country_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::COUNTRY));
            t_pkmn->set_attribute("hoenn_national_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::NATIONAL));
            t_pkmn->set_attribute("hoenn_earth_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::EARTH));
            t_pkmn->set_attribute("hoenn_world_ribbon", get_hoenn_ribbon((uint32_t*)(&(p_pkm->pkm_data.hribbon1)), Ribbons::Hoenn::WORLD));

            t_pkmn->set_attribute("sinnoh_champion_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::CHAMPION));
            t_pkmn->set_attribute("sinnoh_ability_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::ABILITY));
            t_pkmn->set_attribute("sinnoh_great_ability_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::GREAT_ABILITY));
            t_pkmn->set_attribute("sinnoh_double_ability_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::DOUBLE_ABILITY));
            t_pkmn->set_attribute("sinnoh_multi_ability_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::MULTI_ABILITY));
            t_pkmn->set_attribute("sinnoh_pair_ability_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::PAIR_ABILITY));
            t_pkmn->set_attribute("sinnoh_world_ability_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::WORLD_ABILITY));
            t_pkmn->set_attribute("sinnoh_alert_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::ALERT));
            t_pkmn->set_attribute("sinnoh_shock_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::SHOCK));
            t_pkmn->set_attribute("sinnoh_downcast_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::DOWNCAST));
            t_pkmn->set_attribute("sinnoh_careless_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::CARELESS));
            t_pkmn->set_attribute("sinnoh_relax_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::RELAX));
            t_pkmn->set_attribute("sinnoh_snooze_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::SNOOZE));
            t_pkmn->set_attribute("sinnoh_smile_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::SMILE));
            t_pkmn->set_attribute("sinnoh_gorgeous_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::GORGEOUS));
            t_pkmn->set_attribute("sinnoh_royal_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::ROYAL));
            t_pkmn->set_attribute("sinnoh_gorgeous_royal_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::GORGEOUS_ROYAL));
            t_pkmn->set_attribute("sinnoh_footprint_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::FOOTPRINT));
            t_pkmn->set_attribute("sinnoh_record_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::RECORD));
            t_pkmn->set_attribute("sinnoh_history_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::HISTORY));
            t_pkmn->set_attribute("sinnoh_legend_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::LEGEND));
            t_pkmn->set_attribute("sinnoh_red_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::RED));
            t_pkmn->set_attribute("sinnoh_green_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::GREEN));
            t_pkmn->set_attribute("sinnoh_blue_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::BLUE));
            t_pkmn->set_attribute("sinnoh_festival_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_carnival_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_classic_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::CLASSIC));
            t_pkmn->set_attribute("sinnoh_premier_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon1)), Ribbons::Sinnoh::PREMIER));

            t_pkmn->set_attribute("sinnoh_cool_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::COOL-23));
            t_pkmn->set_attribute("sinnoh_cool_great_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::COOL_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cool_ultra_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::COOL_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cool_master_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::COOL_MASTER-23));
            t_pkmn->set_attribute("sinnoh_beauty_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::BEAUTY-23));
            t_pkmn->set_attribute("sinnoh_beauty_great_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::BEAUTY_GREAT-23));
            t_pkmn->set_attribute("sinnoh_beauty_ultra_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::BEAUTY_GREAT-23));
            t_pkmn->set_attribute("sinnoh_beauty_master_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::BEAUTY_MASTER-23));
            t_pkmn->set_attribute("sinnoh_cute_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::CUTE-23));
            t_pkmn->set_attribute("sinnoh_cute_great_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::CUTE_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cute_ultra_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::CUTE_GREAT-23));
            t_pkmn->set_attribute("sinnoh_cute_master_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::CUTE_MASTER-23));
            t_pkmn->set_attribute("sinnoh_smart_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::SMART-23));
            t_pkmn->set_attribute("sinnoh_smart_great_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::SMART_GREAT-23));
            t_pkmn->set_attribute("sinnoh_smart_ultra_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::SMART_GREAT-23));
            t_pkmn->set_attribute("sinnoh_smart_master_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::SMART_MASTER-23));
            t_pkmn->set_attribute("sinnoh_tough_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::TOUGH-23));
            t_pkmn->set_attribute("sinnoh_tough_great_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::TOUGH_GREAT-23));
            t_pkmn->set_attribute("sinnoh_tough_ultra_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::TOUGH_GREAT-23));
            t_pkmn->set_attribute("sinnoh_tough_master_ribbon", get_sinnoh_ribbon32((uint32_t*)(&(p_pkm->pkm_data.sribbon3)), Ribbons::Sinnoh::TOUGH_MASTER-23));

            t_pkmn->set_attribute("eggmet_year", p_pkm->pkm_data.metdate.year);
            t_pkmn->set_attribute("eggmet_month", p_pkm->pkm_data.metdate.month);
            t_pkmn->set_attribute("eggmet_day", p_pkm->pkm_data.metdate.day);
            t_pkmn->set_attribute("met_year", p_pkm->pkm_data.metdate.year);
            t_pkmn->set_attribute("met_month", p_pkm->pkm_data.metdate.month);
            t_pkmn->set_attribute("met_day", p_pkm->pkm_data.metdate.day);

            closedb();

            return t_pkmn;
        }

/*
    void team_pokemon_to_pkmds_pkmn(team_pokemon::sptr s_pkmn, party_pkm* p_pkm)
    {
        p_pkm->pkm_data.species = ::Species::pkmspecies(s_pkmn->get_base_pokemon()->get_nat_pokedex_num());
        p_pkm->pkm_data.moves[0] = ::Moves::moves(s_pkmn->get_moves()[0]->get_move_id());
        if(s_pkmn->get_moves()[1]->get_name() == "Struggle") p_pkm->pkm_data.moves[1] = ::Moves::NOTHING;
        else p_pkm->pkm_data.moves[1] = ::Moves::moves(s_pkmn->get_moves()[1]->get_move_id());
        if(s_pkmn->get_moves()[2]->get_name() == "Struggle") p_pkm->pkm_data.moves[2] = ::Moves::NOTHING;
        else p_pkm->pkm_data.moves[2] = ::Moves::moves(s_pkmn->get_moves()[2]->get_move_id());
        if(s_pkmn->get_moves()[3]->get_name() == "Struggle") p_pkm->pkm_data.moves[3] = ::Moves::NOTHING;
        else p_pkm->pkm_data.moves[3] = ::Moves::moves(s_pkmn->get_moves()[3]->get_move_id());

        //Unlike species and moves, nature ID's don't correspond to anything PKMNsim uses
        unsigned char nature_map[] = {0,1,6,16,21,11,2,7,17,22,12,4,9,19,24,20,14,5,10,25,15,3,8,18,23,13};
        p_pkm->pkm_data.nature = ::Natures::natures(nature_map[s_pkmn->nature->get_nature_id()]);
        setlevel(p_pkm->pkm_data, s_pkmn->level);
        #ifdef __linux__
        setpkmnickname(p_pkm->pkm_data, (wchar_t*)getwstring((char*)s_pkmn->nickname.c_str(), s_pkmn->nickname.size()).c_str(), s_pkmn->nickname.size());
        #else
        setpkmnickname(p_pkm->pkm_data, (wchar_t*)getwstring(s_pkmn->nickname).c_str(), s_pkmn->nickname.size());
        #endif
        p_pkm->pkm_data.pid = s_pkmn->personality;
        p_pkm->pkm_data.sid = s_pkmn->secret_id;
        p_pkm->pkm_data.tid = s_pkmn->trainer_id;

        p_pkm->party_data.maxhp = s_pkmn->HP;
        p_pkm->party_data.attack = s_pkmn->ATK;
        p_pkm->party_data.defense = s_pkmn->DEF;
        p_pkm->party_data.spatk = s_pkmn->SATK;
        p_pkm->party_data.spdef = s_pkmn->SDEF;
        p_pkm->party_data.speed = s_pkmn->SPD;

        p_pkm->pkm_data.ivs.hp = s_pkmn->ivHP;
        p_pkm->pkm_data.ivs.attack = s_pkmn->ivATK;
        p_pkm->pkm_data.ivs.defense = s_pkmn->ivDEF;
        p_pkm->pkm_data.ivs.spatk = s_pkmn->ivSATK;
        p_pkm->pkm_data.ivs.spdef = s_pkmn->ivSDEF;
        p_pkm->pkm_data.ivs.speed = s_pkmn->ivSPD;

        p_pkm->pkm_data.evs.hp = s_pkmn->evHP;
        p_pkm->pkm_data.evs.attack = s_pkmn->evATK;
        p_pkm->pkm_data.evs.defense = s_pkmn->evDEF;
        p_pkm->pkm_data.evs.spatk = s_pkmn->evSATK;
        p_pkm->pkm_data.evs.spdef = s_pkmn->evSDEF;
        p_pkm->pkm_data.evs.speed = s_pkmn->evSPD;
        
        switch(s_pkmn->from_game)
        {
            case Games::RUBY:
                p_pkm->pkm_data.hometown = Hometowns::ruby;
                break;
            case Games::SAPPHIRE:
                p_pkm->pkm_data.hometown = Hometowns::sapphire;
                break;
            case Games::EMERALD:
                p_pkm->pkm_data.hometown = Hometowns::emerald;
                break;
            case Games::FIRE_RED:
                p_pkm->pkm_data.hometown = Hometowns::firered;
                break;
            case Games::LEAF_GREEN:
                p_pkm->pkm_data.hometown = Hometowns::leafgreen;
                break;
            case Games::COLOSSEUM:
            case Games::XD:
                p_pkm->pkm_data.hometown = Hometowns::colosseum;
                break;
            case Games::DIAMOND:
                p_pkm->pkm_data.hometown = Hometowns::diamond;
                break;
            case Games::PEARL:
                p_pkm->pkm_data.hometown = Hometowns::pearl;
                break;
            case Games::PLATINUM:
                p_pkm->pkm_data.hometown = Hometowns::platinum;
                break;
            case Games::HEART_GOLD:
                p_pkm->pkm_data.hometown = Hometowns::heartgold;
                break;
            case Games::SOUL_SILVER:
                p_pkm->pkm_data.hometown = Hometowns::soulsilver;
                break;
            case Games::BLACK:
                p_pkm->pkm_data.hometown = Hometowns::black;
                break;
            case Games::WHITE:
                p_pkm->pkm_data.hometown = Hometowns::white;
                break;
            case Games::BLACK2:
                p_pkm->pkm_data.hometown = Hometowns::black2;
                break;
            case Games::WHITE2:
                p_pkm->pkm_data.hometown = Hometowns::white2;
                break;
            default:
                p_pkm->pkm_data.hometown = Hometowns::black;
                break;
        }

        //Attributes
        if(s_pkmn->attributes.has_key("friendship"))
            p_pkm->pkm_data.tameness = byte(s_pkmn->attributes["friendship"]);
        if(s_pkmn->attributes.has_key("circle"))
            p_pkm->pkm_data.markings.circle = bool(s_pkmn->attributes["circle"]);
        if(s_pkmn->attributes.has_key("triangle"))
            p_pkm->pkm_data.markings.circle = bool(s_pkmn->attributes["triangle"]);
        if(s_pkmn->attributes.has_key("square"))
            p_pkm->pkm_data.markings.circle = bool(s_pkmn->attributes["square"]);
        if(s_pkmn->attributes.has_key("heart")) 
            p_pkm->pkm_data.markings.circle = bool(s_pkmn->attributes["heart"]);
        if(s_pkmn->attributes.has_key("star"))
            p_pkm->pkm_data.markings.circle = bool(s_pkmn->attributes["star"]);
        if(s_pkmn->attributes.has_key("diamond"))
            p_pkm->pkm_data.markings.circle = bool(s_pkmn->attributes["diamond"]);
        if(s_pkmn->attributes.has_key("country"))
            p_pkm->pkm_data.country = Countries::countries(s_pkmn->attributes["country"]);
        if(s_pkmn->attributes.has_key("cool"))
            p_pkm->pkm_data.contest.cool = byte(s_pkmn->attributes["cool"]);
        if(s_pkmn->attributes.has_key("beauty"))
            p_pkm->pkm_data.contest.cool = byte(s_pkmn->attributes["beauty"]);
        if(s_pkmn->attributes.has_key("cute"))
            p_pkm->pkm_data.contest.cool = byte(s_pkmn->attributes["cute"]);
        if(s_pkmn->attributes.has_key("smart"))
            p_pkm->pkm_data.contest.cool = byte(s_pkmn->attributes["smart"]);
        if(s_pkmn->attributes.has_key("tough"))
            p_pkm->pkm_data.contest.cool = byte(s_pkmn->attributes["tough"]);
        if(s_pkmn->attributes.has_key("sheen"))
            p_pkm->pkm_data.contest.cool = byte(s_pkmn->attributes["sheen"]);
        if(s_pkmn->attributes.has_key("sinnoh_champ_ribbon"))
            p_pkm->pkm_data.sribbon1.sinnoh_champ_ribbon = bool(s_pkmn->attributes["sinnoh_champ_ribbon"]);
        if(s_pkmn->attributes.has_key("ability_ribbon"))
            p_pkm->pkm_data.sribbon1.ability_ribbon = bool(s_pkmn->attributes["sinnoh_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_great_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.great_ability_ribbon = bool(s_pkmn->attributes["sinnoh_great_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_double_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.double_ability_ribbon = bool(s_pkmn->attributes["sinnoh_double_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_multi_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.multi_ability_ribbon = bool(s_pkmn->attributes["sinnoh_multi_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_pair_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.pair_ability_ribbon = bool(s_pkmn->attributes["sinnoh_pair_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_world_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.world_ability_ribbon = bool(s_pkmn->attributes["sinnoh_world_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_alert_ribbon"))
            p_pkm->pkm_data.sribbon1.alert_ribbon = bool(s_pkmn->attributes["sinnoh_alert_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_shock_ribbon"))
            p_pkm->pkm_data.sribbon1.shock_ribbon = bool(s_pkmn->attributes["sinnoh_shock_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_downcast_ribbon"))
            p_pkm->pkm_data.sribbon1.downcast_ribbon = bool(s_pkmn->attributes["sinnoh_downcast_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_careless_ribbon"))
            p_pkm->pkm_data.sribbon1.careless_ribbon = bool(s_pkmn->attributes["sinnoh_careless_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_relax_ribbon"))
            p_pkm->pkm_data.sribbon1.relax_ribbon = bool(s_pkmn->attributes["sinnoh_relax_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_snooze_ribbon"))
            p_pkm->pkm_data.sribbon1.snooze_ribbon = bool(s_pkmn->attributes["sinnoh_snooze_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_smile_ribbon"))
            p_pkm->pkm_data.sribbon1.smile_ribbon = bool(s_pkmn->attributes["sinnoh_smile_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_gorgeous_ribbon"))
            p_pkm->pkm_data.sribbon1.gorgeous_ribbon = bool(s_pkmn->attributes["sinnoh_gorgeous_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_royal_ribbon"))
            p_pkm->pkm_data.sribbon1.royal_ribbon = bool(s_pkmn->attributes["sinnoh_royal_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_gorgeous_royal_ribbon"))
            p_pkm->pkm_data.sribbon2.gorgeous_royal_ribbon = bool(s_pkmn->attributes["sinnoh_gorgeous_royal_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_footprint_ribbon"))
            p_pkm->pkm_data.sribbon2.footprint_ribbon = bool(s_pkmn->attributes["sinnoh_footprint_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_record_ribbon"))
            p_pkm->pkm_data.sribbon2.record_ribbon = bool(s_pkmn->attributes["sinnoh_record_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_history_ribbon"))
            p_pkm->pkm_data.sribbon2.history_ribbon = bool(s_pkmn->attributes["sinnoh_history_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_legend_ribbon"))
            p_pkm->pkm_data.sribbon2.legend_ribbon = bool(s_pkmn->attributes["sinnoh_legend_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_red_ribbon"))
            p_pkm->pkm_data.sribbon2.red_ribbon = bool(s_pkmn->attributes["sinnoh_red_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_green_ribbon"))
            p_pkm->pkm_data.sribbon2.green_ribbon = bool(s_pkmn->attributes["sinnoh_green_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_blue_ribbon"))
            p_pkm->pkm_data.sribbon2.blue_ribbon = bool(s_pkmn->attributes["sinnoh_blue_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_festival_ribbon"))
            p_pkm->pkm_data.sribbon2.festival_ribbon = bool(s_pkmn->attributes["sinnoh_festival_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_carnival_ribbon"))
            p_pkm->pkm_data.sribbon2.carnival_ribbon = bool(s_pkmn->attributes["sinnoh_carnival_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_classic_ribbon"))
            p_pkm->pkm_data.sribbon2.classic_ribbon = bool(s_pkmn->attributes["sinnoh_classic_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_premier_ribbon"))
            p_pkm->pkm_data.sribbon2.premier_ribbon = bool(s_pkmn->attributes["sinnoh_premier_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon"))
            p_pkm->pkm_data.hribbon1.cute_ribbon = bool(s_pkmn->attributes["hoenn_cute_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_super"))
            p_pkm->pkm_data.hribbon1.cool_ribbon_super = bool(s_pkmn->attributes["hoenn_cool_ribbon_super"]);
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_hyper"))
            p_pkm->pkm_data.hribbon1.cool_ribbon_hyper = bool(s_pkmn->attributes["hoenn_cool_ribbon_hyper"]);
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_master"))
            p_pkm->pkm_data.hribbon1.cool_ribbon_master = bool(s_pkmn->attributes["hoenn_cool_ribbon_master"]);
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon"))
            p_pkm->pkm_data.hribbon1.beauty_ribbon = bool(s_pkmn->attributes["hoenn_beauty_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_super"))
            p_pkm->pkm_data.hribbon1.beauty_ribbon_super = bool(s_pkmn->attributes["hoenn_beauty_ribbon_super"]);
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_hyper"))
            p_pkm->pkm_data.hribbon1.beauty_ribbon_hyper = bool(s_pkmn->attributes["hoenn_beauty_ribbon_hyper"]);
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_master"))
            p_pkm->pkm_data.hribbon1.beauty_ribbon_master = bool(s_pkmn->attributes["hoenn_beauty_ribbon_master"]);
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon"))
            p_pkm->pkm_data.hribbon1.cute_ribbon = bool(s_pkmn->attributes["hoenn_cute_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_super"))
            p_pkm->pkm_data.hribbon1.cute_ribbon_super = bool(s_pkmn->attributes["hoenn_cute_ribbon_super"]);
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_hyper"))
            p_pkm->pkm_data.hribbon1.cute_ribbon_hyper = bool(s_pkmn->attributes["hoenn_cute_ribbon_hyper"]);
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_master"))
            p_pkm->pkm_data.hribbon1.cute_ribbon_master = bool(s_pkmn->attributes["hoenn_cute_ribbon_master"]);
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon"))
            p_pkm->pkm_data.hribbon1.smart_ribbon = bool(s_pkmn->attributes["hoenn_smart_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_super"))
            p_pkm->pkm_data.hribbon1.smart_ribbon_super = bool(s_pkmn->attributes["hoenn_smart_ribbon_super"]);
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_hyper"))
            p_pkm->pkm_data.hribbon1.smart_ribbon_hyper = bool(s_pkmn->attributes["hoenn_smart_ribbon_hyper"]);
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_master"))
            p_pkm->pkm_data.hribbon1.smart_ribbon_master = bool(s_pkmn->attributes["hoenn_smart_ribbon_master"]);
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon"))
            p_pkm->pkm_data.hribbon2.tough_ribbon = bool(s_pkmn->attributes["hoenn_tough_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_super"))
            p_pkm->pkm_data.hribbon2.tough_ribbon_super = bool(s_pkmn->attributes["hoenn_tough_ribbon_super"]);
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_hyper"))
            p_pkm->pkm_data.hribbon2.tough_ribbon_hyper = bool(s_pkmn->attributes["hoenn_tough_ribbon_hyper"]);
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_master"))
            p_pkm->pkm_data.hribbon2.tough_ribbon_master = bool(s_pkmn->attributes["hoenn_tough_ribbon_master"]);
        if(s_pkmn->attributes.has_key("hoenn_champion_ribbon"))
            p_pkm->pkm_data.hribbon2.champion_ribbon = bool(s_pkmn->attributes["hoenn_champion_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_winning_ribbon"))
            p_pkm->pkm_data.hribbon2.winning_ribbon = bool(s_pkmn->attributes["hoenn_winning_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_victory_ribbon"))
            p_pkm->pkm_data.hribbon2.victory_ribbon = bool(s_pkmn->attributes["hoenn_victory_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_artist_ribbon"))
            p_pkm->pkm_data.hribbon2.artist_ribbon = bool(s_pkmn->attributes["hoenn_artist_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_effort_ribbon"))
            p_pkm->pkm_data.hribbon2.effort_ribbon = bool(s_pkmn->attributes["hoenn_effort_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_marine_ribbon"))
            p_pkm->pkm_data.hribbon2.marine_ribbon = bool(s_pkmn->attributes["hoenn_marine_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_land_ribbon"))
            p_pkm->pkm_data.hribbon2.land_ribbon = bool(s_pkmn->attributes["hoenn_land_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_sky_ribbon"))
            p_pkm->pkm_data.hribbon2.sky_ribbon = bool(s_pkmn->attributes["hoenn_sky_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_country_ribbon"))
            p_pkm->pkm_data.hribbon2.country_ribbon = bool(s_pkmn->attributes["hoenn_country_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_national_ribbon"))
            p_pkm->pkm_data.hribbon2.national_ribbon = bool(s_pkmn->attributes["hoenn_national_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_earth_ribbon"))
            p_pkm->pkm_data.hribbon2.earth_ribbon = bool(s_pkmn->attributes["hoenn_earth_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_world_ribbon"))
            p_pkm->pkm_data.hribbon2.world_ribbon = bool(s_pkmn->attributes["hoenn_world_ribbon"]);
        if(s_pkmn->attributes.has_key("fateful_encounter"))
            p_pkm->pkm_data.forms.fencounter = bool(s_pkmn->attributes["fateful_encounter"]);
        if(s_pkmn->attributes.has_key("has_dream_world_ability"))
            p_pkm->pkm_data.dwability.hasdwability = bool(s_pkmn->attributes["has_dream_world_ability"]);
        if(s_pkmn->attributes.has_key("n_flag"))
            p_pkm->pkm_data.dwability.n_pkm = bool(s_pkmn->attributes["n_flag"]);
        if(s_pkmn->attributes.has_key("original_game"))
            p_pkm->pkm_data.hometown = Hometowns::hometowns(s_pkmn->attributes["original_game"]);
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon"))
            p_pkm->pkm_data.sribbon3.cute_ribbon = bool(s_pkmn->attributes["sinnoh_cute_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon_great"))
            p_pkm->pkm_data.sribbon3.cool_ribbon_great = bool(s_pkmn->attributes["sinnoh_cool_ribbon_great"]);
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon_ultra"))
            p_pkm->pkm_data.sribbon3.cool_ribbon_ultra = bool(s_pkmn->attributes["sinnoh_cool_ribbon_ultra"]);
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon_master"))
            p_pkm->pkm_data.sribbon3.cool_ribbon_master = bool(s_pkmn->attributes["sinnoh_cool_ribbon_master"]);
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon"))
            p_pkm->pkm_data.sribbon3.beauty_ribbon = bool(s_pkmn->attributes["sinnoh_beauty_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon_great"))
            p_pkm->pkm_data.sribbon3.beauty_ribbon_great = bool(s_pkmn->attributes["sinnoh_beauty_ribbon_great"]);
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon_ultra"))
            p_pkm->pkm_data.sribbon3.beauty_ribbon_ultra = bool(s_pkmn->attributes["sinnoh_beauty_ribbon_ultra"]);
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon_master"))
            p_pkm->pkm_data.sribbon3.beauty_ribbon_master = bool(s_pkmn->attributes["sinnoh_beauty_ribbon_master"]);
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon"))
            p_pkm->pkm_data.sribbon3.cute_ribbon = bool(s_pkmn->attributes["sinnoh_cute_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon_great"))
            p_pkm->pkm_data.sribbon3.cute_ribbon_great = bool(s_pkmn->attributes["sinnoh_cute_ribbon_great"]);
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon_ultra"))
            p_pkm->pkm_data.sribbon3.cute_ribbon_ultra = bool(s_pkmn->attributes["sinnoh_cute_ribbon_ultra"]);
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon_master"))
            p_pkm->pkm_data.sribbon3.cute_ribbon_master = bool(s_pkmn->attributes["sinnoh_cute_ribbon_master"]);
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon"))
            p_pkm->pkm_data.sribbon3.smart_ribbon = bool(s_pkmn->attributes["sinnoh_smart_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon_great"))
            p_pkm->pkm_data.sribbon3.smart_ribbon_great = bool(s_pkmn->attributes["sinnoh_smart_ribbon_great"]);
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon_ultra"))
            p_pkm->pkm_data.sribbon3.smart_ribbon_ultra = bool(s_pkmn->attributes["sinnoh_smart_ribbon_ultra"]);
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon_master"))
            p_pkm->pkm_data.sribbon3.smart_ribbon_master = bool(s_pkmn->attributes["sinnoh_smart_ribbon_master"]);
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon"))
            p_pkm->pkm_data.sribbon4.tough_ribbon = bool(s_pkmn->attributes["sinnoh_tough_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon_great"))
            p_pkm->pkm_data.sribbon4.tough_ribbon_great = bool(s_pkmn->attributes["sinnoh_tough_ribbon_great"]);
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon_ultra"))
            p_pkm->pkm_data.sribbon4.tough_ribbon_ultra = bool(s_pkmn->attributes["sinnoh_tough_ribbon_ultra"]);
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon_master"))
            p_pkm->pkm_data.sribbon4.tough_ribbon_master = bool(s_pkmn->attributes["sinnoh_tough_ribbon_master"]);
        if(s_pkmn->attributes.has_key("unova_ball"))
            p_pkm->pkm_data.ball = Balls::balls(s_pkmn->attributes["unova_ball"]);

        //Don't fill in datefields unless all three attributes are present
        if(s_pkmn->attributes.has_key("eggmet_year") and s_pkmn->attributes.has_key("eggmet_month") and
           s_pkmn->attributes.has_key("eggmet_day"))
        {
            p_pkm->pkm_data.eggdate.year = byte(s_pkmn->attributes["eggmet_year"]);
            p_pkm->pkm_data.eggdate.month = byte(s_pkmn->attributes["eggmet_month"]);
            p_pkm->pkm_data.eggdate.day = byte(s_pkmn->attributes["eggmet_day"]);
        }
        if(s_pkmn->attributes.has_key("met_year") and s_pkmn->attributes.has_key("met_month") and
           s_pkmn->attributes.has_key("met_day"))
        {
            p_pkm->pkm_data.metdate.year = byte(s_pkmn->attributes["met_year"]);
            p_pkm->pkm_data.metdate.month = byte(s_pkmn->attributes["met_month"]);
            p_pkm->pkm_data.metdate.day = byte(s_pkmn->attributes["met_day"]);
        }

        if(s_pkmn->attributes.has_key("eggmet_location"))
            p_pkm->pkm_data.eggmet = Locations::locations(s_pkmn->attributes["eggmet_location"]);
        if(s_pkmn->attributes.has_key("met_location"))
            p_pkm->pkm_data.met = Locations::locations(s_pkmn->attributes["met_location"]);
        if(s_pkmn->attributes.has_key("met_level"))
            p_pkm->pkm_data.metlevel_otgender.metlevel = byte(s_pkmn->attributes["met_level"]);
        if(s_pkmn->attributes.has_key("met_level"))
            #if defined(__linux__)
            p_pkm->pkm_data.metlevel_otgender.metlevel = byte(s_pkmn->attributes["ot_is_female"]);
            #else
            p_pkm->pkm_data.metlevel_otgender.metlevel = ::Genders::genders(s_pkmn->attributes["ot_is_female"]);
            #endif
        if(s_pkmn->attributes.has_key("unova_encounter"))
            p_pkm->pkm_data.encounter = Encounters::encounters(s_pkmn->attributes["unova_encounter"]);
    }

    PokeLib::Pokemon pokehack_pkmn_to_pokelib_pkmn(belt_pokemon_t* b_pkmn_t,
                                                              pokemon_attacks_t* pkmn_a_t,
                                                              pokemon_effort_t* pkmn_e_t,
                                                              pokemon_misc_t* pkmn_m_t,
                                                              pokemon_growth_t* pkmn_g_t)
    {
        PokeLib::Pokemon pokelib_pkmn = team_pokemon_to_pokelib_pkmn(pokehack_pkmn_to_team_pokemon(b_pkmn_t,
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

    void pokehack_pkmn_to_pkmds_pkmn(belt_pokemon_t* b_pkmn_t,
                                                pokemon_attacks_t* pkmn_a_t,
                                                pokemon_effort_t* pkmn_e_t,
                                                pokemon_misc_t* pkmn_m_t,
                                                pokemon_growth_t* pkmn_g_t,
                                                party_pkm* p_pkm)
    {
        team_pokemon_to_pkmds_pkmn(pokehack_pkmn_to_team_pokemon(b_pkmn_t, pkmn_a_t, pkmn_e_t, pkmn_m_t, pkmn_g_t), p_pkm);

        //Manually set egg met and met locations to Faraway Place
        p_pkm->pkm_data.eggmet = Locations::farawayplace;
        p_pkm->pkm_data.met = Locations::farawayplace;
    }

    void pokelib_pkmn_to_pokehack_pkmn(PokeLib::Pokemon pokelib_pkmn,
                                                  belt_pokemon_t* b_pkmn_t,
                                                  pokemon_attacks_t* pkmn_a_t,
                                                  pokemon_effort_t* pkmn_e_t,
                                                  pokemon_misc_t* pkmn_m_t,
                                                  pokemon_growth_t* pkmn_g_t)
    {
        team_pokemon_to_pokehack_pkmn(pokelib_pkmn_to_team_pokemon(pokelib_pkmn), b_pkmn_t, pkmn_a_t, pkmn_e_t, pkmn_m_t, pkmn_g_t);

        //Manually set met location to In-Game Trade
        pkmn_m_t->locationcaught = 255;

        //If Pokemon was caught in a Gen 4 ball, set it to standard Poke Ball
        if(pkmn_m_t->pokeball > 0xC) pkmn_m_t->pokeball = 0x4;
    }

    void pokelib_pkmn_to_pkmds_pkmn(PokeLib::Pokemon pokelib_pkmn, party_pkm* p_pkm)
    {
        team_pokemon_to_pkmds_pkmn(pokelib_pkmn_to_team_pokemon(pokelib_pkmn), p_pkm);

        //Manually set egg met location to Faraway place and met location to Poke Transfer
        p_pkm->pkm_data.eggmet = Locations::farawayplace;
        p_pkm->pkm_data.met = Locations::poketransfer;
    }

    void pkmds_pkmn_to_pokehack_pkmn(party_pkm* p_pkm,
                                                belt_pokemon_t* b_pkmn_t,
                                                pokemon_attacks_t* pkmn_a_t,
                                                pokemon_effort_t* pkmn_e_t,
                                                pokemon_misc_t* pkmn_m_t,
                                                pokemon_growth_t* pkmn_g_t)
    {
        team_pokemon_to_pokehack_pkmn(pkmds_pkmn_to_team_pokemon(p_pkm), b_pkmn_t, pkmn_a_t, pkmn_e_t, pkmn_m_t, pkmn_g_t);
    }

    PokeLib::Pokemon pkmds_pkmn_to_pokelib_pkmn(party_pkm* p_pkm)
    {
        PokeLib::Pokemon pokelib_pkmn = team_pokemon_to_pokelib_pkmn(pkmds_pkmn_to_team_pokemon(p_pkm));

        //Manually set egg met and met locations to Faraway Place
        pokelib_pkmn.pkm->pkm.eggLoc_DP = char(3002);
        pokelib_pkmn.pkm->pkm.eggLoc_Plat = char(3002);
        pokelib_pkmn.pkm->pkm.metLoc_DP = char(3002);
        pokelib_pkmn.pkm->pkm.metLoc_Plat = char(3002);

        //If Pokemon was caught in a Gen 5 ball, set it to standard Poke Ball
        if(pokelib_pkmn.pkm->pkm.pokeball > char(Balls::compball)) pokelib_pkmn.pkm->pkm.pokeball = 0x4;

        //Manually set encounter type to Special Event
        pokelib_pkmn.pkm->pkm.encounterType = 0x0;

        return pokelib_pkmn;
    }*/
    }
}
