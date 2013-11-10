/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_CONVERSIONS_ITEMS_HPP
#define INCLUDED_PKMNSIM_CONVERSIONS_ITEMS_HPP

#include <cstdint>

#include <pkmnsim/bag.hpp>

#include <pokehack/pokestructs.h>
#include <pokelib/pokelib.h>
#include <pkmds/pkmds_g5.h>

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

namespace pkmnsim
{
    namespace conversions
    {
        struct pokehack_item
        {
            uint16_t index;
            uint16_t quantity;
        };
    
        bag::sptr import_items_from_pokehack(unsigned char* data);
        
        void export_items_to_pokehack(bag::sptr item_bag, unsigned char* data);
        
        bag::sptr import_items_from_pokelib(PokeLib::Trainer pokelib_trainer, unsigned int game_id);
        
        void export_items_to_pokelib(bag::sptr item_bag, PokeLib::Trainer* pokelib_trainer, unsigned int game_id);
        
        bag::sptr import_items_from_pkmds_g5(::bag_obj* pkmds_bag, unsigned int game_id);
        
        void export_items_to_pkmds_g5(bag::sptr item_bag, ::bag_obj* pkmds_bag);
    }
}

#endif /* INCLUDED_PKMNSIM_CONVERSIONS_ITEMS_HPP */
