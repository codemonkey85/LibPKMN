/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_POKEMON_GEN1IMPL_HPP
#define INCLUDED_BASE_POKEMON_GEN1IMPL_HPP

#include <string>

#include "base_pokemon_impl.hpp"

namespace pkmn
{
    class base_pokemon_gen1impl: public base_pokemon_impl
    {
        public:
            base_pokemon_gen1impl(unsigned int id, unsigned int game_id);

            //Non-Battle Attributes
            void get_egg_groups(std::vector<std::string>& egg_group_vec) const;
            bool has_gender_differences() const;
            double get_chance_male() const;
            double get_chance_female() const;
            string_pair_t get_abilities() const;
            std::string get_hidden_ability() const;

            //Battle Stats
            dict<std::string, unsigned int> get_base_stats() const;
            dict<std::string, unsigned int> get_ev_yields() const;

            void set_form(unsigned int form);
            void set_form(std::string form);
            void repair(unsigned int id);

            std::string get_icon_path(bool is_male) const;
            std::string get_sprite_path(bool is_male, bool is_shiny) const;

            //Database Info
            void get_egg_group_ids(std::vector<unsigned int>& egg_group_id_vec) const;
    };
}

#endif /* INCLUDED_BASE_POKEMON_GEN1IMPL_HPP */
