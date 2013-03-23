/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include "base_move_mainimpl.hpp"
#include <boost/format.hpp>
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    base_move_mainimpl::base_move_mainimpl(string identifier, int gen): base_move(identifier, gen)
    {
        SQLite::Database db("@PKMNSIM_DB@");
        string query_string;

       //Effect and chance
        query_string = str(boost::format("SELECT effect_id FROM moves WHERE id=%d")
                                         % move_id);
        int effect_id = db.execAndGet(query_string.c_str());
        query_string = str(boost::format("SELECT short_effect FROM move_effect_prose WHERE move_effect_id=%d AND local_language_id=9")
                                         % effect_id);
        base_effect = db.execAndGetStr(query_string.c_str(), move_identifier);
        query_string = str(boost::format("SELECT effect_chance FROM moves WHERE move_id=%d")
                                         % move_id);
        base_effect_chance = db.execAndGet(query_string.c_str());
    }
}
