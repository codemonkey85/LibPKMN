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
            
            virtual item::sptr get_item(unsigned int pos, bool copy = true) = 0;
            virtual void remove_item(unsigned int pos) = 0;
            virtual void set_item(unsigned int pos, item::sptr new_item) = 0;
            
        protected:
            
            unsigned int game_id, pocket_size;
            std::string pocket_name;
            
            vla<item::sptr> contents;
    };
}

#endif /* INCLUDED_PKMNSIM_POCKET_HPP */
