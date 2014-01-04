/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_CONVERSIONS_POKEMON_HPP
#define INCLUDED_PKMNSIM_CONVERSIONS_POKEMON_HPP

#include <pkmn/config.hpp>
#include <pkmn/team_pokemon.hpp>
#include <pkmn/types/dict.hpp>

#include <rpokesav/gen1_pokemon.hpp>
#include <pokehack/pokestructs.h>
#include <pokelib/pokelib.h>
#include <pkmds/pkmds_g5.h>
#include <pkmds/pkmds_g6.h>

namespace pkmn
{
    namespace conversions
    {
        team_pokemon::sptr PKMNSIM_API rpokesav_gen1_pokemon_to_team_pokemon(rpokesav::gen1_pokemon pkmn,
                                                                             pokemon_text trainer_name);
    
        team_pokemon::sptr PKMNSIM_API pokehack_pokemon_to_team_pokemon(belt_pokemon_t* b_pkmn_t,
                                                                        pokemon_attacks_t* pkmn_a_t,
                                                                        pokemon_effort_t* pkmn_e_t,
                                                                        pokemon_misc_t* pkmn_m_t,
                                                                        pokemon_growth_t* pkmn_g_t);

        void PKMNSIM_API team_pokemon_to_pokehack_pokemon(team_pokemon::sptr t_pkmn,
                                                          belt_pokemon_t* b_pkmn_t,
                                                          pokemon_attacks_t* pkmn_a_t,
                                                          pokemon_effort_t* pkmn_e_t,
                                                          pokemon_misc_t* pkmn_m_t,
                                                          pokemon_growth_t* pkmn_g_t);

        team_pokemon::sptr PKMNSIM_API pokelib_pokemon_to_team_pokemon(PokeLib::Pokemon pokelib_pkmn);

        PokeLib::Pokemon PKMNSIM_API team_pokemon_to_pokelib_pokemon(team_pokemon::sptr t_pkmn);

        team_pokemon::sptr PKMNSIM_API pkmds_g5_pokemon_to_team_pokemon(party_pkm* p_pkm);

        void PKMNSIM_API team_pokemon_to_pkmds_g5_pokemon(team_pokemon::sptr t_pkmn, party_pkm* p_pkm);

        team_pokemon::sptr PKMNSIM_API pkmds_g6_pokemon_to_team_pokemon(party_pkx* p_pkx);
        
        void PKMNSIM_API team_pokemon_to_pkmds_g6_pokemon(team_pokemon::sptr t_pkmn, party_pkx* p_pkx);

        PokeLib::Pokemon PKMNSIM_API pokehack_pokemon_to_pokelib_pokemon(belt_pokemon_t* b_pkmn_t,
                                                                         pokemon_attacks_t* pkmn_a_t,
                                                                         pokemon_effort_t* pkmn_e_t,
                                                                         pokemon_misc_t* pkmn_m_t,
                                                                         pokemon_growth_t* pkmn_g_t);

        void PKMNSIM_API pokehack_pkmn_to_pkmds_g5_pokemon(belt_pokemon_t* b_pkmn_t,
                                                           pokemon_attacks_t* pkmn_a_t,
                                                           pokemon_effort_t* pkmn_e_t,
                                                           pokemon_misc_t* pkmn_m_t,
                                                           pokemon_growth_t* pkmn_g_t,
                                                           party_pkm* p_pkm);

        void PKMNSIM_API pokelib_pokemon_to_pokehack_pokemon(PokeLib::Pokemon pokelib_pkmn,
                                                             belt_pokemon_t* b_pkmn_t,
                                                             pokemon_attacks_t* pkmn_a_t,
                                                             pokemon_effort_t* pkmn_e_t,
                                                             pokemon_misc_t* pkmn_m_t,
                                                             pokemon_growth_t* pkmn_g_t);

        void PKMNSIM_API pokelib_pokemon_to_pkmds_g5_pokemon(PokeLib::Pokemon pokelib_pkmn, party_pkm* p_pkm);

        void PKMNSIM_API pkmds_g5_pokemon_to_pokehack_pokemon(party_pkm* p_pkm,
                                                              belt_pokemon_t* b_pkmn_t,
                                                              pokemon_attacks_t* pkmn_a_t,
                                                              pokemon_effort_t* pkmn_e_t,
                                                              pokemon_misc_t* pkmn_m_t,
                                                              pokemon_growth_t* pkmn_g_t);

        PokeLib::Pokemon PKMNSIM_API pkmds_g5_pokemon_to_pokelib_pokemon(party_pkm* p_pkm);
    }
}

#endif /* INCLUDED_PKMNSIM_CONVERSIONS_POKEMON_HPP */
