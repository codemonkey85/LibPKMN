/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN5IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN5IMPL_HPP

#include "game_save_impl.hpp"

#include "conversions/trainer.hpp"

namespace pkmn
{
    class game_save_gen5impl: public game_save_impl
    {
        public:
        
            game_save_gen5impl(pkmds_g5_sptr ptr);
            
            void load();
            void save();
            
        private:
        
            pkmds_g5_sptr _sav;
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN5IMPL_HPP */
