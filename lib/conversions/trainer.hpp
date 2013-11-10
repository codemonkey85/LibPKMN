/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_CONVERSIONS_TRAINER_HPP
#define INCLUDED_PKMNSIM_CONVERSIONS_TRAINER_HPP

#include <pkmnsim/trainer.hpp>

#include <pokehack/SaveParser.h>
#include <pokelib/pokelib.h>
#include <pkmds/pkmds_g5.h>

/*
 * Trainer info and money aren't currently implemented
 * in Pokehack, so it will need to be done here for now.
 */

#define POKEHACK_PLAYER_NAME       0x0000
#define POKEHACK_PLAYER_GENDER     0x0008
#define POKEHACK_TRAINER_ID        0x000A
#define POKEHACK_TIME_PLAYED       0x000E
#define POKEHACK_GAME_CODE         0x00AC
#define POKEHACK_E_SECURITY_KEY    0x00AC
#define POKEHACK_FRLG_SECURITY_KEY 0x0AF8
#define POKEHACK_RIVAL_NAME        0x3A4C

namespace pkmnsim
{
    namespace conversions
    {
        trainer::sptr import_trainer_from_pokehack(SaveParser* parser, char* game_data);

        void export_trainer_to_pokehack(trainer::sptr pkmnsim_trainer, SaveParser* parser, char* game_data);

        /*trainer::sptr import_trainer_from_pokelib(PokeLib::Trainer pokelib_trainer);

        PokeLib::Trainer export_trainer_to_pokelib(trainer::sptr pkmnsim_trainer);

        trainer::sptr import_trainer_from_pkmds_g5(bw2savblock_obj* pkmds_save);

        void export_trainer_to_pkmds_g5(trainer::sptr pkmnsim_trainer, bw2savblock_obj* pkmds_save);*/
    }
}
#endif /* INCLUDED_PKMNSIM_CONVERSIONS_TRAINER_HPP */
