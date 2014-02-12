/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_POCKET_HPP
#define INCLUDED_PKMN_POCKET_HPP

#include <string>

#include <pkmn/config.hpp>
#include <pkmn/item.hpp>
#include <pkmn/types/shared_ptr.hpp>
#include <pkmn/types/array.hpp>

namespace pkmn
{
    class PKMN_API pocket
    {
        public:

            typedef pkmn::shared_ptr<pocket> sptr;
            static sptr make(unsigned int game, std::string name, unsigned int size);
            static sptr make(std::string game, std::string name, unsigned int size);

            pocket() {};
            virtual ~pocket() {};
            
            virtual unsigned int get_game_id() = 0;
            virtual std::string get_name() = 0;
            virtual unsigned int get_size() = 0;
            
            virtual item::sptr get_item(unsigned int pos) = 0;
            virtual void remove_item(unsigned int pos) = 0;
            virtual void set_item(unsigned int pos, item::sptr item_sptr, unsigned int amount) = 0;
            virtual void set_item(unsigned int pos, unsigned int item_id, unsigned int amount) = 0;
            virtual void set_item(item::sptr item_sptr, unsigned int amount) = 0;
            virtual void set_item(unsigned int item_id, unsigned int amount) = 0;
            
            virtual unsigned int get_amount(unsigned int pos) = 0;
            virtual void set_amount(unsigned int pos, unsigned int amount) = 0;
    };
}

#endif /* INCLUDED_PKMN_POCKET_HPP */
