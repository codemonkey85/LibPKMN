/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_BAG_HPP
#define INCLUDED_PKMN_BAG_HPP

#include <string>

#include <pkmn/config.hpp>
#include <pkmn/item.hpp>
#include <pkmn/pocket.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    class PKMN_API bag
    {
        public:

            typedef pkmn::shared_ptr<bag> sptr;
            static sptr make(unsigned int game);
            static sptr make(std::string game);

            bag() {};
            virtual ~bag() {};

            virtual std::string get_game() = 0;
            virtual unsigned int get_generation() = 0;

            virtual void add_item(pokemon_text item_name, unsigned int amount) = 0;
            virtual void add_item(unsigned int item_id, unsigned int amount) = 0;
            virtual void add_item(item::sptr item_sptr, unsigned int amount) = 0;
            virtual void remove_item(pokemon_text item_name, unsigned int amount) = 0;
            virtual void remove_item(unsigned int item_id, unsigned int amount) = 0;
            virtual void remove_item(item::sptr item_sptr, unsigned int amount) = 0;
            virtual unsigned int get_item_amount(pokemon_text item_name);
            virtual unsigned int get_item_amount(unsigned int item_id);
            virtual unsigned int get_item_amount(item::sptr item_sptr);
            
            virtual pocket::sptr get_pocket(std::string name) = 0;
            virtual pkmn::dict<std::string, pocket::sptr> get_pockets() = 0;

            virtual unsigned int get_game_id() = 0;
    };
}

#endif /* INCLUDED_PKMN_BAG_HPP */
