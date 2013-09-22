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

namespace pkmnsim
{
    class PKMNSIM_API base_pokemon
    {
        public:

            typedef std::shared_ptr<base_pokemon> sptr;
            static sptr make(int species_id, int game);

			base_pokemon(void) {};
            virtual ~base_pokemon() {};

            virtual std::string get_info(void) const = 0;
            virtual std::string get_info_verbose(void) const = 0;

            virtual int get_pokedex_num(void) const = 0;
            std::string get_pokedex_entry(void) const = 0;
            virtual dict<int, std::string> get_types(void) const = 0;
            virtual double get_height(void) const = 0;
            virtual double get_weight(void) const = 0;
            virtual dict<int, unsigned int> get_base_stats(void) const = 0;
            virtual dict<int, unsigned int> get_ev_yields(void) const = 0;
            virtual double get_chance_male(void) const = 0;
            virtual double get_chance_female(void) const = 0;
            virtual dict<unsigned int, unsigned int> get_abilities(void) const = 0;
			virtual void get_evolutions(std::vector<sptr> &evolution_vec) const = 0;
			virtual bool is_fully_evolved(void) const = 0;
            virtual unsigned int get_generation(void) const = 0;
            virtual bool has_gender_differences(void) const = 0;

            virtual std::string get_icon_path(bool is_male) const = 0;
            virtual std::string get_sprite_path(bool is_male, bool is_shiny) const = 0;
            virtual void set_form(int form) const = 0;
            virtual void repair(int id) const = 0;

            virtual std::string get_egg_group_name(void) const = 0;
            virtual std::string get_form_name(void) const = 0;
            virtual std::string get_game_name(void) const = 0;
            virtual std::string get_species_name(void) const = 0;
            
            int get_egg_group_id(void) const = 0;
            int get_form_id(void) const = 0;
            int get_game_id(void) const = 0;
            int get_pokemon_id(void) const = 0;
            int get_species_id(void) const = 0;
            
        protected:
            //Database values
            unsigned int from_gen, form_id, pokemon_id, species_id, type1_id, type2_id;
            
            int from_game;
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