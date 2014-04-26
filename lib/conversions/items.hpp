/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
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
#include <pokelib/pokelib.h>
#include <pkmds/pkmds_g5.h>

#include "trainer.hpp"

#include "../libspec/libspec.h"

namespace pkmn
{
    namespace conversions
    {
        void import_gen3_items(bag::sptr item_bag, gba_storage_t* storage, gba_savetype_t save_type);

        void PKMN_API import_items_from_rpokesav_gen1(bag::sptr item_bag, rpokesav_gen1_sptr sav);
    
        void PKMN_API import_items_from_pokelib(bag::sptr item_bag, PokeLib::Trainer pokelib_trainer);
        
        void PKMN_API export_items_to_pokelib(bag::sptr item_bag, PokeLib::Trainer* pokelib_trainer);
        
        void PKMN_API import_items_from_pkmds_g5(bag::sptr item_bag, ::bag_obj* pkmds_bag);
        
        void PKMN_API export_items_to_pkmds_g5(bag::sptr item_bag, ::bag_obj* pkmds_bag);
    }
}

#endif /* INCLUDED_PKMN_CONVERSIONS_ITEMS_HPP */
