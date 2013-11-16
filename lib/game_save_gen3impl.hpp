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

#include "conversions/trainer.hpp"

namespace pkmnsim
{
    class game_save_gen3impl: public game_save
    {
        public:
            game_save_gen3impl(pokehack_sptr ptr);
            
            void load();
            void save();
            
            trainer::sptr get_trainer() const;
            
        private:
            pokehack_sptr parser;
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN3IMPL_HPP */
