/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */


#include <algorithm>
#include <iostream>

#include <boost/format.hpp>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

//TODO: Other includes for Pokémon-specific move implementations (Curse, Hidden Power,etc)
#include "base_move_mainimpl.hpp"
#include "sqlitecpp/SQLiteCPP.h"


using namespace std;

namespace pkmnsim
{
    base_move::base_move(string identifier, int gen)
    {
        string query_string;
        move_identifier = identifier;

        SQLite::Database db(get_database_path().c_str());

        //Fail if move's generation_id > specified generation
        query_string = "SELECT generation_id FROM moves WHERE identifier='" + identifier + "'";
        int gen_id = db.execAndGet(query_string.c_str(), identifier);

        if(gen_id > gen)
        {
            string error_message = identifier + " not present in Generation " + to_string(gen) + ".";
            throw runtime_error(error_message.c_str());
        }

        //After move verified as valid, generate next available queries
        query_string = "SELECT id FROM moves WHERE identifier='" + identifier + "'";
        move_id = db.execAndGet(query_string.c_str(), identifier);

        query_string = "SELECT * FROM moves WHERE id=" + to_string(move_id);
        SQLite::Statement moves_query(db, query_string.c_str());
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
        query_string = str(boost::format("SELECT name FROM move_names WHERE move_id=%d AND local_language_id=9")
                                         % move_id);
        name = db.execAndGetStr(query_string.c_str(), identifier);

        //Type
        query_string = str(boost::format("SELECT name FROM type_names WHERE type_id=%d AND local_language_id=9")
                                         % type_id);
        type = db.execAndGetStr(query_string.c_str(), identifier);
    }

    base_move::sptr base_move::make(string identifier, int gen)
    {
        try
        {
            //Match database's identifier format
            identifier = database::to_database_format(identifier);
            
            /*
                if(identifier == "curse") {}
                else if(identifier == "hidden power") {}
                else{
            */
            return sptr(new base_move_mainimpl(identifier, gen));
        }
        catch(const exception &e)
        {
            cout << "Caught exception: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    string base_move::get_name(void) {return name;}

    string base_move::get_description(void) {return description;}

    string base_move::get_type(void) {return type;}

    int base_move::get_base_power(void) {return base_power;}

    int base_move::get_base_pp(void) {return base_pp;}

    double base_move::get_base_accuracy(void) {return base_accuracy;}

    int base_move::get_move_damage_class(void) {return move_damage_class;}

    string base_move::get_base_effect(void) {return base_effect;}

    double base_move::get_base_effect_chance(void) {return base_effect_chance;}
    
    int base_move::get_move_id(void) {return move_id;}
    
    int base_move::get_type_id(void) {return type_id;}
    
    int base_move::get_target_id(void) {return target_id;}
}
