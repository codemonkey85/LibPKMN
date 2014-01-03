/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_CONVERSIONS_PKSQL_HPP
#define INCLUDED_PKMNSIM_CONVERSIONS_PKSQL_HPP

#include <pkmn/config.hpp>

#include <pokehack/pokestructs.h>
#include <pokelib/pokelib.h>
#include <pkmds/pkmds_g5.h>
#include <pkmds/pkmds_g6.h>

/*
 * To avoid the extra SQLite queries of going through a team_pokemon::sptr,
 * these functions skip the step completely.
 */

namespace pkmnsim
{
    namespace conversions
    {
        void pokehack_pokemon_to_pksql(belt_pokemon_t* b_pkmn_t,
                                       pokemon_attacks_t* pkmn_a_t,
                                       pokemon_effort_t* pkmn_e_t,
                                       pokemon_misc_t* pkmn_m_t,
                                       pokemon_growth_t* pkmn_g_t,
                                       std::string filename);

        /*void pksql_to_pokehack_pokemon(std::string filename,
                                       belt_pokemon_t* b_pkmn_t,
                                       pokemon_attacks_t* pkmn_a_t,
                                       pokemon_effort_t* pkmn_e_t,
                                       pokemon_misc_t* pkmn_m_t,
                                       pokemon_growth_t* pkmn_g_t);

        void pokelib_pokemon_to_pksql(PokeLib::Pokemon pokelib_pkmn,
                                      std::string filename);

        PokeLib::Pokemon pksql_to_pokelib_pokemon(std::string filename);

        void pkmds_g5_pokemon_to_pksql(party_pkm* p_pkm,
                                       std::string filename);

        void pksql_to_pkmds_g5_pokemon(std::string filename,
                                       party_pkm* p_pkm);

        void pkmds_g6_pokemon_to_pksql(party_pkx* p_pkx,
                                       std::string filename);
        
        void pksql_to_pkmds_g6_pokemon(std::string filename,
                                       party_pkx* p_pkx);*/

    }
}

#endif /* INCLUDED_PKMNSIM_CONVERSIONS_PKSQL_HPP */
