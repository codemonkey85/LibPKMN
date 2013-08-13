/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifdef _MSC_VER
#pragma warning(disable:4800)
#endif

#include <algorithm>
#include <bitset>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/conversions.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/pkmn_nature.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include <pkmnsim/database/queries.hpp>

#include <pkmds/pkmds_g5_sqlite.h>

#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

#define MAX_NICKNAME_LEN 10
#define MAX_TRAINER_NAME_LEN 7

namespace pkmnsim
{
    converter::converter()
    {
        pokehack_reverse_char_map = boost::assign::map_list_of
            ('0', int(0xA1))
            ('1', int(0xA2))
            ('2', int(0xA3))
            ('3', int(0xA4))
            ('4', int(0xA5))
            ('5', int(0xA6))
            ('6', int(0xA7))
            ('7', int(0xA8))
            ('8', int(0xA9))
            ('9', int(0xAA))
            ('!', int(0xAB))
            ('?', int(0xAC))
            ('.', int(0xAD))
            ('-', int(0xAE))
            ('"', int(0xB1))
            ('\'', int(0xB2))
            ('/', int(0xBA))
            ('A', int(0xBB))
            ('B', int(0xBC))
            ('C', int(0xBD))
            ('D', int(0xBE))
            ('E', int(0xBF))
            ('F', int(0xC0))
            ('G', int(0xC1))
            ('H', int(0xC2))
            ('I', int(0xC3))
            ('J', int(0xC4))
            ('K', int(0xC5))
            ('L', int(0xC6))
            ('M', int(0xC7))
            ('N', int(0xC8))
            ('O', int(0xC9))
            ('P', int(0xCA))
            ('Q', int(0xCB))
            ('R', int(0xCC))
            ('S', int(0xCD))
            ('T', int(0xCE))
            ('U', int(0xCF))
            ('V', int(0xD0))
            ('W', int(0xD1))
            ('X', int(0xD2))
            ('Y', int(0xD3))
            ('Z', int(0xD4))
            ('a', int(0xD5))
            ('b', int(0xD6))
            ('c', int(0xD7))
            ('d', int(0xD8))
            ('e', int(0xD9))
            ('f', int(0xDA))
            ('g', int(0xDB))
            ('h', int(0xDC))
            ('i', int(0xDD))
            ('j', int(0xDE))
            ('k', int(0xDF))
            ('l', int(0xE0))
            ('m', int(0xE1))
            ('n', int(0xE2))
            ('o', int(0xE3))
            ('p', int(0xE4))
            ('q', int(0xE5))
            ('r', int(0xE6))
            ('s', int(0xE7))
            ('t', int(0xE8))
            ('u', int(0xE9))
            ('v', int(0xEA))
            ('w', int(0xEB))
            ('x', int(0xEC))
            ('y', int(0xED))
            ('z', int(0xEE))
            (' ', int(0xEF))
            ('\0', int(0xFF));
    }

    //To avoid C/C++ include messiness while avoiding bringing in pokehack/SaveParser.h
    char* converter::pokehack_get_text(unsigned char* raw, bool is_nickname)
    {
        char* actual_text;
        int len;

        if(is_nickname)
        {
            actual_text = new char[MAX_NICKNAME_LEN];
            len = MAX_NICKNAME_LEN;
        }
        else
        {
            actual_text = new char[MAX_TRAINER_NAME_LEN];
            len = MAX_TRAINER_NAME_LEN;
        }

        for(int i = 0; i < len; i++)
        {
            if(int(raw[i]) != 255) actual_text[i] = text[int(raw[i])];
            else actual_text[i] = '\0';
        }

        return actual_text;
    }

    spec_pkmn::sptr converter::pokehack_pkmn_to_spec_pkmn(belt_pokemon_t* b_pkmn_t,
                                                          pokemon_attacks_t* pkmn_a_t,
                                                          pokemon_effort_t* pkmn_e_t,
                                                          pokemon_misc_t* pkmn_m_t,
                                                          pokemon_growth_t* pkmn_g_t)
    {
        string identifier, move1, move2, move3, move4;
        int level, gender, from_game;

        identifier = pokemon_species[pkmn_g_t->species];
        level = b_pkmn_t->level;

        move1 = database::to_database_format(attacks[pkmn_a_t->atk1]);
        if(string(attacks[pkmn_a_t->atk2]) == "No attack") move2 = "None";
        else move2 = database::to_database_format(attacks[pkmn_a_t->atk2]);
        if(string(attacks[pkmn_a_t->atk3]) == "No attack") move3 = "None";
        else move3 = database::to_database_format(attacks[pkmn_a_t->atk3]);
        if(string(attacks[pkmn_a_t->atk4]) == "No attack") move4 = "None";
        else move4 = database::to_database_format(attacks[pkmn_a_t->atk4]);

        switch(pkmn_m_t->gamemsbs)
        {
            case 0: //Pokemon Colosseum Bonus Disk
                from_game = Games::COLOSSEUM;
                break;
            case 1:
                from_game = Games::SAPPHIRE;
                break;
            case 2:
                from_game = Games::RUBY;
                break;
            case 3:
                from_game = Games::EMERALD;
                break;
            case 4:
                from_game = Games::FIRE_RED;
                break;
            case 5:
                from_game = Games::LEAF_GREEN;
                break;
            case 15: //Colosseum or XD
                from_game = Games::XD;
                break;
            default:
                from_game = Games::RUBY;
                break;
        }
        
        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, from_game, level,
                                                 move1, move2, move3, move4);

        s_pkmn->nickname = pokehack_get_text(b_pkmn_t->name, true);
        s_pkmn->held_item = items[pkmn_g_t->held];

        string nature_from_pokehack = natures[b_pkmn_t->personality % 25];
        vector<string> nature_halves;
        boost::split(nature_halves, nature_from_pokehack, boost::is_any_of(" "));
        s_pkmn->nature = pkmn_nature::make(nature_halves[0]);
        s_pkmn->pid = b_pkmn_t->personality;
        s_pkmn->tid = ((unsigned short*)(&b_pkmn_t->otid))[0];
        s_pkmn->sid = ((unsigned short*)(&b_pkmn_t->otid))[1];

        //Gender
        int gender_from_pokehack = b_pkmn_t->personality % 256;
        double chance_male = s_pkmn->get_base_pkmn()->get_chance_male();
        double chance_female = s_pkmn->get_base_pkmn()->get_chance_female();
        if(chance_male == 0.0 and chance_female == 0.0) gender = Genders::GENDERLESS;
        else if(chance_male == 0.0) gender = Genders::FEMALE;
        else if(chance_female == 0.0) gender = Genders::MALE;
        else
        {
            if(gender_from_pokehack > int(floor(255*(1-chance_male)))) gender = Genders::MALE;
            else gender = Genders::FEMALE;
        }

        s_pkmn->HP = b_pkmn_t->maxHP;
        s_pkmn->ATK = b_pkmn_t->attack;
        s_pkmn->DEF = b_pkmn_t->defense;
        s_pkmn->SATK = b_pkmn_t->spatk;
        s_pkmn->SDEF = b_pkmn_t->spdef;
        s_pkmn->SPD = b_pkmn_t->speed;

        s_pkmn->ivHP = pkmn_m_t->IVs.hp;
        s_pkmn->ivATK = pkmn_m_t->IVs.atk;
        s_pkmn->ivDEF = pkmn_m_t->IVs.def;
        s_pkmn->ivSATK = pkmn_m_t->IVs.spatk;
        s_pkmn->ivSDEF = pkmn_m_t->IVs.spdef;
        s_pkmn->ivSPD = pkmn_m_t->IVs.spd;

        s_pkmn->evHP = pkmn_e_t->hp;
        s_pkmn->evATK = pkmn_e_t->attack;
        s_pkmn->evDEF = pkmn_e_t->defense;
        s_pkmn->evSATK = pkmn_e_t->spatk;
        s_pkmn->evSDEF = pkmn_e_t->spdef;
        s_pkmn->evSPD = pkmn_e_t->speed;

        //Attributes
        s_pkmn->attributes["friendship"] = pkmn_g_t->happiness;
        s_pkmn->attributes["circle"] = b_pkmn_t->mark.circle;
        s_pkmn->attributes["triangle"] = b_pkmn_t->mark.triangle;
        s_pkmn->attributes["square"] = b_pkmn_t->mark.square;
        s_pkmn->attributes["heart"] = b_pkmn_t->mark.heart;
        s_pkmn->attributes["country"] = b_pkmn_t->language;
        s_pkmn->attributes["cool"] = pkmn_e_t->coolness;
        s_pkmn->attributes["beauty"] = pkmn_e_t->beauty;
        s_pkmn->attributes["cute"] = pkmn_e_t->cuteness;
        s_pkmn->attributes["smart"] = pkmn_e_t->smartness;
        s_pkmn->attributes["tough"] = pkmn_e_t->toughness;
        s_pkmn->attributes["hoenn_cool_ribbon"] = pkmn_m_t->ribbons.coolnormal;
        s_pkmn->attributes["hoenn_cool_ribbon_super"] = pkmn_m_t->ribbons.coolsuper;
        s_pkmn->attributes["hoenn_cool_ribbon_hyper"] = pkmn_m_t->ribbons.coolhyper;
        s_pkmn->attributes["hoenn_cool_ribbon_master"] = pkmn_m_t->ribbons.coolmaster;
        s_pkmn->attributes["hoenn_beauty_ribbon"] = pkmn_m_t->ribbons.beautynormal;
        s_pkmn->attributes["hoenn_beauty_ribbon_super"] = pkmn_m_t->ribbons.beautysuper;
        s_pkmn->attributes["hoenn_beauty_ribbon_hyper"] = pkmn_m_t->ribbons.beautyhyper;
        s_pkmn->attributes["hoenn_beauty_ribbon_master"] = pkmn_m_t->ribbons.beautymaster;
        s_pkmn->attributes["hoenn_cute_ribbon"] = pkmn_m_t->ribbons.cutenormal;
        s_pkmn->attributes["hoenn_cute_ribbon_super"] = pkmn_m_t->ribbons.cutesuper;
        s_pkmn->attributes["hoenn_cute_ribbon_hyper"] = pkmn_m_t->ribbons.cutehyper;
        s_pkmn->attributes["hoenn_cute_ribbon_master"] = pkmn_m_t->ribbons.cutemaster;
        s_pkmn->attributes["hoenn_smart_ribbon"] = pkmn_m_t->ribbons.smartnormal;
        s_pkmn->attributes["hoenn_smart_ribbon_super"] = pkmn_m_t->ribbons.smartsuper;
        s_pkmn->attributes["hoenn_smart_ribbon_hyper"] = pkmn_m_t->ribbons.smarthyper;
        s_pkmn->attributes["hoenn_smart_ribbon_master"] = pkmn_m_t->ribbons.smartmaster;
        s_pkmn->attributes["hoenn_tough_ribbon"] = pkmn_m_t->ribbons.toughnormal;
        s_pkmn->attributes["hoenn_tough_ribbon_super"] = pkmn_m_t->ribbons.toughsuper;
        s_pkmn->attributes["hoenn_tough_ribbon_hyper"] = pkmn_m_t->ribbons.toughhyper;
        s_pkmn->attributes["hoenn_tough_ribbon_master"] = pkmn_m_t->ribbons.toughmaster;
        s_pkmn->attributes["hoenn_champion_ribbon"] = pkmn_m_t->ribbons.champion;
        s_pkmn->attributes["hoenn_winning_ribbon"] = pkmn_m_t->ribbons.winning;
        s_pkmn->attributes["hoenn_victory_ribbon"] = pkmn_m_t->ribbons.victory;
        s_pkmn->attributes["hoenn_artist_ribbon"] = pkmn_m_t->ribbons.artist;
        s_pkmn->attributes["hoenn_effort_ribbon"] = pkmn_m_t->ribbons.effort;
        s_pkmn->attributes["hoenn_marine_ribbon"] = pkmn_m_t->ribbons.marine;
        s_pkmn->attributes["hoenn_land_ribbon"] = pkmn_m_t->ribbons.land;
        s_pkmn->attributes["hoenn_sky_ribbon"] = pkmn_m_t->ribbons.sky;
        s_pkmn->attributes["hoenn_country_ribbon"] = pkmn_m_t->ribbons.country;
        s_pkmn->attributes["hoenn_national_ribbon"] = pkmn_m_t->ribbons.national;
        s_pkmn->attributes["hoenn_earth_ribbon"] = pkmn_m_t->ribbons.earth;
        s_pkmn->attributes["hoenn_world_ribbon"] = pkmn_m_t->ribbons.world;

        return s_pkmn;
    }

    void converter::spec_pkmn_to_pokehack_pkmn(spec_pkmn::sptr s_pkmn,
                                               belt_pokemon_t* b_pkmn_t,
                                               pokemon_attacks_t* pkmn_a_t,
                                               pokemon_effort_t* pkmn_e_t,
                                               pokemon_misc_t* pkmn_m_t,
                                               pokemon_growth_t* pkmn_g_t)
    {
        //Species position needs to be manually set for Nidoran's due to gender character
        if(s_pkmn->base->get_nat_pokedex_num() == 29) pkmn_g_t->species = 29; //Nidoran F
        if(s_pkmn->base->get_nat_pokedex_num() == 32) pkmn_g_t->species = 32; //Nidoran M
        else pkmn_g_t->species = distance(pokemon_species, (find(pokemon_species, pokemon_species+439, s_pkmn->get_species_name().c_str())));

        b_pkmn_t->personality = s_pkmn->pid;
        unsigned short otid[2] = {s_pkmn->tid, s_pkmn->sid};
        b_pkmn_t->otid = *((unsigned int*)(&otid[0]));
        b_pkmn_t->level = s_pkmn->level;
        for(int i = 0; i < 10; i++)
        {
            if(i < s_pkmn->nickname.size()) b_pkmn_t->name[i] = pokehack_reverse_char_map[s_pkmn->nickname[i]];
            else b_pkmn_t->name[i] = 0xFF;
        }
        pkmn_a_t->atk1 = s_pkmn->moves[0]->get_move_id();
        pkmn_a_t->atk2 = s_pkmn->moves[1]->get_move_id();
        pkmn_a_t->atk3 = s_pkmn->moves[2]->get_move_id();
        pkmn_a_t->atk4 = s_pkmn->moves[3]->get_move_id();
        pkmn_g_t->held = distance(items, (find(items, items+376, s_pkmn->held_item.c_str())));

        b_pkmn_t->maxHP = s_pkmn->HP;
        b_pkmn_t->attack = s_pkmn->ATK;
        b_pkmn_t->defense = s_pkmn->DEF;
        b_pkmn_t->spatk = s_pkmn->SATK;
        b_pkmn_t->spdef = s_pkmn->SDEF;
        b_pkmn_t->speed = s_pkmn->SPD;

        pkmn_m_t->IVs.hp = s_pkmn->ivHP;
        pkmn_m_t->IVs.atk = s_pkmn->ivATK;
        pkmn_m_t->IVs.def = s_pkmn->ivDEF;
        pkmn_m_t->IVs.spatk = s_pkmn->ivSATK;
        pkmn_m_t->IVs.spdef = s_pkmn->ivSDEF;
        pkmn_m_t->IVs.spd = s_pkmn->ivSPD;

        pkmn_e_t->hp = s_pkmn->HP;
        pkmn_e_t->attack = s_pkmn->ATK;
        pkmn_e_t->defense = s_pkmn->DEF;
        pkmn_e_t->spatk = s_pkmn->SATK;
        pkmn_e_t->spdef = s_pkmn->SDEF;
        pkmn_e_t->speed = s_pkmn->SPD;

        switch(s_pkmn->from_game)
        {
            case Games::RUBY:
                pkmn_m_t->gamemsbs = 2;
                break;
            case Games::SAPPHIRE:
                pkmn_m_t->gamemsbs = 1;
                break;
            case Games::EMERALD:
                pkmn_m_t->gamemsbs = 3;
                break;
            case Games::FIRE_RED:
                pkmn_m_t->gamemsbs = 4;
                break;
            case Games::LEAF_GREEN:
                pkmn_m_t->gamemsbs = 5;
                break;
            case Games::COLOSSEUM:
            case Games::XD:
                pkmn_m_t->gamemsbs = 15;
                break;
            default:
                pkmn_m_t->gamemsbs = 2;
        }
        
        //Attributes
        if(s_pkmn->attributes.has_key("friendship"))
            pkmn_g_t->happiness = s_pkmn->attributes["friendship"];
        if(s_pkmn->attributes.has_key("circle"))
            b_pkmn_t->mark.circle = s_pkmn->attributes["circle"];
        if(s_pkmn->attributes.has_key("triangle"))
            b_pkmn_t->mark.triangle = s_pkmn->attributes["triangle"];
        if(s_pkmn->attributes.has_key("square"))
            b_pkmn_t->mark.square = s_pkmn->attributes["square"];
        if(s_pkmn->attributes.has_key("heart"))
            b_pkmn_t->mark.heart = s_pkmn->attributes["heart"];
        if(s_pkmn->attributes.has_key("country"))
            b_pkmn_t->language = s_pkmn->attributes["country"];
        if(s_pkmn->attributes.has_key("cool"))
            pkmn_e_t->coolness = s_pkmn->attributes["coolness"];
        if(s_pkmn->attributes.has_key("beauty"))
            pkmn_e_t->beauty = s_pkmn->attributes["beauty"];
        if(s_pkmn->attributes.has_key("cute"))
            pkmn_e_t->cuteness = s_pkmn->attributes["cute"];
        if(s_pkmn->attributes.has_key("smart"))
            pkmn_e_t->smartness = s_pkmn->attributes["smart"];
        if(s_pkmn->attributes.has_key("tough"))
            pkmn_e_t->toughness = s_pkmn->attributes["tough"];
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon"))
            pkmn_m_t->ribbons.coolnormal = s_pkmn->attributes.has_key("hoenn_cool_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_super"))
            pkmn_m_t->ribbons.coolsuper = s_pkmn->attributes.has_key("hoenn_cool_ribbon_super");
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_hyper"))
            pkmn_m_t->ribbons.coolhyper = s_pkmn->attributes.has_key("hoenn_cool_ribbon_hyper");
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_master"))
            pkmn_m_t->ribbons.coolmaster = s_pkmn->attributes.has_key("hoenn_cool_ribbon_master");
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon"))
            pkmn_m_t->ribbons.beautynormal = s_pkmn->attributes.has_key("hoenn_beauty_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_super"))
            pkmn_m_t->ribbons.beautysuper = s_pkmn->attributes.has_key("hoenn_beauty_ribbon_super");
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_hyper"))
            pkmn_m_t->ribbons.beautyhyper = s_pkmn->attributes.has_key("hoenn_beauty_ribbon_hyper");
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_master"))
            pkmn_m_t->ribbons.beautymaster = s_pkmn->attributes.has_key("hoenn_beauty_ribbon_master");
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon"))
            pkmn_m_t->ribbons.cutenormal = s_pkmn->attributes.has_key("hoenn_cute_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_super"))
            pkmn_m_t->ribbons.cutesuper = s_pkmn->attributes.has_key("hoenn_cute_ribbon_super");
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_hyper"))
            pkmn_m_t->ribbons.cutehyper = s_pkmn->attributes.has_key("hoenn_cute_ribbon_hyper");
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_master"))
            pkmn_m_t->ribbons.cutemaster = s_pkmn->attributes.has_key("hoenn_cute_ribbon_master");
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon"))
            pkmn_m_t->ribbons.smartnormal = s_pkmn->attributes.has_key("hoenn_smart_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_super"))
            pkmn_m_t->ribbons.smartsuper = s_pkmn->attributes.has_key("hoenn_smart_ribbon_super");
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_hyper"))
            pkmn_m_t->ribbons.smarthyper = s_pkmn->attributes.has_key("hoenn_smart_ribbon_hyper");
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_master"))
            pkmn_m_t->ribbons.smartmaster = s_pkmn->attributes.has_key("hoenn_smart_ribbon_master");
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon"))
            pkmn_m_t->ribbons.toughnormal = s_pkmn->attributes.has_key("hoenn_tough_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_super"))
            pkmn_m_t->ribbons.toughsuper = s_pkmn->attributes.has_key("hoenn_tough_ribbon_super");
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_hyper"))
            pkmn_m_t->ribbons.toughhyper = s_pkmn->attributes.has_key("hoenn_tough_ribbon_hyper");
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_master"))
            pkmn_m_t->ribbons.toughmaster = s_pkmn->attributes.has_key("hoenn_tough_ribbon_master");
        if(s_pkmn->attributes.has_key("hoenn_champion_ribbon"))
            pkmn_m_t->ribbons.champion = s_pkmn->attributes.has_key("hoenn_champion_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_winning_ribbon"))
            pkmn_m_t->ribbons.winning = s_pkmn->attributes.has_key("hoenn_winning_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_victory_ribbon"))
            pkmn_m_t->ribbons.victory = s_pkmn->attributes.has_key("hoenn_victory_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_artist_ribbon"))
            pkmn_m_t->ribbons.artist = s_pkmn->attributes.has_key("hoenn_artist_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_effort_ribbon"))
            pkmn_m_t->ribbons.effort = s_pkmn->attributes.has_key("hoenn_effort_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_marine_ribbon"))
            pkmn_m_t->ribbons.marine = s_pkmn->attributes.has_key("hoenn_marine_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_land_ribbon"))
            pkmn_m_t->ribbons.land = s_pkmn->attributes.has_key("hoenn_land_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_sky_ribbon"))
            pkmn_m_t->ribbons.sky = s_pkmn->attributes.has_key("hoenn_sky_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_country_ribbon"))
            pkmn_m_t->ribbons.country = s_pkmn->attributes.has_key("hoenn_country_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_national_ribbon"))
            pkmn_m_t->ribbons.national = s_pkmn->attributes.has_key("hoenn_national_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_earth_ribbon"))
            pkmn_m_t->ribbons.earth = s_pkmn->attributes.has_key("hoenn_earth_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_world_ribbon"))
            pkmn_m_t->ribbons.world = s_pkmn->attributes.has_key("hoenn_world_ribbon");
    }

    spec_pkmn::sptr converter::pokelib_pkmn_to_spec_pkmn(PokeLib::Pokemon pokelib_pkmn)
    {
        string identifier, move1, move2, move3, move4;
        int level, gender, from_game;

        identifier = PokeLib::species[int(pokelib_pkmn.pkm->pkm.species)];
        level = int(pokelib_pkmn.getLevel());

        move1 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[0])]);
        move2 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[1])]);
        move3 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[2])]);
        move4 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[3])]);

        switch(pokelib_pkmn.pkm->pkm.hometown)
        {
            case 1:
                from_game = Games::SAPPHIRE;
                break;
            case 2:
                from_game = Games::RUBY;
                break;
            case 3:
                from_game = Games::EMERALD;
                break;
            case 4:
                from_game = Games::FIRE_RED;
                break;
            case 5:
                from_game = Games::LEAF_GREEN;
                break;
            case 7:
                from_game = Games::HEART_GOLD;
                break;
            case 8:
                from_game = Games::SOUL_SILVER;
                break;
            case 10:
                from_game = Games::DIAMOND;
                break;
            case 11:
                from_game = Games::PEARL;
                break;
            case 12:
                from_game = Games::PLATINUM;
                break;
            case 15:
                from_game = Games::COLOSSEUM;
                break;
            default:
                from_game = Games::DIAMOND;
                break;
        }
        
        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, from_game, level, move1, move2, move3, move4);

        PokeLib::widetext nickname_wide = pokelib_pkmn.getNickname();
        char nickname_buffer[10];
        memset(nickname_buffer,0,10);
        wcstombs(nickname_buffer, nickname_wide.c_str(), 10);
        s_pkmn->nickname = nickname_buffer;
        s_pkmn->held_item = PokeLib::items[pokelib_pkmn.pkm->pkm.held_item];
        s_pkmn->nature = pkmn_nature::make(PokeLib::nature[int(pokelib_pkmn.getNatureValue())]);
        s_pkmn->pid = pokelib_pkmn.pkm->pkm.pid;
        s_pkmn->tid = pokelib_pkmn.pkm->pkm.ot_id;
        s_pkmn->sid = pokelib_pkmn.pkm->pkm.ot_sid;
        switch(int(pokelib_pkmn.getGenderValue()))
        {
            case PokeLib::MALE:
                gender = Genders::MALE;
                break;

            case PokeLib::FEMALE:
                gender = Genders::FEMALE;
                break;

            default:
                gender = Genders::GENDERLESS;
                break;
        }

        s_pkmn->HP = pokelib_pkmn.pkm->pkm.battle_max_hp;
        s_pkmn->ATK = pokelib_pkmn.pkm->pkm.battle_atk;
        s_pkmn->DEF = pokelib_pkmn.pkm->pkm.battle_def;
        s_pkmn->SATK = pokelib_pkmn.pkm->pkm.battle_satk;
        s_pkmn->SDEF = pokelib_pkmn.pkm->pkm.battle_sdef;
        s_pkmn->SPD = pokelib_pkmn.pkm->pkm.battle_spd;

        s_pkmn->ivHP = pokelib_pkmn.pkm->pkm.iv_hp;
        s_pkmn->ivATK = pokelib_pkmn.pkm->pkm.iv_atk;
        s_pkmn->ivDEF = pokelib_pkmn.pkm->pkm.iv_def;
        s_pkmn->ivSATK = pokelib_pkmn.pkm->pkm.iv_satk;
        s_pkmn->ivSDEF = pokelib_pkmn.pkm->pkm.iv_sdef;
        s_pkmn->ivSPD = pokelib_pkmn.pkm->pkm.iv_spd;

        s_pkmn->evHP = pokelib_pkmn.pkm->pkm.ev_hp;
        s_pkmn->evATK = pokelib_pkmn.pkm->pkm.ev_atk;
        s_pkmn->evDEF = pokelib_pkmn.pkm->pkm.ev_def;
        s_pkmn->evSATK = pokelib_pkmn.pkm->pkm.ev_satk;
        s_pkmn->evSDEF = pokelib_pkmn.pkm->pkm.ev_sdef;
        s_pkmn->evSPD = pokelib_pkmn.pkm->pkm.ev_spd;

        //Attributes
        s_pkmn->attributes["friendship"] = pokelib_pkmn.pkm->pkm.friendship;

        //Need to get markings manually, no bitfield in PokeLib
        bitset<8> markings_bitset(pokelib_pkmn.pkm->pkm.markings);
        s_pkmn->attributes["circle"] = int(markings_bitset[0]);
        s_pkmn->attributes["triangle"] = int(markings_bitset[1]);
        s_pkmn->attributes["square"] = int(markings_bitset[2]);
        s_pkmn->attributes["heart"] = int(markings_bitset[3]);
        s_pkmn->attributes["star"] = int(markings_bitset[4]);
        s_pkmn->attributes["diamond"] = int(markings_bitset[5]);

        s_pkmn->attributes["country"] = pokelib_pkmn.pkm->pkm.country;
        s_pkmn->attributes["cool"] = pokelib_pkmn.pkm->pkm.contest_cool;
        s_pkmn->attributes["beauty"] = pokelib_pkmn.pkm->pkm.contest_beauty;
        s_pkmn->attributes["cute"] = pokelib_pkmn.pkm->pkm.contest_cute;
        s_pkmn->attributes["smart"] = pokelib_pkmn.pkm->pkm.contest_smart;
        s_pkmn->attributes["tough"] = pokelib_pkmn.pkm->pkm.contest_tough;
        s_pkmn->attributes["sheen"] = pokelib_pkmn.pkm->pkm.contest_sheen;
        s_pkmn->attributes["sinnoh_champ_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.sinnoh_champ;
        s_pkmn->attributes["sinnoh_ability_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.ability;
        s_pkmn->attributes["sinnoh_great_ability_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.great_ability;
        s_pkmn->attributes["sinnoh_double_ability_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.double_ability;
        s_pkmn->attributes["sinnoh_multi_ability_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.multi_ability;
        s_pkmn->attributes["sinnoh_pair_ability_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.pair_ability;
        s_pkmn->attributes["sinnoh_world_ability_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.world_ability;
        s_pkmn->attributes["sinnoh_alert_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.alert;
        s_pkmn->attributes["sinnoh_shock_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.shock;
        s_pkmn->attributes["sinnoh_downcast_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.downcast;
        s_pkmn->attributes["sinnoh_careless_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.careless;
        s_pkmn->attributes["sinnoh_relax_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.relax;
        s_pkmn->attributes["sinnoh_snooze_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.ability;
        s_pkmn->attributes["sinnoh_smile_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.smile;
        s_pkmn->attributes["sinnoh_gorgeous_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.gorgeous;
        s_pkmn->attributes["sinnoh_royal_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.royal;
        s_pkmn->attributes["sinnoh_gorgeous_royal_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.gorgeous_royal;
        s_pkmn->attributes["sinnoh_footprint_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.footprint;
        s_pkmn->attributes["sinnoh_record_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.record;
        s_pkmn->attributes["sinnoh_history_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.history;
        s_pkmn->attributes["sinnoh_legend_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.legend;
        s_pkmn->attributes["sinnoh_red_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.red;
        s_pkmn->attributes["sinnoh_green_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.green;
        s_pkmn->attributes["sinnoh_blue_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.blue;
        s_pkmn->attributes["sinnoh_festival_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.festival;
        s_pkmn->attributes["sinnoh_carnival_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.carnival;
        s_pkmn->attributes["sinnoh_classic_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.classic;
        s_pkmn->attributes["sinnoh_premier_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.premier;
        s_pkmn->attributes["hoenn_cool_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_normal;
        s_pkmn->attributes["hoenn_cool_ribbon_super"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_super;
        s_pkmn->attributes["hoenn_cool_ribbon_hyper"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_hyper;
        s_pkmn->attributes["hoenn_cool_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_master;
        s_pkmn->attributes["hoenn_beauty_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_normal;
        s_pkmn->attributes["hoenn_beauty_ribbon_super"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_super;
        s_pkmn->attributes["hoenn_beauty_ribbon_hyper"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_hyper;
        s_pkmn->attributes["hoenn_cute_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_master;
        s_pkmn->attributes["hoenn_cute_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_normal;
        s_pkmn->attributes["hoenn_cute_ribbon_super"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_super;
        s_pkmn->attributes["hoenn_cute_ribbon_hyper"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_hyper;
        s_pkmn->attributes["hoenn_cute_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_master;
        s_pkmn->attributes["hoenn_smart_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_normal;
        s_pkmn->attributes["hoenn_smart_ribbon_super"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_super;
        s_pkmn->attributes["hoenn_smart_ribbon_hyper"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_hyper;
        s_pkmn->attributes["hoenn_smart_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_master;
        s_pkmn->attributes["hoenn_tough_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_normal;
        s_pkmn->attributes["hoenn_tough_ribbon_super"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_super;
        s_pkmn->attributes["hoenn_tough_ribbon_hyper"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_hyper;
        s_pkmn->attributes["hoenn_tough_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_master;
        s_pkmn->attributes["hoenn_champion_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.champion;
        s_pkmn->attributes["hoenn_winning_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.winning;
        s_pkmn->attributes["hoenn_victory_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.victory;
        s_pkmn->attributes["hoenn_artist_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.artist;
        s_pkmn->attributes["hoenn_effort_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.effort;
        s_pkmn->attributes["hoenn_marine_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.marine;
        s_pkmn->attributes["hoenn_land_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.land;
        s_pkmn->attributes["hoenn_sky_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.sky;
        s_pkmn->attributes["hoenn_country_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.country;
        s_pkmn->attributes["hoenn_national_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.national;
        s_pkmn->attributes["hoenn_earth_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.earth;
        s_pkmn->attributes["hoenn_world_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.world;
        s_pkmn->attributes["sinnoh_encounter"] = pokelib_pkmn.pkm->pkm.encounterType;
        s_pkmn->attributes["original_game"] = pokelib_pkmn.pkm->pkm.hometown;
        s_pkmn->attributes["sinnoh_cool_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_normal;
        s_pkmn->attributes["sinnoh_cool_ribbon_great"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_super;
        s_pkmn->attributes["sinnoh_cool_ribbon_ultra"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_hyper;
        s_pkmn->attributes["sinnoh_cool_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_master;
        s_pkmn->attributes["sinnoh_beauty_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_normal;
        s_pkmn->attributes["sinnoh_beauty_ribbon_great"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_super;
        s_pkmn->attributes["sinnoh_beauty_ribbon_ultra"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_hyper;
        s_pkmn->attributes["sinnoh_cute_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_master;
        s_pkmn->attributes["sinnoh_cute_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_normal;
        s_pkmn->attributes["sinnoh_cute_ribbon_great"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_super;
        s_pkmn->attributes["sinnoh_cute_ribbon_ultra"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_hyper;
        s_pkmn->attributes["sinnoh_cute_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_master;
        s_pkmn->attributes["sinnoh_smart_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_normal;
        s_pkmn->attributes["sinnoh_smart_ribbon_great"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_super;
        s_pkmn->attributes["sinnoh_smart_ribbon_ultra"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_hyper;
        s_pkmn->attributes["sinnoh_smart_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_master;
        s_pkmn->attributes["sinnoh_tough_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_normal;
        s_pkmn->attributes["sinnoh_tough_ribbon_great"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_super;
        s_pkmn->attributes["sinnoh_tough_ribbon_ultra"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_hyper;
        s_pkmn->attributes["sinnoh_tough_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_master;
        s_pkmn->attributes["eggmet_year"] = pokelib_pkmn.pkm->pkm.eggDate[0];
        s_pkmn->attributes["eggmet_month"] = pokelib_pkmn.pkm->pkm.eggDate[1];
        s_pkmn->attributes["eggmet_day"] = pokelib_pkmn.pkm->pkm.eggDate[2];
        s_pkmn->attributes["met_year"] = pokelib_pkmn.pkm->pkm.metDate[0];
        s_pkmn->attributes["met_month"] = pokelib_pkmn.pkm->pkm.metDate[1];
        s_pkmn->attributes["met_day"] = pokelib_pkmn.pkm->pkm.metDate[2];
        s_pkmn->attributes["met_level"] = pokelib_pkmn.pkm->pkm.metLevel;
        s_pkmn->attributes["ot_is_female"] = pokelib_pkmn.pkm->pkm.ot_female;

        return s_pkmn;
    }

    PokeLib::Pokemon converter::spec_pkmn_to_pokelib_pkmn(spec_pkmn::sptr s_pkmn)
    {
        PokeLib::Pokemon pokelib_pkmn;

        pokelib_pkmn.pkm->pkm.species = s_pkmn->get_species_id();
        pokelib_pkmn.setLevel(uint8_t(s_pkmn->level));
        pokelib_pkmn.setNickname(s_pkmn->nickname.c_str(), s_pkmn->nickname.size());
        pokelib_pkmn.pkm->pkm.pid = s_pkmn->pid;
        pokelib_pkmn.pkm->pkm.ot_id = s_pkmn->tid;
        pokelib_pkmn.pkm->pkm.ot_sid = s_pkmn->sid;

        pokelib_pkmn.pkm->pkm.move[0] = s_pkmn->moves[0]->get_move_id();
        pokelib_pkmn.pkm->pkm.move[1] = s_pkmn->moves[1]->get_move_id();
        pokelib_pkmn.pkm->pkm.move[2] = s_pkmn->moves[2]->get_move_id();
        pokelib_pkmn.pkm->pkm.move[3] = s_pkmn->moves[3]->get_move_id();

        pokelib_pkmn.pkm->pkm.battle_max_hp = s_pkmn->HP;
        pokelib_pkmn.pkm->pkm.battle_atk = s_pkmn->ATK;
        pokelib_pkmn.pkm->pkm.battle_def = s_pkmn->DEF;
        pokelib_pkmn.pkm->pkm.battle_satk = s_pkmn->SATK;
        pokelib_pkmn.pkm->pkm.battle_sdef = s_pkmn->SDEF;
        pokelib_pkmn.pkm->pkm.battle_spd = s_pkmn->SPD;

        pokelib_pkmn.pkm->pkm.iv_hp = s_pkmn->ivHP;
        pokelib_pkmn.pkm->pkm.iv_atk = s_pkmn->ivATK;
        pokelib_pkmn.pkm->pkm.iv_def = s_pkmn->ivDEF;
        pokelib_pkmn.pkm->pkm.iv_satk = s_pkmn->ivSATK;
        pokelib_pkmn.pkm->pkm.iv_sdef = s_pkmn->ivSDEF;
        pokelib_pkmn.pkm->pkm.iv_spd = s_pkmn->ivSPD;

        pokelib_pkmn.pkm->pkm.ev_hp = s_pkmn->evHP;
        pokelib_pkmn.pkm->pkm.ev_atk = s_pkmn->evATK;
        pokelib_pkmn.pkm->pkm.ev_def = s_pkmn->evDEF;
        pokelib_pkmn.pkm->pkm.ev_satk = s_pkmn->evSATK;
        pokelib_pkmn.pkm->pkm.ev_sdef = s_pkmn->evSDEF;
        pokelib_pkmn.pkm->pkm.ev_spd = s_pkmn->evSPD;

        switch(s_pkmn->from_game)
        {
            case Games::RUBY:
                pokelib_pkmn.pkm->pkm.hometown = 2;
                break;
            case Games::SAPPHIRE:
                pokelib_pkmn.pkm->pkm.hometown = 1;
                break;
            case Games::EMERALD:
                pokelib_pkmn.pkm->pkm.hometown = 3;
                break;
            case Games::FIRE_RED:
                pokelib_pkmn.pkm->pkm.hometown = 4;
                break;
            case Games::LEAF_GREEN:
                pokelib_pkmn.pkm->pkm.hometown = 5;
                break;
            case Games::COLOSSEUM:
            case Games::XD:
                pokelib_pkmn.pkm->pkm.hometown = 15;
                break;
            case Games::DIAMOND:
                pokelib_pkmn.pkm->pkm.hometown = 10;
                break;
            case Games::PEARL:
                pokelib_pkmn.pkm->pkm.hometown = 11;
                break;
            case Games::PLATINUM:
                pokelib_pkmn.pkm->pkm.hometown = 12;
                break;
            case Games::HEART_GOLD:
                pokelib_pkmn.pkm->pkm.hometown = 7;
                break;
            case Games::SOUL_SILVER:
                pokelib_pkmn.pkm->pkm.hometown = 8;
                break;
            default:
                pokelib_pkmn.pkm->pkm.hometown = 10;
                break;
        }
        
        //Attributes
        if(s_pkmn->attributes.has_key("friendship"))
            pokelib_pkmn.pkm->pkm.friendship = s_pkmn->attributes["friendship"];

        //Need to set markings manually, no bitfield in PokeLib
        bitset<8> markings_bitset(0);
        if(s_pkmn->attributes.has_key("circle"))
            markings_bitset[0] = s_pkmn->attributes["circle"];
        if(s_pkmn->attributes.has_key("triangle"))
            markings_bitset[1] = s_pkmn->attributes["triangle"];
        if(s_pkmn->attributes.has_key("square"))
            markings_bitset[2] = s_pkmn->attributes["square"];
        if(s_pkmn->attributes.has_key("heart"))
            markings_bitset[3] = s_pkmn->attributes["heart"];
        if(s_pkmn->attributes.has_key("star"))
            markings_bitset[4] = s_pkmn->attributes["star"];
        if(s_pkmn->attributes.has_key("diamond"))
            markings_bitset[5] = s_pkmn->attributes["diamond"];
        pokelib_pkmn.pkm->pkm.markings = markings_bitset.to_ulong();

        if(s_pkmn->attributes.has_key("country"))
            pokelib_pkmn.pkm->pkm.country = char(s_pkmn->attributes["country"]);
        if(s_pkmn->attributes.has_key("cool"))
            pokelib_pkmn.pkm->pkm.contest_cool = char(s_pkmn->attributes["cool"]);
        if(s_pkmn->attributes.has_key("beauty"))
            pokelib_pkmn.pkm->pkm.contest_beauty = char(s_pkmn->attributes["beauty"]);
        if(s_pkmn->attributes.has_key("cute"))
            pokelib_pkmn.pkm->pkm.contest_cute = char(s_pkmn->attributes["cute"]);
        if(s_pkmn->attributes.has_key("smart"))
            pokelib_pkmn.pkm->pkm.contest_smart = char(s_pkmn->attributes["smart"]);
        if(s_pkmn->attributes.has_key("tough"))
            pokelib_pkmn.pkm->pkm.contest_tough = char(s_pkmn->attributes["tough"]);
        if(s_pkmn->attributes.has_key("sheen"))
            pokelib_pkmn.pkm->pkm.contest_sheen = char(s_pkmn->attributes["sheen"]);
        if(s_pkmn->attributes.has_key("sinnoh_champ_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.sinnoh_champ = s_pkmn->attributes["sinnoh_champ_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_ability_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.ability = s_pkmn->attributes["sinnoh_ability_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_great_ability_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.great_ability = s_pkmn->attributes["sinnoh_great_ability_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_double_ability_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.double_ability = s_pkmn->attributes["sinnoh_double_ability_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_multi_ability_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.multi_ability = s_pkmn->attributes["sinnoh_multi_ability_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_pair_ability_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.pair_ability = s_pkmn->attributes["sinnoh_pair_ability_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_world_ability_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.world_ability = s_pkmn->attributes["sinnoh_world_ability_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_alert_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.alert = s_pkmn->attributes["sinnoh_alert_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_shock_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.shock = s_pkmn->attributes["sinnoh_shock_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_downcast_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.downcast = s_pkmn->attributes["sinnoh_downcast_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_careless_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.careless = s_pkmn->attributes["sinnoh_careless_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_relax_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.relax = s_pkmn->attributes["sinnoh_relax_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_snooze_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.snooze = s_pkmn->attributes["sinnoh_snooze_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_smile_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.smile = s_pkmn->attributes["sinnoh_smile_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_gorgeous_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.gorgeous = s_pkmn->attributes["sinnoh_gorgeous_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_royal_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.royal = s_pkmn->attributes["sinnoh_royal_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_gorgeous_royal_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.gorgeous_royal = s_pkmn->attributes["sinnoh_gorgeous_royal_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_footprint_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.footprint = s_pkmn->attributes["sinnoh_footprint_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_record_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.record = s_pkmn->attributes["sinnoh_record_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_history_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.history = s_pkmn->attributes["sinnoh_history_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_legend_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.legend = s_pkmn->attributes["sinnoh_legend_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_red_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.red = s_pkmn->attributes["sinnoh_red_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_green_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.green = s_pkmn->attributes["sinnoh_green_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_blue_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.blue = s_pkmn->attributes["sinnoh_blue_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_festival_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.festival = s_pkmn->attributes["sinnoh_festival_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_carnival_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.carnival = s_pkmn->attributes["sinnoh_carnival_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_classic_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.classic = s_pkmn->attributes["sinnoh_classic_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_premier_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.premier = s_pkmn->attributes["sinnoh_premier_ribbon"];
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_normal = s_pkmn->attributes["hoenn_cool_ribbon"];
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_super = s_pkmn->attributes["hoenn_cool_ribbon_super"];
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_hyper = s_pkmn->attributes["hoenn_cool_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_master = s_pkmn->attributes["hoenn_cool_ribbon_master"];
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_normal = s_pkmn->attributes["hoenn_beauty_ribbon"];
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_super = s_pkmn->attributes["hoenn_beauty_ribbon_super"];
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_hyper = s_pkmn->attributes["hoenn_beauty_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_master = s_pkmn->attributes["hoenn_beauty_ribbon_master"];
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_normal = s_pkmn->attributes["hoenn_cute_ribbon"];
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_super = s_pkmn->attributes["hoenn_cute_ribbon_super"];
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_hyper = s_pkmn->attributes["hoenn_cute_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_master = s_pkmn->attributes["hoenn_cute_ribbon_master"];
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_normal = s_pkmn->attributes["hoenn_smart_ribbon"];
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_super = s_pkmn->attributes["hoenn_smart_ribbon_super"];
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_hyper = s_pkmn->attributes["hoenn_smart_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_master = s_pkmn->attributes["hoenn_smart_ribbon_master"];
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_normal = s_pkmn->attributes["hoenn_tough_ribbon"];
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_super = s_pkmn->attributes["hoenn_tough_ribbon_super"];
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_hyper = s_pkmn->attributes["hoenn_tough_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_master = s_pkmn->attributes["hoenn_tough_ribbon_master"];
        if(s_pkmn->attributes.has_key("hoenn_champion_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.champion = s_pkmn->attributes["hoenn_champion_master"];
        if(s_pkmn->attributes.has_key("hoenn_winning_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.winning = s_pkmn->attributes["hoenn_winning_master"];
        if(s_pkmn->attributes.has_key("hoenn_victory_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.victory = s_pkmn->attributes["hoenn_victory_master"];
        if(s_pkmn->attributes.has_key("hoenn_artist_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.artist = s_pkmn->attributes["hoenn_artist_master"];
        if(s_pkmn->attributes.has_key("hoenn_effort_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.effort = s_pkmn->attributes["hoenn_effort_master"];
        if(s_pkmn->attributes.has_key("hoenn_marine_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.marine = s_pkmn->attributes["hoenn_marine_master"];
        if(s_pkmn->attributes.has_key("hoenn_land_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.land = s_pkmn->attributes["hoenn_land_master"];
        if(s_pkmn->attributes.has_key("hoenn_sky_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.sky = s_pkmn->attributes["hoenn_sky_master"];
        if(s_pkmn->attributes.has_key("hoenn_country_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.country = s_pkmn->attributes["hoenn_country_master"];
        if(s_pkmn->attributes.has_key("hoenn_national_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.national = s_pkmn->attributes["hoenn_national_master"];
        if(s_pkmn->attributes.has_key("hoenn_earth_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.earth = s_pkmn->attributes["hoenn_earth_master"];
        if(s_pkmn->attributes.has_key("hoenn_world_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.world = s_pkmn->attributes["hoenn_world_master"];
        if(s_pkmn->attributes.has_key("sinnoh_encounter"))
            pokelib_pkmn.pkm->pkm.encounterType = s_pkmn->attributes["sinnoh_encounter"];
        if(s_pkmn->attributes.has_key("original_game"))
            pokelib_pkmn.pkm->pkm.hometown = s_pkmn->attributes["original_game"];
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_normal = s_pkmn->attributes["sinnoh_cool_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_super = s_pkmn->attributes["sinnoh_cool_ribbon_super"];
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_hyper = s_pkmn->attributes["sinnoh_cool_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_master = s_pkmn->attributes["sinnoh_cool_ribbon_master"];
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_normal = s_pkmn->attributes["sinnoh_beauty_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_super = s_pkmn->attributes["sinnoh_beauty_ribbon_super"];
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_hyper = s_pkmn->attributes["sinnoh_beauty_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_master = s_pkmn->attributes["sinnoh_beauty_ribbon_master"];
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_normal = s_pkmn->attributes["sinnoh_cute_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_super = s_pkmn->attributes["sinnoh_cute_ribbon_super"];
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_hyper = s_pkmn->attributes["sinnoh_cute_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_master = s_pkmn->attributes["sinnoh_cute_ribbon_master"];
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_normal = s_pkmn->attributes["sinnoh_smart_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_super = s_pkmn->attributes["sinnoh_smart_ribbon_super"];
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_hyper = s_pkmn->attributes["sinnoh_smart_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_master = s_pkmn->attributes["sinnoh_smart_ribbon_master"];
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_normal = s_pkmn->attributes["sinnoh_tough_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_super = s_pkmn->attributes["sinnoh_tough_ribbon_super"];
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_hyper = s_pkmn->attributes["sinnoh_tough_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_master = s_pkmn->attributes["sinnoh_tough_ribbon_master"];
        if(s_pkmn->attributes.has_key("eggmet_year"))
            pokelib_pkmn.pkm->pkm.eggDate[0] = char(s_pkmn->attributes["eggmet_year"]);
        if(s_pkmn->attributes.has_key("eggmet_month"))
            pokelib_pkmn.pkm->pkm.eggDate[1] = char(s_pkmn->attributes["eggmet_month"]);
        if(s_pkmn->attributes.has_key("eggmet_day"))
            pokelib_pkmn.pkm->pkm.eggDate[2] = char(s_pkmn->attributes["eggmet_day"]);
        if(s_pkmn->attributes.has_key("met_year"))
            pokelib_pkmn.pkm->pkm.metDate[0] = char(s_pkmn->attributes["met_year"]);
        if(s_pkmn->attributes.has_key("met_month"))
            pokelib_pkmn.pkm->pkm.metDate[1] = char(s_pkmn->attributes["met_month"]);
        if(s_pkmn->attributes.has_key("met_day"))
            pokelib_pkmn.pkm->pkm.metDate[2] = char(s_pkmn->attributes["met_day"]);
        if(s_pkmn->attributes.has_key("met_level"))
            pokelib_pkmn.pkm->pkm.metLevel = char(s_pkmn->attributes["met_level"]);
        if(s_pkmn->attributes.has_key("ot_is_female"))
            pokelib_pkmn.pkm->pkm.ot_female = char(s_pkmn->attributes["ot_is_female"]);

        return pokelib_pkmn;
    }

    spec_pkmn::sptr converter::pkmds_pkmn_to_spec_pkmn(party_pkm* p_pkm)
    {
        opendb(get_database_path().c_str());

        string identifier, move1, move2, move3, move4;
        int level, from_game;

        identifier = database::to_database_format(lookuppkmname(p_pkm->pkm_data));
        level = getpkmlevel(p_pkm->pkm_data);

        move1 = lookupmovename(p_pkm->pkm_data.moves[0]);
        if(p_pkm->pkm_data.moves[1] == Moves::NOTHING) move2 = "None";
        else move2 = lookupmovename(p_pkm->pkm_data.moves[1]);
        if(p_pkm->pkm_data.moves[2] == Moves::NOTHING) move3 = "None";
        else move3 = lookupmovename(p_pkm->pkm_data.moves[2]);
        if(p_pkm->pkm_data.moves[3] == Moves::NOTHING) move4 = "None";
        else move4 = lookupmovename(p_pkm->pkm_data.moves[3]);

        switch(int(p_pkm->pkm_data.hometown))
        {
            case int(Hometowns::ruby):
                from_game = Games::RUBY;
                break;
            case int(Hometowns::sapphire):
                from_game = Games::SAPPHIRE;
                break;
            case int(Hometowns::emerald):
                from_game = Games::EMERALD;
                break;
            case int(Hometowns::firered):
                from_game = Games::FIRE_RED;
                break;
            case int(Hometowns::leafgreen):
                from_game = Games::LEAF_GREEN;
                break;
            case int(Hometowns::colosseum):
                from_game = Games::COLOSSEUM;
                break;
            case int(Hometowns::diamond):
                from_game = Games::DIAMOND;
                break;
            case int(Hometowns::pearl):
                from_game = Games::PEARL;
                break;
            case int(Hometowns::platinum):
                from_game = Games::PLATINUM;
                break;
            case int(Hometowns::heartgold):
                from_game = Games::HEART_GOLD;
                break;
            case int(Hometowns::soulsilver):
                from_game = Games::SOUL_SILVER;
                break;
            case int(Hometowns::black):
                from_game = Games::BLACK;
                break;
            case int(Hometowns::white):
                from_game = Games::WHITE;
                break;
            case int(Hometowns::black2):
                from_game = Games::BLACK2;
                break;
            case int(Hometowns::white2):
                from_game = Games::WHITE2;
                break;
            default:
                from_game = Games::BLACK;
                break;
        }
        
        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, 5, level, move1, move2, move3, move4);

        wstring nickname_wide = getpkmnickname(p_pkm->pkm_data);
        char nickname_buffer[11];
        memset(nickname_buffer,0,11);
        wcstombs(nickname_buffer, nickname_wide.c_str(), 11);
        s_pkmn->nickname = nickname_buffer;

        if(p_pkm->pkm_data.item == Items::NOTHING) s_pkmn->held_item = "None";
        else s_pkmn->held_item = lookupitemname(p_pkm->pkm_data);
        s_pkmn->nature = pkmn_nature::make(getnaturename(p_pkm->pkm_data));
        s_pkmn->pid = p_pkm->pkm_data.pid;
        s_pkmn->sid = p_pkm->pkm_data.sid;
        s_pkmn->tid = p_pkm->pkm_data.tid;
        switch(int(getpkmgender(p_pkm->pkm_data)))
        {
            case ::Genders::male:
                s_pkmn->gender = Genders::MALE;
                break;

            case ::Genders::female:
                s_pkmn->gender = Genders::FEMALE;
                break;

            default:
                s_pkmn->gender = Genders::GENDERLESS;
                break;
        }

        //Stats
        s_pkmn->HP = getpkmstat(p_pkm->pkm_data, Stat_IDs::hp);
        s_pkmn->ATK = getpkmstat(p_pkm->pkm_data, Stat_IDs::attack);
        s_pkmn->DEF = getpkmstat(p_pkm->pkm_data, Stat_IDs::defense);
        s_pkmn->SATK = getpkmstat(p_pkm->pkm_data, Stat_IDs::spatk);
        s_pkmn->SDEF = getpkmstat(p_pkm->pkm_data, Stat_IDs::spdef);
        s_pkmn->SPD = getpkmstat(p_pkm->pkm_data, Stat_IDs::speed);

        //IV's
        s_pkmn->ivHP = p_pkm->pkm_data.ivs.hp;
        s_pkmn->ivATK = p_pkm->pkm_data.ivs.attack;
        s_pkmn->ivDEF = p_pkm->pkm_data.ivs.defense;
        s_pkmn->ivSATK = p_pkm->pkm_data.ivs.spatk;
        s_pkmn->ivSDEF = p_pkm->pkm_data.ivs.spdef;
        s_pkmn->ivSPD = p_pkm->pkm_data.ivs.speed;

        //EV's
        s_pkmn->evHP = p_pkm->pkm_data.evs.hp;
        s_pkmn->evATK = p_pkm->pkm_data.evs.attack;
        s_pkmn->evDEF = p_pkm->pkm_data.evs.defense;
        s_pkmn->evSATK = p_pkm->pkm_data.evs.spatk;
        s_pkmn->evSDEF = p_pkm->pkm_data.evs.spdef;
        s_pkmn->evSPD = p_pkm->pkm_data.evs.speed;

        //Attributes
        s_pkmn->attributes["friendship"] = p_pkm->pkm_data.tameness;
        s_pkmn->attributes["circle"] = p_pkm->pkm_data.markings.circle;
        s_pkmn->attributes["triangle"] = p_pkm->pkm_data.markings.triangle;
        s_pkmn->attributes["square"] = p_pkm->pkm_data.markings.square;
        s_pkmn->attributes["heart"] = p_pkm->pkm_data.markings.heart;
        s_pkmn->attributes["star"] = p_pkm->pkm_data.markings.star;
        s_pkmn->attributes["diamond"] = p_pkm->pkm_data.markings.diamond;
        s_pkmn->attributes["country"] = int(p_pkm->pkm_data.country);
        s_pkmn->attributes["cool"] = p_pkm->pkm_data.contest.cool;
        s_pkmn->attributes["beauty"] = p_pkm->pkm_data.contest.beauty;
        s_pkmn->attributes["cute"] = p_pkm->pkm_data.contest.cute;
        s_pkmn->attributes["smart"] = p_pkm->pkm_data.contest.smart;
        s_pkmn->attributes["tough"] = p_pkm->pkm_data.contest.tough;
        s_pkmn->attributes["sheen"] = p_pkm->pkm_data.contest.sheen;
        s_pkmn->attributes["sinnoh_champ_ribbon"] = p_pkm->pkm_data.sribbon1.sinnoh_champ_ribbon;
        s_pkmn->attributes["sinnoh_ability_ribbon"] = p_pkm->pkm_data.sribbon1.ability_ribbon;
        s_pkmn->attributes["sinnoh_great_ability_ribbon"] = p_pkm->pkm_data.sribbon1.great_ability_ribbon;
        s_pkmn->attributes["sinnoh_double_ability_ribbon"] = p_pkm->pkm_data.sribbon1.double_ability_ribbon;
        s_pkmn->attributes["sinnoh_multi_ability_ribbon"] = p_pkm->pkm_data.sribbon1.multi_ability_ribbon;
        s_pkmn->attributes["sinnoh_pair_ability_ribbon"] = p_pkm->pkm_data.sribbon1.pair_ability_ribbon;
        s_pkmn->attributes["sinnoh_world_ability_ribbon"] = p_pkm->pkm_data.sribbon1.world_ability_ribbon;
        s_pkmn->attributes["sinnoh_alert_ribbon"] = p_pkm->pkm_data.sribbon1.alert_ribbon;
        s_pkmn->attributes["sinnoh_shock_ribbon"] = p_pkm->pkm_data.sribbon1.shock_ribbon;
        s_pkmn->attributes["sinnoh_downcast_ribbon"] = p_pkm->pkm_data.sribbon1.downcast_ribbon;
        s_pkmn->attributes["sinnoh_careless_ribbon"] = p_pkm->pkm_data.sribbon1.careless_ribbon;
        s_pkmn->attributes["sinnoh_relax_ribbon"] = p_pkm->pkm_data.sribbon1.relax_ribbon;
        s_pkmn->attributes["sinnoh_snooze_ribbon"] = p_pkm->pkm_data.sribbon1.snooze_ribbon;
        s_pkmn->attributes["sinnoh_smile_ribbon"] = p_pkm->pkm_data.sribbon1.smile_ribbon;
        s_pkmn->attributes["sinnoh_gorgeous_ribbon"] = p_pkm->pkm_data.sribbon1.gorgeous_ribbon;
        s_pkmn->attributes["sinnoh_royal_ribbon"] = p_pkm->pkm_data.sribbon1.royal_ribbon;
        s_pkmn->attributes["sinnoh_gorgeous_royal_ribbon"] = p_pkm->pkm_data.sribbon2.gorgeous_royal_ribbon;
        s_pkmn->attributes["sinnoh_footprint_ribbon"] = p_pkm->pkm_data.sribbon2.footprint_ribbon;
        s_pkmn->attributes["sinnoh_record_ribbon"] = p_pkm->pkm_data.sribbon2.record_ribbon;
        s_pkmn->attributes["sinnoh_history_ribbon"] = p_pkm->pkm_data.sribbon2.history_ribbon;
        s_pkmn->attributes["sinnoh_legend_ribbon"] = p_pkm->pkm_data.sribbon2.legend_ribbon;
        s_pkmn->attributes["sinnoh_red_ribbon"] = p_pkm->pkm_data.sribbon2.red_ribbon;
        s_pkmn->attributes["sinnoh_green_ribbon"] = p_pkm->pkm_data.sribbon2.green_ribbon;
        s_pkmn->attributes["sinnoh_blue_ribbon"] = p_pkm->pkm_data.sribbon2.blue_ribbon;
        s_pkmn->attributes["sinnoh_festival_ribbon"] = p_pkm->pkm_data.sribbon2.festival_ribbon;
        s_pkmn->attributes["sinnoh_carnival_ribbon"] = p_pkm->pkm_data.sribbon2.carnival_ribbon;
        s_pkmn->attributes["sinnoh_classic_ribbon"] = p_pkm->pkm_data.sribbon2.classic_ribbon;
        s_pkmn->attributes["sinnoh_premier_ribbon"] = p_pkm->pkm_data.sribbon2.premier_ribbon;
        s_pkmn->attributes["hoenn_cool_ribbon"] = p_pkm->pkm_data.hribbon1.cool_ribbon;
        s_pkmn->attributes["hoenn_cool_ribbon_super"] = p_pkm->pkm_data.hribbon1.cool_ribbon_super;
        s_pkmn->attributes["hoenn_cool_ribbon_hyper"] = p_pkm->pkm_data.hribbon1.cool_ribbon_hyper;
        s_pkmn->attributes["hoenn_cool_ribbon_master"] = p_pkm->pkm_data.hribbon1.cool_ribbon_master;
        s_pkmn->attributes["hoenn_beauty_ribbon"] = p_pkm->pkm_data.hribbon1.beauty_ribbon;
        s_pkmn->attributes["hoenn_beauty_ribbon_super"] = p_pkm->pkm_data.hribbon1.beauty_ribbon_super;
        s_pkmn->attributes["hoenn_beauty_ribbon_hyper"] = p_pkm->pkm_data.hribbon1.beauty_ribbon_hyper;
        s_pkmn->attributes["hoenn_beauty_ribbon_master"] = p_pkm->pkm_data.hribbon1.beauty_ribbon_master;
        s_pkmn->attributes["hoenn_cute_ribbon"] = p_pkm->pkm_data.hribbon1.cute_ribbon;
        s_pkmn->attributes["hoenn_cute_ribbon_super"] = p_pkm->pkm_data.hribbon1.cute_ribbon_super;
        s_pkmn->attributes["hoenn_cute_ribbon_hyper"] = p_pkm->pkm_data.hribbon1.cute_ribbon_hyper;
        s_pkmn->attributes["hoenn_cute_ribbon_master"] = p_pkm->pkm_data.hribbon1.cute_ribbon_master;
        s_pkmn->attributes["hoenn_smart_ribbon"] = p_pkm->pkm_data.hribbon1.smart_ribbon;
        s_pkmn->attributes["hoenn_smart_ribbon_super"] = p_pkm->pkm_data.hribbon1.smart_ribbon_super;
        s_pkmn->attributes["hoenn_smart_ribbon_hyper"] = p_pkm->pkm_data.hribbon1.smart_ribbon_hyper;
        s_pkmn->attributes["hoenn_smart_ribbon_master"] = p_pkm->pkm_data.hribbon1.smart_ribbon_master;
        s_pkmn->attributes["hoenn_tough_ribbon"] = p_pkm->pkm_data.hribbon2.tough_ribbon;
        s_pkmn->attributes["hoenn_tough_ribbon_super"] = p_pkm->pkm_data.hribbon2.tough_ribbon_super;
        s_pkmn->attributes["hoenn_tough_ribbon_hyper"] = p_pkm->pkm_data.hribbon2.tough_ribbon_hyper;
        s_pkmn->attributes["hoenn_tough_ribbon_master"] = p_pkm->pkm_data.hribbon2.tough_ribbon_master;
        s_pkmn->attributes["hoenn_champion_ribbon"] = p_pkm->pkm_data.hribbon2.champion_ribbon;
        s_pkmn->attributes["hoenn_winning_ribbon"] = p_pkm->pkm_data.hribbon2.winning_ribbon;
        s_pkmn->attributes["hoenn_victory_ribbon"] = p_pkm->pkm_data.hribbon2.victory_ribbon;
        s_pkmn->attributes["hoenn_artist_ribbon"] = p_pkm->pkm_data.hribbon2.artist_ribbon;
        s_pkmn->attributes["hoenn_effort_ribbon"] = p_pkm->pkm_data.hribbon2.effort_ribbon;
        s_pkmn->attributes["hoenn_marine_ribbon"] = p_pkm->pkm_data.hribbon2.marine_ribbon;
        s_pkmn->attributes["hoenn_land_ribbon"] = p_pkm->pkm_data.hribbon2.land_ribbon;
        s_pkmn->attributes["hoenn_sky_ribbon"] = p_pkm->pkm_data.hribbon2.sky_ribbon;
        s_pkmn->attributes["hoenn_country_ribbon"] = p_pkm->pkm_data.hribbon2.country_ribbon;
        s_pkmn->attributes["hoenn_national_ribbon"] = p_pkm->pkm_data.hribbon2.national_ribbon;
        s_pkmn->attributes["hoenn_earth_ribbon"] = p_pkm->pkm_data.hribbon2.earth_ribbon;
        s_pkmn->attributes["hoenn_world_ribbon"] = p_pkm->pkm_data.hribbon2.world_ribbon;
        s_pkmn->attributes["fateful_encounter"] = p_pkm->pkm_data.forms.fencounter;
        s_pkmn->attributes["has_dream_world_ability"] = p_pkm->pkm_data.dwability.hasdwability;
        s_pkmn->attributes["n_flag"] = p_pkm->pkm_data.dwability.n_pkm;
        s_pkmn->attributes["original_game"] = int(p_pkm->pkm_data.hometown);
        s_pkmn->attributes["sinnoh_cool_ribbon"] = p_pkm->pkm_data.sribbon3.cool_ribbon;
        s_pkmn->attributes["sinnoh_cool_ribbon_great"] = p_pkm->pkm_data.sribbon3.cool_ribbon_great;
        s_pkmn->attributes["sinnoh_cool_ribbon_ultra"] = p_pkm->pkm_data.sribbon3.cool_ribbon_ultra;
        s_pkmn->attributes["sinnoh_cool_ribbon_master"] = p_pkm->pkm_data.sribbon3.cool_ribbon_master;
        s_pkmn->attributes["sinnoh_beauty_ribbon"] = p_pkm->pkm_data.sribbon3.beauty_ribbon;
        s_pkmn->attributes["sinnoh_beauty_ribbon_great"] = p_pkm->pkm_data.sribbon3.beauty_ribbon_great;
        s_pkmn->attributes["sinnoh_beauty_ribbon_ultra"] = p_pkm->pkm_data.sribbon3.beauty_ribbon_ultra;
        s_pkmn->attributes["sinnoh_beauty_ribbon_master"] = p_pkm->pkm_data.sribbon3.beauty_ribbon_master;
        s_pkmn->attributes["sinnoh_cute_ribbon"] = p_pkm->pkm_data.sribbon3.cute_ribbon;
        s_pkmn->attributes["sinnoh_cute_ribbon_great"] = p_pkm->pkm_data.sribbon3.cute_ribbon_great;
        s_pkmn->attributes["sinnoh_cute_ribbon_ultra"] = p_pkm->pkm_data.sribbon3.cute_ribbon_ultra;
        s_pkmn->attributes["sinnoh_cute_ribbon_master"] = p_pkm->pkm_data.sribbon3.cute_ribbon_master;
        s_pkmn->attributes["sinnoh_smart_ribbon"] = p_pkm->pkm_data.sribbon3.smart_ribbon;
        s_pkmn->attributes["sinnoh_smart_ribbon_great"] = p_pkm->pkm_data.sribbon3.smart_ribbon_great;
        s_pkmn->attributes["sinnoh_smart_ribbon_ultra"] = p_pkm->pkm_data.sribbon3.smart_ribbon_ultra;
        s_pkmn->attributes["sinnoh_smart_ribbon_master"] = p_pkm->pkm_data.sribbon3.smart_ribbon_master;
        s_pkmn->attributes["sinnoh_tough_ribbon"] = p_pkm->pkm_data.sribbon4.tough_ribbon;
        s_pkmn->attributes["sinnoh_tough_ribbon_great"] = p_pkm->pkm_data.sribbon4.tough_ribbon_great;
        s_pkmn->attributes["sinnoh_tough_ribbon_ultra"] = p_pkm->pkm_data.sribbon4.tough_ribbon_ultra;
        s_pkmn->attributes["sinnoh_tough_ribbon_master"] = p_pkm->pkm_data.sribbon4.tough_ribbon_master;
        s_pkmn->attributes["eggmet_year"] = p_pkm->pkm_data.eggdate.year;
        s_pkmn->attributes["eggmet_month"] = p_pkm->pkm_data.eggdate.month;
        s_pkmn->attributes["eggmet_day"] = p_pkm->pkm_data.eggdate.day;
        s_pkmn->attributes["met_year"] = p_pkm->pkm_data.metdate.year;
        s_pkmn->attributes["met_month"] = p_pkm->pkm_data.metdate.month;
        s_pkmn->attributes["met_day"] = p_pkm->pkm_data.metdate.day;
        s_pkmn->attributes["eggmet_location"] = int(p_pkm->pkm_data.eggmet);
        s_pkmn->attributes["met_location"] = int(p_pkm->pkm_data.met);
        s_pkmn->attributes["unova_ball"] = int(p_pkm->pkm_data.ball);
        s_pkmn->attributes["met_level"] = p_pkm->pkm_data.metlevel_otgender.metlevel;
        s_pkmn->attributes["ot_is_female"] = int(p_pkm->pkm_data.metlevel_otgender.metlevel);
        s_pkmn->attributes["unova_encounter"] = int(p_pkm->pkm_data.encounter);

        return s_pkmn;
    }

    void converter::spec_pkmn_to_pkmds_pkmn(spec_pkmn::sptr s_pkmn, party_pkm* p_pkm)
    {
        p_pkm->pkm_data.species = Species::pkmspecies(s_pkmn->get_base_pkmn()->get_nat_pokedex_num());
        p_pkm->pkm_data.moves[0] = Moves::moves(s_pkmn->get_moves()[0]->get_move_id());
        if(s_pkmn->get_moves()[1]->get_name() == "Struggle") p_pkm->pkm_data.moves[1] = Moves::NOTHING;
        else p_pkm->pkm_data.moves[1] = Moves::moves(s_pkmn->get_moves()[1]->get_move_id());
        if(s_pkmn->get_moves()[2]->get_name() == "Struggle") p_pkm->pkm_data.moves[2] = Moves::NOTHING;
        else p_pkm->pkm_data.moves[2] = Moves::moves(s_pkmn->get_moves()[2]->get_move_id());
        if(s_pkmn->get_moves()[3]->get_name() == "Struggle") p_pkm->pkm_data.moves[3] = Moves::NOTHING;
        else p_pkm->pkm_data.moves[3] = Moves::moves(s_pkmn->get_moves()[3]->get_move_id());

        //Unlike species and moves, nature ID's don't correspond to anything PKMNsim uses
        unsigned char nature_map[] = {0,1,6,16,21,11,2,7,17,22,12,4,9,19,24,20,14,5,10,25,15,3,8,18,23,13};
        p_pkm->pkm_data.nature = Natures::natures(nature_map[s_pkmn->nature->get_nature_id()]);
        setlevel(p_pkm->pkm_data, s_pkmn->level);
        #ifdef __linux__
        setpkmnickname(p_pkm->pkm_data, (wchar_t*)getwstring((char*)s_pkmn->nickname.c_str(), s_pkmn->nickname.size()).c_str(), s_pkmn->nickname.size());
        #else
        setpkmnickname(p_pkm->pkm_data, (wchar_t*)getwstring(s_pkmn->nickname).c_str(), s_pkmn->nickname.size());
        #endif
        p_pkm->pkm_data.pid = s_pkmn->pid;
        p_pkm->pkm_data.sid = s_pkmn->sid;
        p_pkm->pkm_data.tid = s_pkmn->tid;

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

    PokeLib::Pokemon converter::pokehack_pkmn_to_pokelib_pkmn(belt_pokemon_t* b_pkmn_t,
                                                              pokemon_attacks_t* pkmn_a_t,
                                                              pokemon_effort_t* pkmn_e_t,
                                                              pokemon_misc_t* pkmn_m_t,
                                                              pokemon_growth_t* pkmn_g_t)
    {
        PokeLib::Pokemon pokelib_pkmn = spec_pkmn_to_pokelib_pkmn(pokehack_pkmn_to_spec_pkmn(b_pkmn_t,
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

    void converter::pokehack_pkmn_to_pkmds_pkmn(belt_pokemon_t* b_pkmn_t,
                                                pokemon_attacks_t* pkmn_a_t,
                                                pokemon_effort_t* pkmn_e_t,
                                                pokemon_misc_t* pkmn_m_t,
                                                pokemon_growth_t* pkmn_g_t,
                                                party_pkm* p_pkm)
    {
        spec_pkmn_to_pkmds_pkmn(pokehack_pkmn_to_spec_pkmn(b_pkmn_t, pkmn_a_t, pkmn_e_t, pkmn_m_t, pkmn_g_t), p_pkm);

        //Manually set egg met and met locations to Faraway Place
        p_pkm->pkm_data.eggmet = Locations::farawayplace;
        p_pkm->pkm_data.met = Locations::farawayplace;
    }

    void converter::pokelib_pkmn_to_pokehack_pkmn(PokeLib::Pokemon pokelib_pkmn,
                                                  belt_pokemon_t* b_pkmn_t,
                                                  pokemon_attacks_t* pkmn_a_t,
                                                  pokemon_effort_t* pkmn_e_t,
                                                  pokemon_misc_t* pkmn_m_t,
                                                  pokemon_growth_t* pkmn_g_t)
    {
        spec_pkmn_to_pokehack_pkmn(pokelib_pkmn_to_spec_pkmn(pokelib_pkmn), b_pkmn_t, pkmn_a_t, pkmn_e_t, pkmn_m_t, pkmn_g_t);

        //Manually set met location to In-Game Trade
        pkmn_m_t->locationcaught = 255;

        //If Pokemon was caught in a Gen 4 ball, set it to standard Poke Ball
        if(pkmn_m_t->pokeball > 0xC) pkmn_m_t->pokeball = 0x4;
    }

    void converter::pokelib_pkmn_to_pkmds_pkmn(PokeLib::Pokemon pokelib_pkmn, party_pkm* p_pkm)
    {
        spec_pkmn_to_pkmds_pkmn(pokelib_pkmn_to_spec_pkmn(pokelib_pkmn), p_pkm);

        //Manually set egg met location to Faraway place and met location to Poke Transfer
        p_pkm->pkm_data.eggmet = Locations::farawayplace;
        p_pkm->pkm_data.met = Locations::poketransfer;
    }

    void converter::pkmds_pkmn_to_pokehack_pkmn(party_pkm* p_pkm,
                                                belt_pokemon_t* b_pkmn_t,
                                                pokemon_attacks_t* pkmn_a_t,
                                                pokemon_effort_t* pkmn_e_t,
                                                pokemon_misc_t* pkmn_m_t,
                                                pokemon_growth_t* pkmn_g_t)
    {
        spec_pkmn_to_pokehack_pkmn(pkmds_pkmn_to_spec_pkmn(p_pkm), b_pkmn_t, pkmn_a_t, pkmn_e_t, pkmn_m_t, pkmn_g_t);
    }

    PokeLib::Pokemon converter::pkmds_pkmn_to_pokelib_pkmn(party_pkm* p_pkm)
    {
        PokeLib::Pokemon pokelib_pkmn = spec_pkmn_to_pokelib_pkmn(pkmds_pkmn_to_spec_pkmn(p_pkm));

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
    }
}