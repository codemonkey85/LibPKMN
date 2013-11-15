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
        void import_items_from_pokehack(bag::sptr item_bag, unsigned char* data)
        {
            /*
             * Find out which game this is
             * NOTE: This only narrows it down to RS, FRLG, or E
             */
            unsigned int game = item_bag->get_game_id();
            unsigned int item_offset, key_item_offset, ball_offset, tm_offset, berry_offset;
            
            switch(game)
            {
                case Games::RUBY:
                case Games::SAPPHIRE:
                    item_offset = RS_ITEM_POCKET_OFFSET;
                    key_item_offset = RS_KEY_ITEM_POCKET_OFFSET;
                    ball_offset = RS_BALL_POCKET_OFFSET;
                    tm_offset = RS_TM_POCKET_OFFSET;
                    berry_offset = RS_BERRY_POCKET_OFFSET;
                    break;
                    
                case Games::FIRE_RED:
                case Games::LEAF_GREEN:
                    item_offset = FRLG_ITEM_POCKET_OFFSET;
                    key_item_offset = FRLG_KEY_ITEM_POCKET_OFFSET;
                    ball_offset = FRLG_BALL_POCKET_OFFSET;
                    tm_offset = FRLG_TM_POCKET_OFFSET;
                    berry_offset = FRLG_BERRY_POCKET_OFFSET;
                    break;
                    
                default:
                    item_offset = E_ITEM_POCKET_OFFSET;
                    key_item_offset = E_KEY_ITEM_POCKET_OFFSET;
                    ball_offset = E_BALL_POCKET_OFFSET;
                    tm_offset = E_TM_POCKET_OFFSET;
                    berry_offset = E_BERRY_POCKET_OFFSET;
            }
            
            pocket::sptr item_pocket = item_bag->get_pocket("Items");
            pocket::sptr key_item_pocket = item_bag->get_pocket("Key Items");
            pocket::sptr ball_pocket = item_bag->get_pocket("Poke Balls");
            
            pocket::sptr tm_pocket;
            if(game == Games::FIRE_RED or game == Games::LEAF_GREEN) tm_pocket = item_bag->get_pocket("TM Case");
            else tm_pocket = item_bag->get_pocket("TMs and HMs");
            
            pocket::sptr berry_pocket;
            if(game == Games::FIRE_RED or game == Games::LEAF_GREEN) berry_pocket = item_bag->get_pocket("Berry Pouch");
            else berry_pocket = item_bag->get_pocket("Berries");
            
            for(size_t i = item_offset; i < item_pocket->get_size(); i+= 4)
            {
                pokehack_item* raw_item = reinterpret_cast<pokehack_item*>(&data[i]);
                if(raw_item->index == 0) break;
                else
                {
                    unsigned int pkmnsim_item_id = database::get_item_id(raw_item->index, game);
                    item_pocket->set_item(item::make(pkmnsim_item_id, game), raw_item->quantity);
                }
            }
            for(size_t i = key_item_offset; i < key_item_pocket->get_size(); i+= 4)
            {
                pokehack_item* raw_item = reinterpret_cast<pokehack_item*>(&data[i]);
                if(raw_item->index == 0) break;
                else
                {
                    unsigned int pkmnsim_item_id = database::get_item_id(raw_item->index, game);
                    key_item_pocket->set_item(item::make(pkmnsim_item_id, game), raw_item->quantity);
                }
            }
            for(size_t i = ball_offset; i < ball_pocket->get_size(); i+= 4)
            {
                pokehack_item* raw_item = (pokehack_item*)(&data[i]);
                if(raw_item->index == 0) break;
                else
                {
                    unsigned int pkmnsim_item_id = database::get_item_id(raw_item->index, game);
                    ball_pocket->set_item(item::make(pkmnsim_item_id, game), raw_item->quantity);
                }
            }
            for(size_t i = tm_offset; i < tm_pocket->get_size(); i+= 4)
            {
                pokehack_item* raw_item = reinterpret_cast<pokehack_item*>(&data[i]);
                if(raw_item->index == 0) break;
                else
                {
                    unsigned int pkmnsim_item_id = database::get_item_id(raw_item->index, game);
                    tm_pocket->set_item(item::make(pkmnsim_item_id, game), raw_item->quantity);
                }
            }
            for(size_t i = berry_offset; i < berry_pocket->get_size(); i+= 4)
            {
                pokehack_item* raw_item = reinterpret_cast<pokehack_item*>(&data[i]);
                if(raw_item->index == 0) break;
                else
                {
                    unsigned int pkmnsim_item_id = database::get_item_id(raw_item->index, game);
                    berry_pocket->set_item(item::make(pkmnsim_item_id, game), raw_item->quantity);
                }
            }
        }
        
        void export_items_to_pokehack(bag::sptr item_bag, unsigned char* data)
        {
            unsigned int game, item_offset, key_item_offset, ball_offset, tm_offset, berry_offset;
            game = item_bag->get_game_id();
        
            switch(game)
            {
                case Games::RUBY:
                case Games::SAPPHIRE:
                    item_offset = RS_ITEM_POCKET_OFFSET;
                    key_item_offset = RS_KEY_ITEM_POCKET_OFFSET;
                    ball_offset = RS_BALL_POCKET_OFFSET;
                    tm_offset = RS_TM_POCKET_OFFSET;
                    berry_offset = RS_BERRY_POCKET_OFFSET;
                    break;
                    
                case Games::FIRE_RED:
                case Games::LEAF_GREEN:
                    item_offset = FRLG_ITEM_POCKET_OFFSET;
                    key_item_offset = FRLG_KEY_ITEM_POCKET_OFFSET;
                    ball_offset = FRLG_BALL_POCKET_OFFSET;
                    tm_offset = FRLG_TM_POCKET_OFFSET;
                    berry_offset = FRLG_BERRY_POCKET_OFFSET;
                    break;
                    
                default:
                    item_offset = E_ITEM_POCKET_OFFSET;
                    key_item_offset = E_KEY_ITEM_POCKET_OFFSET;
                    ball_offset = E_BALL_POCKET_OFFSET;
                    tm_offset = E_TM_POCKET_OFFSET;
                    berry_offset = E_BERRY_POCKET_OFFSET;
            }
            
            pocket::sptr item_pocket = item_bag->get_pocket("Items");
            pocket::sptr key_item_pocket = item_bag->get_pocket("Key Items");
            pocket::sptr ball_pocket = item_bag->get_pocket("Poke Balls");
            
            pocket::sptr tm_pocket;
            if(game == Games::FIRE_RED) tm_pocket = item_bag->get_pocket("TM Case");
            else tm_pocket = item_bag->get_pocket("TMs and HMs");
            
            pocket::sptr berry_pocket;
            if(game == Games::FIRE_RED) berry_pocket = item_bag->get_pocket("Berry Pouch");
            else berry_pocket = item_bag->get_pocket("Berries");
            
            for(size_t i = 0; i < item_pocket->get_size(); i++)
            {
                pokehack_item* raw_item = reinterpret_cast<pokehack_item*>(&data[item_offset+(i*4)]);
                unsigned int item_id = item_pocket->get_item(i+1)->get_item_id();
                
                if(item_id == Items::NONE) break;
                else
                {
                    raw_item->index = database::get_item_index(item_id, game);
                    raw_item->quantity = item_pocket->get_amount(i);
                }
            }
            for(size_t i = 0; i < key_item_pocket->get_size(); i++)
            {
                pokehack_item* raw_item = reinterpret_cast<pokehack_item*>(&data[key_item_offset+(i*4)]);
                unsigned int item_id = key_item_pocket->get_item(i+1)->get_item_id();
                
                if(item_id == Items::NONE) break;
                else
                {
                    raw_item->index = database::get_item_index(item_id, game);
                    raw_item->quantity = 1;
                }
            }
            for(size_t i = 0; i < ball_pocket->get_size(); i++)
            {
                pokehack_item* raw_item = reinterpret_cast<pokehack_item*>(&data[ball_offset+(i*4)]);
                unsigned int item_id = ball_pocket->get_item(i+1)->get_item_id();
                
                if(item_id == Items::NONE) break;
                else
                {
                    raw_item->index = database::get_item_index(item_id, game);
                    raw_item->quantity = 1;
                }
            }
            for(size_t i = 0; i < tm_pocket->get_size(); i++)
            {
                pokehack_item* raw_item = reinterpret_cast<pokehack_item*>(&data[tm_offset+(i*4)]);
                unsigned int item_id = tm_pocket->get_item(i+1)->get_item_id();
                
                if(item_id == Items::NONE) break;
                else
                {
                    raw_item->index = database::get_item_index(item_id, game);
                    raw_item->quantity = 1;
                }
            }
            for(size_t i = 0; i < berry_pocket->get_size(); i++)
            {
                pokehack_item* raw_item = reinterpret_cast<pokehack_item*>(&data[berry_offset+(i*4)]);
                unsigned int item_id = berry_pocket->get_item(i+1)->get_item_id();
                
                if(item_id == Items::NONE) break;
                else
                {
                    raw_item->index = database::get_item_index(item_id, game);
                    raw_item->quantity = 1;
                }
            }
        }
        
        void import_items_from_pokelib(bag::sptr item_bag, PokeLib::Trainer pokelib_trainer)
        {
            unsigned int game_id = item_bag->get_game_id();

            pocket::sptr item_pocket = item_bag->get_pocket("Items");
            pocket::sptr medicine_pocket = item_bag->get_pocket("Medicine");
            pocket::sptr ball_pocket = item_bag->get_pocket("Poke Balls");
            pocket::sptr tm_pocket = item_bag->get_pocket("TMs and HMs");
            pocket::sptr berry_pocket = item_bag->get_pocket("Berries");
            pocket::sptr mail_pocket = item_bag->get_pocket("Mail");
            pocket::sptr battle_item_pocket = item_bag->get_pocket("Battle Items");
            pocket::sptr key_item_pocket = item_bag->get_pocket("Key Items");
            
            for(size_t i = 0; i < item_pocket->get_size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemGeneral, i+1);
                
                if(pokelib_bagitem.item == 0) break;
                else item_pocket->set_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                           pokelib_bagitem.count);
            }
            for(size_t i = 0; i < medicine_pocket->get_size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemMedicine, i+1);
                
                if(pokelib_bagitem.item == 0) break;
                else medicine_pocket->set_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                               pokelib_bagitem.count);
            }
            for(size_t i = 0; i < ball_pocket->get_size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemBall, i+1);
                
                if(pokelib_bagitem.item == 0) break;
                else ball_pocket->set_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                           pokelib_bagitem.count);
            }
            for(size_t i = 0; i < tm_pocket->get_size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemTM, i+1);
                
                if(pokelib_bagitem.item == 0) break;
                else tm_pocket->set_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                         pokelib_bagitem.count);
            }
            for(size_t i = 0; i < berry_pocket->get_size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemBerry, i+1);
                
                if(pokelib_bagitem.item == 0) break;
                else berry_pocket->set_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                            pokelib_bagitem.count);
            }
            for(size_t i = 0; i < mail_pocket->get_size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemMail, i+1);
                
                if(pokelib_bagitem.item == 0) break;
                else mail_pocket->set_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                           pokelib_bagitem.count);
            }
            for(size_t i = 0; i < battle_item_pocket->get_size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemBattle, i+1);
                
                if(pokelib_bagitem.item == 0) break;
                else battle_item_pocket->set_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                                  pokelib_bagitem.count);
            }
            for(size_t i = 0; i < key_item_pocket->get_size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemKey, i+1);
                
                if(pokelib_bagitem.item == 0) break;
                else key_item_pocket->set_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                               pokelib_bagitem.count);
            }
        }
        
        void export_items_to_pokelib(bag::sptr item_bag, PokeLib::Trainer* pokelib_trainer)
        {
            unsigned int game_id = item_bag->get_game_id();

            pocket::sptr item_pocket = item_bag->get_pocket("Items");
            pocket::sptr medicine_pocket = item_bag->get_pocket("Medicine");
            pocket::sptr ball_pocket = item_bag->get_pocket("Poke Balls");
            pocket::sptr tm_pocket = item_bag->get_pocket("TMs and HMs");
            pocket::sptr berry_pocket = item_bag->get_pocket("Berries");
            pocket::sptr mail_pocket = item_bag->get_pocket("Mail");
            pocket::sptr battle_item_pocket = item_bag->get_pocket("Battle Items");
            pocket::sptr key_item_pocket = item_bag->get_pocket("Key Items");
            
            for(size_t i = 0; i < item_pocket->get_size(); i++)
            {
                unsigned int item_id = item_pocket->get_item(i+1)->get_item_id();
                unsigned int amount = item_pocket->get_amount(i+1);
            
                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_index(item_id, game_id);
                    pokelib_bagitem.count = amount;
                    
                    pokelib_trainer->setItem(PokeLib::ItemGeneral, (i+1), pokelib_bagitem);
                }
            }
            for(size_t i = 0; i < medicine_pocket->get_size(); i++)
            {
                unsigned int item_id = medicine_pocket->get_item(i+1)->get_item_id();
                unsigned int amount = medicine_pocket->get_amount(i+1);
            
                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_index(item_id, game_id);
                    pokelib_bagitem.count = amount;
                    
                    pokelib_trainer->setItem(PokeLib::ItemMedicine, (i+1), pokelib_bagitem);
                }
            }
            for(size_t i = 0; i < ball_pocket->get_size(); i++)
            {
                unsigned int item_id = ball_pocket->get_item(i+1)->get_item_id();
                unsigned int amount = ball_pocket->get_amount(i+1);
            
                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_index(item_id, game_id);
                    pokelib_bagitem.count = amount;
                    
                    pokelib_trainer->setItem(PokeLib::ItemBall, (i+1), pokelib_bagitem);
                }
            }
            for(size_t i = 0; i < tm_pocket->get_size(); i++)
            {
                unsigned int item_id = tm_pocket->get_item(i+1)->get_item_id();
                unsigned int amount = tm_pocket->get_amount(i+1);
            
                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_index(item_id, game_id);
                    pokelib_bagitem.count = amount;
                    
                    pokelib_trainer->setItem(PokeLib::ItemTM, (i+1), pokelib_bagitem);
                }
            }
            for(size_t i = 0; i < berry_pocket->get_size(); i++)
            {
                unsigned int item_id = berry_pocket->get_item(i+1)->get_item_id();
                unsigned int amount = berry_pocket->get_amount(i+1);
            
                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_index(item_id, game_id);
                    pokelib_bagitem.count = amount;
                    
                    pokelib_trainer->setItem(PokeLib::ItemBerry, (i+1), pokelib_bagitem);
                }
            }
            for(size_t i = 0; i < mail_pocket->get_size(); i++)
            {
                unsigned int item_id = mail_pocket->get_item(i+1)->get_item_id();
                unsigned int amount = mail_pocket->get_amount(i+1);
            
                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_index(item_id, game_id);
                    pokelib_bagitem.count = amount;
                    
                    pokelib_trainer->setItem(PokeLib::ItemMail, (i+1), pokelib_bagitem);
                }
            }
            for(size_t i = 0; i < battle_item_pocket->get_size(); i++)
            {
                unsigned int item_id = battle_item_pocket->get_item(i+1)->get_item_id();
                unsigned int amount = battle_item_pocket->get_amount(i+1);
            
                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_index(item_id, game_id);
                    pokelib_bagitem.count = amount;
                    
                    pokelib_trainer->setItem(PokeLib::ItemBattle, (i+1), pokelib_bagitem);
                }
            }
            for(size_t i = 0; i < key_item_pocket->get_size(); i++)
            {
                unsigned int item_id = key_item_pocket->get_item(i+1)->get_item_id();
                unsigned int amount = key_item_pocket->get_amount(i+1);
            
                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_index(item_id, game_id);
                    pokelib_bagitem.count = amount;
                    
                    pokelib_trainer->setItem(PokeLib::ItemKey, (i+1), pokelib_bagitem);
                }
            }
        }
        
        void import_items_from_pkmds_g5(bag::sptr item_bag, ::bag_obj* pkmds_bag)
        {
            unsigned int game_id = item_bag->get_game_id();
            
            pocket::sptr item_pocket = item_bag->get_pocket("Items");
            pocket::sptr medicine_pocket = item_bag->get_pocket("Medicine");
            pocket::sptr tm_pocket = item_bag->get_pocket("TMs and HMs");
            pocket::sptr berry_pocket = item_bag->get_pocket("Berries");
            pocket::sptr key_item_pocket = item_bag->get_pocket("Key Items");
            
            for(size_t i = 0; i < ITEMS_POCKET_SIZE; i++)
            {
                ::item_obj pkmds_item = pkmds_bag->items_pocket[i];
            
                if(pkmds_item.id == ::Items::NOTHING) break;
                else item_pocket->set_item(database::get_item_id(pkmds_item.id, game_id),
                                           pkmds_item.quantity);
            }
            for(size_t i = 0; i < MEDICINE_POCKET_SIZE; i++)
            {
                ::item_obj pkmds_item = pkmds_bag->medicine_pocket[i];
            
                if(pkmds_item.id == ::Items::NOTHING) break;
                else medicine_pocket->set_item(item::make(pkmds_item.id, game_id),
                                               pkmds_item.quantity);
            }
            for(size_t i = 0; i < TMS_POCKET_SIZE; i++)
            {
                ::item_obj pkmds_item = pkmds_bag->tms_pocket[i];
            
                if(pkmds_item.id == ::Items::NOTHING) break;
                else tm_pocket->set_item(database::get_item_id(pkmds_item.id, game_id),
                                         pkmds_item.quantity);
            }
            for(size_t i = 0; i < BERRIES_POCKET_SIZE; i++)
            {
                ::item_obj pkmds_item = pkmds_bag->berries_pocket[i];
            
                if(pkmds_item.id == ::Items::NOTHING) break;
                else berry_pocket->set_item(database::get_item_id(pkmds_item.id, game_id),
                                            pkmds_item.quantity);
            }
            for(size_t i = 0; i < KEYITEMS_POCKET_SIZE; i++)
            {
                ::item_obj pkmds_item = pkmds_bag->keyitems_pocket[i];
            
                if(pkmds_item.id == ::Items::NOTHING) break;
                else key_item_pocket->set_item(database::get_item_id(pkmds_item.id, game_id),
                                               1);
            }
        }
        
        void export_items_to_pkmds_g5(bag::sptr item_bag, ::bag_obj* pkmds_bag)
        {
            unsigned int game_id = item_bag->get_game_id();

            pocket::sptr item_pocket = item_bag->get_pocket("Items");
            pocket::sptr medicine_pocket = item_bag->get_pocket("Medicine");
            pocket::sptr tm_pocket = item_bag->get_pocket("TMs and HMs");
            pocket::sptr berry_pocket = item_bag->get_pocket("Berries");
            pocket::sptr key_item_pocket = item_bag->get_pocket("Key Items");
            
            for(size_t i = 0; i < item_pocket->get_size(); i++)
            {
                unsigned int item_id = item_pocket->get_item(i+1)->get_item_id();
                item_id = database::get_item_index(item_id, item_bag->get_game_id());
                unsigned int amount = item_pocket->get_amount(i+1);
                
                if(item_id == Items::NONE) break;
                else
                {
                    pkmds_bag->items_pocket[i].id = ::Items::items(item_id);
                    pkmds_bag->items_pocket[i].quantity = amount;
                }
            }
            for(size_t i = 0; i < medicine_pocket->get_size(); i++)
            {
                unsigned int item_id = medicine_pocket->get_item(i+1)->get_item_id();
                item_id = database::get_item_index(item_id, item_bag->get_game_id());
                unsigned int amount = medicine_pocket->get_amount(i+1);
                
                if(item_id == Items::NONE) break;
                else
                {
                    pkmds_bag->medicine_pocket[i].id = ::Items::items(item_id);
                    pkmds_bag->medicine_pocket[i].quantity = amount;
                }
            }
            for(size_t i = 0; i < tm_pocket->get_size(); i++)
            {
                unsigned int item_id = tm_pocket->get_item(i+1)->get_item_id();
                item_id = database::get_item_index(item_id, item_bag->get_game_id());
                unsigned int amount = tm_pocket->get_amount(i+1);
                
                if(item_id == Items::NONE) break;
                else
                {
                    pkmds_bag->tms_pocket[i].id = ::Items::items(item_id);
                    pkmds_bag->tms_pocket[i].quantity = amount;
                }
            }
            for(size_t i = 0; i < berry_pocket->get_size(); i++)
            {
                unsigned int item_id = berry_pocket->get_item(i+1)->get_item_id();
                item_id = database::get_item_index(item_id, item_bag->get_game_id());
                unsigned int amount = berry_pocket->get_amount(i+1);
                
                if(item_id == Items::NONE) break;
                else
                {
                    pkmds_bag->berries_pocket[i].id = ::Items::items(item_id);
                    pkmds_bag->berries_pocket[i].quantity = amount;
                }
            }
            for(size_t i = 0; i < berry_pocket->get_size(); i++)
            {
                unsigned int item_id = key_item_pocket->get_item(i+1)->get_item_id();
                item_id = database::get_item_index(item_id, item_bag->get_game_id());
                
                if(item_id == Items::NONE) break;
                else
                {
                    pkmds_bag->keyitems_pocket[i].id = ::Items::items(item_id);
                    pkmds_bag->keyitems_pocket[i].quantity = 1;
                }
            }
        }
    }
}
