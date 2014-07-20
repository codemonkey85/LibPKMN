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
        return make(database::get_move_id(name), database::get_version_id(game));
    }

    pkmn::shared_ptr<SQLite::Database> move_impl::_db;

    move_impl::move_impl(unsigned int id, unsigned int game): move()
    {
        if(!_db) _db = pkmn::shared_ptr<SQLite::Database>(new SQLite::Database(get_database_path().c_str()));

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
            unsigned int gen_id = _db->execAndGet(query_stream.str().c_str());

            if(gen_id > _generation)
            {
                std::ostringstream error_stream;
                error_stream << get_name() << " not present in Generation " << _generation << ".";
                throw std::runtime_error(error_stream.str().c_str());
            }

            query_stream.str("");
            query_stream << "SELECT * FROM moves WHERE id=" << _move_id;
            SQLite::Statement moves_query(*_db, query_stream.str().c_str());
            moves_query.executeStep();

            //Get available values from queries
            _type_id = moves_query.getColumn(3); //type_id
            _base_power = moves_query.getColumn(4); //power
            _base_pp = moves_query.getColumn(5); //pp
            _base_accuracy = moves_query.getColumn(6); //accuracy
            _base_accuracy /= 100.0; //Stored as 0 < int < 100
            _base_priority = moves_query.getColumn(7); //priority
            _target_id = moves_query.getColumn(8); //target_id
            _move_damage_class_id = moves_query.getColumn(9); //damage_class_id
            _effect_id = moves_query.getColumn(10); //effect_id
            _effect_chance = moves_query.getColumn(11); //effect_chance

            query_stream.str("");
            query_stream << "SELECT name FROM move_names WHERE local_language_id=9 AND move_id=" << _move_id;
            _move_name = (const char*)(_db->execAndGet(query_stream.str().c_str()));

            if(_generation < 6) _set_old_values();
        }
    }

    std::string move_impl::get_game() const {return database::get_version_name(_game_id);}

    unsigned int move_impl::get_generation() const {return _generation;}

    std::string move_impl::get_name() const {return _move_name;}

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

            std::string move_damage_class = (const char*)(_db->execAndGet(query_stream.str().c_str()));
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
            SQLite::Statement query(*_db, query_stream.str().c_str());

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
        return (const char*)(_db->execAndGet(query_stream.str().c_str()));
    }

    unsigned int move_impl::get_move_id() const {return _move_id;}

    unsigned int move_impl::get_target_id() const {return _target_id;}

    unsigned int move_impl::get_game_id() const {return _game_id;}

    unsigned int move_impl::get_move_damage_class_id() const {return _move_damage_class_id;}

    unsigned int move_impl::get_effect_id() const {return _effect_id;}

    void move_impl::_set_old_values()

    {
        std::ostringstream query_stream;
        query_stream << "SELECT gen" << _generation << "_accuracy FROM old_move_accuracies WHERE move_id=" << _move_id;
        SQLite::Statement accuracy_query(*_db, query_stream.str().c_str());
        if(accuracy_query.executeStep()) _base_accuracy = int(accuracy_query.getColumn(0)) / 100.0;
        
        //Hypnosis varies in accuracy between games
        if(_move_id == Moves::HYPNOSIS and (_game_id == Versions::DIAMOND or _game_id == Versions::PEARL)) _base_accuracy = 0.7;

        query_stream.str("");
        query_stream << "SELECT gen" << _generation << "_power FROM old_move_powers WHERE move_id=" << _move_id;
        SQLite::Statement power_query(*_db, query_stream.str().c_str());
        if(power_query.executeStep()) _base_power = power_query.getColumn(0);

        //Shadow Rush varies in power between Gamecube games
        if(_move_id == Moves::SHADOW_RUSH and _game_id == Versions::COLOSSEUM) _base_power = 90;

        query_stream.str("");
        query_stream << "SELECT gen" << _generation << "_pp FROM old_move_pps WHERE move_id=" << _move_id;
        SQLite::Statement pp_query(*_db, query_stream.str().c_str());
        if(pp_query.executeStep()) _base_pp = pp_query.getColumn(0);

        //Not enough type changes to warrant a database table
        if(_generation == 1)
        {
            if(_move_id == Moves::BITE or _move_id == Moves::GUST or
               _move_id == Moves::KARATE_CHOP or _move_id == Moves::SAND_ATTACK) _type_id = Types::NORMAL;
        }
        else if(_move_id == Moves::CURSE and _generation < 4) _type_id = Types::QUESTION_MARK; // ???
        else if(_move_id == Moves::CHARM or _move_id == Moves::MOONLIGHT or
                _move_id == Moves::SWEET_KISS) _type_id = Types::NORMAL;

        //Only one move changes categories before Generation IV
        if(_generation == 1 and _move_id == Moves::BITE) _move_damage_class_id = Move_Classes::PHYSICAL;
        
        //TODO: targeting changes, making contact

        query_stream.str("");
        query_stream << "SELECT gen" << _generation << "_priority FROM old_move_priorities WHERE move_id=" << _move_id;
        SQLite::Statement priority_query(*_db, query_stream.str().c_str());
        if(priority_query.executeStep()) _base_priority = priority_query.getColumn(0);

        //Only one move changed name between Generation II-III
        if(_move_id == Moves::CONVERSION_2 and _generation < 3) _move_name = "Conversion2";

        query_stream.str("");
        query_stream << "SELECT name FROM old_move_names WHERE move_id=" << _move_id;
        SQLite::Statement name_query(*_db, query_stream.str().c_str());
        if(name_query.executeStep()) _move_name = (const char*)(name_query.getColumn(0));
    }
} /* namespace pkmn */
