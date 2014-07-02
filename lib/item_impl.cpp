/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/item.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

#include "item_impl.hpp"
#include "item_machineimpl.hpp"

namespace pkmn
{
    item::sptr item::make(unsigned int id, unsigned int game)
    {

        if((id >= Items::TM01 and id <= Items::HM08)
           or (id >= Items::TM93 and id <= Items::TM95)
           or (id >= Items::TM96 and id <= Items::TM100))
        {
            return sptr(new item_machineimpl(id, game));
        }
        else return sptr(new item_impl(id, game));
    }

    item::sptr item::make(std::string name, std::string game)
    {
        return make(database::get_item_id(name), database::get_game_id(game));
    }

    pkmn::shared_ptr<SQLite::Database> item_impl::_db = NULL;

    item_impl::item_impl(unsigned int id, unsigned int game): item()
    {
        if(!_db) _db = pkmn::shared_ptr<SQLite::Database>(new SQLite::Database(get_database_path().c_str()));

        _item_id = id;
        _game_id = game;
        _generation = database::get_generation(_game_id);
        _item_name = database::get_item_name(_item_id);
        _category_id = database::get_item_category(_item_id);
        _game_index = database::get_item_index(_item_id, _game_id);
        _description = database::get_item_description(_item_id, _game_id);
    }

    std::string item_impl::get_game() const {return database::get_game_name(_game_id);}

    unsigned int item_impl::get_generation() const {return _generation;}

    std::string item_impl::get_name() const {return _item_name;}

    std::string item_impl::get_description() const {return _description;}

    std::string item_impl::get_category() const
    {
        std::string query_string = "SELECT name FROM item_category_prose "
                                   "WHERE local_language_id=9 AND item_category_id="
                                 + to_string(_category_id);
        return (const char*)(_db->execAndGet(query_string.c_str()));
    }

    unsigned int item_impl::get_game_index() const {return _game_index;}

    unsigned int item_impl::get_item_id() const {return _item_id;}

    unsigned int item_impl::get_category_id() const {return _category_id;}

    unsigned int item_impl::get_game_id() const {return _game_id;}
} /* namespace pkmn */
