/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_CONVERSIONS_HPP
#define INCLUDED_CONVERSIONS_HPP

#include <pkmnsim/spec_pkmn.hpp>

#include "sqlitecpp/SQLiteCPP.h"

#include <pokehack/pokestructs.h>
#include <pokelib/pokelib.h>

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

        private:
            static char* pokehack_get_text(unsigned char* raw, bool is_nickname);
    };
}

#endif /* INCLUDED_CONVERSIONS_HPP */
