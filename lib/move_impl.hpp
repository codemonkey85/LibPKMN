/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_MOVE_IMPL_HPP
#define INCLUDED_MOVE_IMPL_HPP

#include <pkmn/move.hpp>

#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    class move_impl: public move
    {
        public:

            move_impl(unsigned int id, unsigned int game);

            //Game-specific Info
            std::string get_game() const;
            unsigned int get_generation() const;

            //Non-battle Stats
            std::string get_name() const;
            std::string get_description() const;

            //Battle Stats
            std::string get_type() const;
            unsigned int get_base_power() const;
            unsigned int get_base_pp() const;
            std::string get_move_damage_class() const;
            double get_base_accuracy() const;
            std::string get_effect() const;
            double get_effect_chance() const;
            int get_priority() const;
            std::string get_target() const;

            //Database Info
            unsigned int get_move_id() const;
            unsigned int get_target_id() const;
            unsigned int get_game_id() const;
            unsigned int get_move_damage_class_id() const;
            unsigned int get_effect_id() const;

        protected:

            //Database values
            unsigned int _game_id, _generation, _move_id, _type_id, _target_id;
            unsigned int _move_damage_class_id, _effect_id;

            unsigned int _base_power, _base_pp;
            int _base_priority;
            double _base_accuracy, _effect_chance;
            unsigned int _move_damage_class;
            std::string _effect;

            static SQLite::Database _db;
    };
}

#endif /* INCLUDED_MOVE_IMPL_HPP */
