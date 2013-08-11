/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_CONVERSIONS_HPP
#define INCLUDED_CONVERSIONS_HPP

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

            spec_pkmn::sptr pokehack_pkmn_to_spec_pkmn(belt_pokemon_t* b_pkmn_t,
                                                              pokemon_attacks_t* pkmn_a_t,
                                                              pokemon_effort_t* pkmn_e_t,
                                                              pokemon_misc_t* pkmn_m_t,
                                                              pokemon_growth_t* pkmn_g_t);

            void spec_pkmn_to_pokehack_pkmn(spec_pkmn::sptr s_pkmn,
                                                   belt_pokemon_t* b_pkmn_t,
                                                   pokemon_attacks_t* pkmn_a_t,
                                                   pokemon_effort_t* pkmn_e_t,
                                                   pokemon_misc_t* pkmn_m_t,
                                                   pokemon_growth_t* pkmn_g_t);

            spec_pkmn::sptr pokelib_pkmn_to_spec_pkmn(PokeLib::Pokemon pokelib_pkmn);

            PokeLib::Pokemon spec_pkmn_to_pokelib_pkmn(spec_pkmn::sptr s_pkmn);

            spec_pkmn::sptr pkmds_pkmn_to_spec_pkmn(party_pkm* p_pkm);

            void spec_pkmn_to_pkmds_pkmn(spec_pkmn::sptr s_pkmn, party_pkm* p_pkm);
            
        private:
            char* pokehack_get_text(unsigned char* raw, bool is_nickname);

            dict<char, int> pokehack_reverse_char_map;
    };    
}

#endif /* INCLUDED_CONVERSIONS_HPP */
