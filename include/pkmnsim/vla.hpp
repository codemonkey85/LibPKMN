/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_VLA_HPP
#define INCLUDED_VLA_HPP

#include <string>
#include <vector>

namespace pkmnsim
{
    template <typename item_type>
    class vla
    {
        public:
            //Create empty VLA
            vla(void);

            //Constructor with generic item name
            vla(int max);

            //Get or set item at position pos
            item_type& operator[](int pos);

        private:
            std::vector<item_type> _vec;
            std::string invalid_pos_err_msg;
            int max_items;
    };
}

#include <pkmnsim/vla.ipp>

#endif /* INCLUDED_VLA_HPP */
