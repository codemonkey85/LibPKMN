/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <string>

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
        _pocket_size = size;
        _generation = database::get_generation(_game_id);
        _pocket_name = name;
        _item_dict = pkmn::dict<uint16_t, uint8_t>();
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
        if(database::get_item_index(item_id, _game_id) != 0)
        {
            if(_item_dict.has_key(item_id) or _item_dict.size() < _pocket_size)
                _item_dict[item_id] = std::min(int(amount), 99);
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
        if(_item_dict.has_key(item_id))
        {
            if(amount < _item_dict[item_id]) _item_dict[item_id] -= amount;
            else _item_dict.erase(item_id);
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
        return _item_dict.at(item_id, 0);
    }

    unsigned int pocket_impl::get_item_amount(item::sptr item_sptr) const
    {
        return get_item_amount(item_sptr->get_item_id());
    }

    /*
     * Return list of items with more intuitive interface without exposing
     * internal memory structure.
     */
    void pocket_impl::get_item_list(item_list_t &item_list) const
    {
        item_list.clear();

        std::vector<uint16_t> keys = _item_dict.keys();
        std::vector<uint8_t> vals = _item_dict.vals();

        for(size_t i = 0; i < keys.size(); i++)
        {
            item_list.push_back(std::make_pair(item::make(keys[i], _game_id),
                                               vals[i]));
        }
    }

    unsigned int pocket_impl::get_game_id() const {return _game_id;}
} /* namespace pkmn */
