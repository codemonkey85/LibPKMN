/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_POKEMON_MODERNIMPL_HPP
#define INCLUDED_BASE_POKEMON_MODERNIMPL_HPP

//For code cleanliness in source file
#define HAS_DIFFERENT_FEMALE_ICON _species_id == Species::UNFEZANT \
                                  or _species_id == Species::FRILLISH \
                                  or _species_id == Species::JELLICENT

#include "base_pokemon_impl.hpp"

namespace pkmnsim
{
    class base_pokemon_modernimpl: public base_pokemon_impl
    {
        public:
        
            base_pokemon_modernimpl(unsigned int id, unsigned int game_id);
            
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
            
            void get_egg_group_names(std::vector<std::string>
                                     &egg_group_name_vec) const;
            std::string get_form_name() const;
            
            void get_egg_group_ids(std::vector<unsigned int>
                                   &egg_group_id_vec) const;
            unsigned int get_form_id() const;
    };
}

#endif /* INCLUDED_BASE_POKEMON_MODERNIMPL_HPP */
