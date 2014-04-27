/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN1IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN1IMPL_HPP

#include "game_save_impl.hpp"

#include "conversions/trainer.hpp"

namespace pkmn
{
    class game_save_gen1impl: public game_save_impl
    {
        public:

            game_save_gen1impl();
            game_save_gen1impl(rpokesav_gen1_sptr sav, std::string orig_filename);
            
            void load();
            void save_as(std::string filename) const;
            
        private:
        
            rpokesav_gen1_sptr _sav;
    };
}
#endif /* INCLUDED_GAME_SAVE_GEN1IMPL_HPP */
