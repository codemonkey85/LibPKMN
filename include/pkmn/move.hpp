/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_MOVE_HPP
#define INCLUDED_PKMN_MOVE_HPP

#include <string>

#include <pkmn/config.hpp>
#include <pkmn/types/array.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    class PKMN_API move
    {
        public:

            typedef pkmn::shared_ptr<move> sptr;
            static sptr make(unsigned int id, unsigned int game);
            static sptr make(std::string name, std::string game);

            move() {};
            virtual ~move() {};

            //Game-specific Info
            virtual std::string get_game() const = 0;
            virtual unsigned int get_generation() const = 0;

            //Non-battle Stats
            virtual std::string get_name() const = 0;
            virtual std::string get_description() const = 0;

            //Battle Stats
            virtual std::string get_type() const = 0;
            virtual unsigned int get_base_power() const = 0;
            virtual unsigned int get_base_pp() const = 0;
            virtual std::string get_move_damage_class() const = 0;
            virtual double get_base_accuracy() const = 0;
            virtual std::string get_effect() const = 0;
            virtual double get_effect_chance() const = 0;
            virtual unsigned int get_priority() const = 0;
            virtual std::string get_target() const = 0;

            //Database Info
            virtual unsigned int get_move_id() const = 0;
            virtual unsigned int get_target_id() const = 0;
            virtual unsigned int get_game_id() const = 0;
            virtual unsigned int get_move_damage_class_id() const = 0;
            virtual unsigned int get_effect_id() const = 0;
    };

    //Related typedefs
    typedef std::vector<move::sptr> move_vector_t;
    typedef pkmn::array<move::sptr> moveset_t;
}

#endif /* INCLUDED_PKMN_MOVE_HPP */
