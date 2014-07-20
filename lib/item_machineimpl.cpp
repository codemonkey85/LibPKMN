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
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

#include "item_machineimpl.hpp"

namespace pkmn
{
    item_machineimpl::item_machineimpl(unsigned int id, unsigned int game): item_impl(id, game)
    {
        if(_item_id >= Items::TM01 and _item_id <= Items::TM92) _machine_id = _item_id - 304;
        else _machine_id = 101 + (_item_id - Items::HM01);

        std::string query_string(str(boost::format("SELECT move_id FROM machines WHERE machine_number=%d AND version_group_id=%d")
                                     % _machine_id % database::get_version_group_id(_game_id)));
        _move_id = _db->execAndGet(query_string.c_str());

        _item_name = str(boost::format("%s (%s)") % database::get_item_name(_item_id)
                                                  % database::get_move_name(_move_id));
        _description = str(boost::format("%s - %s") % database::get_item_name(_item_id)
                                                    % database::get_move_description(_move_id, _game_id));
    }
} /* namespace pkmn */
