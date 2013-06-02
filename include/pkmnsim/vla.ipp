/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_VLA_IPP
#define INCLUDED_VLA_IPP

#include <stdexcept>

#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    template<typename item_type>
    vla<item_type>::vla () {}

    template<typename item_type>
    vla<item_type>::vla(int items)
    {
        _vec.clear();
        max_items = items;

        for(int i = 0; i < max_items; i++) _vec.push_back(*(new item_type()));

        invalid_pos_err_msg = "Position must be 0-" + to_string(max_items) + ".";
    }

    template<typename item_type>
    item_type& vla<item_type>::operator[](int pos)
    {
        if(pos < 0 or pos >= max_items) throw std::runtime_error(invalid_pos_err_msg.c_str());
        else return _vec[pos];
    }
}

#endif /* INCLUDED_VLA_IPP */
