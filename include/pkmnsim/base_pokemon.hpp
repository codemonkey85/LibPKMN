/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_BASE_POKEMON_HPP
#define INCLUDED_PKMNSIM_BASE_POKEMON_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <string>
#include <vector>

#include <pkmnsim/config.hpp>
#include <pkmnsim/types/dict.hpp>
#include <pkmnsim/types/vla.hpp>

namespace pkmnsim
{
    class PKMNSIM_API base_pokemon
    {
        public:

            typedef std::shared_ptr<base_pokemon> sptr;
            static sptr make(unsigned int species_id, unsigned int game);

			base_pokemon() {};
            virtual ~base_pokemon() {};

            virtual std::string get_info() const = 0;
            virtual std::string get_info_verbose() const = 0;

            virtual unsigned int get_pokedex_num() const = 0;
            virtual std::string get_pokedex_entry() const = 0;
            virtual dict<unsigned int, unsigned int> get_types() const = 0;
            virtual double get_height() const = 0;
            virtual double get_weight() const = 0;
            virtual dict<unsigned int, unsigned int> get_base_stats() const = 0;
            virtual dict<unsigned int, unsigned int> get_ev_yields() const = 0;
            virtual double get_chance_male() const = 0;
            virtual double get_chance_female() const = 0;
            virtual dict<unsigned int, unsigned int> get_abilities() const = 0;
			virtual void get_evolutions(std::vector<sptr> &evolution_vec) const = 0;
			virtual bool is_fully_evolved() const = 0;
            virtual unsigned int get_generation() const = 0;
            virtual bool has_gender_differences() const = 0;

            virtual std::string get_icon_path(bool is_male) const = 0;
            virtual std::string get_sprite_path(bool is_male, bool is_shiny) const = 0;
            virtual void set_form(unsigned int form) = 0;
            virtual void set_form(std::string form) = 0;
            virtual void repair(unsigned int id) = 0;

            virtual std::vector<std::string> get_egg_group_names() const = 0;
            virtual std::string get_form_name() const = 0;
            virtual std::string get_game_name() const = 0;
            virtual std::string get_species_name() const = 0;
            
            virtual std::vector<unsigned int> get_egg_group_ids() const = 0;
            virtual unsigned int get_form_id() const = 0;
            virtual unsigned int get_game_id() const = 0;
            virtual unsigned int get_pokemon_id() const = 0;
            virtual unsigned int get_species_id() const = 0;
            
        protected:
            
            unsigned int from_gen, form_id, pokemon_id, species_id, type1_id, type2_id;
            unsigned int hp, attack, defense, speed, special, special_attack, special_defense;
            
            unsigned int from_game;
            std::string game_string, images_game_string;
            std::string male_icon_path, female_icon_path;
            std::string male_sprite_path, female_sprite_path;
            std::string male_shiny_sprite_path, female_shiny_sprite_path;

    };

    //Related typedefs
    typedef std::vector<base_pokemon::sptr> b_pkmn_vec_t;
    typedef vla<base_pokemon::sptr> b_pkmn_vla_t;
}

#endif /* INCLUDED_PKMNSIM_BASE_POKEMON_HPP */