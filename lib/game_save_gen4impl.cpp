/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <boost/algorithm/string.hpp>
#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>
#include <pkmnsim/base_pkmn.hpp>
#include "PokeLib/data_tables.h"
#include "PokeLib/pokelib.h"
#include "game_save_gen4impl.hpp"
#include <stdexcept>
#include <stdlib.h>
#include <vector>

using namespace std;

namespace pkmnsim
{
    game_save_gen4impl::game_save_gen4impl(std::string filename): game_save(filename)
    {
        try
        {
            ifstream ifile(filename.c_str(), ios::binary | ios::ate);
            char buffer[0x80000];
            ifile.read(buffer, ifile.tellg());

            PokeLib::Save gen4_save(0x80000);
            gen4_save.data = (uint8_t*)&buffer;
            gen4_save.parseRawSave();

            PokeLib::Party *gen4_party = gen4_save.getParty();
            party.clear(); //PKMNsim party

            for(uint8_t i = 0; i < gen4_party->count(); i++)
            {
                PokeLib::Pokemon pkmn = gen4_party->getPokemon(i);
            }
        }
        catch(const exception &e)
        {
            cout << "Caught exception: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    spec_pkmn::sptr game_save_gen4impl::PokeLib_Pokemon_to_spec_pkmn(PokeLib::Pokemon pkmn)
    {
        PokeLib::pPKM *pkmn_structure = pkmn.getStructure();

        string identifier = PokeLib::species[pkmn_structure->species];
        int level = pkmn_structure->battle_level;
        string moves[4];
        for(int i = 0; i < 4; i++) moves[i] = PokeLib::movelist[pkmn_structure->move[i]];

        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, level, 4, moves[0], moves[1], moves[2], moves[3], true);

        //Get values to manually set s_pkmn's private variables
        PokeLib::widetext PokeLib_nickname = pkmn.getNickname();
        char nickname_buffer[11];
        wcstombs(nickname_buffer, PokeLib_nickname.c_str(), sizeof(nickname_buffer));
        s_pkmn->nickname = nickname_buffer;
        s_pkmn->HP = pkmn_structure->battle_max_hp;
        s_pkmn->ATK = pkmn_structure->battle_atk;
        s_pkmn->DEF = pkmn_structure->battle_def;
        s_pkmn->SPD = pkmn_structure->battle_spd;
        s_pkmn->SATK = pkmn_structure->battle_satk;
        s_pkmn->SDEF = pkmn_structure->battle_sdef;
        s_pkmn->ivHP = pkmn_structure->iv_hp;
        s_pkmn->ivATK = pkmn_structure->iv_atk;
        s_pkmn->ivDEF = pkmn_structure->iv_def;
        s_pkmn->ivSPD = pkmn_structure->iv_spd;
        s_pkmn->ivSATK = pkmn_structure->iv_satk;
        s_pkmn->ivSDEF = pkmn_structure->iv_sdef;
        s_pkmn->evHP = pkmn_structure->ev_hp;
        s_pkmn->evATK = pkmn_structure->ev_atk;
        s_pkmn->evDEF = pkmn_structure->ev_def;
        s_pkmn->evSPD = pkmn_structure->ev_spd;
        s_pkmn->evSATK = pkmn_structure->ev_satk;
        s_pkmn->evSDEF = pkmn_structure->ev_sdef;
        PokeLib::Gender PokeLib_gender = pkmn.getPersonalityGender();
        switch(PokeLib_gender)
        {
            case PokeLib::MALE:
                s_pkmn->gender = 'M';
                break;

            case PokeLib::FEMALE:
                s_pkmn->gender = 'F';
                break;

            default:
                s_pkmn->gender = 'N';
                break;
        }

        return s_pkmn;
    }
}
