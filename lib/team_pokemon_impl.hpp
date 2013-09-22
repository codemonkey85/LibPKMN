/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TEAM_POKEMON_IMPL_HPP
#define INCLUDED_TEAM_POKEMON_IMPL_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <string>

#include <pkmnsim/team_pokemon.hpp>

namespace pkmnsim
{
    class PKMNSIM_API team_pokemon_impl: public team_pokemon
    {
        public:

            team_pokemon_impl(unsigned int identifier, unsigned int game, unsigned int level,
                              unsigned int move1, unsigned int move2,
                              unsigned int move3, unsigned int move4);

            virtual std::string get_info() const = 0;
            virtual std::string get_info_verbose() const = 0;
            
            base_pokemon::sptr get_base_pokemon() const;
            std::string get_nickname() const;
            unsigned int get_level() const;
            vla<move::sptr> get_moves();
            vla<unsigned int> get_move_PPs() = 0;
            virtual unsigned int get_volatile_status(int status) const = 0;
            virtual void set_volatile_status(int status, int val) = 0;
            
            unsigned int get_personality() const;
            virtual unsigned int get_ability() const = 0;
            virtual unsigned int get_gender() const = 0;
            virtual unsigned int get_nature() const = 0;
            virtual bool is_shiny() const = 0;
            
            virtual dict<unsigned int, unsigned int> get_stats() const = 0;
            virtual dict<unsigned int, unsigned int> get_IVs() const = 0;
            virtual dict<unsigned int, unsigned int> get_EVs() const = 0;
            
            void set_personality(unsigned int personality);
            
            virtual void set_EV(unsigned int EV, unsigned int val) = 0;
            virtual void set_IV(unsigned int IV, unsigned int val) = 0;
            virtual void set_move(move::sptr, unsigned int pos) = 0;
            
            unsigned int get_generation() const;
            virtual unsigned int get_nature() const = 0;
            virtual unsigned int get_held_item() const = 0;
            virtual void set_held_item(unsigned int item) = 0;
            std::string get_icon_path() const;
            std::string get_sprite_path() const;
            
			int get_attribute(std::string attribute) const;
            dict<std::string, int> get_attributes() const;
            bool has_attribute(std::string attribute) const;
			void set_attribute(std::string attribute, int value);

            std::string get_egg_group_name() const;
            std::string get_form_name() const;
            std::string get_game_name() const;
            std::string get_species_name() const;
            
            int get_egg_group_id() const;
            int get_form_id() const;
            int get_game_id() const;
            int get_pokemon_id() const;
            int get_species_id() const;
            
            dict<unsigned int, std::string> get_types() const;
            dict<std::string, unsigned int> get_base_stats() const;
            dict<std::string, unsigned int> get_ev_yields() const;
            bool is_fully_evolved() const;
            void set_form(int form) const;
            void set_form(std::string form) const;
            
        protected:
        
            virtual unsigned int get_hp_from_ev_iv() const = 0;
            virtual unsigned int get_stat_from_ev_iv(unsigned int stat, unsigned int EV, unsigned int IV) const = 0;
            
            virtual void reset_volatile_status_map() = 0;
    };
}

#endif /* INCLUDED_TEAM_POKEMON_IMPL_HPP */