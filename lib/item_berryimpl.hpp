/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ITEM_BERRYIMPL_HPP
#define INCLUDED_ITEM_BERRYIMPL_HPP

#include "item_impl.hpp"

namespace pkmnsim
{
    class item_berryimpl: public item_impl
    {
        public:
        
            item_berryimpl(unsigned int id, unsigned int game);
            
        private:
        
            void gen2_to_gen345();
            void gen345_to_gen2();
    };
}

#endif /* INCLUDED_ITEM_BERRYIMPL_HPP */
