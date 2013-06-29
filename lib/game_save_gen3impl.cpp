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
            if(pokemon_species[parser->pokemon_growth[i]->species])
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
        if(attacks[pkmn_a_t->atk2] == "No attack") move2 = "None";
        else move2 = to_database_format(attacks[pkmn_a_t->atk2]);
        if(attacks[pkmn_a_t->atk3] == "No attack") move3 = "None";
        else move3 = to_database_format(attacks[pkmn_a_t->atk3]);
        if(attacks[pkmn_a_t->atk4] == "No attack") move4 = "None";
        else move4 = to_database_format(attacks[pkmn_a_t->atk4]);

        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, 3, level,
                                                 move1, move2, move3, move4,
                                                 true);

        return s_pkmn;
    }

}
