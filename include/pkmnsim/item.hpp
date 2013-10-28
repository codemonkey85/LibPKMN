/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_ITEM_HPP
#define INCLUDED_PKMNSIM_ITEM_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <string>
#include <vector>

#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    class PKMNSIM_API item
    {
        public:

            typedef std::shared_ptr<item> sptr;
            static sptr make(unsigned int id, unsigned int game);

            item() {};
            virtual ~item() {};
            
            virtual unsigned int get_game_id() = 0;
            virtual unsigned int get_category_id() = 0;
            virtual std::string get_category_name() = 0;
            virtual unsigned int get_index() = 0;
            virtual unsigned int get_item_id() = 0;
            virtual std::string get_description() = 0;
            virtual std::string get_name() = 0;
            
        protected:
            
            unsigned int item_id, game_id, category_id, game_index;
            std::string category_name, description, item_name;
    };
}

#endif /* INCLUDED_PKMNSIM_ITEM_HPP */
