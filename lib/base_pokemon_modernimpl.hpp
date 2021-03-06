/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_POKEMON_MODERNIMPL_HPP
#define INCLUDED_BASE_POKEMON_MODERNIMPL_HPP

#include <string>

#include <pkmn/team_pokemon.hpp>

#include "base_pokemon_impl.hpp"
#include "Signal.h"
#include "team_pokemon_modernimpl.hpp"

//For code cleanliness in source file
#define HAS_DIFFERENT_FEMALE_ICON _species_id == Species::UNFEZANT \
                                  or _species_id == Species::FRILLISH \
                                  or _species_id == Species::JELLICENT

namespace pkmn
{
    class base_pokemon_modernimpl: public base_pokemon_impl
    {
        public:

            base_pokemon_modernimpl(unsigned int id, unsigned int game_id);
            base_pokemon_modernimpl(const base_pokemon_modernimpl &other);
            base_pokemon_modernimpl& operator=(const base_pokemon_modernimpl &other);
            ~base_pokemon_modernimpl() {};

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

            std::string get_icon_path(bool is_male) const;
            std::string get_sprite_path(bool is_male, bool is_shiny) const;

            //Database Info
            void get_egg_group_ids(std::vector<unsigned int>& egg_group_id_vec) const;

            friend class team_pokemon;
            friend class team_pokemon_modernimpl;

            Gallant::Signal0<> form_signal1;
            Gallant::Signal0<> form_signal2;
    };
}

#endif /* INCLUDED_BASE_POKEMON_MODERNIMPL_HPP */
