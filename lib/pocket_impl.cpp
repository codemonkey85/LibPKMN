/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <string>
#include <vector>

#include <boost/cast.hpp>

#include "item_impl.hpp"
#include "item_berryimpl.hpp"
#include "item_machineimpl.hpp"

#include "pocket_impl.hpp"

#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/pocket.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

using namespace std;

namespace pkmnsim
{
    pocket::sptr pocket::make(unsigned int game, string name, unsigned int size)
    {
        return sptr(new pocket_impl(game, name, size));
    }
    
    pocket_impl::pocket_impl(unsigned int game, string name, unsigned int size)
    {
        game_id = game;
        pocket_name = name;
        pocket_size = size;
        
        contents = vla<item::sptr>(pocket_size);
        amounts = vla<unsigned int>(pocket_size);
        for(int i = 0; i < pocket_size; i++)
        {
            contents[i] = item::make(Items::NONE, game_id);
            amounts[i] = 0;
        }
    }
    
    unsigned int pocket_impl::get_game_id() {return game_id;}
    
    std::string pocket_impl::get_name() {return pocket_name;}
    
    unsigned int pocket_impl::get_size() {return pocket_size;}
    
    item::sptr pocket_impl::get_item(unsigned int pos, bool copy)
    {
        unsigned int actual_pos = (pos > pocket_size) ? (pocket_size-1) : (pos == 0) ? 0 : (pos-1);
        item::sptr to_return = contents[pos];
        
        unsigned int item_id = to_return->get_item_id();
        
        if(copy)
        {
            if(item_id >= Items::TM01 and item_id <= Items::HM08)
            {
                item_machineimpl actual_machine = *(boost::polymorphic_downcast<item_machineimpl*>(to_return.get()));
                return item::sptr(&actual_machine);
            }
            else if((item_id >= Items::CHERI_BERRY and item_id <= Items::ROWAP_BERRY)
                    or (item_id >= Items::BERRY and item_id <= Items::MYSTERYBERRY))
            {
                item_berryimpl actual_berry = *(boost::polymorphic_downcast<item_berryimpl*>(to_return.get()));
                return item::sptr(&actual_berry);
            }
            else
            {
                item_impl actual = *(boost::polymorphic_downcast<item_impl*>(to_return.get()));
                return item::sptr(&actual);
            }
        }
        else return to_return;
    }
    
    void pocket_impl::remove_item(unsigned int pos)
    {
        unsigned int actual_pos = (pos > pocket_size) ? (pocket_size-1) : (pos == 0) ? 0 : (pos-1);
    
        item::sptr blank_item = item::make(Items::NONE, game_id);

        set_item(actual_pos, blank_item, 0);
        
        //Move over any non-blank item in later positions
        for(int i = (actual_pos+1); i < pocket_size; i++)
        {
            if(contents[i]->get_item_id() == Items::NONE) break;
            else
            {
                contents[i-1] = contents[i];
                contents[i] = blank_item;
            }
        }
    }
    
    void pocket_impl::set_item(unsigned int pos, item::sptr new_item, unsigned int amount)
    {
        unsigned int actual_pos = (pos > pocket_size) ? (pocket_size-1) : (pos == 0) ? 0 : (pos-1);
        contents[actual_pos] = new_item;
        amounts[actual_pos] = amount;
    }
    
    void pocket_impl::set_item(unsigned int pos, unsigned int item_id, unsigned int amount)
    {
        set_item(pos, item::make(item_id, game_id), amount);
    }
    
    void pocket_impl::set_item(item::sptr new_item, unsigned int amount)
    {
        //Find first non-blank item
        unsigned int actual_pos = -1;
        for(int i = 0; i < pocket_size; i++)
        {
            if(contents[i]->get_item_id() == Items::NONE)
            {
                actual_pos = i;
                break;
            }
        }
        
        //No space left in pocket
        if(actual_pos != -1)
        {
            contents[actual_pos] = new_item;
            amounts[actual_pos] = amount;
        }
    }
    
    void pocket_impl::set_item(unsigned int item_id, unsigned int amount)
    {
        set_item(item::make(item_id, game_id), amount);
    }
    
    unsigned int pocket_impl::get_amount(unsigned int pos)
    {
        unsigned int actual_pos = (pos > pocket_size) ? (pocket_size-1) : (pos == 0) ? 0 : (pos-1);
        return amounts[actual_pos];
    }
    
    void pocket_impl::set_amount(unsigned int pos, unsigned int amount)
    {
        unsigned int actual_pos = (pos > pocket_size) ? (pocket_size-1) : (pos == 0) ? 0 : (pos-1);
        amounts[actual_pos] = amount;
    }
}
