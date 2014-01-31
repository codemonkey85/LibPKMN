/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_TYPES_ARRAY_HPP
#define INCLUDED_PKMN_TYPES_ARRAY_HPP

#include <string>
#include <vector>

namespace pkmn
{
    template <typename item_type>
    class array
    {
        public:
        
            array();
            array(size_t size);

            //std::array functions
            size_t size() const;
            item_type& operator[](size_t pos);
            item_type& at(size_t pos) const;
            item_type& front() const;
            item_type& back() const;
            item_type* data() const;

            void set(item_type val, size_t pos);

        private:
        
            std::vector<item_type> _vec;
            std::string invalid_pos_err_msg;
            size_t _size;
    };
}

#include <pkmn/types/array.ipp>

#endif /* INCLUDED_PKMN_TYPES_ARRAY_HPP */
