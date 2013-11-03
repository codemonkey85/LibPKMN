/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifdef _MSC_VER
#pragma warning(disable:4800)
#endif

#include <string>
#include <vector>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/bag.hpp>
#include <pkmnsim/item.hpp>
#include <pkmnsim/pocket.hpp>
#include <pkmnsim/database/queries.hpp>

#include "../SQLiteCpp/src/SQLiteC++.h"

#include "items.hpp"
#include "../library_bridge.hpp"

using namespace std;

namespace pkmnsim
{
    namespace conversions
    {
        bag::sptr import_items_from_pokehack(unsigned char* data)
        {
            /*
             * Find out which game this is
             * NOTE: This only narrows it down to RS, FRLG, or E
             */
            unsigned int game, item_offset, key_item_offset, ball_offset, tm_offset, berry_offset;
            
            switch(int(data[0xAC]))
            {
                case 0:
                    game = Games::RUBY;
                    item_offset = RS_ITEM_POCKET_OFFSET;
                    key_item_offset = RS_KEY_ITEM_POCKET_OFFSET;
                    ball_offset = RS_BALL_POCKET_OFFSET;
                    tm_offset = RS_TM_POCKET_OFFSET;
                    berry_offset = RS_BERRY_POCKET_OFFSET;
                    break;
                    
                case 1:
                    game = Games::FIRE_RED;
                    item_offset = FRLG_ITEM_POCKET_OFFSET;
                    key_item_offset = FRLG_KEY_ITEM_POCKET_OFFSET;
                    ball_offset = FRLG_BALL_POCKET_OFFSET;
                    tm_offset = FRLG_TM_POCKET_OFFSET;
                    berry_offset = FRLG_BERRY_POCKET_OFFSET;
                    break;
                    
                default:
                    game = Games::EMERALD;
                    item_offset = E_ITEM_POCKET_OFFSET;
                    key_item_offset = E_KEY_ITEM_POCKET_OFFSET;
                    ball_offset = E_BALL_POCKET_OFFSET;
                    tm_offset = E_TM_POCKET_OFFSET;
                    berry_offset = E_BERRY_POCKET_OFFSET;
            }
            
            bag::sptr item_bag = bag::make(game);
            
            pocket::sptr item_pocket = item_bag->get_pocket("Items");
            pocket::sptr key_item_pocket = item_bag->get_pocket("Key Items");
            pocket::sptr ball_pocket = item_bag->get_pocket("Poke Balls");
            
            pocket::sptr tm_pocket;
            if(game == Games::FIRE_RED) tm_pocket = item_bag->get_pocket("TM Case");
            else tm_pocket = item_bag->get_pocket("TMs and HMs");
            
            pocket::sptr berry_pocket;
            if(game == Games::FIRE_RED) berry_pocket = item_bag->get_pocket("Berry Pouch");
            else berry_pocket = item_bag->get_pocket("Berries");
            
            for(size_t i = item_offset; i < item_pocket->get_size(); i+= 4)
            {
                pokehack_item* raw_item = (pokehack_item*)(&data[i]);
                if(raw_item->index == 0) break;
                else
                {
                    unsigned int pkmnsim_item_id = database::get_item_id(raw_item->index, game);
                    item_pocket->set_item(item::make(pkmnsim_item_id, game), game);
                }
            }
            for(size_t i = key_item_offset; i < key_item_pocket->get_size(); i+= 4)
            {
                pokehack_item* raw_item = (pokehack_item*)(&data[i]);
                if(raw_item->index == 0) break;
                else
                {
                    unsigned int pkmnsim_item_id = database::get_item_id(raw_item->index, game);
                    key_item_pocket->set_item(item::make(pkmnsim_item_id, game), game);
                }
            }
            for(size_t i = ball_offset; i < ball_pocket->get_size(); i+= 4)
            {
                pokehack_item* raw_item = (pokehack_item*)(&data[i]);
                if(raw_item->index == 0) break;
                else
                {
                    unsigned int pkmnsim_item_id = database::get_item_id(raw_item->index, game);
                    ball_pocket->set_item(item::make(pkmnsim_item_id, game), game);
                }
            }
            for(size_t i = tm_offset; i < tm_pocket->get_size(); i+= 4)
            {
                pokehack_item* raw_item = (pokehack_item*)(&data[i]);
                if(raw_item->index == 0) break;
                else
                {
                    unsigned int pkmnsim_item_id = database::get_item_id(raw_item->index, game);
                    tm_pocket->set_item(item::make(pkmnsim_item_id, game), game);
                }
            }
            for(size_t i = berry_offset; i < berry_pocket->get_size(); i+= 4)
            {
                pokehack_item* raw_item = (pokehack_item*)(&data[i]);
                if(raw_item->index == 0) break;
                else
                {
                    unsigned int pkmnsim_item_id = database::get_item_id(raw_item->index, game);
                    berry_pocket->set_item(item::make(pkmnsim_item_id, game), game);
                }
            }
            
            return item_bag;
        }
    }
}
