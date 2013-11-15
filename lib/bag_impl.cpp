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
        game_id = game;
        contents = dict<string, pocket::sptr>();
    
        switch(game)
        {
            case Games::RED:
            case Games::BLUE:
            case Games::YELLOW:
                contents["Items"] = pocket::make(game_id, "Items", 20);
                break;
                
            case Games::GOLD:
            case Games::SILVER:
            case Games::CRYSTAL:
                contents["Items"] = pocket::make(game_id, "Items", 20);
                contents["Balls"] = pocket::make(game_id, "Balls", 12);
                contents["KeyItems"] = pocket::make(game_id, "KeyItems", 26);
                contents["TM/HM"] = pocket::make(game_id, "TM/HM", 99);
                break;
                
            case Games::RUBY:
            case Games::SAPPHIRE:
                contents["Items"] = pocket::make(game_id, "Items", 20);
                contents["Poke Balls"] = pocket::make(game_id, "Poke Balls", 16);
                contents["TMs and HMs"] = pocket::make(game_id, "TMs and HMs", 64);
                contents["Berries"] = pocket::make(game_id, "Berries", 46);
                contents["Key Items"] = pocket::make(game_id, "Key Items", 20);
                break;
            
            case Games::EMERALD:
                contents["Items"] = pocket::make(game_id, "Items", 20);
                contents["Poke Balls"] = pocket::make(game_id, "Poke Balls", 16);
                contents["TMs and HMs"] = pocket::make(game_id, "TMs and HMs", 64);
                contents["Berries"] = pocket::make(game_id, "Berries", 46);
                contents["Key Items"] = pocket::make(game_id, "Key Items", 30);
                break;
                
            case Games::FIRE_RED:
            case Games::LEAF_GREEN:
                contents["Items"] = pocket::make(game_id, "Items", 42);
                contents["Key Items"] = pocket::make(game_id, "Key Items", 30);
                contents["Poke Balls"] = pocket::make(game_id, "Poke Balls", 13);
                contents["TM Case"] = pocket::make(game_id, "TM Case", 58);
                contents["Berry Pouch"] = pocket::make(game_id, "Berry Pouch", 43);
                break;
            
            case Games::DIAMOND:
            case Games::PEARL:
            case Games::PLATINUM:
            case Games::HEART_GOLD:
            case Games::SOUL_SILVER:
                //Infinite space in each pocket, but 100 should do
                contents["Items"] = pocket::make(game_id, "Items", 100);
                contents["Medicine"] = pocket::make(game_id, "Medicine", 100);
                contents["Poke Balls"] = pocket::make(game_id, "Poke Balls", 100);
                contents["TMs and HMs"] = pocket::make(game_id, "TMs and HMs", 100);
                contents["Berries"] = pocket::make(game_id, "Berries", 100);
                contents["Mail"] = pocket::make(game_id, "Mail", 100);
                contents["Battle Items"] = pocket::make(game_id, "Battle Items", 100);
                contents["Key Items"] = pocket::make(game_id, "Key Items", 100);
                break;
                
            case Games::BLACK:
            case Games::WHITE:
            case Games::BLACK2:
            case Games::WHITE2:
                contents["Items"] = pocket::make(game_id, "Items", 310);
                contents["Medicine"] = pocket::make(game_id, "Medicine", 48);
                contents["TMs and HMs"] = pocket::make(game_id, "TMs and HMs", 109);
                contents["Berries"] = pocket::make(game_id, "Berries", 64);
                contents["Key Items"] = pocket::make(game_id, "Key Items", 83);
            
            default:
                break;
        }
    }
    
    unsigned int bag_impl::get_game_id() {return game_id;}
    
    const pocket::sptr bag_impl::get_pocket(string name) {return contents[name];}
    
    void bag_impl::get_pocket_list(vector<string> &pocket_vec) {pocket_vec = contents.keys();}
}
