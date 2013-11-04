/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMNSIM_TYPES_VLA_IPP
#define INCLUDED_PKMNSIM_TYPES_VLA_IPP

#include <cstdlib>
#include <iostream>

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

        _vec = std::vector<item_type>(items);

        invalid_pos_err_msg = "Position must be 0-" + to_string(max_items - 1) + ".";
    }

    template<typename item_type>
    std::size_t vla<item_type>::size(void) const
    {
        return max_items;
    }

    template<typename item_type>
    item_type& vla<item_type>::operator[](int pos)
    {
        if(pos < 0 or pos >= max_items)
        {
            std::cerr << invalid_pos_err_msg << std::endl;
            exit(EXIT_FAILURE);
        }
        else return _vec[pos];
    }

    template<typename item_type>
    void vla<item_type>::set(int pos, item_type val)
    {
        if(pos < 0 or pos >= max_items)
        {
            std::cerr << invalid_pos_err_msg << std::endl;
            exit(EXIT_FAILURE);
        }
        else _vec[pos] = val;
    }

    template<typename item_type>
    item_type vla<item_type>::get(int pos) const
    {
        if(pos < 0 or pos >= max_items)
        {
            std::cerr << invalid_pos_err_msg << std::endl;
            exit(EXIT_FAILURE);
        }
        return _vec[pos];
    }

}

#endif /* INCLUDED_PKMNSIM_TYPES_VLA_IPP */
