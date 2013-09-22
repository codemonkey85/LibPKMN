/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_POKEMON_GEN1IMPL_HPP
#define INCLUDED_BASE_POKEMON_GEN1IMPL_HPP

#include <string>

#include "base_pkmn_impl.hpp"
#include <pkmnsim/types/dict.hpp>

namespace pkmnsim
{
    class base_pokemon_gen1impl: public base_pokemon_impl
    {
        public:
            base_pokemon_gen1impl(int id, int game);
            
            std::string get_info() const;
            std::string get_info_verbose() const;
            
            dict<unsigned int, std::string> get_types() const;
            dict<unsigned int, unsigned int> get_base_stats() const;
            dict<unsigned int, unsigned int> get_ev_yields() const;
            double get_chance_male(void) const;
            double get_chance_female(void) const;
            dict<unsigned int, unsigned in> get_abilities() const;
            bool has_gender_differences(void) const;
            
            std::string get_sprite_path(bool is_male, bool is_shiny) const;
            void set_form(int form) const;
            void repair(int id) const;
            
            std::string get_egg_group_name() const;
            std::string get_form_name() const;
            
            int get_egg_group_id() const;
            int get_form_id() const;
    };
}

#endif /* INCLUDED_BASE_POKEMON_GEN1IMPL_HPP */