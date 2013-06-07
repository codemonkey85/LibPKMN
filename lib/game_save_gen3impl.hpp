/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN3IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN3IMPL_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <pkmnsim/game_save.hpp>
#include <pkmnsim/spec_pkmn.hpp>

#include "pokehack/pokestructs.h"
#include "pokehack/SaveParser.h"

namespace pkmnsim
{
    /*
     * Game Save Class
     */
    class PKMNSIM_API game_save_gen3impl: public game_save
    {
        public:
            game_save_gen3impl(std::string filename);

        private:
            int save_type; //0 = RSE, 1 = FRLG
            SaveParser *parser;

            int verify();
            spec_pkmn::sptr belt_pokemon_t_to_spec_pkmn(belt_pokemon_t* bpt, pokemon_effort_t* pet,
                                                        pokemon_attacks_t* pat, pokemon_misc_t* pmt, int index);
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN3IMPL_HPP */
