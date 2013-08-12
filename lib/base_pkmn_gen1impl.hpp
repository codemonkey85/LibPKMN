/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_PKMN_GEN1IMPL_HPP
#define INCLUDED_BASE_PKMN_GEN1IMPL_HPP

#include <string>

#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/dict.hpp>

namespace pkmnsim
{
    class base_pkmn_gen1impl: public base_pkmn
    {
        public:
            base_pkmn_gen1impl(std::string identifier, int game);
            std::string get_info(void);
            std::string get_info_verbose(void);
            dict<std::string, int> get_base_stats(void);
            dict<std::string, int> get_ev_yields(void);
            bool has_gender_differences();
            std::string get_sprite_path(bool is_male, bool is_shiny);
    };
}

#endif /* INCLUDED_BASE_PKMN_GEN1IMPL_HPP */
