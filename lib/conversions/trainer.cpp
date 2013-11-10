/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>

#include <pkmnsim/enums.hpp>

#include "../library_bridge.hpp"
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

            string trainer_name = parser->get_text((unsigned char*)&(game_data[POKEHACK_PLAYER_NAME]), false);
            bool trainer_is_female = game_data[POKEHACK_PLAYER_GENDER];
            unsigned int pkmnsim_gender;
            
            if(trainer_is_female) pkmnsim_gender = Genders::FEMALE;
            else pkmnsim_gender = Genders::MALE;

            trainer::sptr pkmnsim_trainer = trainer::make(pkmnsim_game_id, trainer_name, pkmnsim_gender);

            return pkmnsim_trainer;
        }
    }
}
