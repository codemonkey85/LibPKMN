/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_CONVERSIONS_HPP
#define INCLUDED_CONVERSIONS_HPP

#include <boost/assign.hpp>

#include <pkmnsim/spec_pkmn.hpp>

#include "sqlitecpp/SQLiteCPP.h"

#include <pokehack/pokestructs.h>
#include <pokelib/pokelib.h>
#include <pkmds/pkmds_g5.h>

namespace pkmnsim
{
    class converter
    {
        public:
            converter() {};

            static spec_pkmn::sptr pokehack_pkmn_to_spec_pkmn(belt_pokemon_t* b_pkmn_t,
                                                              pokemon_attacks_t* pkmn_a_t,
                                                              pokemon_effort_t* pkmn_e_t,
                                                              pokemon_misc_t* pkmn_m_t,
                                                              pokemon_growth_t* pkmn_g_t);

            static void spec_pkmn_to_pokehack_pkmn(spec_pkmn::sptr s_pkmn,
                                                   belt_pokemon_t* b_pkmn_t,
                                                   pokemon_attacks_t* pkmn_a_t,
                                                   pokemon_effort_t* pkmn_e_t,
                                                   pokemon_misc_t* pkmn_m_t,
                                                   pokemon_growth_t* pkmn_g_t);

            static spec_pkmn::sptr pokelib_pkmn_to_spec_pkmn(PokeLib::Pokemon pokelib_pkmn);

            static PokeLib::Pokemon spec_pkmn_to_pokelib_pkmn(spec_pkmn::sptr s_pkmn);

            static spec_pkmn::sptr pkmds_pkmn_to_spec_pkmn(party_pkm* p_pkm);

            static void spec_pkmn_to_pkmds_pkmn(spec_pkmn::sptr s_pkmn, party_pkm* p_pkm);

            static PokeLib::Pokemon pokehack_pkmn_to_pokelib_pkmn(belt_pokemon_t* b_pkmn_t,
                                                                  pokemon_attacks_t* pkmn_a_t,
                                                                  pokemon_effort_t* pkmn_e_t,
                                                                  pokemon_misc_t* pkmn_m_t,
                                                                  pokemon_growth_t* pkmn_g_t);

            static void pokehack_pkmn_to_pkmds_pkmn(belt_pokemon_t* b_pkmn_t,
                                                    pokemon_attacks_t* pkmn_a_t,
                                                    pokemon_effort_t* pkmn_e_t,
                                                    pokemon_misc_t* pkmn_m_t,
                                                    pokemon_growth_t* pkmn_g_t,
                                                    party_pkm* p_pkm);
                                                    
            static void pokelib_pkmn_to_pokehack_pkmn(PokeLib::Pokemon pokelib_pkmn,
                                                      belt_pokemon_t* b_pkmn_t,
                                                      pokemon_attacks_t* pkmn_a_t,
                                                      pokemon_effort_t* pkmn_e_t,
                                                      pokemon_misc_t* pkmn_m_t,
                                                      pokemon_growth_t* pkmn_g_t);
                                                      
            static void pokelib_pkmn_to_pkmds_pkmn(PokeLib::Pokemon pokelib_pkmn, party_pkm* p_pkm);
            
            static void pkmds_pkmn_to_pokehack_pkmn(party_pkm* p_pkm,
                                                    belt_pokemon_t* b_pkmn_t,
                                                    pokemon_attacks_t* pkmn_a_t,
                                                    pokemon_effort_t* pkmn_e_t,
                                                    pokemon_misc_t* pkmn_m_t,
                                                    pokemon_growth_t* pkmn_g_t);
            
            static PokeLib::Pokemon pkmds_pkmn_to_pokelib_pkmn(party_pkm* p_pkm);
            
        private:
            static char* pokehack_get_text(unsigned char* raw, bool is_nickname);
    };
    
    static dict<char, int> pokehack_reverse_char_map = boost::assign::map_list_of
        ('0', int(0xA1))
        ('1', int(0xA2))
        ('2', int(0xA3))
        ('3', int(0xA4))
        ('4', int(0xA5))
        ('5', int(0xA6))
        ('6', int(0xA7))
        ('7', int(0xA8))
        ('8', int(0xA9))
        ('9', int(0xAA))
        ('!', int(0xAB))
        ('?', int(0xAC))
        ('.', int(0xAD))
        ('-', int(0xAE))
        ('"', int(0xB1))
        ('\'', int(0xB2))
        ('/', int(0xBA))
        ('A', int(0xBB))
        ('B', int(0xBC))
        ('C', int(0xBD))
        ('D', int(0xBE))
        ('E', int(0xBF))
        ('F', int(0xC0))
        ('G', int(0xC1))
        ('H', int(0xC2))
        ('I', int(0xC3))
        ('J', int(0xC4))
        ('K', int(0xC5))
        ('L', int(0xC6))
        ('M', int(0xC7))
        ('N', int(0xC8))
        ('O', int(0xC9))
        ('P', int(0xCA))
        ('Q', int(0xCB))
        ('R', int(0xCC))
        ('S', int(0xCD))
        ('T', int(0xCE))
        ('U', int(0xCF))
        ('V', int(0xD0))
        ('W', int(0xD1))
        ('X', int(0xD2))
        ('Y', int(0xD3))
        ('Z', int(0xD4))
        ('a', int(0xD5))
        ('b', int(0xD6))
        ('c', int(0xD7))
        ('d', int(0xD8))
        ('e', int(0xD9))
        ('f', int(0xDA))
        ('g', int(0xDB))
        ('h', int(0xDC))
        ('i', int(0xDD))
        ('j', int(0xDE))
        ('k', int(0xDF))
        ('l', int(0xE0))
        ('m', int(0xE1))
        ('n', int(0xE2))
        ('o', int(0xE3))
        ('p', int(0xE4))
        ('q', int(0xE5))
        ('r', int(0xE6))
        ('s', int(0xE7))
        ('t', int(0xE8))
        ('u', int(0xE9))
        ('v', int(0xEA))
        ('w', int(0xEB))
        ('x', int(0xEC))
        ('y', int(0xED))
        ('z', int(0xEE))
        (' ', int(0xEF))
        ('\0', int(0xFF));
}

#endif /* INCLUDED_CONVERSIONS_HPP */
