/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/format.hpp>

#include <pkmnsim/paths.hpp>

#include "base_move_mainimpl.hpp"
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    base_move_mainimpl::base_move_mainimpl(int id, int gen): base_move(id, gen)
    {
        SQLite::Database db(get_database_path().c_str());

       //Effect and chance
        string query_string = "SELECT effect_id FROM moves WHERE id=" + to_string(move_id);
        int effect_id = db.execAndGet(query_string.c_str());
        query_string = str(boost::format("SELECT short_effect FROM move_effect_prose WHERE move_effect_id=%d AND local_language_id=9")
                                         % effect_id);
        base_effect = db.execAndGetStr(query_string.c_str(), "");

        query_string = "SELECT effect_chance FROM moves WHERE id=" + to_string(move_id);
        base_effect_chance = db.execAndGet(query_string.c_str());

        query_string = "SELECT damage_class_id FROM moves WHERE id=" + to_string(move_id);
        move_damage_class = db.execAndGet(query_string.c_str());
    }
}
