/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_POKEMON_HPP
#define INCLUDED_PKMN_CONVERSIONS_POKEMON_HPP

#include <pkmn/config.hpp>
#include <pkmn/team_pokemon.hpp>
#include <pkmn/types/dict.hpp>

#include <rpokesav/gen1_pokemon.hpp>
#include "../libspec/game_gba.h"
#include <PokeLib/PokeLib.h>
#include <pkmds/pkmds_g5.h>
#include <pkmds/pkmds_g6.h>

namespace pkmn
{
    namespace conversions
    {
        team_pokemon::sptr import_gen1_pokemon(const rpokesav::gen1_pokemon &pkmn);
        void export_gen1_pokemon(team_pokemon::sptr t_pkmn, rpokesav::gen1_pokemon &pkmn);

        team_pokemon::sptr import_gen3_pokemon(pk3_box_t* pkmn, gba_savetype_t save_type);
        team_pokemon::sptr import_gen3_pokemon(pk3_t* pkmn, gba_savetype_t save_type);
        void export_gen3_pokemon(team_pokemon::sptr t_pkmn, pk3_box_t* pkmn, gba_savetype_t save_type);
        void export_gen3_pokemon(team_pokemon::sptr t_pkmn, pk3_t* pkmn, gba_savetype_t save_type);

        team_pokemon::sptr import_gen4_pokemon(const PokeLib::Pokemon &pokelib_pkmn);
        PokeLib::Pokemon export_gen4_pokemon(team_pokemon::sptr t_pkmn);

        team_pokemon::sptr import_gen5_pokemon(party_pkm* p_pkm);
        void export_gen5_pokemon(team_pokemon::sptr t_pkmn, party_pkm* p_pkm);

        team_pokemon::sptr import_gen6_pokemon(party_pkx* p_pkx);
        void export_gen6_pokemon(team_pokemon::sptr t_pkmn, party_pkx* p_pkx);
    }
}

#endif /* INCLUDED_PKMN_CONVERSIONS_POKEMON_HPP */
