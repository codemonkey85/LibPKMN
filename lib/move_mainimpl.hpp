/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_MOVE_MAINIMPL_HPP
#define INCLUDED_MOVE_MAINIMPL_HPP

#include <string>

#include "move_impl.hpp"

using namespace std;

namespace pkmnsim
{
    class move_mainimpl: public move_impl
    {
        public:
        
            move_mainimpl(unsigned int id, unsigned int game);
    };
}

#endif /* INCLUDED_MOVE_MAINIMPL_HPP */