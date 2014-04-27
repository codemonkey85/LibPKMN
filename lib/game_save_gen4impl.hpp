/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN4IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN4IMPL_HPP

#include "game_save_impl.hpp"

#include "conversions/trainer.hpp"

namespace pkmn
{
    class game_save_gen4impl: public game_save_impl
    {
        public:

            game_save_gen4impl();
            game_save_gen4impl(pokelib_sptr ptr, std::string orig_filename);
            
            void load();
            void save_as(std::string filename) const;
            
        private:
        
            pokelib_sptr _pokelib_save;
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN4IMPL_HPP */
