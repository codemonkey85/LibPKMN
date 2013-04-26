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
#include "internal.hpp"
//TODO: Other includes for Pokémon-specific move implementations (Curse, Hidden Power,etc)
#include <pkmnsim/base_move.hpp>
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    base_move::base_move(string identifier, SQLite::Database *db, int gen)
    {
        string query_string;
        move_identifier = identifier;

        //Fail if move's generation_id > specified generation
        query_string = str(boost::format("SELECT generation_id FROM moves WHERE identifier='%s'")
                                         % identifier);
        int gen_id = db->execAndGet(query_string.c_str(), identifier);

        if(gen_id > gen)
        {
            string error_message = str(boost::format("%s not present in Generation %d.")
                                                     % identifier.c_str() % gen);

            throw runtime_error(error_message.c_str());
        }

        //After move verified as valid, generate next available queries
        query_string = str(boost::format("SELECT id FROM moves WHERE identifier='%s'")
                                         % identifier.c_str());
        move_id = db->execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT * FROM moves WHERE id=%d")
                                         % move_id);
        SQLite::Statement moves_query(*db, query_string.c_str());
        moves_query.executeStep();

        //Get available values from queries
        type_id = moves_query.getColumn(3); //type_id
        base_power = moves_query.getColumn(4); //power
        base_pp = moves_query.getColumn(5); //pp
        base_accuracy = moves_query.getColumn(6); //accuracy
        base_accuracy /= 10; //Stored as 0 < int < 100
        base_priority = moves_query.getColumn(7); //priority
        target_id = moves_query.getColumn(8); //target_id

        //Move name
        query_string = str(boost::format("SELECT name FROM move_names WHERE move_id=%d")
                                         % move_id);
        name = db->execAndGetStr(query_string.c_str(), identifier);

        //Type
        query_string = str(boost::format("SELECT name FROM type_names WHERE type_id=%d")
                                         % type_id);
        type = db->execAndGetStr(query_string.c_str(), identifier);
    }

    base_move::sptr base_move::make(string identifier, int gen)
    {
        //Match database's identifier format
        to_database_format(&identifier);
        
        SQLite::Database db(get_database_path().c_str());
        /*
            if(identifier == "curse") {}
            else if(identifier == "hidden power") {}
            else{
        */
        return sptr(new base_move_mainimpl(identifier, &db, gen));
    }
}
