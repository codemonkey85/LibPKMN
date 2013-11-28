/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_LIBRARY_BRIDGE_HPP
#define INCLUDED_LIBRARY_BRIDGE_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <stdint.h>

#include <pkmnsim/types/dict.hpp>

#include <pokehack/SaveParser.h>

#include <pkmds/pkmds_g5.h>

namespace pkmnsim
{
    dict<char, int> get_pokehack_reverse_char_map();
    
    char* pokehack_get_text(unsigned char* raw, bool is_nickname);
    
    uint8_t gen3_4_5_get_IV(uint32_t* IVint, uint8_t IV);
    
    void gen3_4_5_set_IV(uint32_t* IVint, uint8_t IV, uint8_t val);
    
    bool get_marking(uint8_t* markint, uint8_t mark);
    
    void set_marking(uint8_t* markint, uint8_t mark, bool val);
    
    bool get_hoenn_ribbon(uint32_t* ribbonint, uint8_t ribbon);
    
    void set_hoenn_ribbon(uint32_t* ribbonint, uint8_t ribbon, bool val);
    
    bool get_sinnoh_ribbon32(uint32_t* ribbonint, uint8_t ribbon);
    
    void set_sinnoh_ribbon32(uint32_t* ribbonint, uint8_t ribbon, bool val);

    uint8_t get_gen3_ball(uint16_t* metlevelint);

    void set_gen3_ball(uint16_t* metlevelint, uint8_t ball);

    uint8_t get_gen3_met_level(uint16_t* metlevelint);

    void set_gen3_met_level(uint16_t* metlevelint, uint8_t level);

    uint8_t get_gen4_5_met_level(uint8_t* metlevelint);

    void set_gen4_5_met_level(uint8_t* metlevelint, uint8_t level);

    bool get_gen3_otgender(uint16_t* metlevelint);

    void set_gen3_otgender(uint16_t* metlevelint, bool is_female);

    bool get_gen4_5_otgender(uint8_t* metlevelint);

    void set_gen4_5_otgender(uint8_t* metlevelint, bool is_female);
    
    uint8_t pkmnsim_getpkmstat(pokemon_obj* pkm);
    
    void pkmnsim_pctoparty(party_pkm* p_pkm, pokemon_obj* pkm);

    uint8_t pkmnsim_game_to_hometown(uint8_t game);

    uint8_t hometown_to_pkmnsim_game(uint8_t hometown);

    uint8_t pkmnsim_ball_to_game_ball(uint8_t ball);
    
    uint8_t game_ball_to_pkmnsim_ball(uint8_t game_ball);

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
