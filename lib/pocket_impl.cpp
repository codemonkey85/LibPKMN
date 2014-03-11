/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <string>

#include "item_impl.hpp"
#include "item_berryimpl.hpp"
#include "item_machineimpl.hpp"

#include "pocket_impl.hpp"

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/pocket.hpp>
#include <pkmn/database/queries.hpp>

using namespace std;

namespace pkmn
{
    pocket::sptr pocket::make(unsigned int game, string name, unsigned int size)
    {
        return sptr(new pocket_impl(game, name, size));
    }

    pocket::sptr pocket::make(std::string game, std::string name, unsigned int size)
    {
        return make(database::get_game_id(game), name, size);
    }

    pocket_impl::pocket_impl(unsigned int game, string name, unsigned int size)
    {
        _game_id = game;
        _generation = database::get_generation(_game_id);
        _pocket_name = name;
        _pocket_size = size;
        
        _contents = std::vector<unsigned int>(_pocket_size);
        _amounts = std::vector<unsigned int>(_pocket_size);
        for(size_t i = 0; i < _pocket_size; i++)
        {
            _contents[i] = Items::NONE;
            _amounts[i] = 0;
        }
    }

    std::string pocket_impl::get_game() const {return database::get_game_name(_game_id);}

    unsigned int pocket_impl::get_generation() const {return _generation;}

    std::string pocket_impl::get_name() const {return _pocket_name;}

    unsigned int pocket_impl::size() const {return _pocket_size;}

    void pocket_impl::add_item(pokemon_text item_name, unsigned int amount)
    {
        add_item(database::get_item_id(item_name), amount);
    }

    void pocket_impl::add_item(unsigned int item_id, unsigned int amount)
    {
        for(size_t i = 0; i < _pocket_size; i++)
        {
            if(_contents[i] == item_id and _amounts[i] < 99)
            {
                _amounts[i]++;
                break;
            }
            else if(_contents[i] == 0)
            {
                /*
                 * Assume we've found an empty slot and the item isn't in the bag,
                 * so check to make sure the item is valid for the game.
                 */
                if(database::get_item_index(item_id, _game_id) != 0)
                {
                    _contents[i] = item_id;
                    _amounts[i] = amount;
                    break;
                }
            }
            /*
             * If the function finishes the for-loop without entering either
             * part of the if-statement, then no item was added.
             */
        }
    }

    void pocket_impl::add_item(item::sptr item_sptr, unsigned int amount)
    {
        add_item(item_sptr->get_item_id(), amount);
    }

    void pocket_impl::remove_item(pokemon_text item_name, unsigned int amount)
    {
        remove_item(database::get_item_id(item_name), amount);
    }

    void pocket_impl::remove_item(unsigned int item_id, unsigned int amount)
    {
        for(size_t i = 0; i < _pocket_size; i++)
        {
            if(_contents[i] == item_id)
            {
                if(amount < _amounts[i]) _amounts[i] -= amount;
                else remove_item(i+1);

                break;
            }
        }
    }

    void pocket_impl::remove_item(item::sptr item_sptr, unsigned int amount)
    {
        remove_item(item_sptr->get_item_id(), amount);
    }

    unsigned int pocket_impl::get_item_amount(pokemon_text item_name) const
    {
        return get_item_amount(database::get_item_id(item_name));
    }

    unsigned int pocket_impl::get_item_amount(unsigned int item_id) const
    {
        for(size_t i = 0; i < _pocket_size; i++)
        {
            if(_contents[i] == item_id)
            {
                return _amounts[i];
            }
            else if(_contents[i] == 0) break;
        }
        return 0;
    }

    unsigned int pocket_impl::get_item_amount(item::sptr item_sptr) const
    {
        return get_item_amount(item_sptr->get_item_id());
    }

    bag_slot_t pocket_impl::get_item(unsigned int pos) const
    {
        unsigned int actual_pos = (pos > _pocket_size) ? (_pocket_size-1) : (pos == 0) ? 0 : (pos-1);
        return std::make_pair(item::make(_contents[actual_pos], _game_id), _amounts[actual_pos]);
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
    
    void pocket_impl::set_item(unsigned int pos, item::sptr item_sptr, unsigned int amount)
    {
        set_item(pos, item_sptr->get_item_id(), amount);
    }
    
    void pocket_impl::set_item(unsigned int pos, unsigned int item_id, unsigned int amount)
    {
        unsigned int actual_pos = (pos > _pocket_size) ? (_pocket_size-1) : (pos == 0) ? 0 : (pos-1);
        _contents[actual_pos] = item_id;
        _amounts[actual_pos] = amount;
    }
    
    void pocket_impl::set_item(item::sptr item_sptr, unsigned int amount)
    {
        set_item(item_sptr->get_item_id(), amount);
    }
    
    void pocket_impl::set_item(unsigned int item_id, unsigned int amount)
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
            _contents[actual_pos] = item_id;
            _amounts[actual_pos] = amount;
        }
    }
    
    void pocket_impl::set_amount(unsigned int pos, unsigned int amount)
    {
        unsigned int actual_pos = (pos > _pocket_size) ? (_pocket_size-1) : (pos == 0) ? 0 : (pos-1);
        _amounts[actual_pos] = amount;
    }

    unsigned int pocket_impl::get_game_id() const {return _game_id;}
} /* namespace pkmn */
