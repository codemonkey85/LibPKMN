/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
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

            game_save_gen5impl();
            game_save_gen5impl(pkmds_g5_sptr ptr, const std::string &filename);

            void load();
            void save_as(const std::string &filename);

        private:

            pkmds_g5_sptr _sav;
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN5IMPL_HPP */
