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

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/pocket.hpp>

using namespace std;

namespace pkmn
{
    pocket::sptr pocket::make(unsigned int game, string name, unsigned int size)
    {
        return sptr(new pocket_impl(game, name, size));
    }
    
    pocket_impl::pocket_impl(unsigned int game, string name, unsigned int size)
    {
        _game_id = game;
        _pocket_name = name;
        _pocket_size = size;
        
        _contents = vla<unsigned int>(_pocket_size);
        _amounts = vla<unsigned int>(_pocket_size);
        for(size_t i = 0; i < _pocket_size; i++)
        {
            _contents[i] = Items::NONE;
            _amounts[i] = 0;
        }
    }
    
    unsigned int pocket_impl::get_game_id() {return _game_id;}
    
    std::string pocket_impl::get_name() {return _pocket_name;}
    
    unsigned int pocket_impl::get_size() {return _pocket_size;}
    
    item::sptr pocket_impl::get_item(unsigned int pos)
    {
        unsigned int actual_pos = (pos > _pocket_size) ? (_pocket_size-1) : (pos == 0) ? 0 : (pos-1);
        return item::make(_contents[actual_pos], _game_id);
    }
    
    void pocket_impl::remove_item(unsigned int pos)
    {
        unsigned int actual_pos = (pos > _pocket_size) ? (_pocket_size-1) : (pos == 0) ? 0 : (pos-1);
        set_item(actual_pos, Items::NONE, 0);
        
        //Move over any non-blank item in later positions
        for(size_t i = (actual_pos+1); i < _pocket_size; i++)
        {
            if(_contents[i] == Items::NONE) break;
            else
            {
                _contents[i-1] = _contents[i];
                _contents[i] = Items::NONE;
                
                _amounts[i-1] = _amounts[i];
                _amounts[i] = 0;
            }
        }
    }
    
    void pocket_impl::set_item(unsigned int pos, item::sptr item, unsigned int amount)
    {
        set_item(pos, item->get_item_id(), amount);
    }
    
    void pocket_impl::set_item(unsigned int pos, unsigned int item_id, unsigned int amount)
    {
        unsigned int actual_pos = (pos > _pocket_size) ? (_pocket_size-1) : (pos == 0) ? 0 : (pos-1);
        _contents[actual_pos] = item_id;
        _amounts[actual_pos] = amount;
    }
    
    void pocket_impl::set_item(item::sptr item, unsigned int amount)
    {
        //Find first non-blank item
        unsigned int actual_pos = -1;
        for(size_t i = 0; i < _pocket_size; i++)
        {
            if(_contents[i] == Items::NONE)
            {
                actual_pos = i;
                break;
            }
        }
        
        //No space left in pocket
        if(actual_pos != -1)
        {
            _contents[actual_pos] = item->get_item_id();
            _amounts[actual_pos] = amount;
        }
    }
    
    void pocket_impl::set_item(unsigned int item_id, unsigned int amount)
    {
        set_item(item::make(item_id, _game_id), amount);
    }
    
    unsigned int pocket_impl::get_amount(unsigned int pos)
    {
        unsigned int actual_pos = (pos > _pocket_size) ? (_pocket_size-1) : (pos == 0) ? 0 : (pos-1);
        return _amounts[actual_pos];
    }
    
    void pocket_impl::set_amount(unsigned int pos, unsigned int amount)
    {
        unsigned int actual_pos = (pos > _pocket_size) ? (_pocket_size-1) : (pos == 0) ? 0 : (pos-1);
        _amounts[actual_pos] = amount;
    }
} /* namespace pkmn */
