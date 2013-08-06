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

namespace pkmnsim
{
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

        return pokelib_pkmn;
    }
    
    void converter::pokelib_pkmn_to_pokehack_pkmn(PokeLib::Pokemon pokelib_pkmn,
                                                  belt_pokemon_t* b_pkmn_t,
                                                  pokemon_attacks_t* pkmn_a_t,
                                                  pokemon_effort_t* pkmn_e_t,
                                                  pokemon_misc_t* pkmn_m_t,
                                                  pokemon_growth_t* pkmn_g_t)
    {        
        pkmn_g_t->species = pokelib_pkmn.pkm->pkm.species;
        unsigned short otid[2] = {pokelib_pkmn.pkm->pkm.ot_id, pokelib_pkmn.pkm->pkm.ot_sid};
        b_pkmn_t->otid = *((unsigned int*)(&otid[0]));
        b_pkmn_t->level = pokelib_pkmn.pkm->pkm.battle_level;
        PokeLib::widetext nickname_wide = pokelib_pkmn.getNickname();
        char nickname_buffer[10];
        memset(nickname_buffer,0,10);
        wcstombs(nickname_buffer, nickname_wide.c_str(), 10);
        for(int i = 0; i < 10; i++)
        {
            if(nickname_buffer != 0) b_pkmn_t->name[i] = pokehack_reverse_char_map[nickname_buffer[i]];
            else b_pkmn_t->name[i] = 0xFF;
        }
        
        //Moves
        pkmn_a_t->atk1 = pokelib_pkmn.pkm->pkm.move[0];
        pkmn_a_t->atk2 = pokelib_pkmn.pkm->pkm.move[1];
        pkmn_a_t->atk3 = pokelib_pkmn.pkm->pkm.move[2];
        pkmn_a_t->atk4 = pokelib_pkmn.pkm->pkm.move[3];
        pkmn_a_t->pp1 = pokelib_pkmn.pkm->pkm.movePP[0];
        pkmn_a_t->pp2 = pokelib_pkmn.pkm->pkm.movePP[1];
        pkmn_a_t->pp3 = pokelib_pkmn.pkm->pkm.movePP[2];
        pkmn_a_t->pp4 = pokelib_pkmn.pkm->pkm.movePP[3];
        
        //Stats
        b_pkmn_t->currentHP = pokelib_pkmn.pkm->pkm.battle_hp;
        b_pkmn_t->maxHP = pokelib_pkmn.pkm->pkm.battle_max_hp;
        b_pkmn_t->attack = pokelib_pkmn.pkm->pkm.battle_atk;
        b_pkmn_t->defense = pokelib_pkmn.pkm->pkm.battle_def;
        b_pkmn_t->spatk = pokelib_pkmn.pkm->pkm.battle_satk;
        b_pkmn_t->spdef = pokelib_pkmn.pkm->pkm.battle_sdef;
        b_pkmn_t->speed = pokelib_pkmn.pkm->pkm.battle_spd;
        
        //IV's
        pkmn_m_t->IVs.hp = pokelib_pkmn.pkm->pkm.iv_hp;
        pkmn_m_t->IVs.atk = pokelib_pkmn.pkm->pkm.iv_atk;
        pkmn_m_t->IVs.def = pokelib_pkmn.pkm->pkm.iv_def;
        pkmn_m_t->IVs.spatk = pokelib_pkmn.pkm->pkm.iv_satk;
        pkmn_m_t->IVs.spdef = pokelib_pkmn.pkm->pkm.iv_sdef;
        pkmn_m_t->IVs.spd = pokelib_pkmn.pkm->pkm.iv_spd;
        
        //EV's
        pkmn_e_t->hp = pokelib_pkmn.pkm->pkm.ev_hp;
        pkmn_e_t->attack = pokelib_pkmn.pkm->pkm.ev_atk;
        pkmn_e_t->defense = pokelib_pkmn.pkm->pkm.ev_def;
        pkmn_e_t->spatk = pokelib_pkmn.pkm->pkm.ev_satk;
        pkmn_e_t->spdef = pokelib_pkmn.pkm->pkm.ev_sdef;
        pkmn_e_t->speed = pokelib_pkmn.pkm->pkm.ev_spd;
    }
    
    void converter::pokelib_pkmn_to_pkmds_pkmn(PokeLib::Pokemon pokelib_pkmn, party_pkm* p_pkm)
    {
        p_pkm->pkm_data.species = Species::pkmspecies(pokelib_pkmn.pkm->pkm.species);
        setlevel(p_pkm->pkm_data, pokelib_pkmn.getLevel());
        setpkmnickname(p_pkm->pkm_data, (wchar_t*)(pokelib_pkmn.getNickname().c_str()), pokelib_pkmn.getNickname().size());
        p_pkm->pkm_data.pid = pokelib_pkmn.pkm->pkm.pid;
        p_pkm->pkm_data.tid = pokelib_pkmn.pkm->pkm.ot_id;
        p_pkm->pkm_data.sid = pokelib_pkmn.pkm->pkm.ot_sid;
        
        p_pkm->pkm_data.moves[0] = Moves::moves(pokelib_pkmn.pkm->pkm.move[0]);
        p_pkm->pkm_data.moves[1] = Moves::moves(pokelib_pkmn.pkm->pkm.move[1]);
        p_pkm->pkm_data.moves[2] = Moves::moves(pokelib_pkmn.pkm->pkm.move[2]);
        p_pkm->pkm_data.moves[3] = Moves::moves(pokelib_pkmn.pkm->pkm.move[3]);
        p_pkm->pkm_data.pp[0] = pokelib_pkmn.pkm->pkm.movePP[0];
        p_pkm->pkm_data.pp[1] = pokelib_pkmn.pkm->pkm.movePP[1];
        p_pkm->pkm_data.pp[2] = pokelib_pkmn.pkm->pkm.movePP[2];
        p_pkm->pkm_data.pp[3] = pokelib_pkmn.pkm->pkm.movePP[3];
        
        //Stats
        p_pkm->party_data.hp = pokelib_pkmn.pkm->pkm.battle_hp;
        p_pkm->party_data.maxhp = pokelib_pkmn.pkm->pkm.battle_max_hp;
        p_pkm->party_data.attack = pokelib_pkmn.pkm->pkm.battle_atk;
        p_pkm->party_data.defense = pokelib_pkmn.pkm->pkm.battle_def;
        p_pkm->party_data.spatk = pokelib_pkmn.pkm->pkm.battle_satk;
        p_pkm->party_data.spdef = pokelib_pkmn.pkm->pkm.battle_sdef;
        p_pkm->party_data.speed = pokelib_pkmn.pkm->pkm.battle_spd;
        
        //IV's
        p_pkm->pkm_data.ivs.hp = pokelib_pkmn.pkm->pkm.iv_hp;
        p_pkm->pkm_data.ivs.attack = pokelib_pkmn.pkm->pkm.iv_atk;
        p_pkm->pkm_data.ivs.defense = pokelib_pkmn.pkm->pkm.iv_def;
        p_pkm->pkm_data.ivs.spatk = pokelib_pkmn.pkm->pkm.iv_satk;
        p_pkm->pkm_data.ivs.spdef = pokelib_pkmn.pkm->pkm.iv_sdef;
        p_pkm->pkm_data.ivs.speed = pokelib_pkmn.pkm->pkm.iv_spd;
        
        //EV's
        p_pkm->pkm_data.evs.hp = pokelib_pkmn.pkm->pkm.ev_hp;
        p_pkm->pkm_data.evs.attack = pokelib_pkmn.pkm->pkm.ev_atk;
        p_pkm->pkm_data.evs.defense = pokelib_pkmn.pkm->pkm.ev_def;
        p_pkm->pkm_data.evs.spatk = pokelib_pkmn.pkm->pkm.ev_satk;
        p_pkm->pkm_data.evs.spdef = pokelib_pkmn.pkm->pkm.ev_sdef;
        p_pkm->pkm_data.evs.speed = pokelib_pkmn.pkm->pkm.ev_spd;
    }
}
