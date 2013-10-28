/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ITEM_IMPL_HPP
#define INCLUDED_ITEM_IMPL_HPP

#include <pkmnsim/item.hpp>

namespace pkmnsim
{
    class item_impl: public item
    {
        public:

            item_impl(unsigned int id, unsigned int game);

            unsigned int get_game_id();
            unsigned int get_category_id();
            std::string get_category_name();
            virtual unsigned int get_index();
            unsigned int get_item_id();
            virtual std::string get_description();
            virtual std::string get_name();
    };
}

#endif /* INCLUDED_ITEM_IMPL_HPP */
