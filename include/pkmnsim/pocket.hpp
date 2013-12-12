/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_POCKET_HPP
#define INCLUDED_PKMNSIM_POCKET_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <string>

#include <pkmnsim/config.hpp>
#include <pkmnsim/item.hpp>
#include <pkmnsim/types/vla.hpp>

namespace pkmnsim
{
    class PKMNSIM_API pocket
    {
        public:

            typedef std::shared_ptr<pocket> sptr;
            static sptr make(unsigned int game, std::string name, unsigned int size);

            pocket() {};
            virtual ~pocket() {};
            
            virtual unsigned int get_game_id() = 0;
            virtual std::string get_name() = 0;
            virtual unsigned int get_size() = 0;
            
            virtual item::sptr get_item(unsigned int pos) = 0;
            virtual void remove_item(unsigned int pos) = 0;
            virtual void set_item(unsigned int pos, item::sptr item, unsigned int amount) = 0;
            virtual void set_item(unsigned int pos, unsigned int item_id, unsigned int amount) = 0;
            virtual void set_item(item::sptr item, unsigned int amount) = 0;
            virtual void set_item(unsigned int item_id, unsigned int amount) = 0;
            
            virtual unsigned int get_amount(unsigned int pos) = 0;
            virtual void set_amount(unsigned int pos, unsigned int amount) = 0;
            
        protected:
            
            unsigned int _game_id, _pocket_size;
            std::string _pocket_name;
            
            vla<unsigned int> _contents;
            vla<unsigned int> _amounts;
    };
}

#endif /* INCLUDED_PKMNSIM_POCKET_HPP */
