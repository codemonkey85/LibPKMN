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
#include <pokehack/pokestructs.h>
#include <pokelib/pokelib.h>
#include <pkmds/pkmds_g5.h>

#include "trainer.hpp"

/*
 * Items aren't currently implemented in Pokehack, so
 * they will need to be done here for now.
 */

#define RS_ITEM_POCKET_OFFSET 0x560+3968
#define RS_KEY_ITEM_POCKET_OFFSET 0x5B0+3968
#define RS_BALL_POCKET_OFFSET 0x600+3968
#define RS_TM_POCKET_OFFSET 0x640+3968
#define RS_BERRY_POCKET_OFFSET 0x740+3968

#define E_ITEM_POCKET_OFFSET 0x560+3968
#define E_KEY_ITEM_POCKET_OFFSET 0x5D8+3968
#define E_BALL_POCKET_OFFSET 0x650+3968
#define E_TM_POCKET_OFFSET 0x690+3968
#define E_BERRY_POCKET_OFFSET 0x790+3968

#define FRLG_ITEM_POCKET_OFFSET 0x310+3968
#define FRLG_KEY_ITEM_POCKET_OFFSET 0x3B8+3968
#define FRLG_BALL_POCKET_OFFSET 0x430+3968
#define FRLG_TM_POCKET_OFFSET 0x464+3968
#define FRLG_BERRY_POCKET_OFFSET 0x54C+3968

namespace pkmn
{
    namespace conversions
    {
        struct pokehack_item
        {
            uint16_t index;
            uint16_t quantity;
        };

        void PKMN_API import_items_from_rpokesav_gen1(bag::sptr item_bag, rpokesav_gen1_sptr sav);
    
        void PKMN_API import_items_from_pokehack(bag::sptr item_bag, unsigned char* data);
        
        void PKMN_API export_items_to_pokehack(bag::sptr item_bag, unsigned char* data);
        
        void PKMN_API import_items_from_pokelib(bag::sptr item_bag, PokeLib::Trainer pokelib_trainer);
        
        void PKMN_API export_items_to_pokelib(bag::sptr item_bag, PokeLib::Trainer* pokelib_trainer);
        
        void PKMN_API import_items_from_pkmds_g5(bag::sptr item_bag, ::bag_obj* pkmds_bag);
        
        void PKMN_API export_items_to_pkmds_g5(bag::sptr item_bag, ::bag_obj* pkmds_bag);
    }
}

#endif /* INCLUDED_PKMN_CONVERSIONS_ITEMS_HPP */