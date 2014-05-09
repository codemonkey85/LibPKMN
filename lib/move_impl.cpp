/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <iostream>
#include <sstream>

#include <pkmn/move.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

#include "move_impl.hpp"

namespace pkmn
{
    move::sptr move::make(unsigned int id, unsigned int game)
    {
        return sptr(new move_impl(id, game));
    }

    move::sptr move::make(std::string name, std::string game)
    {
        return make(database::get_move_id(name), database::get_game_id(game));
    }

    SQLite::Database move_impl::_db(get_database_path().c_str());
    
    move_impl::move_impl(unsigned int id, unsigned int game): move()
    {
        _game_id = game;
        _generation = database::get_generation(_game_id);

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
            _move_id = id;

            //Fail if move's generation_id > specified generation
            std::ostringstream query_stream;
            query_stream << "SELECT generation_id FROM moves WHERE id=" << _move_id;
            unsigned int gen_id = int(_db.execAndGet(query_stream.str().c_str()));

            if(gen_id > _generation)
            {
                std::ostringstream error_stream;
                error_stream << get_name() << " not present in Generation " << _generation << ".";
                throw std::runtime_error(error_stream.str().c_str());
            }

            query_stream.str("");
            query_stream << "SELECT * FROM moves WHERE id=" << _move_id;
            SQLite::Statement moves_query(_db, query_stream.str().c_str());
            moves_query.executeStep();

            //Get available values from queries
            _type_id = int(moves_query.getColumn(3)); //type_id
            _base_power = int(moves_query.getColumn(4)); //power
            _base_pp = int(moves_query.getColumn(5)); //pp
            _base_accuracy = int(moves_query.getColumn(6)); //accuracy
            _base_accuracy /= 100.0; //Stored as 0 < int < 100
            _base_priority = moves_query.getColumn(7); //priority
            _target_id = int(moves_query.getColumn(8)); //target_id
            _move_damage_class_id = int(moves_query.getColumn(9)); //damage_class_id
            _effect_id = int(moves_query.getColumn(10)); //effect_id
            _effect_chance = int(moves_query.getColumn(11)); //effect_chance
        }
    }

    std::string move_impl::get_game() const {return database::get_game_name(_game_id);}

    unsigned int move_impl::get_generation() const {return _generation;}

    std::string move_impl::get_name() const {return database::get_move_name(_move_id);}

    std::string move_impl::get_description() const {return database::get_move_description(_move_id, _game_id);}

    std::string move_impl::get_type() const {return database::get_type_name(_type_id);}

    unsigned int move_impl::get_base_power() const {return _base_power;}

    unsigned int move_impl::get_base_pp() const {return _base_pp;}

    std::string move_impl::get_move_damage_class() const
    {
        if(_move_id == Moves::NONE or _move_id == Moves::INVALID) return "None";
        else
        {
            std::ostringstream query_stream;
            query_stream << "SELECT name FROM move_damage_class_prose WHERE local_language_id=9 AND move_damage_class_id="
                         << _move_damage_class_id;

            std::string move_damage_class = (const char*)(_db.execAndGet(query_stream.str().c_str()));
            move_damage_class[0] = toupper(move_damage_class[0]); //"name" field is all-lowercase

            return move_damage_class;
        }
    }

    double move_impl::get_base_accuracy() const {return _base_accuracy;}

    std::string move_impl::get_effect() const
    {
        if(_move_id == Moves::NONE or _move_id == Moves::INVALID) return "None";
        else
        {
            std::ostringstream query_stream;
            query_stream << "SELECT short_effect FROM move_effect_prose WHERE local_language_id=9 AND move_effect_id=" << _effect_id;
            SQLite::Statement query(_db, query_stream.str().c_str());

            std::string entry = (query.executeStep()) ? ((const char*)(query.getColumn(0))) : "None";

            std::string s;
            std::istringstream iss(entry);
            entry = "";
            while(iss >> s)
            {
                if (entry != "") entry += " " + s;
                else entry = s;
            }

            return entry;
        }
    }

    double move_impl::get_effect_chance() const {return _effect_chance;}

    int move_impl::get_priority() const {return _base_priority;}

    std::string move_impl::get_target() const
    {
        std::ostringstream query_stream;
        query_stream << "SELECT name FROM move_target_prose WHERE local_language_id=9 AND move_target_id=" << _target_id;
        return (const char*)(_db.execAndGet(query_stream.str().c_str()));
    }

    unsigned int move_impl::get_move_id() const {return _move_id;}

    unsigned int move_impl::get_target_id() const {return _target_id;}

    unsigned int move_impl::get_game_id() const {return _game_id;}

    unsigned int move_impl::get_move_damage_class_id() const {return _move_damage_class_id;}

    unsigned int move_impl::get_effect_id() const {return _effect_id;}
} /* namespace pkmn */
