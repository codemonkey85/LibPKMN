/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_MOVE_HPP
#define INCLUDED_BASE_MOVE_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif
#include <string>

#include <pkmnsim/config.hpp>

#include "../../lib/sqlitecpp/SQLiteCPP.h"

namespace pkmnsim
{
    /*
     * Base Move Class
     *
     * This class generates an object with all the information for a battle move,
     * as appropriate to the specified generation.
     */
    class PKMNSIM_API base_move
    {
        public:

            typedef std::shared_ptr<base_move> sptr;

            //Class Constructors (should never be called directly)
            base_move(void) {};
            base_move(std::string identifier, SQLite::Database *db, int gen);

            /*
             * Returns a std::shared_ptr<base_move> of specified move.
             * Verifies validity of move+generation before returning value.
             *
             * Parameters:
             *  - identifier: name of move
             *  - gen: generation (1-5) from which to use move
             */
            static sptr make(std::string identifier, int gen);

            //Used for moves with different effects based on Pokemon
            //static sptr make(std::string identifier, int gen, spec_pkmn::sptr<spec_pkmn> s_pkmn);

            /*
             * Returns move's name.
             */
            std::string get_name(void);

            /*
             * Returns description of move's effects.
             */
            std::string get_description(void);

            /*
             * Returns move's type.
             */
            std::string get_type(void);

            /*
             * Returns move's base power value (to be input into damage algorithm).
             */
            int get_base_power(void);

            /*
             * Returns move's Power Points (number of times move can be used without replenishing)
             */
            int get_base_pp(void);

            /*
             * Returns moves chances of succeeding (0.0-1.0).
             */
            double get_base_accuracy(void);

            //TODO: implement
            //std::string get_move_damage_class(void) {return move_damage_class;}

            /*
             * Returns effect (burn, paralyze, etc) of move (without any in-battle changes).
             */
            std::string get_base_effect(void);

            /*
             * Returns chance of causing effect (0.0-1.0).
             */
            double get_base_effect_chance(void);

        protected:
            //Database values
            int move_id, type_id, target_id;

            std::string name;
            std::string description;
            std::string move_identifier;
            std::string type;
            int base_power;
            int base_pp;
            double base_accuracy;
            std::string move_damage_class;
            int base_priority;
            std::string base_effect;
            double base_effect_chance;
    };
}

#endif /* INCLUDED_BASE_MOVE_HPP */
