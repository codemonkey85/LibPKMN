/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BAG_IMPL_HPP
#define INCLUDED_BAG_IMPL_HPP

#include <string>
#include <vector>

#include <pkmnsim/bag.hpp>

namespace pkmnsim
{
    class bag_impl: public bag
    {
        public:

            bag_impl(unsigned int game);

            unsigned int get_game_id();
            const pocket::sptr get_pocket(std::string name);
            void get_pocket_list(std::vector<std::string> &pocket_vec);
    };
}

#endif /* INCLUDED_BAG_IMPL_HPP */
