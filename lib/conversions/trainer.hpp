/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_TRAINER_HPP
#define INCLUDED_PKMN_CONVERSIONS_TRAINER_HPP

#include <pkmn/config.hpp>
#include <pkmn/trainer.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include <rpokesav/gen1_save.hpp>
#include <pokehack/SaveParser.h>
#include <PokeLib/PokeLib.h>
#include <pkmds/pkmds_g5.h>

typedef pkmn::shared_ptr<rpokesav::gen1_save> rpokesav_gen1_sptr;
typedef pkmn::shared_ptr<PokeLib::Save> pokelib_sptr;
typedef pkmn::shared_ptr<bw2sav_obj> pkmds_g5_sptr;

namespace pkmn
{
    namespace conversions
    {
        trainer::sptr import_gen1_trainer(rpokesav_gen1_sptr sav);

        trainer::sptr import_gen4_trainer(pokelib_sptr pokelib_save);
        void export_gen4_trainer(trainer::sptr libpkmn_trainer, pokelib_sptr pokelib_trainer);

        trainer::sptr import_gen5_trainer(pkmds_g5_sptr pkmds_save);
        void export_gen5_trainer(trainer::sptr libpkmn_trainer, pkmds_g5_sptr pkmds_save);
    }
}
#endif /* INCLUDED_PKMN_CONVERSIONS_TRAINER_HPP */
