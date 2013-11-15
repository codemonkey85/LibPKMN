/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN5IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN5IMPL_HPP

#include <pkmnsim/game_save.hpp>

#include "conversions/trainer.hpp"

namespace pkmnsim
{
    class game_save_gen5impl: public game_save
    {
        public:
            game_save_gen5impl(pkmds_g5_sptr ptr);
            
            void load();
            void save();
            void save_as(std::string filename);
            
            trainer::sptr get_trainer() const;
            
        private:
            pkmds_g5_sptr sav;
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN5IMPL_HPP */
