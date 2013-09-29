/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_MOVE_IMPL_HPP
#define INCLUDED_MOVE_IMPL_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <string>

#include <pkmnsim/config.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/move.hpp>
#include <pkmnsim/types/dict.hpp>
#include <pkmnsim/types/vla.hpp>

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
            virtual unsigned int get_type_id() const;
            virtual unsigned int get_target_id() const;
            virtual unsigned int get_game_id() const;
    };

    //Related typedefs
    typedef std::vector<move::sptr> b_move_vec_t;
    typedef vla<move::sptr> b_move_vla_t;
}

#endif /* INCLUDED_MOVE_IMPL_HPP */
