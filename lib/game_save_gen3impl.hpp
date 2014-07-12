/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_LS3IMPL_HPP
#define INCLUDED_GAME_SAVE_LS3IMPL_HPP

#include "game_save_impl.hpp"

#include "libspec/game_gba.h"

namespace pkmn
{
    class game_save_gen3impl: public game_save_impl
    {
        public:

            game_save_gen3impl();
            game_save_gen3impl(uint8_t* buffer, const std::string &filename);
            ~game_save_gen3impl();
            
            void load();
            void save_as(const std::string &filename);
            
        private:

            uint8_t* _data;        
            gba_save_t* _save;
            gba_trainer_t* __trainer;
            gba_party_t* _party;
            gba_pc_t* _pc;
    };
}

#endif /* INCLUDED_GAME_SAVE_LS3IMPL_HPP */
