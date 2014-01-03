/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */


#include <algorithm>
#include <iostream>

#include <boost/format.hpp>

#include <pkmn/move.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

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

    SQLite::Database move_impl::_db(get_database_path().c_str());
    
    move_impl::move_impl(unsigned int id, unsigned int game): move()
    {
        _game_id = game;
        if(id == Moves::NONE)
        {
            _move_id = Moves::NONE;
            _type_id = 0;
            _base_power = 0;
            _base_pp = 0;
            _base_accuracy = 0.0;
            _base_priority = 0;
            _target_id = 0;
        }
        else if(id == Moves::INVALID)
        {
            _move_id = Moves::INVALID;
            _type_id = 0;
            _base_power = 0;
            _base_pp = 0;
            _base_accuracy = 0.0;
            _base_priority = 0;
            _target_id = 0;
        }
        else
        {
            string query_string;
            _move_id = id;

            //Fail if move's generation_id > specified generation
            query_string = "SELECT generation_id FROM moves WHERE id=" + to_string(id);
            int gen_id = _db.execAndGet(query_string.c_str());
            int game_gen = database::get_generation(_game_id);

            if(gen_id > game_gen)
            {
                string error_message = get_name() + " not present in Generation " + to_string(game_gen) + ".";
                throw runtime_error(error_message.c_str());
            }

            query_string = "SELECT * FROM moves WHERE id=" + to_string(_move_id);
            SQLite::Statement moves_query(_db, query_string.c_str());
            moves_query.executeStep();

            //Get available values from queries
            _type_id = int(moves_query.getColumn(3)); //type_id
            _base_power = int(moves_query.getColumn(4)); //power
            _base_pp = int(moves_query.getColumn(5)); //pp
            _base_accuracy = int(moves_query.getColumn(6)); //accuracy
            _base_accuracy /= 100; //Stored as 0 < int < 100
            _base_priority = int(moves_query.getColumn(7)); //priority
            _target_id = int(moves_query.getColumn(8)); //target_id
            _move_damage_class = int(moves_query.getColumn(9)); //damage_class_id
            _base_effect = int(moves_query.getColumn(10)); //effect_id
            _base_effect_chance = int(moves_query.getColumn(11)); //effect_chance
        }
    }

    string move_impl::get_name() const
    {
        switch(_move_id)
        {
            case Moves::NONE:
                return "None";

            case Moves::INVALID:
                return "Invalid Move";

            default:
               return database::get_move_name(_move_id);
        }
    }

    string move_impl::get_description() const
    {
        switch(_move_id)
        {
            case Moves::NONE:
            case Moves::INVALID:
                return "No info";

            default:
                return database::get_move_description(_move_id, _game_id);
        }
    }

    unsigned int move_impl::get_type() const {return _type_id;}

    unsigned int move_impl::get_base_power() const {return _base_power;}

    unsigned int move_impl::get_base_pp() const {return _base_pp;}

    double move_impl::get_base_accuracy() const {return _base_accuracy;}
    
    unsigned int move_impl::get_move_damage_class() const
    {
        switch(_move_id)
        {
            case Moves::NONE:
            case Moves::INVALID:
                return 0;

            default:
                //In Gens 1-3, damage class depended on move type
                //In Gens 4-5, damage class is specific to each move
                unsigned int from_gen = database::get_generation(_game_id);
                if(from_gen >= 4) return _move_damage_class;
                else return database::get_damage_class(_type_id);
        }
    }

    string move_impl::get_base_effect() const {return _base_effect;}

    double move_impl::get_base_effect_chance() const {return _base_effect_chance;}

    unsigned int move_impl::get_priority() const {return _base_priority;}

    unsigned int move_impl::get_move_id() const {return _move_id;}

    unsigned int move_impl::get_target_id() const {return _target_id;}

    unsigned int move_impl::get_game_id() const {return _game_id;}
} /* namespace pkmnsim */
