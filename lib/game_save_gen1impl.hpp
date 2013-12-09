/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN1IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN1IMPL_HPP

#include <pkmnsim/game_save.hpp>

#include "conversions/trainer.hpp"

namespace pkmnsim
{
    class game_save_gen1impl: public game_save
    {
        public:
        
            game_save_gen1impl(rpokesav_gen1_sptr sav, char* buffer);
            
            void load();
            void save();
            
            trainer::sptr get_trainer() const;
            
        private:
        
            rpokesav_gen1_sptr ptr;
            char* data;
    };
}
#endif /* INCLUDED_GAME_SAVE_GEN1IMPL_HPP */