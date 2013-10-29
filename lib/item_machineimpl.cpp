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

#include "item_machineimpl.hpp"
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

using namespace std;

namespace pkmnsim
{
    item_machineimpl::item_machineimpl(unsigned int id, unsigned int game): item_impl(id, game)
    {
        SQLite::Database db(get_database_path().c_str());
        
        if(item_id >= Items::TM01 and item_id <= Items::TM92) machine_id = item_id - 304;
        else machine_id = 100 + (item_id - Items::HM01);
        
        string query_string = "SELECT move_id FROM machines WHERE machine_number=" + to_string(machine_id);
        move_id = int(db.execAndGet(query_string.c_str()));
        
        item_name = str(boost::format("%s (%s)") % database::get_item_name(item_id) % database::get_move_name(move_id));
        description = str(boost::format("%s - %s") % database::get_item_name(item_id) % database::get_move_description(move_id, game_id));
    }
}