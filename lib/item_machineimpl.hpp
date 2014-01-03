/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ITEM_MACHINEIMPL_HPP
#define INCLUDED_ITEM_MACHINEIMPL_HPP

#include "item_impl.hpp"

namespace pkmn
{
    class item_machineimpl: public item_impl
    {
        public:
        
            item_machineimpl(unsigned int id, unsigned int game);
            
        private:
        
            unsigned int _machine_id, _move_id;
    };
}

#endif /* INCLUDED_ITEM_MACHINEIMPL_HPP */
