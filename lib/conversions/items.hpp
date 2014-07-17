/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_ITEMS_HPP
#define INCLUDED_PKMN_CONVERSIONS_ITEMS_HPP

#include <cstdint>

#include <pkmn/bag.hpp>
#include <pkmn/config.hpp>

#include <rpokesav/gen1_save.hpp>
#include "../libspec/game_gba.h"
#include <PokeLib/PokeLib.h>
#include <pkmds/pkmds_g5.h>

#include "trainer.hpp"

namespace pkmn
{
    namespace conversions
    {
        void import_gen1_bag(std::array<rpokesav::gen1_item_t,20> &rpokesav_bag,
                             bag::sptr libpkmn_bag,
                             uint8_t bag_item_count);
        /*
         * TODO: Don't return bag item amount, pass in rpokesav::gen1_save and use
         * that to set item count once that function is there
         */
        uint8_t export_gen1_bag(bag::sptr libpkmn_bag,
                                std::array<rpokesav::gen1_item_t,20> &rpokesav_bag);

        void import_gen3_items(bag::sptr item_bag, gba_save_t *save);

        void import_gen4_items(bag::sptr item_bag, PokeLib::Trainer pokelib_trainer);
        void export_gen4_items(bag::sptr item_bag, PokeLib::Trainer* pokelib_trainer);

        void import_gen5_items(bag::sptr item_bag, ::bag_obj* pkmds_bag);
        void export_gen5_items(bag::sptr item_bag, ::bag_obj* pkmds_bag);
    }
}

#endif /* INCLUDED_PKMN_CONVERSIONS_ITEMS_HPP */
