/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_MOVE_HPP
#define INCLUDED_PKMNSIM_MOVE_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <string>

#include <pkmn/config.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/vla.hpp>

namespace pkmn
{
    class PKMNSIM_API move
    {
        public:

            typedef std::shared_ptr<move> sptr;
            static sptr make(unsigned int id, unsigned int game);

            move() {};
            virtual ~move() {};

            virtual std::string get_name() const = 0;
            virtual std::string get_description() const = 0;
            virtual unsigned int get_type() const = 0;
            virtual unsigned int get_base_power() const = 0;
            virtual unsigned int get_base_pp() const = 0;
            virtual double get_base_accuracy() const = 0;
            virtual unsigned int get_move_damage_class() const = 0;
            virtual std::string get_base_effect() const = 0;
            virtual double get_base_effect_chance() const = 0;
            virtual unsigned int get_priority() const = 0;

            virtual unsigned int get_move_id() const = 0;
            virtual unsigned int get_target_id() const = 0;
            virtual unsigned int get_game_id() const = 0;
    };

    //Related typedefs
    typedef std::vector<move::sptr> move_vec_t;
    typedef vla<move::sptr> move_vla_t;
    typedef vla<move::sptr> moveset_t;
}

#endif /* INCLUDED_PKMNSIM_MOVE_HPP */
