/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_CONVERSIONS_POKEMON_HPP
#define INCLUDED_PKMNSIM_CONVERSIONS_POKEMON_HPP

#include <pkmnsim/config.hpp>
#include <pkmnsim/team_pokemon.hpp>
#include <pkmnsim/types/dict.hpp>

#include <pokehack/pokestructs.h>
#include <pokelib/pokelib.h>
#include <pkmds/pkmds_g5.h>

namespace pkmnsim
{
    namespace conversions
    {
        team_pokemon::sptr pokehack_pkmn_to_team_pokemon(belt_pokemon_t* b_pkmn_t,
                                                         pokemon_attacks_t* pkmn_a_t,
                                                         pokemon_effort_t* pkmn_e_t,
                                                         pokemon_misc_t* pkmn_m_t,
                                                         pokemon_growth_t* pkmn_g_t);

        void team_pokemon_to_pokehack_pkmn(team_pokemon::sptr s_pkmn,
                                           belt_pokemon_t* b_pkmn_t,
                                           pokemon_attacks_t* pkmn_a_t,
                                           pokemon_effort_t* pkmn_e_t,
                                           pokemon_misc_t* pkmn_m_t,
                                           pokemon_growth_t* pkmn_g_t);

        team_pokemon::sptr pokelib_pkmn_to_team_pokemon(PokeLib::Pokemon pokelib_pkmn);

        PokeLib::Pokemon team_pokemon_to_pokelib_pkmn(team_pokemon::sptr s_pkmn);

        team_pokemon::sptr pkmds_pkmn_to_team_pokemon(party_pkm* p_pkm);

        void team_pokemon_to_pkmds_pkmn(team_pokemon::sptr s_pkmn, party_pkm* p_pkm);

        PokeLib::Pokemon pokehack_pkmn_to_pokelib_pkmn(belt_pokemon_t* b_pkmn_t,
                                                       pokemon_attacks_t* pkmn_a_t,
                                                       pokemon_effort_t* pkmn_e_t,
                                                       pokemon_misc_t* pkmn_m_t,
                                                       pokemon_growth_t* pkmn_g_t);

        void pokehack_pkmn_to_pkmds_pkmn(belt_pokemon_t* b_pkmn_t,
                                         pokemon_attacks_t* pkmn_a_t,
                                         pokemon_effort_t* pkmn_e_t,
                                         pokemon_misc_t* pkmn_m_t,
                                         pokemon_growth_t* pkmn_g_t,
                                         party_pkm* p_pkm);

        void pokelib_pkmn_to_pokehack_pkmn(PokeLib::Pokemon pokelib_pkmn,
                                           belt_pokemon_t* b_pkmn_t,
                                           pokemon_attacks_t* pkmn_a_t,
                                           pokemon_effort_t* pkmn_e_t,
                                           pokemon_misc_t* pkmn_m_t,
                                           pokemon_growth_t* pkmn_g_t);

        void pokelib_pkmn_to_pkmds_pkmn(PokeLib::Pokemon pokelib_pkmn, party_pkm* p_pkm);

        void pkmds_pkmn_to_pokehack_pkmn(party_pkm* p_pkm,
                                         belt_pokemon_t* b_pkmn_t,
                                         pokemon_attacks_t* pkmn_a_t,
                                         pokemon_effort_t* pkmn_e_t,
                                         pokemon_misc_t* pkmn_m_t,
                                         pokemon_growth_t* pkmn_g_t);

        PokeLib::Pokemon pkmds_pkmn_to_pokelib_pkmn(party_pkm* p_pkm);
    }
}

#endif /* INCLUDED_PKMNSIM_CONVERSIONS_POKEMON_HPP */
