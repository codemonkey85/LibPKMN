/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_LIBRARY_WORKAROUNDS_HPP
#define INCLUDED_LIBRARY_WORKAROUNDS_HPP

#include <stdint.h>

#include <pkmnsim/types/dict.hpp>

namespace pkmnsim
{
    dict<char, int> get_pokehack_reverse_char_map();
    
    char* pokehack_get_text();
    
    uint8_t pokehack_get_IV(uint32_t* IVint, uint8_t IV);
    
    void pokehack_set_IV(uint32_t* IVint, uint8_t IV, uint8_t val);
    
    bool pokehack_get_ribbon(uint32_t* ribbonint, uint8_t ribbon);
    
    void pokehack_set_ribbon(uint32_t* ribbonint, uint8_t ribbon, bool val);
}

#endif /* INCLUDED_LIBRARY_WORKAROUNDS_HPP */