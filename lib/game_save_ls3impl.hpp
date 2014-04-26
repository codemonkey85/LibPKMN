/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_LS3IMPL_HPP
#define INCLUDED_GAME_SAVE_LS3IMPL_HPP

#include "game_save_impl.hpp"

#include "libspec/libspec.h"

namespace pkmn
{
    class game_save_ls3impl: public game_save_impl
    {
        public:

            game_save_ls3impl();
            game_save_ls3impl(uint8_t* buffer);
            ~game_save_ls3impl();
            
            void load();
            void save();
            
        private:
        
            uint8_t* _data;
            gba_save_t* _save;
            gba_trainer_t* __trainer;
            gba_storage_t* _storage;
            gba_party_t* _party;
            gba_pc_t* _pc;
    };
}

#endif /* INCLUDED_GAME_SAVE_LS3IMPL_HPP */
