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
            vla(int max_items);

            //Constructor with specified item name
            vla(std::string nice_name, int max);

            //Put item in back
            void put(item_type item);

            //Put item in specified position
            void put(item_type item, int pos);

            //Get item in specified position
            item_type get(int pos);

            //Get item in front
            item_type get_lead();

            //Remove item in specified position
            void remove(int pos);

            //Get size
            int size();

        private:
            std::vector<item_type> _vec;
            std::string nice_name, full_vec_err_msg, invalid_pos_err_msg, empty_vec_err_msg;
            int max_items;
    };
}

#include <pkmnsim/vla.ipp>

#endif /* INCLUDED_VLA_HPP */
