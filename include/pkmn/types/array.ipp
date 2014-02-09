/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_TYPES_ARRAY_IPP
#define INCLUDED_PKMN_TYPES_ARRAY_IPP

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <pkmn/config.hpp>

namespace pkmn
{
    template<typename item_type>
    array<item_type>::array () {}

    template<typename item_type>
    array<item_type>::array(size_t size)
    {
        _vec = std::vector<item_type>(size);
        _size = size;

        invalid_pos_err_msg = "Position must be 0-" + to_string(_size - 1) + ".";
    }

    template<typename item_type>
    size_t array<item_type>::size() const
    {
        return _size;
    }

    template<typename item_type>
    const item_type& array<item_type>::operator[](const size_t pos) const
    {
        if(pos >= _size) throw std::runtime_error(invalid_pos_err_msg.c_str());
        else return _vec[pos];
    }

    template <typename item_type>
    item_type& array<item_type>::operator[](const size_t pos)
    {
        if(pos >= _size) throw std::runtime_error(invalid_pos_err_msg.c_str());
        else return _vec[pos];
    }

    template<typename item_type>
    item_type array<item_type>::at(size_t pos) const
    {
        if(pos < 0 or pos >= _size) throw std::runtime_error(invalid_pos_err_msg.c_str());
        else return _vec[pos];
    }

    template<typename item_type>
    item_type array<item_type>::front() const {return _vec[0];}

    template<typename item_type>
    item_type array<item_type>::back() const {return _vec[_size-1];}

    template<typename item_type>
    void array<item_type>::set(item_type val, size_t pos)
    {
        if(pos < 0 or pos >= _size) throw std::runtime_error(invalid_pos_err_msg.c_str());
        else _vec[pos] = val;
    }
}

#endif /* INCLUDED_PKMN_TYPES_ARRAY_IPP */
