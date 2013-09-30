/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_LIBRARY_BRIDGE_HPP
#define INCLUDED_LIBRARY_BRIDGE_HPP

#include <stdint.h>

#include <pkmnsim/types/dict.hpp>

namespace pkmnsim
{
    dict<char, int> get_pokehack_reverse_char_map();
    
    char* pokehack_get_text(unsigned char* raw, bool is_nickname);
    
    uint8_t pokehack_get_IV(uint32_t* IVint, uint8_t IV);
    
    void pokehack_set_IV(uint32_t* IVint, uint8_t IV, uint8_t val);
    
    bool get_marking(uint8_t* markint, uint8_t mark);
    
    void set_marking(uint8_t* markint, uint8_t mark, bool val);
    
    bool get_hoenn_ribbon(uint32_t* ribbonint, uint8_t ribbon);
    
    void set_hoenn_ribbon(uint32_t* ribbonint, uint8_t ribbon, bool val);
    
    bool get_sinnohA_ribbon(uint64_t* ribbonint, uint8_t ribbon);
    
    void set_sinnohA_ribbon(uint64_t* ribbonint, uint8_t ribbon, bool val);
    
    bool get_sinnohB_ribbon(uint64_t* ribbonint, uint8_t ribbon);
    
    void set_sinnohB_ribbon(uint64_t* ribbonint, uint8_t ribbon, bool val);

    uint8_t pkmnsim_game_to_hometown(uint8_t game);

    uint8_t hometown_to_pkmnsim_game(uint8_t hometown);

    namespace Hometowns
    {
        enum hometowns
        {
            NONE,
            SAPPHIRE,
            RUBY,
            EMERALD,
            FIRE_RED,
            LEAF_GREEN,
            HEART_GOLD = 7,
            SOUL_SILVER,
            DIAMOND = 10,
            PEARL,
            PLATINUM,
            COLOSSEUM_XD = 15,
            WHITE = 20,
            BLACK,
            WHITE2,
            BLACK2
        };
    }
}

#endif /* INCLUDED_LIBRARY_BRIDGE_HPP */
