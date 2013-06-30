/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>

#include "game_save_gen3impl.hpp"

using namespace std;

namespace pkmnsim
{
    game_save_gen3impl::game_save_gen3impl(string filename, int game): game_save()
    {
        parser = SaveParser::Instance();

        int game_type;

        switch(game)
        {
            case Games::RUBY:
            case Games::SAPPHIRE:
            case Games::EMERALD:
                game_type = 0;
                break;

            case Games::FIRE_RED:
            case Games::LEAF_GREEN:
                game_type = 1;
                break;

            default:
                cerr << "Only Ruby, Sapphire, Emerald, Fire Red, and Leaf Green currently supported." << endl;
                exit(EXIT_FAILURE);
        }

        int err = parser->load(filename.c_str(), game_type);
        if(err)
        {
            cerr << "Error loading game save file." << endl;
            exit(EXIT_FAILURE);
        }

        money = 0; //Placeholder until money is implemented in Pokehack
        trainer_name = "Ash"; //Placeholder until trainer name is implemented in Pokehack

        party.clear();
        for(int i = 0; i < 6; i++)
            if(string(pokemon_species[parser->pokemon_growth[i]->species]) != "No pokemon")
                party.push_back(convert_to_spec_pkmn(parser->pokemon[i],
                                                     parser->pokemon_attacks[i],
                                                     parser->pokemon_effort[i],
                                                     parser->pokemon_misc[i],
                                                     parser->pokemon_growth[i]));
    }

    spec_pkmn::sptr game_save_gen3impl::convert_to_spec_pkmn(belt_pokemon_t* b_pkmn_t,
                                                             pokemon_attacks_t* pkmn_a_t,
                                                             pokemon_effort_t* pkmn_e_t,
                                                             pokemon_misc_t* pkmn_m_t,
                                                             pokemon_growth_t* pkmn_g_t)
    {
        string identifier, move1, move2, move3, move4;
        int level;

        identifier = pokemon_species[pkmn_g_t->species];
        level = b_pkmn_t->level;

        move1 = to_database_format(attacks[pkmn_a_t->atk1]);
        if(string(attacks[pkmn_a_t->atk2]) == "No attack") move2 = "None";
        else move2 = to_database_format(attacks[pkmn_a_t->atk2]);
        if(string(attacks[pkmn_a_t->atk3]) == "No attack") move3 = "None";
        else move3 = to_database_format(attacks[pkmn_a_t->atk3]);
        if(string(attacks[pkmn_a_t->atk4]) == "No attack") move4 = "None";
        else move4 = to_database_format(attacks[pkmn_a_t->atk4]);

        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, 3, level,
                                                 move1, move2, move3, move4,
                                                 true);

        s_pkmn->held_item = items[pkmn_g_t->held];

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

}
