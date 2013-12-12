/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/pocket.hpp>

#include "bag_impl.hpp"

using namespace std;

namespace pkmnsim
{
    bag::sptr bag::make(unsigned int game)
    {
        return sptr(new bag_impl(game));
    }
    
    bag_impl::bag_impl(unsigned int game): bag()
    {
        _game_id = game;
        _contents = dict<string, pocket::sptr>();
    
        switch(game)
        {
            case Games::RED:
            case Games::BLUE:
            case Games::YELLOW:
                _contents["Items"] = pocket::make(_game_id, "Items", 20);
                break;
                
            case Games::GOLD:
            case Games::SILVER:
            case Games::CRYSTAL:
                _contents["Items"] = pocket::make(_game_id, "Items", 20);
                _contents["Balls"] = pocket::make(_game_id, "Balls", 12);
                _contents["KeyItems"] = pocket::make(_game_id, "KeyItems", 26);
                _contents["TM/HM"] = pocket::make(_game_id, "TM/HM", 99);
                break;
                
            case Games::RUBY:
            case Games::SAPPHIRE:
                _contents["Items"] = pocket::make(_game_id, "Items", 20);
                _contents["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 16);
                _contents["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 64);
                _contents["Berries"] = pocket::make(_game_id, "Berries", 46);
                _contents["Key Items"] = pocket::make(_game_id, "Key Items", 20);
                break;
            
            case Games::EMERALD:
                _contents["Items"] = pocket::make(_game_id, "Items", 20);
                _contents["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 16);
                _contents["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 64);
                _contents["Berries"] = pocket::make(_game_id, "Berries", 46);
                _contents["Key Items"] = pocket::make(_game_id, "Key Items", 30);
                break;
                
            case Games::FIRE_RED:
            case Games::LEAF_GREEN:
                _contents["Items"] = pocket::make(_game_id, "Items", 42);
                _contents["Key Items"] = pocket::make(_game_id, "Key Items", 30);
                _contents["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 13);
                _contents["TM Case"] = pocket::make(_game_id, "TM Case", 58);
                _contents["Berry Pouch"] = pocket::make(_game_id, "Berry Pouch", 43);
                break;
            
            case Games::DIAMOND:
            case Games::PEARL:
            case Games::PLATINUM:
            case Games::HEART_GOLD:
            case Games::SOUL_SILVER:
                //"Infinite" space in each pocket, but 100 should do
                _contents["Items"] = pocket::make(_game_id, "Items", 100);
                _contents["Medicine"] = pocket::make(_game_id, "Medicine", 100);
                _contents["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 100);
                _contents["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 100);
                _contents["Berries"] = pocket::make(_game_id, "Berries", 100);
                _contents["Mail"] = pocket::make(_game_id, "Mail", 100);
                _contents["Battle Items"] = pocket::make(_game_id, "Battle Items", 100);
                _contents["Key Items"] = pocket::make(_game_id, "Key Items", 100);
                break;
                
            case Games::BLACK:
            case Games::WHITE:
            case Games::BLACK2:
            case Games::WHITE2:
                _contents["Items"] = pocket::make(_game_id, "Items", 310);
                _contents["Medicine"] = pocket::make(_game_id, "Medicine", 48);
                _contents["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 109);
                _contents["Berries"] = pocket::make(_game_id, "Berries", 64);
                _contents["Key Items"] = pocket::make(_game_id, "Key Items", 83);
            
            default:
                break;
        }
    }
    
    unsigned int bag_impl::get_game_id() {return _game_id;}
    
    const pocket::sptr bag_impl::get_pocket(string name) {return _contents[name];}
    
    void bag_impl::get_pocket_list(vector<string> &pocket_vec) {pocket_vec = _contents.keys();}
} /* namespace pkmnsim */
