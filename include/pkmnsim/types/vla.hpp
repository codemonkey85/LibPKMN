/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMNSIM_TYPES_VLA_HPP
#define INCLUDED_PKMNSIM_TYPES_VLA_HPP

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

            std::size_t size(void) const;

            //Get or set item at position pos
            item_type& operator[](int pos);

            //Can't use [] with Python
            item_type get(int pos) const;
            void set(int pos, item_type val);

        private:
        
            std::vector<item_type> _vec;
            std::string invalid_pos_err_msg;
            int max_items;
    };
}

#include <pkmnsim/types/vla.ipp>

#endif /* INCLUDED_PKMNSIM_TYPES_VLA_HPP */
