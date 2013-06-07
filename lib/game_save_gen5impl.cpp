/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

#include <pkmnsim/base_pkmn.hpp>

#include "game_save_gen5impl.hpp"
#include "internal.hpp"

using namespace std;

namespace pkmnsim
{
    game_save_gen5impl::game_save_gen5impl(std::string filename)
    {
        try
        {
            //Determine if B,W,B2,W2

            bw2sav_obj* sav = new bw2sav_obj;
            pokemon_obj* pkm = new pokemon_obj;
            read(filename.c_str(),sav);
            opendb(get_database_path().c_str());

            for(unsigned int i = 0; i < sav->cur.party.size; i++)
            {
                pkm = &(sav->cur.party.pokemon[i].pkm_data);
                decryptpkm(pkm);
                party.push_back(pkm_to_spec_pkmn(pkm));
            }
        }
        catch(const exception &e)
        {
            cout << "Caught exception: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    spec_pkmn::sptr game_save_gen5impl::pkm_to_spec_pkmn(pokemon_obj* pkm)
    {
        //Easy access to certain data
        pkmblocka blkA = pkm->blocka;
        pkmblockb blkB = pkm->blockb;
        pkmblockc blkC = pkm->blockc;
        pkmblockd blkD = pkm->blockd;

        //9 is the (now unnecessary) local_language_id for English in the SQLite database
        string identifier = lookuppkmname(blkA.species, 9);
        int level = getpkmlevel(blkA.species, blkA.exp);
        string moves[4];
        for(int i = 0; i < 4; i++) moves[i] = lookupmovename(blkB.moves[i], 9);
        //TODO: figure out what PKMDS does with an empty move slot

        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, level, 5, moves[0], moves[1], moves[2], moves[3], true);

        //Get values to manually set s_pkmn's private values
        char nickname_buf[11];
        s_pkmn->nickname = wcstombs(nickname_buf, blkC.nickname, sizeof(nickname_buf));
        s_pkmn->HP = getpkmstat(pkm, Stat_IDs::hp);
        s_pkmn->ATK = getpkmstat(pkm, Stat_IDs::attack);
        s_pkmn->DEF = getpkmstat(pkm, Stat_IDs::defense);
        s_pkmn->SPD = getpkmstat(pkm, Stat_IDs::speed);
        s_pkmn->SATK = getpkmstat(pkm, Stat_IDs::spatk);
        s_pkmn->SDEF = getpkmstat(pkm, Stat_IDs::spdef);
        s_pkmn->ivHP = blkB.ivs.hp;
        s_pkmn->ivATK = blkB.ivs.attack;
        s_pkmn->ivDEF = blkB.ivs.defense;
        s_pkmn->ivSPD = blkB.ivs.speed;
        s_pkmn->ivSATK = blkB.ivs.spatk;
        s_pkmn->ivSDEF = blkB.ivs.spdef;
        s_pkmn->evHP = blkA.evs.hp;
        s_pkmn->evATK = blkA.evs.atk;
        s_pkmn->evDEF = blkA.evs.def;
        s_pkmn->evSPD = blkA.evs.spd;
        s_pkmn->evSATK = blkA.evs.spatk;
        s_pkmn->evSDEF = blkA.evs.spdef;
        if(blkB.forms.female) s_pkmn->gender = 'F';
        else if(blkB.forms.genderless) s_pkmn->gender = 'N';
        else s_pkmn->gender = 'M';

        return s_pkmn;
    }
}
