/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_LIBRARY_WORKAROUNDS_HPP
#define INCLUDED_LIBRARY_WORKAROUNDS_HPP

#include <pkmnsim/types/dict.hpp>

namespace pkmnsim
{
    dict<char, int> get_pokehack_reverse_char_map();
    
    char* pokehack_get_text();
}

#endif /* INCLUDED_LIBRARY_WORKAROUNDS_HPP */