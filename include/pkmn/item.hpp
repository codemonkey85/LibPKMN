/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_ITEM_HPP
#define INCLUDED_PKMN_ITEM_HPP

#include <string>
#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    class PKMN_API item
    {
        public:

            typedef pkmn::shared_ptr<item> sptr;
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
    };
}

#endif /* INCLUDED_PKMN_ITEM_HPP */
