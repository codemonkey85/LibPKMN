/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BAG_IMPL_HPP
#define INCLUDED_BAG_IMPL_HPP

#include <string>
#include <vector>

#include <pkmn/bag.hpp>

namespace pkmn
{
    class bag_impl: public bag
    {
        public:

            bag_impl();
            bag_impl(unsigned int game);

            unsigned int get_game_id();
            pocket::sptr get_pocket(std::string name);
            void get_pocket_list(std::vector<std::string> &pocket_vec);
            
        private:
        
            unsigned int _game_id;
            dict<std::string, pocket::sptr> _contents;

            void _check() const;
    };
}

#endif /* INCLUDED_BAG_IMPL_HPP */
