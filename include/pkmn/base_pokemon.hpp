/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_BASE_POKEMON_HPP
#define INCLUDED_PKMN_BASE_POKEMON_HPP

#include <string>
#include <utility>
#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/shared_ptr.hpp>
#include <pkmn/types/vla.hpp>

namespace pkmn
{
    typedef std::pair<std::string, std::string> types_t;

    class PKMN_API base_pokemon
    {
        public:

            typedef pkmn::shared_ptr<base_pokemon> sptr;
            static sptr make(unsigned int species_id, unsigned int game_id);

			base_pokemon() {};
            virtual ~base_pokemon() {};

            //Pokedex Data
            virtual std::string get_name() const = 0;
            virtual std::string get_species() const = 0;
            virtual unsigned int get_pokedex_num() const = 0;
            virtual std::string get_pokedex_entry() const = 0;
            virtual types_t get_types() const = 0;
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

            virtual void get_egg_group_names(std::vector<std::string>
                                             &egg_group_name_vec) const = 0;
            virtual std::string get_form_name() const = 0;
            
            virtual void get_egg_group_ids(std::vector<unsigned int>
                                           &egg_group_id_vec) const = 0;
            virtual unsigned int get_form_id() const = 0;
            virtual unsigned int get_game_id() const = 0;
            virtual unsigned int get_pokemon_id() const = 0;
            virtual unsigned int get_species_id() const = 0;
            
            virtual unsigned int get_exp_yield() const = 0;
    };

    //Related typedefs
    typedef std::vector<base_pokemon::sptr> b_pkmn_vec_t;
    typedef vla<base_pokemon::sptr> b_pkmn_vla_t;
}

#endif /* INCLUDED_PKMN_BASE_POKEMON_HPP */
