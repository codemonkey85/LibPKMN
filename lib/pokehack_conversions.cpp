/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <bitset>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/pkmn_nature.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include <pkmnsim/database/queries.hpp>

#include <pkmds/pkmds_g5_sqlite.h>

#include "conversions.hpp"
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

#define MAX_NICKNAME_LEN 10
#define MAX_TRAINER_NAME_LEN 7

namespace pkmnsim
{
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
        int level, gender;

        identifier = pokemon_species[pkmn_g_t->species];
        level = b_pkmn_t->level;

        move1 = database::to_database_format(attacks[pkmn_a_t->atk1]);
        if(string(attacks[pkmn_a_t->atk2]) == "No attack") move2 = "None";
        else move2 = database::to_database_format(attacks[pkmn_a_t->atk2]);
        if(string(attacks[pkmn_a_t->atk3]) == "No attack") move3 = "None";
        else move3 = database::to_database_format(attacks[pkmn_a_t->atk3]);
        if(string(attacks[pkmn_a_t->atk4]) == "No attack") move4 = "None";
        else move4 = database::to_database_format(attacks[pkmn_a_t->atk4]);

        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, 3, level,
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
        s_pkmn->attributes["friendship"] = int(pkmn_g_t->happiness);

        //Need to get markings manually, Pokehack has no bitfield
        bitset<4> markings_bitset(b_pkmn_t->mark);
        s_pkmn->attributes["circle"] = int(markings_bitset[0]);
        s_pkmn->attributes["triangle"] = int(markings_bitset[1]);
        s_pkmn->attributes["square"] = int(markings_bitset[2]);
        s_pkmn->attributes["heart"] = int(markings_bitset[3]);

        s_pkmn->attributes["country"] = int(b_pkmn_t->language);
        s_pkmn->attributes["cool"] = int(pkmn_e_t->coolness);
        s_pkmn->attributes["beauty"] = int(pkmn_e_t->beauty);
        s_pkmn->attributes["cute"] = int(pkmn_e_t->cuteness);
        s_pkmn->attributes["smart"] = int(pkmn_e_t->smartness);
        s_pkmn->attributes["tough"] = int(pkmn_e_t->toughness);

        //Need to get ribbons manually, Pokehack has no bitfield
        bitset<32> ribbons_bitset(pkmn_m_t->ribbons);
        s_pkmn->attributes["hoenn_cool_ribbon"] = ribbons_bitset[0];
        s_pkmn->attributes["hoenn_cool_ribbon_super"] = ribbons_bitset[1];
        s_pkmn->attributes["hoenn_cool_ribbon_hyper"] = ribbons_bitset[2];
        s_pkmn->attributes["hoenn_cool_ribbon_master"] = ribbons_bitset[3];
        s_pkmn->attributes["hoenn_beauty_ribbon"] = ribbons_bitset[4];
        s_pkmn->attributes["hoenn_beauty_ribbon_super"] = ribbons_bitset[5];
        s_pkmn->attributes["hoenn_beauty_ribbon_hyper"] = ribbons_bitset[6];
        s_pkmn->attributes["hoenn_beauty_ribbon_master"] = ribbons_bitset[7];
        s_pkmn->attributes["hoenn_cute_ribbon"] = ribbons_bitset[8];
        s_pkmn->attributes["hoenn_cute_ribbon_super"] = ribbons_bitset[9];
        s_pkmn->attributes["hoenn_cute_ribbon_hyper"] = ribbons_bitset[10];
        s_pkmn->attributes["hoenn_cute_ribbon_master"] = ribbons_bitset[11];
        s_pkmn->attributes["hoenn_smart_ribbon"] = ribbons_bitset[12];
        s_pkmn->attributes["hoenn_smart_ribbon_super"] = ribbons_bitset[13];
        s_pkmn->attributes["hoenn_smart_ribbon_hyper"] = ribbons_bitset[14];
        s_pkmn->attributes["hoenn_smart_ribbon_master"] = ribbons_bitset[15];
        s_pkmn->attributes["hoenn_tough_ribbon"] = ribbons_bitset[16];
        s_pkmn->attributes["hoenn_tough_ribbon_super"] = ribbons_bitset[17];
        s_pkmn->attributes["hoenn_tough_ribbon_hyper"] = ribbons_bitset[18];
        s_pkmn->attributes["hoenn_tough_ribbon_master"] = ribbons_bitset[19];
        s_pkmn->attributes["hoenn_champion_ribbon"] = ribbons_bitset[20];
        s_pkmn->attributes["hoenn_winning_ribbon"] = ribbons_bitset[21];
        s_pkmn->attributes["hoenn_victory_ribbon"] = ribbons_bitset[22];
        s_pkmn->attributes["hoenn_artist_ribbon"] = ribbons_bitset[23];
        s_pkmn->attributes["hoenn_effort_ribbon"] = ribbons_bitset[24];
        s_pkmn->attributes["hoenn_marine_ribbon"] = ribbons_bitset[25];
        s_pkmn->attributes["hoenn_land_ribbon"] = ribbons_bitset[26];
        s_pkmn->attributes["hoenn_sky_ribbon"] = ribbons_bitset[27];
        s_pkmn->attributes["hoenn_country_ribbon"] = ribbons_bitset[28];
        s_pkmn->attributes["hoenn_national_ribbon"] = ribbons_bitset[29];
        s_pkmn->attributes["hoenn_earth_ribbon"] = ribbons_bitset[30];
        s_pkmn->attributes["hoenn_world_ribbon"] = ribbons_bitset[31];

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

        //Attributes
        if(s_pkmn->attributes.has_key("friendship"))
            pkmn_g_t->happiness = (unsigned char)(s_pkmn->attributes["friendship"]);

        //Need to set markings manually, Pokehack has no bitfield
        bitset<4> markings_bitset(0);
        if(s_pkmn->attributes.has_key("circle"))
            markings_bitset[0] = s_pkmn->attributes["circle"];
        if(s_pkmn->attributes.has_key("triangle"))
            markings_bitset[1] = s_pkmn->attributes["triangle"];
        if(s_pkmn->attributes.has_key("square"))
            markings_bitset[2] = s_pkmn->attributes["square"];
        if(s_pkmn->attributes.has_key("heart"))
            markings_bitset[3] = s_pkmn->attributes["heart"];
        b_pkmn_t->mark = (unsigned char)(markings_bitset.to_ulong());

        if(s_pkmn->attributes.has_key("country"))
            b_pkmn_t->language = (unsigned short)(s_pkmn->attributes["country"]);
        if(s_pkmn->attributes.has_key("cool"))
            pkmn_e_t->coolness = (unsigned char)(s_pkmn->attributes["coolness"]);
        if(s_pkmn->attributes.has_key("beauty"))
            pkmn_e_t->beauty = (unsigned char)(s_pkmn->attributes["beauty"]);
        if(s_pkmn->attributes.has_key("cute"))
            pkmn_e_t->cuteness = (unsigned char)(s_pkmn->attributes["cute"]);
        if(s_pkmn->attributes.has_key("smart"))
            pkmn_e_t->smartness = (unsigned char)(s_pkmn->attributes["smart"]);
        if(s_pkmn->attributes.has_key("tough"))
            pkmn_e_t->toughness = (unsigned char)(s_pkmn->attributes["tough"]);

        //Need to set ribbons manually, Pokehack has no bitfield
        bitset<32> ribbons_bitset(0);
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon"))
            ribbons_bitset[0] = s_pkmn->attributes.has_key("hoenn_cool_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_super"))
            ribbons_bitset[1] = s_pkmn->attributes.has_key("hoenn_cool_ribbon_super");
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_hyper"))
            ribbons_bitset[2] = s_pkmn->attributes.has_key("hoenn_cool_ribbon_hyper");
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_master"))
            ribbons_bitset[3] = s_pkmn->attributes.has_key("hoenn_cool_ribbon_master");
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon"))
            ribbons_bitset[4] = s_pkmn->attributes.has_key("hoenn_beauty_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_super"))
            ribbons_bitset[5] = s_pkmn->attributes.has_key("hoenn_beauty_ribbon_super");
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_hyper"))
            ribbons_bitset[6] = s_pkmn->attributes.has_key("hoenn_beauty_ribbon_hyper");
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_master"))
            ribbons_bitset[7] = s_pkmn->attributes.has_key("hoenn_beauty_ribbon_master");
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon"))
            ribbons_bitset[8] = s_pkmn->attributes.has_key("hoenn_cute_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_super"))
            ribbons_bitset[9] = s_pkmn->attributes.has_key("hoenn_cute_ribbon_super");
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_hyper"))
            ribbons_bitset[10] = s_pkmn->attributes.has_key("hoenn_cute_ribbon_hyper");
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_master"))
            ribbons_bitset[11] = s_pkmn->attributes.has_key("hoenn_cute_ribbon_master");
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon"))
            ribbons_bitset[12] = s_pkmn->attributes.has_key("hoenn_smart_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_super"))
            ribbons_bitset[13] = s_pkmn->attributes.has_key("hoenn_smart_ribbon_super");
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_hyper"))
            ribbons_bitset[14] = s_pkmn->attributes.has_key("hoenn_smart_ribbon_hyper");
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_master"))
            ribbons_bitset[15] = s_pkmn->attributes.has_key("hoenn_smart_ribbon_master");
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon"))
            ribbons_bitset[16] = s_pkmn->attributes.has_key("hoenn_tough_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_super"))
            ribbons_bitset[17] = s_pkmn->attributes.has_key("hoenn_tough_ribbon_super");
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_hyper"))
            ribbons_bitset[18] = s_pkmn->attributes.has_key("hoenn_tough_ribbon_hyper");
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_master"))
            ribbons_bitset[19] = s_pkmn->attributes.has_key("hoenn_tough_ribbon_master");
        if(s_pkmn->attributes.has_key("hoenn_champion_ribbon"))
            ribbons_bitset[20] = s_pkmn->attributes.has_key("hoenn_champion_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_winning_ribbon"))
            ribbons_bitset[21] = s_pkmn->attributes.has_key("hoenn_winning_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_victory_ribbon"))
            ribbons_bitset[22] = s_pkmn->attributes.has_key("hoenn_victory_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_artist_ribbon"))
            ribbons_bitset[23] = s_pkmn->attributes.has_key("hoenn_artist_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_effort_ribbon"))
            ribbons_bitset[24] = s_pkmn->attributes.has_key("hoenn_effort_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_marine_ribbon"))
            ribbons_bitset[25] = s_pkmn->attributes.has_key("hoenn_marine_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_land_ribbon"))
            ribbons_bitset[26] = s_pkmn->attributes.has_key("hoenn_land_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_sky_ribbon"))
            ribbons_bitset[27] = s_pkmn->attributes.has_key("hoenn_sky_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_country_ribbon"))
            ribbons_bitset[28] = s_pkmn->attributes.has_key("hoenn_country_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_national_ribbon"))
            ribbons_bitset[29] = s_pkmn->attributes.has_key("hoenn_national_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_earth_ribbon"))
            ribbons_bitset[30] = s_pkmn->attributes.has_key("hoenn_earth_ribbon");
        if(s_pkmn->attributes.has_key("hoenn_world_ribbon"))
            ribbons_bitset[31] = s_pkmn->attributes.has_key("hoenn_world_ribbon");
        pkmn_m_t->ribbons = (unsigned int)(ribbons_bitset.to_ulong());
    }
}
