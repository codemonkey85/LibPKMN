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

#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "item_machineimpl.hpp"

using namespace std;

namespace pkmnsim
{
    item_machineimpl::item_machineimpl(unsigned int id, unsigned int game): item_impl(id, game)
    {
        if(_item_id >= Items::TM01 and _item_id <= Items::TM92) _machine_id = _item_id - 304;
        else _machine_id = 100 + (_item_id - Items::HM01);
        
        string query_string = "SELECT move_id FROM machines WHERE machine_number="
                            + to_string(_machine_id);
        _move_id = int(_db.execAndGet(query_string.c_str()));
        
        _item_name = str(boost::format("%s (%s)") % database::get_item_name(_item_id)
                                                  % database::get_move_name(_move_id));
        _description = str(boost::format("%s - %s") % database::get_item_name(_item_id)
                                                    % database::get_move_description(_move_id, _game_id));
    }
} /* namespace pkmnsim */
