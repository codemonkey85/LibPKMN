/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN5IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN5IMPL_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <pkmnsim/game_save.hpp>
#include <pkmnsim/spec_pkmn.hpp>

#include "PKMDS/pkmds_g5.h"
#include "PKMDS/pkmds_g5_sqlite.h"

namespace pkmnsim
{
    /*
     * Game Save Class
     */
    class PKMNSIM_API game_save_gen5impl: public game_save
    {
        public:
            game_save_gen5impl(std::string filename);

        private:
            bw2sav_obj* sav;

            int verify();

            spec_pkmn::sptr pkm_to_spec_pkmn(pokemon_obj* pkm);
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN5IMPL_HPP */
