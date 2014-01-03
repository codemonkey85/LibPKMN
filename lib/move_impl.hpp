/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_MOVE_IMPL_HPP
#define INCLUDED_MOVE_IMPL_HPP

#include <pkmn/move.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

namespace pkmnsim
{
    class move_impl: public move
    {
        public:
        
            move_impl(unsigned int id, unsigned int game);
        
            virtual std::string get_name() const;
            virtual std::string get_description() const;
            virtual unsigned int get_type() const;
            virtual unsigned int get_base_power() const;
            virtual unsigned int get_base_pp() const;
            virtual double get_base_accuracy() const;
            virtual unsigned int get_move_damage_class() const;
            virtual std::string get_base_effect() const;
            virtual double get_base_effect_chance() const;
            virtual unsigned int get_priority() const;

            virtual unsigned int get_move_id() const;
            virtual unsigned int get_target_id() const;
            virtual unsigned int get_game_id() const;

        protected:
        
            //Database values
            int _move_id, _type_id, _target_id;

            unsigned int _game_id;
            unsigned int _base_power;
            unsigned int _base_pp;
            double _base_accuracy;
            unsigned int _move_damage_class;
            unsigned int _base_priority;
            std::string _base_effect;
            double _base_effect_chance;
            
            static SQLite::Database _db;
    };
}

#endif /* INCLUDED_MOVE_IMPL_HPP */
