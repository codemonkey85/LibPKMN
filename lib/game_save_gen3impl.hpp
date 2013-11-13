/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN3IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN3IMPL_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <pkmnsim/game_save.hpp>

#include <pokehack/SaveParser.h>

typedef std::shared_ptr<SaveParser> pokehack_sptr;

namespace pkmnsim
{
    class game_save_gen3impl: public game_save
    {
        public:
            game_save_gen3impl(pokehack_sptr ptr);
            
            void load();
            void save();
            void save_as(std::string filename);
            
            trainer::sptr get_trainer() const;
            
        private:
            pokehack_sptr parser;
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN3IMPL_HPP */
