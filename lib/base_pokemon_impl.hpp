/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_POKEMON_IMPL_HPP
#define INCLUDED_BASE_POKEMON_IMPL_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <string>
#include <vector>

#include <pkmnsim/config.hpp>
#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/types/dict.hpp>

namespace pkmnsim
{
    class PKMNSIM_API base_pokemon_impl: public base_pokemon
    {
        public:

            base_pokemon_impl(unsigned int species_id, unsigned int game);

            virtual std::string get_info() const = 0;
            virtual std::string get_info_verbose() const = 0;

            unsigned int get_pokedex_num() const;
            std::string get_pokedex_entry() const;
            dict<unsigned int, unsigned int> get_types() const;
            double get_height() const;
            double get_weight() const;
            virtual dict<int, unsigned int> get_base_stats() const = 0;
            virtual dict<int, unsigned int> get_ev_yields() const = 0;
            virtual double get_chance_male() const = 0;
            virtual double get_chance_female() const = 0;
            virtual dict<unsigned int, unsigned int> get_abilities() const = 0;
			void get_evolutions(std::vector<sptr> &evolution_vec) const;
			bool is_fully_evolved() const;
            unsigned int get_generation() const;
            virtual bool has_gender_differences() const = 0;

            virtual std::string get_icon_path(bool is_male) const = 0;
            virtual std::string get_sprite_path(bool is_male, bool is_shiny) const = 0;
            virtual void set_form(int form) const = 0;
            virtual void repair(int id) const = 0;

            virtual std::string get_egg_group_name() const = 0;
            virtual std::string get_form_name() const = 0;
            std::string get_game_name() const;
            std::string get_species_name() const;
            
            virtual unsigned int get_egg_group_id() const = 0;
            virtual unsigned int get_form_id() const = 0;
            unsigned int get_game_id() const;
            unsigned int get_pokemon_id() const;
            unsigned int get_species_id() const;
    };
}

#endif /* INCLUDED_BASE_POKEMON_IMPL_HPP */