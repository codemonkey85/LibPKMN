/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_LIBRARY_BRIDGE_HPP
#define INCLUDED_LIBRARY_BRIDGE_HPP

#include <cstdint>

#include <boost/assign.hpp>

#include <pkmn/config.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/pokemon_text.hpp>

#include <pkmds/pkmds_g5.h>
#include <pkmds/pkmds_g6.h>

namespace pkmn
{
    static const pkmn::dict<uint8_t, pokemon_text> ball_dict = boost::assign::map_list_of
        (PokeBalls::UNKNOWN, "Poke Ball")
        (PokeBalls::POKE_BALL, "Poke Ball")
        (PokeBalls::GREAT_BALL, "Great Ball")
        (PokeBalls::ULTRA_BALL, "Ultra Ball")
        (PokeBalls::MASTER_BALL, "Master Ball")
        (PokeBalls::SAFARI_BALL, "Safari Ball")
        (PokeBalls::LEVEL_BALL, "Level Ball")
        (PokeBalls::LURE_BALL, "Lure Ball")
        (PokeBalls::MOON_BALL, "Moon Ball")
        (PokeBalls::FRIEND_BALL, "Friend Ball")
        (PokeBalls::LOVE_BALL, "Love Ball")
        (PokeBalls::HEAVY_BALL, "Heavy Ball")
        (PokeBalls::FAST_BALL, "Fast Ball")
        (PokeBalls::SPORT_BALL, "Sport Ball")
        (PokeBalls::PREMIER_BALL, "Premier Ball")
        (PokeBalls::REPEAT_BALL, "Repeat Ball")
        (PokeBalls::TIMER_BALL, "Timer Ball")
        (PokeBalls::NEST_BALL, "Nest Ball")
        (PokeBalls::NET_BALL, "Net Ball")
        (PokeBalls::DIVE_BALL, "Dive Ball")
        (PokeBalls::LUXURY_BALL, "Luxury Ball")
        (PokeBalls::HEAL_BALL, "Heal Ball")
        (PokeBalls::QUICK_BALL, "Quick Ball")
        (PokeBalls::DUSK_BALL, "Dusk Ball")
        (PokeBalls::CHERISH_BALL, "Cherish Ball")
        (PokeBalls::PARK_BALL, "Park Ball")
        (PokeBalls::DREAM_BALL, "Dream Ball")
    ;

    static const pkmn::dict<pokemon_text, uint8_t> reverse_ball_dict = boost::assign::map_list_of
        ("None", PokeBalls::POKE_BALL)
        ("Poke Ball", PokeBalls::POKE_BALL)
        ("Great Ball", PokeBalls::GREAT_BALL)
        ("Ultra Ball", PokeBalls::ULTRA_BALL)
        ("Master Ball", PokeBalls::MASTER_BALL)
        ("Safari Ball", PokeBalls::SAFARI_BALL)
        ("Level Ball", PokeBalls::LEVEL_BALL)
        ("Lure Ball", PokeBalls::LURE_BALL)
        ("Moon Ball", PokeBalls::MOON_BALL)
        ("Friend Ball", PokeBalls::FRIEND_BALL)
        ("Love Ball", PokeBalls::LOVE_BALL)
        ("Heavy Ball", PokeBalls::HEAVY_BALL)
        ("Fast Ball", PokeBalls::FAST_BALL)
        ("Sport Ball", PokeBalls::SPORT_BALL)
        ("Premier Ball", PokeBalls::PREMIER_BALL)
        ("Repeat Ball", PokeBalls::REPEAT_BALL)
        ("Timer Ball", PokeBalls::TIMER_BALL)
        ("Nest Ball", PokeBalls::NEST_BALL)
        ("Net Ball", PokeBalls::NET_BALL)
        ("Dive Ball", PokeBalls::DIVE_BALL)
        ("Luxury Ball", PokeBalls::LUXURY_BALL)
        ("Heal Ball", PokeBalls::HEAL_BALL)
        ("Quick Ball", PokeBalls::QUICK_BALL)
        ("Dusk Ball", PokeBalls::DUSK_BALL)
        ("Cherish Ball", PokeBalls::CHERISH_BALL)
        ("Park Ball", PokeBalls::PARK_BALL)
        ("Dream Ball", PokeBalls::DREAM_BALL)
    ;

    uint8_t PKMN_API modern_get_IV(uint32_t* IVint, uint8_t IV);

    void modern_set_IV(uint32_t* IVint, uint8_t IV, uint8_t val);
    
    bool get_marking(uint8_t* markint, uint8_t mark);
    
    void set_marking(uint8_t* markint, uint8_t mark, bool val);
    
    bool get_ribbon(uint32_t* ribbonint, uint8_t ribbon);
    
    void set_ribbon(uint32_t* ribbonint, uint8_t ribbon, bool val);

    uint8_t get_gen3_ball(uint16_t* metlevelint);

    void set_gen3_ball(uint16_t* metlevelint, uint8_t ball);

    uint8_t get_gen3_met_level(uint16_t* metlevelint);

    void set_gen3_met_level(uint16_t* metlevelint, uint8_t level);

    uint8_t get_gen_456_met_level(uint8_t* metlevelint);

    void set_gen_456_met_level(uint8_t* metlevelint, uint8_t level);

    bool get_gen3_otgender(uint16_t* metlevelint);

    void set_gen3_otgender(uint16_t* metlevelint, bool is_female);

    bool get_gen_456_otgender(uint8_t* metlevelint);

    void set_gen_456_otgender(uint8_t* metlevelint, bool is_female);
    
    uint8_t libpkmn_getpkmstat(pokemon_obj* pkm);
    
    void libpkmn_pctoparty(party_pkm* p_pkm, pokemon_obj* pkm);
    
    std::string libpkmn_getpkxformnamesql(pokemonx_obj *pkx);
    
    uint16_t getpkxstat(pokemonx_obj *pkx, unsigned int stat_id);
    
    uint8_t libpkmn_pkxstat(pokemonx_obj* pkx);
    
    void libpkmn_pctopartyx(party_pkx* p_pkx, pokemonx_obj* pkx);

    uint8_t libpkmn_game_to_hometown(uint8_t game);

    uint8_t hometown_to_libpkmn_game(uint8_t hometown);

    uint8_t libpkmn_ball_to_game_ball(uint8_t ball);
    
    uint8_t game_ball_to_libpkmn_ball(uint8_t game_ball);

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
            BLACK2,
            X,
            Y
        };
    }
}

#endif /* INCLUDED_LIBRARY_BRIDGE_HPP */
