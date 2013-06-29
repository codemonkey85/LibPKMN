/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN3IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN3IMPL_HPP

#include <pkmnsim/game_save.hpp>

#include <pokehack/SaveParser.h>

namespace pkmnsim
{
    class PKMNSIM_API game_save_gen3impl: public game_save
    {
        public:
            game_save_gen3impl(std::string filename, int game);

            int get_money();

            void get_party(std::vector<spec_pkmn::sptr>& party_vec);

            std::string get_trainer_name();

        private:
            SaveParser *parser;

            spec_pkmn::sptr convert_to_spec_pkmn(belt_pokemon_t* b_pkmn_t,
                                                 pokemon_attacks_t* pkmn_a_t,
                                                 pokemon_effort_t* pkmn_e_t,
                                                 pokemon_misc_t* pkmn_m_t,
                                                 pokemon_growth_t* pkmn_g_t);
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN3IMPL_HPP */
