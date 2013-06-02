/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "base_move_mainimpl.hpp"
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    base_move_mainimpl::base_move_mainimpl(string identifier, SQLite::Database *db, int gen): base_move(identifier, db, gen)
    {
       //Effect and chance
        string query_string = "SELECT effect_id FROM moves WHERE id=" + to_string(move_id);
        int effect_id = db->execAndGet(query_string.c_str());

        query_string = "SELECT short_effect FROM move_effect_prose WHERE move_effect_id=" + to_string(effect_id);
        base_effect = db->execAndGetStr(query_string.c_str(), move_identifier);

        query_string = "SELECT effect_chance FROM moves WHERE id=" + to_string(move_id);
        base_effect_chance = db->execAndGet(query_string.c_str());
    }
}
