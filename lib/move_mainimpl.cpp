/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/format.hpp>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>

#include "move_mainimpl.hpp"

using namespace std;

namespace pkmnsim
{
    move_mainimpl::move_mainimpl(unsigned int id, unsigned int game): move_impl(id, game)
    {
        switch(_move_id)
        {
            case Moves::NONE:
            case Moves::INVALID:
                _base_effect = "None";
                _base_effect_chance = 0;
                _move_damage_class = 0;
                break;

            default:
                SQLite::Database db(get_database_path().c_str());

               //Effect and chance
                string query_string = "SELECT effect_id FROM moves WHERE id=" + to_string(_move_id);
                int effect_id = _db.execAndGet(query_string.c_str());
                query_string = str(boost::format("SELECT short_effect FROM move_effect_prose WHERE move_effect_id=%d AND local_language_id=9")
                                                 % effect_id);
                _base_effect = int(_db.execAndGet(query_string.c_str()));

                query_string = "SELECT effect_chance FROM moves WHERE id=" + to_string(_move_id);
                _base_effect_chance = int(_db.execAndGet(query_string.c_str())) / 100;

                query_string = "SELECT damage_class_id FROM moves WHERE id=" + to_string(_move_id);
                _move_damage_class = int(_db.execAndGet(query_string.c_str()));
                break;
        }
    }
} /* namespace pkmnsim */
