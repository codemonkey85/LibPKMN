/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN4IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN4IMPL_HPP

#include <pkmn/game_save.hpp>

#include "conversions/trainer.hpp"

namespace pkmn
{
    class game_save_gen4impl: public game_save
    {
        public:
        
            game_save_gen4impl(pokelib_sptr ptr);
            
            void load();
            void save();
            
            trainer::sptr get_trainer() const;
            
        private:
        
            pokelib_sptr _pokelib_save;
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN4IMPL_HPP */
