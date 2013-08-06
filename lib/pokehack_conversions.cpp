/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
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
    }

    PokeLib::Pokemon converter::pokehack_pkmn_to_pokelib_pkmn(belt_pokemon_t* b_pkmn_t,
                                                              pokemon_attacks_t* pkmn_a_t,
                                                              pokemon_effort_t* pkmn_e_t,
                                                              pokemon_misc_t* pkmn_m_t,
                                                              pokemon_growth_t* pkmn_g_t)
    {
        PokeLib::Pokemon pokelib_pkmn;

        pokelib_pkmn.pkm->pkm.species = pkmn_g_t->species;
        pokelib_pkmn.setLevel(uint8_t(b_pkmn_t->level));
        string pokehack_nickname(pokehack_get_text(b_pkmn_t->name, true));
        pokelib_pkmn.setNickname(pokehack_nickname.c_str(), pokehack_nickname.size());
        pokelib_pkmn.pkm->pkm.pid = b_pkmn_t->personality; //Covers gender, shiny, and nature
        pokelib_pkmn.pkm->pkm.ot_id = ((unsigned short*)(&b_pkmn_t->otid))[0];
        pokelib_pkmn.pkm->pkm.ot_sid = ((unsigned short*)(&b_pkmn_t->otid))[1];

        //Moves should have the same indices
        pokelib_pkmn.pkm->pkm.move[0] = pkmn_a_t->atk1;
        pokelib_pkmn.pkm->pkm.move[1] = pkmn_a_t->atk2;
        pokelib_pkmn.pkm->pkm.move[2] = pkmn_a_t->atk3;
        pokelib_pkmn.pkm->pkm.move[3] = pkmn_a_t->atk4;
        pokelib_pkmn.pkm->pkm.movePP[0] = pkmn_a_t->pp1;
        pokelib_pkmn.pkm->pkm.movePP[1] = pkmn_a_t->pp2;
        pokelib_pkmn.pkm->pkm.movePP[2] = pkmn_a_t->pp3;
        pokelib_pkmn.pkm->pkm.movePP[3] = pkmn_a_t->pp4;

        //Stats
        pokelib_pkmn.pkm->pkm.battle_hp = b_pkmn_t->currentHP;
        pokelib_pkmn.pkm->pkm.battle_max_hp = b_pkmn_t->maxHP;
        pokelib_pkmn.pkm->pkm.battle_atk = b_pkmn_t->attack;
        pokelib_pkmn.pkm->pkm.battle_def = b_pkmn_t->defense;
        pokelib_pkmn.pkm->pkm.battle_satk = b_pkmn_t->spatk;
        pokelib_pkmn.pkm->pkm.battle_sdef = b_pkmn_t->spdef;
        pokelib_pkmn.pkm->pkm.battle_spd = b_pkmn_t->speed;

        //IV's
        pokelib_pkmn.pkm->pkm.iv_hp = pkmn_m_t->IVs.hp;
        pokelib_pkmn.pkm->pkm.iv_atk = pkmn_m_t->IVs.atk;
        pokelib_pkmn.pkm->pkm.iv_def = pkmn_m_t->IVs.def;
        pokelib_pkmn.pkm->pkm.iv_satk = pkmn_m_t->IVs.spatk;
        pokelib_pkmn.pkm->pkm.iv_sdef = pkmn_m_t->IVs.spdef;
        pokelib_pkmn.pkm->pkm.iv_spd = pkmn_m_t->IVs.spd;

        //EV's
        pokelib_pkmn.pkm->pkm.ev_hp = pkmn_e_t->hp;
        pokelib_pkmn.pkm->pkm.ev_atk = pkmn_e_t->attack;
        pokelib_pkmn.pkm->pkm.ev_def = pkmn_e_t->defense;
        pokelib_pkmn.pkm->pkm.ev_satk = pkmn_e_t->spatk;
        pokelib_pkmn.pkm->pkm.ev_sdef = pkmn_e_t->spdef;
        pokelib_pkmn.pkm->pkm.ev_spd = pkmn_e_t->speed;

        return pokelib_pkmn;
    }

    void converter::pokehack_pkmn_to_pkmds_pkmn(belt_pokemon_t* b_pkmn_t,
                                                pokemon_attacks_t* pkmn_a_t,
                                                pokemon_effort_t* pkmn_e_t,
                                                pokemon_misc_t* pkmn_m_t,
                                                pokemon_growth_t* pkmn_g_t,
                                                party_pkm* p_pkm)
    {
        p_pkm->pkm_data.species = Species::pkmspecies(pkmn_g_t->species);
        setlevel(p_pkm->pkm_data, b_pkmn_t->level);
        string pokehack_nickname(pokehack_get_text(b_pkmn_t->name, true));
        #ifdef __linux__
        setpkmnickname(p_pkm->pkm_data, (wchar_t*)getwstring((char*)pokehack_nickname.c_str(), pokehack_nickname.size()).c_str(), pokehack_nickname.size());
        #else
        setpkmnickname(p_pkm->pkm_data, (wchar_t*)getwstring(pokehack_nickname).c_str(), pokehack_nickname.size());
        #endif
        p_pkm->pkm_data.pid = b_pkmn_t->personality;
        p_pkm->pkm_data.tid = ((unsigned short*)(&b_pkmn_t->otid))[0];
        p_pkm->pkm_data.sid = ((unsigned short*)(&b_pkmn_t->otid))[1];

        //Moves should have the same indices
        p_pkm->pkm_data.moves[0] = Moves::moves(pkmn_a_t->atk1);
        p_pkm->pkm_data.moves[1] = Moves::moves(pkmn_a_t->atk2);
        p_pkm->pkm_data.moves[2] = Moves::moves(pkmn_a_t->atk3);
        p_pkm->pkm_data.moves[3] = Moves::moves(pkmn_a_t->atk4);
        p_pkm->pkm_data.pp[0] = pkmn_a_t->pp1;
        p_pkm->pkm_data.pp[1] = pkmn_a_t->pp2;
        p_pkm->pkm_data.pp[2] = pkmn_a_t->pp3;
        p_pkm->pkm_data.pp[3] = pkmn_a_t->pp4;

        //Stats
        p_pkm->party_data.hp = b_pkmn_t->currentHP;
        p_pkm->party_data.maxhp = b_pkmn_t->maxHP;
        p_pkm->party_data.attack = b_pkmn_t->attack;
        p_pkm->party_data.defense = b_pkmn_t->defense;
        p_pkm->party_data.spatk = b_pkmn_t->spatk;
        p_pkm->party_data.spdef = b_pkmn_t->spdef;
        p_pkm->party_data.speed = b_pkmn_t->speed;

        p_pkm->pkm_data.ivs.hp = pkmn_m_t->IVs.hp;
        p_pkm->pkm_data.ivs.attack = pkmn_m_t->IVs.atk;
        p_pkm->pkm_data.ivs.defense = pkmn_m_t->IVs.def;
        p_pkm->pkm_data.ivs.spatk = pkmn_m_t->IVs.spatk;
        p_pkm->pkm_data.ivs.spdef = pkmn_m_t->IVs.spdef;
        p_pkm->pkm_data.ivs.speed = pkmn_m_t->IVs.spd;

        p_pkm->pkm_data.evs.hp = pkmn_e_t->hp;
        p_pkm->pkm_data.evs.attack = pkmn_e_t->attack;
        p_pkm->pkm_data.evs.defense = pkmn_e_t->defense;
        p_pkm->pkm_data.evs.spatk = pkmn_e_t->spatk;
        p_pkm->pkm_data.evs.spdef = pkmn_e_t->spdef;
        p_pkm->pkm_data.evs.speed = pkmn_e_t->speed;
    }
}
