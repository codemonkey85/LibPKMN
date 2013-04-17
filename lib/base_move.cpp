/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <boost/format.hpp>
#include <iostream>

#include "base_move_mainimpl.hpp"
//TODO: Other includes for Pokemon-specific move implemenntations (Curse, Hidden Power,etc)
#include <pkmnsim/base_move.hpp>
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    base_move::base_move(string identifier, int gen)
    {
        //Match database's identifier format
        transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);
        replace(identifier.begin(), identifier.end(), ' ', '-');
        move_identifier = identifier;
        
        SQLite::Database db("@PKMNSIM_DB@");
        string query_string;

        //Fail if move's generation_id > specified generation
        query_string = str(boost::format("SELECT generation_id FROM moves WHERE identifier='%s'")
                                         % identifier);
        int gen_id = db.execAndGet(query_string.c_str(), identifier);

        if(gen_id > gen)
        {
            string error_message = str(boost::format("%s not present in Generation %d.")
                                                     % identifier.c_str() % gen);

            throw runtime_error(error_message.c_str());
        }

        //Get relevant ID's
        query_string = str(boost::format("SELECT id FROM moves WHERE identifier='%s'")
                                         % identifier.c_str());
        move_id = db.execAndGet(query_string.c_str(), identifier);
        query_string = str(boost::format("SELECT type_id FROM moves WHERE id=%d")
                                         % move_id);
        type_id = db.execAndGet(query_string.c_str());

        //Move name
        query_string = str(boost::format("SELECT name FROM move_names WHERE move_id=%d")
                                         % move_id);
        name = db.execAndGetStr(query_string.c_str(), identifier);

        //Type
        query_string = str(boost::format("SELECT name FROM type_names WHERE type_id=%d")
                                         % type_id);
        type = db.execAndGetStr(query_string.c_str(), identifier);

        //Target ID (useful for 2/3 vs. 2/3 battles)
        query_string = str(boost::format("SELECT target_id FROM moves WHERE id=%d")
                                         % move_id);
        target_id = db.execAndGet(query_string.c_str());

        //Base power
        query_string = str(boost::format("SELECT power FROM moves WHERE id=%d")
                                         % move_id);
        base_power = db.execAndGet(query_string.c_str());

        //Base PP
        query_string = str(boost::format("SELECT pp FROM moves WHERE id=%d")
                                         % move_id);
        base_pp = db.execAndGet(query_string.c_str());

        //Base accuracy
        query_string = str(boost::format("SELECT accuracy FROM moves WHERE id=%d")
                                         % move_id);
        base_accuracy = db.execAndGet(query_string.c_str()); //Stored as 0 < int < 100
        base_accuracy /= 100;

        //Priority
        //TODO: priority explanation
        query_string = str(boost::format("SELECT priority FROM moves WHERE id=%d")
                                         % move_id);
        base_priority = db.execAndGet(query_string.c_str());
    }

    base_move::sptr base_move::make(string identifier, int gen)
    {
        /*
            if(identifier == "curse") {}
            else if(identifier == "hidden power") {}
            else{
        */
        return sptr(new base_move_mainimpl(identifier, gen));
    }
}
