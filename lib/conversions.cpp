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
#include <boost/assign.hpp>
#include <boost/format.hpp>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/pkmn_nature.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include <pkmnsim/database/queries.hpp>

#include <pokelib/data_tables.h>
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
        s_pkmn->sid = ((unsigned short*)(&b_pkmn_t->otid))[0];
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
        dict<char, int> pokehack_reverse_char_map = boost::assign::map_list_of
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
            ('\0', int(0xFF))
        ;

        //Species position needs to be manually set for Nidoran's due to gender character
        if(s_pkmn->base->get_nat_pokedex_num() == 29) pkmn_g_t->species = 29; //Nidoran F
        if(s_pkmn->base->get_nat_pokedex_num() == 32) pkmn_g_t->species = 32; //Nidoran M
        else pkmn_g_t->species = distance(pokemon_species, (find(pokemon_species, pokemon_species+439, s_pkmn->get_species_name().c_str())));

        b_pkmn_t->level = s_pkmn->level;
        for(int i = 0; i < 10; i++)
        {
            if(i < s_pkmn->nickname.size()) b_pkmn_t->name[i] = pokehack_reverse_char_map[s_pkmn->nickname[i]];
            else b_pkmn_t->name[i] = 0xFF;
        }
        pkmn_a_t->atk1 = distance(items, (find(attacks, attacks+354, s_pkmn->moves[0]->get_name().c_str())));
        pkmn_a_t->atk2 = distance(items, (find(attacks, attacks+354, s_pkmn->moves[1]->get_name().c_str())));
        pkmn_a_t->atk3 = distance(items, (find(attacks, attacks+354, s_pkmn->moves[2]->get_name().c_str())));
        pkmn_a_t->atk4 = distance(items, (find(attacks, attacks+354, s_pkmn->moves[3]->get_name().c_str())));
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

    spec_pkmn::sptr converter::pokelib_pkmn_to_spec_pkmn(PokeLib::Pokemon pokelib_pkmn)
    {
        string identifier, move1, move2, move3, move4;
        int level, gender;

        identifier = PokeLib::species[int(pokelib_pkmn.pkm->pkm.species)];
        level = int(pokelib_pkmn.getLevel());

        move1 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[0])]);
        move2 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[1])]);
        move3 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[2])]);
        move4 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[3])]);

        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, 4, level, move1, move2, move3, move4);

        PokeLib::widetext nickname_wide = pokelib_pkmn.getNickname();
        char nickname_buffer[10];
        memset(nickname_buffer,0,10);
        wcstombs(nickname_buffer, nickname_wide.c_str(), 10);
        s_pkmn->nickname = nickname_buffer;
        s_pkmn->held_item = PokeLib::items[pokelib_pkmn.pkm->pkm.held_item];
        s_pkmn->nature = pkmn_nature::make(PokeLib::nature[int(pokelib_pkmn.getNatureValue())]);
        s_pkmn->pid = pokelib_pkmn.pkm->pkm.pid;
        s_pkmn->sid = pokelib_pkmn.pkm->pkm.ot_sid;
        s_pkmn->tid = pokelib_pkmn.pkm->pkm.ot_id;
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

        return s_pkmn;
    }

    PokeLib::Pokemon converter::spec_pkmn_to_pokelib_pkmn(spec_pkmn::sptr s_pkmn)
    {
        int species_pos, move1_pos, move2_pos, move3_pos, move4_pos, nature_pos, ability_pos;

        PokeLib::Pokemon pokelib_pkmn;
        species_pos = distance(PokeLib::species, (find(PokeLib::species, PokeLib::species+656, s_pkmn->get_species_name().c_str())));
        move1_pos = distance(PokeLib::movelist, (find(PokeLib::movelist, PokeLib::movelist+559, s_pkmn->get_moves()[0]->get_name().c_str())));
        move2_pos = distance(PokeLib::movelist, (find(PokeLib::movelist, PokeLib::movelist+559, s_pkmn->get_moves()[1]->get_name().c_str())));
        move3_pos = distance(PokeLib::movelist, (find(PokeLib::movelist, PokeLib::movelist+559, s_pkmn->get_moves()[2]->get_name().c_str())));
        move4_pos = distance(PokeLib::movelist, (find(PokeLib::movelist, PokeLib::movelist+559, s_pkmn->get_moves()[3]->get_name().c_str())));
        move4_pos = distance(PokeLib::movelist, (find(PokeLib::movelist, PokeLib::movelist+559, s_pkmn->get_moves()[3]->get_name().c_str())));
        nature_pos = distance(PokeLib::nature, (find(PokeLib::nature, PokeLib::nature+25, s_pkmn->nature->get_name().c_str())));
        ability_pos = distance(PokeLib::ability, (find(PokeLib::ability, PokeLib::ability+161, s_pkmn->ability.c_str())));

        pokelib_pkmn.pkm->pkm.species = species_pos;
        pokelib_pkmn.setLevel(uint8_t(s_pkmn->level));
        pokelib_pkmn.setNickname(s_pkmn->nickname.c_str(), s_pkmn->nickname.size());
        //Shiny, gender, and nature will be annoying
        pokelib_pkmn.pkm->pkm.move[0] = move1_pos;
        pokelib_pkmn.pkm->pkm.move[1] = move2_pos;
        pokelib_pkmn.pkm->pkm.move[2] = move3_pos;
        pokelib_pkmn.pkm->pkm.move[3] = move4_pos;

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

        return pokelib_pkmn;
    }

    spec_pkmn::sptr converter::pkmds_pkmn_to_spec_pkmn(party_pkm* p_pkm)
    {
        opendb(get_database_path().c_str());

        string identifier, move1, move2, move3, move4;
        int level, gender;

        identifier = database::to_database_format(lookuppkmname(p_pkm->pkm_data));
        level = getpkmlevel(p_pkm->pkm_data);

        move1 = lookupmovename(p_pkm->pkm_data.moves[0]);
        if(p_pkm->pkm_data.moves[1] == Moves::NOTHING) move2 = "None";
        else move2 = lookupmovename(p_pkm->pkm_data.moves[1]);
        if(p_pkm->pkm_data.moves[2] == Moves::NOTHING) move3 = "None";
        else move3 = lookupmovename(p_pkm->pkm_data.moves[2]);
        if(p_pkm->pkm_data.moves[3] == Moves::NOTHING) move4 = "None";
        else move4 = lookupmovename(p_pkm->pkm_data.moves[3]);

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

        s_pkmn->HP = getpkmstat(p_pkm->pkm_data, Stat_IDs::hp);
        s_pkmn->ATK = getpkmstat(p_pkm->pkm_data, Stat_IDs::attack);
        s_pkmn->DEF = getpkmstat(p_pkm->pkm_data, Stat_IDs::defense);
        s_pkmn->SATK = getpkmstat(p_pkm->pkm_data, Stat_IDs::spatk);
        s_pkmn->SDEF = getpkmstat(p_pkm->pkm_data, Stat_IDs::spdef);
        s_pkmn->SPD = getpkmstat(p_pkm->pkm_data, Stat_IDs::speed);

        s_pkmn->ivHP = p_pkm->pkm_data.ivs.hp;
        s_pkmn->ivATK = p_pkm->pkm_data.ivs.attack;
        s_pkmn->ivDEF = p_pkm->pkm_data.ivs.defense;
        s_pkmn->ivSATK = p_pkm->pkm_data.ivs.spatk;
        s_pkmn->ivSDEF = p_pkm->pkm_data.ivs.spdef;
        s_pkmn->ivSPD = p_pkm->pkm_data.ivs.speed;

        s_pkmn->evHP = p_pkm->pkm_data.evs.hp;
        s_pkmn->evATK = p_pkm->pkm_data.evs.attack;
        s_pkmn->evDEF = p_pkm->pkm_data.evs.defense;
        s_pkmn->evSATK = p_pkm->pkm_data.evs.spatk;
        s_pkmn->evSDEF = p_pkm->pkm_data.evs.spdef;
        s_pkmn->evSPD = p_pkm->pkm_data.evs.speed;

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
        p_pkm->pkm_data.sid = b_pkmn_t->personality;

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
        
    }
}
