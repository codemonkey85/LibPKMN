/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_MOVE_HPP
#define INCLUDED_BASE_MOVE_HPP

#include <boost/shared_ptr.hpp>
#include <pkmnsim/config.hpp>
#include <string>

namespace pkmnsim
{
    class PKMNSIM_API base_move
    {
        public:

            typedef boost::shared_ptr<base_move> sptr;

            base_move() {};
            base_move(std::string identifier, int gen);

            static sptr make(std::string identifier, int gen);

            //Used for moves with different effects based on Pokemon
            //static sptr make(std::string identifier, int gen, spec_pkmn::sptr<spec_pkmn> s_pkmn);

            std::string get_name() {return name;}
            std::string get_description() {return description;}
            std::string get_type() {return type;}
            int get_base_power() {return base_power;}
            int get_base_pp() {return base_pp;}
            double get_base_accuracy() {return base_accuracy;}
            std::string get_move_damage_class() {return move_damage_class;}
            std::string get_base_effect() {return base_effect;}
            double get_base_effect_chance() {return base_effect_chance;}

        protected:
            std::string name;
            std::string description;
            int move_id;
            std::string move_identifier;
            int type_id;
            std::string type;
            int target_id;
            int base_power;
            int base_pp;
            double base_accuracy;
            std::string move_damage_class;
            int base_priority;
            std::string base_effect;
            double base_effect_chance;
    };

    //User-friendly interface for base_move::make()
    base_move::sptr PKMNSIM_API get_base_move(std::string identifier, int gen);
}

#endif /* INCLUDED_BASE_MOVE_HPP */
