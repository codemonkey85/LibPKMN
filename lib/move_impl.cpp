/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */


#include <algorithm>
#include <iostream>

#include <boost/format.hpp>

#include <pkmnsim/move.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

//TODO: Other includes for Pokémon-specific move implementations (Curse, Hidden Power,etc)
#include "move_mainimpl.hpp"

using namespace std;

namespace pkmnsim
{
    move::sptr move::make(unsigned int id, unsigned int game)
    {
        try
        {
            return sptr(new move_mainimpl(id, game));
        }
        catch(const exception &e)
        {
            cout << "Caught exception: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }

    move_impl::move_impl(unsigned int id, unsigned int game): move()
    {
        from_game = game;
        if(id == Moves::NONE)
        {
            move_id = Moves::NONE;
            type_id = 0;
            base_power = 0;
            base_pp = 0;
            base_accuracy = 0.0;
            base_priority = 0;
            target_id = 0;
        }
        else if(id == Moves::INVALID)
        {
            move_id = Moves::INVALID;
            type_id = 0;
            base_power = 0;
            base_pp = 0;
            base_accuracy = 0.0;
            base_priority = 0;
            target_id = 0;
        }
        else
        {
            string query_string;
            move_id = id;

            SQLite::Database db(get_database_path().c_str());

            //Fail if move's generation_id > specified generation
            query_string = "SELECT generation_id FROM moves WHERE id=" + to_string(id);
            int gen_id = db.execAndGet(query_string.c_str());
            int game_gen = database::get_generation_from_game_id(from_game);

            if(gen_id > game_gen)
            {
                string error_message = get_name() + " not present in Generation " + to_string(game_gen) + ".";
                throw runtime_error(error_message.c_str());
            }

            query_string = "SELECT * FROM moves WHERE id=" + to_string(move_id);
            SQLite::Statement moves_query(db, query_string.c_str());
            moves_query.executeStep();

            //Get available values from queries
            type_id = int(moves_query.getColumn(3)); //type_id
            base_power = int(moves_query.getColumn(4)); //power
            base_pp = int(moves_query.getColumn(5)); //pp
            base_accuracy = int(moves_query.getColumn(6)); //accuracy
            base_accuracy /= 10; //Stored as 0 < int < 100
            base_priority = int(moves_query.getColumn(7)); //priority
            target_id = int(moves_query.getColumn(8)); //target_id
            move_damage_class = int(moves_query.getColumn(9)); //damage_class_id
        }
    }

    string move_impl::get_name() const
    {
        switch(move_id)
        {
            case Moves::NONE:
                return "None";

            case Moves::INVALID:
                return "Invalid Move";

            default:
               return database::get_move_name_from_id(move_id);
        }
    }

    string move_impl::get_description() const
    {
        switch(move_id)
        {
            case Moves::NONE:
            case Moves::INVALID:
                return "No info";

            default:
                return database::get_move_description_from_id(move_id, from_game);
        }
    }

    unsigned int move_impl::get_type() const {return type_id;}

    unsigned int move_impl::get_base_power() const {return base_power;}

    unsigned int move_impl::get_base_pp() const {return base_pp;}

    double move_impl::get_base_accuracy() const {return base_accuracy;}
    
    unsigned int move_impl::get_move_damage_class() const
    {
        switch(move_id)
        {
            case Moves::NONE:
            case Moves::INVALID:
                return 0;

            default:
                //In Gens 1-3, damage class depended on move type
                //In Gens 4-5, damage class is specific to each move
                unsigned int from_gen = database::get_generation_from_game_id(from_game);
                if(from_gen >= 4) return move_damage_class;
                else return database::get_damage_class_from_type(type_id);
        }
    }

    string move_impl::get_base_effect() const {return base_effect;}

    double move_impl::get_base_effect_chance() const {return base_effect_chance;}

    unsigned int move_impl::get_priority() const {return base_priority;}

    unsigned int move_impl::get_move_id() const {return move_id;}

    unsigned int move_impl::get_type_id() const {return type_id;}

    unsigned int move_impl::get_target_id() const {return target_id;}

    unsigned int move_impl::get_game_id() const {return from_game;}
}
