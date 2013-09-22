/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_CONVERSIONS_HPP
#define INCLUDED_PKMNSIM_CONVERSIONS_HPP

#include <pkmnsim/config.hpp>
#include <pkmnsim/dict.hpp>
#include <pkmnsim/spec_pokemon.hpp>

#include <pokehack/pokestructs.h>
#include <pokelib/pokelib.h>
#include <pkmds/pkmds_g5.h>

namespace pkmnsim
{
    class PKMNSIM_API conversions
    {
        public:
            converter();

            static spec_pokemon::sptr pokehack_pkmn_to_spec_pokemon(belt_pokemon_t* b_pkmn_t,
                                                       pokemon_attacks_t* pkmn_a_t,
                                                       pokemon_effort_t* pkmn_e_t,
                                                       pokemon_misc_t* pkmn_m_t,
                                                       pokemon_growth_t* pkmn_g_t);

            static void spec_pokemon_to_pokehack_pkmn(spec_pokemon::sptr s_pkmn,
                                            belt_pokemon_t* b_pkmn_t,
                                            pokemon_attacks_t* pkmn_a_t,
                                            pokemon_effort_t* pkmn_e_t,
                                            pokemon_misc_t* pkmn_m_t,
                                            pokemon_growth_t* pkmn_g_t);

            static spec_pokemon::sptr pokelib_pkmn_to_spec_pokemon(PokeLib::Pokemon pokelib_pkmn);

            static PokeLib::Pokemon spec_pokemon_to_pokelib_pkmn(spec_pokemon::sptr s_pkmn);

            static spec_pokemon::sptr pkmds_pkmn_to_spec_pokemon(party_pkm* p_pkm);

            static void spec_pokemon_to_pkmds_pkmn(spec_pokemon::sptr s_pkmn, party_pkm* p_pkm);

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
            char* pokehack_get_text(unsigned char* raw, bool is_nickname);

            dict<char, int> pokehack_reverse_char_map;
    };    
}

#endif /* INCLUDED_PKMNSIM_CONVERSIONS_HPP */
