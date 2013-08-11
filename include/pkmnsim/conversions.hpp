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
#include <pkmnsim/spec_pkmn.hpp>

#include <pokehack/pokestructs.h>
#include <pokelib/data_tables.h>
#include <pokelib/pokelib.h>
#include <pkmds/pkmds_g5.h>

namespace pkmnsim
{
    class PKMNSIM_API converter
    {
        public:
            converter();

            /*
             * Converts Pokehack's Pokemon data structures to PKMNsim's spec_pkmn.
             *
             * Parameters:
             *  - b_pkmn_t: Pointer to Pokehack's Belt Pokemon data structure
             *  - pkmn_a_t: Pointer to Pokehack's Pokemon Attacks data structure
             *  - pkmn_e_t: Pointer to Pokehack's Pokemon Effort data structure
             *  - pkmn_m_t: Pointer to Pokehack's Pokemon Misc Stats data structure
             *  - pkmn_g_t: Pointer to Pokehack's Pokemon Growth data structure
             */
            spec_pkmn::sptr pokehack_pkmn_to_spec_pkmn(belt_pokemon_t* b_pkmn_t,
                                                       pokemon_attacks_t* pkmn_a_t,
                                                       pokemon_effort_t* pkmn_e_t,
                                                       pokemon_misc_t* pkmn_m_t,
                                                       pokemon_growth_t* pkmn_g_t);

            /*
             * Converts PKMNsim's spec_pkmn to Pokehack's Pokemon data structures.
             *
             * Parameters:
             *  - s_pkmn: PKMNsim's spec_pkmn to convert
             *  - b_pkmn_t: Pointer to Pokehack's Belt Pokemon data structure
             *  - pkmn_a_t: Pointer to Pokehack's Pokemon Attacks data structure
             *  - pkmn_e_t: Pointer to Pokehack's Pokemon Effort data structure
             *  - pkmn_m_t: Pointer to Pokehack's Pokemon Misc Stats data structure
             *  - pkmn_g_t: Pointer to Pokehack's Pokemon Growth data structure
             */
            void spec_pkmn_to_pokehack_pkmn(spec_pkmn::sptr s_pkmn,
                                            belt_pokemon_t* b_pkmn_t,
                                            pokemon_attacks_t* pkmn_a_t,
                                            pokemon_effort_t* pkmn_e_t,
                                            pokemon_misc_t* pkmn_m_t,
                                            pokemon_growth_t* pkmn_g_t);

            /*
             * Converts PokeLib's Pokemon data structure to PKMNsim's spec_pkmn.
             *
             * Parameters:
             *  - pokelib_pkmn: PokeLib's Pokemon data structure to convert
             *  - b_pkmn_t: Pointer to Pokehack's Belt Pokemon data structure
             *  - pkmn_a_t: Pointer to Pokehack's Pokemon Attacks data structure
             *  - pkmn_e_t: Pointer to Pokehack's Pokemon Effort data structure
             *  - pkmn_m_t: Pointer to Pokehack's Pokemon Misc Stats data structure
             *  - pkmn_g_t: Pointer to Pokehack's Pokemon Growth data structure
             */
            spec_pkmn::sptr pokelib_pkmn_to_spec_pkmn(PokeLib::Pokemon pokelib_pkmn);

            /*
             * Converts PKMNsim's spec_pkmn to Pokelib's Pokemon data structure.
             *
             * Parameters:
             *  - s_pkmn: PKMNsim's spec_pkmn to convert
             */
            PokeLib::Pokemon spec_pkmn_to_pokelib_pkmn(spec_pkmn::sptr s_pkmn);

            /*
             * Converts PKMDS's Pokemon data structure to PKMNsim's spec_pkmn.
             *
             * Parameters:
             *  - p_pkm: PKMDS's Pokemon data structure to convert
             */
            spec_pkmn::sptr pkmds_pkmn_to_spec_pkmn(party_pkm* p_pkm);

            /*
             * Converts PKMNsim's spec_pkmn to PKMDS's Pokemon data structure.
             *
             * Parameters:
             *  - s_pkmn: PKMNsim's spec_pkmn to convert
             *  - p_pkm: PKMDS's Pokemon data structure to return conversion
             */
            void spec_pkmn_to_pkmds_pkmn(spec_pkmn::sptr s_pkmn, party_pkm* p_pkm);

            /*
             * Converts Pokehack's Pokemon data structures to PokeLib's through
             * PKMNsim's spec_pkmn.
             *
             * Parameters:
             *  - b_pkmn_t: Pointer to Pokehack's Belt Pokemon data structure
             *  - pkmn_a_t: Pointer to Pokehack's Pokemon Attacks data structure
             *  - pkmn_e_t: Pointer to Pokehack's Pokemon Effort data structure
             *  - pkmn_m_t: Pointer to Pokehack's Pokemon Misc Stats data structure
             *  - pkmn_g_t: Pointer to Pokehack's Pokemon Growth data structure
             */
            PokeLib::Pokemon pokehack_pkmn_to_pokelib_pkmn(belt_pokemon_t* b_pkmn_t,
                                                           pokemon_attacks_t* pkmn_a_t,
                                                           pokemon_effort_t* pkmn_e_t,
                                                           pokemon_misc_t* pkmn_m_t,
                                                           pokemon_growth_t* pkmn_g_t);

            /*
             * Converts Pokehack's Pokemon data structures to PKMDS's through
             * PKMNsim's spec_pkmn.
             *
             * Parameters:
             *  - b_pkmn_t: Pointer to Pokehack's Belt Pokemon data structure
             *  - pkmn_a_t: Pointer to Pokehack's Pokemon Attacks data structure
             *  - pkmn_e_t: Pointer to Pokehack's Pokemon Effort data structure
             *  - pkmn_m_t: Pointer to Pokehack's Pokemon Misc Stats data structure
             *  - pkmn_g_t: Pointer to Pokehack's Pokemon Growth data structure
             *  - p_pkm: PKMDS's Pokemon data structure to return conversion
             */            
            void pokehack_pkmn_to_pkmds_pkmn(belt_pokemon_t* b_pkmn_t,
                                             pokemon_attacks_t* pkmn_a_t,
                                             pokemon_effort_t* pkmn_e_t,
                                             pokemon_misc_t* pkmn_m_t,
                                             pokemon_growth_t* pkmn_g_t,
                                             party_pkm* p_pkm);

            /*
             * Converts PokeLib's Pokemon data structure to Pokehack's through
             * PKMNsim's spec_pkmn.
             *
             * Parameters:
             *  - pokelib_pkmn: PokeLib's Pokemon data structure to convert
             *  - b_pkmn_t: Pointer to Pokehack's Belt Pokemon data structure
             *  - pkmn_a_t: Pointer to Pokehack's Pokemon Attacks data structure
             *  - pkmn_e_t: Pointer to Pokehack's Pokemon Effort data structure
             *  - pkmn_m_t: Pointer to Pokehack's Pokemon Misc Stats data structure
             *  - pkmn_g_t: Pointer to Pokehack's Pokemon Growth data structure
             */
            void pokelib_pkmn_to_pokehack_pkmn(PokeLib::Pokemon pokelib_pkmn,
                                               belt_pokemon_t* b_pkmn_t,
                                               pokemon_attacks_t* pkmn_a_t,
                                               pokemon_effort_t* pkmn_e_t,
                                               pokemon_misc_t* pkmn_m_t,
                                               pokemon_growth_t* pkmn_g_t);

            /*
             * Converts PokeLib's Pokemon data structure to Pokehack's through
             * PKMNsim's spec_pkmn.
             *
             * Parameters:
             *  - pokelib_pkmn: PokeLib's Pokemon data structure to convert
             *  - p_pkm: PKMDS's Pokemon data structure to return conversion
             */
            void pokelib_pkmn_to_pkmds_pkmn(PokeLib::Pokemon pokelib_pkmn, party_pkm* p_pkm);

            /*
             * Converts PKMDS's Pokemon data structure to Pokehack's through
             * PKMNsim's spec_pkmn.
             *
             * Parameters:
             *  - p_pkm: PKMDS's Pokemon data structure to convert
             *  - b_pkmn_t: Pointer to Pokehack's Belt Pokemon data structure
             *  - pkmn_a_t: Pointer to Pokehack's Pokemon Attacks data structure
             *  - pkmn_e_t: Pointer to Pokehack's Pokemon Effort data structure
             *  - pkmn_m_t: Pointer to Pokehack's Pokemon Misc Stats data structure
             *  - pkmn_g_t: Pointer to Pokehack's Pokemon Growth data structure
             */
            void pkmds_pkmn_to_pokehack_pkmn(party_pkm* p_pkm,
                                             belt_pokemon_t* b_pkmn_t,
                                             pokemon_attacks_t* pkmn_a_t,
                                             pokemon_effort_t* pkmn_e_t,
                                             pokemon_misc_t* pkmn_m_t,
                                             pokemon_growth_t* pkmn_g_t);

            /*
             * Converts PKMDS's Pokemon data structure to PokeLib's through
             * PKMNsim's spec_pkmn.
             *
             * Parameters:
             *  - p_pkm: PKMDS's Pokemon data structure to convert
             */
            PokeLib::Pokemon pkmds_pkmn_to_pokelib_pkmn(party_pkm* p_pkm);

        private:
            char* pokehack_get_text(unsigned char* raw, bool is_nickname);

            dict<char, int> pokehack_reverse_char_map;
    };    
}

#endif /* INCLUDED_PKMNSIM_CONVERSIONS_HPP */
