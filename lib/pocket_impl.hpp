/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_POCKET_IMPL_HPP
#define INCLUDED_POCKET_IMPL_HPP

#include <pkmnsim/pocket.hpp>

namespace pkmnsim
{
    class pocket_impl: public pocket
    {
        public:

            pocket_impl(unsigned int game, std::string name, unsigned int size);

            unsigned int get_game_id();
            std::string get_name();
            unsigned int get_size();
            
            item::sptr get_item(unsigned int pos, bool copy = true);
            void remove_item(unsigned int pos);
            void set_item(unsigned int pos, item::sptr new_item);
    };
}

#endif /* INCLUDED_POCKET_IMPL_HPP */
