/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>

#include <pkmnsim/enums.hpp>

#include "../library_bridge.hpp"
#include "items.hpp"
#include "pokemon.hpp"
#include "trainer.hpp"

using namespace std;

namespace pkmnsim
{
    namespace conversions
    {
        trainer::sptr import_trainer_from_pokehack(SaveParser* parser, char* game_data)
        {
            unsigned int game_code = game_data[POKEHACK_GAME_CODE];
            unsigned int pkmnsim_game_id = 0;

            switch(game_code)
            {
                case 0:
                    pkmnsim_game_id = Games::RUBY; //TODO: distinguish between Ruby/Sapphire
                    break;

                case 1:
                    pkmnsim_game_id = Games::FIRE_RED; //TODO: distinguish between FR/LG
                    break;

                default:
                    pkmnsim_game_id = Games::EMERALD;
                    break;
            }

            string trainer_name = parser->get_text(reinterpret_cast<unsigned char*>(&(game_data[POKEHACK_PLAYER_NAME])), false);
            bool trainer_is_female = game_data[POKEHACK_PLAYER_GENDER];
            unsigned int pkmnsim_gender;
            
            if(trainer_is_female) pkmnsim_gender = Genders::FEMALE;
            else pkmnsim_gender = Genders::MALE;

            trainer::sptr pkmnsim_trainer = trainer::make(pkmnsim_game_id, trainer_name, pkmnsim_gender);

            pkmnsim_trainer->set_id(*(reinterpret_cast<uint32_t*>(&game_data[POKEHACK_TRAINER_ID])));
            pkmnsim_trainer->set_money(0); //Currently unimplemented in Pokehack

            bag::sptr bag_from_pokehack = import_items_from_pokehack(reinterpret_cast<unsigned char*>(game_data));
            bag::sptr trainer_bag = pkmnsim_trainer->get_bag();
            trainer_bag = bag_from_pokehack;

            for(size_t i = 0; i < 6; i++)
            {
                if(parser->pokemon_growth[i]->species != 0)
                {
                    team_pokemon::sptr t_pkmn = pokehack_pokemon_to_team_pokemon(parser->pokemon[i],
                                                                                 parser->pokemon_attacks[i],
                                                                                 parser->pokemon_effort[i],
                                                                                 parser->pokemon_misc[i],
                                                                                 parser->pokemon_growth[i]);
                    pkmnsim_trainer->set_pokemon(i+1, t_pkmn);
                }
            }

            return pkmnsim_trainer;
        }
    }
}
