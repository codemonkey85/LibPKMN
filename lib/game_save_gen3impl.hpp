/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN3IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN3IMPL_HPP

#include "game_save_impl.hpp"

#include <pokehack/SaveParser.h>

#include "conversions/trainer.hpp"

namespace pkmn
{
    class game_save_gen3impl: public game_save_impl
    {
        public:
        
            game_save_gen3impl(pokehack_sptr ptr, char* buffer);
            
            void load();
            void save();
            
        private:
        
            pokehack_sptr _parser;
            char* _data;
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN3IMPL_HPP */
