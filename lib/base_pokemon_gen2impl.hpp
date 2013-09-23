/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_POKEMON_GEN2IMPL_HPP
#define INCLUDED_BASE_POKEMON_GEN2IMPL_HPP

#include <string>

#include "base_pokemon_impl.hpp"
#include <pkmnsim/types/dict.hpp>

namespace pkmnsim
{
    class base_pokemon_gen2impl: public base_pokemon_impl
    {
        public:
            base_pokemon_gen2impl(unsigned int id, unsigned int game);
            
            std::string get_info() const;
            std::string get_info_verbose() const;
            
            dict<unsigned int, unsigned int> get_base_stats() const;
            dict<unsigned int, unsigned int> get_ev_yields() const;
            double get_chance_male() const;
            double get_chance_female() const;
            bool has_gender_differences() const;
            dict<unsigned int, unsigned int> get_abilities() const;
            
            std::string get_icon_path(bool is_male) const;
            std::string get_sprite_path(bool is_male, bool is_shiny) const;
            void set_form(unsigned int form);
            void set_form(std::string form);
            void repair(unsigned int id);
            
            std::vector<std::string> get_egg_group_names() const;
            std::string get_form_name() const;
            
            std::vector<unsigned int> get_egg_group_ids() const;
            unsigned int get_form_id() const;
    };
}

#endif /* INCLUDED_BASE_POKEMON_GEN2IMPL_HPP */