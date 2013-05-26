/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN3IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN3IMPL_HPP

#include <iostream>
#include <pkmnsim/game_save.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include "pokehack/pokestructs.h"
#include "pokehack/SaveParser.h"
#include <stdexcept>
#include <string>
#include <vector>

namespace pkmnsim
{
    /*
     * Game Save Class
     */
    class PKMNSIM_API game_save_gen3impl: public game_save
    {
        public:
            game_save_gen3impl(std::string filename);
            std::string get_trainer_name(void) = 0;
            void get_party(std::vector<spec_pkmn::sptr>& belt_pkmn_vec) = 0;
            //Better way to do this?
            void get_boxes(std::vector<std::vector<spec_pkmn::sptr> >& boxes_vec) = 0;

        private:
            int save_type; //0 = RSE, 1 = FRLG
            SaveParser *parser;

            spec_pkmn::sptr belt_pokemon_t_to_spec_pkmn(belt_pokemon_t* bpt, pokemon_effort_t* pet,
                                                        pokemon_attacks_t* pat, pokemon_misc_t* pmt, int index);
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN3IMPL_HPP */
