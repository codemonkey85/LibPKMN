/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "item_impl.hpp"
#include <pkmnsim/enums.hpp>
#include <pkmnsim/item.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

using namespace std;

namespace pkmnsim
{
    item::sptr item::make(unsigned int id, unsigned int game)
    {
        /*if(id >= Items::TM01 and id <= Items::HM08)
        {
            return sptr(new item_machineimpl(id, game));
        }
        else if((id >= Items::CHERI_BERRY and id <= Items::ROWAP_BERRY)
                or (id >= Items::BERRY and id <= Items::MYSTERYBERRY))
        {
            return sptr(new item_berryimpl(id, game));
        }
        else */return sptr(new item_impl(id, game));
    }
    
    item_impl::item_impl(unsigned int id, unsigned int game): item()
    {
        item_id = id;
        game_id = game;
        category_id = database::get_item_category(id);
        game_index = database::get_item_index(item_id, game_id);
        description = database::get_item_description(item_id, game_id);
        item_name = database::get_item_name(id);
    }
    
    unsigned int item_impl::get_game_id() {return item_id;}
    
    unsigned int item_impl::get_category_id() {return category_id;}
    
    string item_impl::get_category_name()
    {
        SQLite::Database db(get_database_path().c_str());
        string query_string = "SELECT name FROM item_category_prose WHERE local_language_id=9 AND item_category_id=" + to_string(category_id);
        return string((const char*)db.execAndGet(query_string.c_str()));
    }
    
    unsigned int item_impl::get_index() {return database::get_item_index(item_id, game_id);}
    
    unsigned int item_impl::get_item_id() {return item_id;}
    
    string item_impl::get_description() {return description;}
    
    string item_impl::get_name() {return item_name;}
}
