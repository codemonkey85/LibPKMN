/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TEAM_POKEMON_IMPL_HPP
#define INCLUDED_TEAM_POKEMON_IMPL_HPP

#include <pkmnsim/team_pokemon.hpp>

namespace pkmnsim
{
    class team_pokemon_impl: public team_pokemon
    {
        public:

            team_pokemon_impl(base_pokemon::sptr base_pkmn, unsigned int g, unsigned int lvl,
                              unsigned int move1, unsigned int move2,
                              unsigned int move3, unsigned int move4);

            base_pokemon::sptr get_base_pokemon() const;
            std::string get_nickname() const;
            void set_nickname(std::string name);
            unsigned int get_level() const;
            void set_level(unsigned int lvl);
            unsigned int get_met_level() const;
            void set_met_level(unsigned int lvl);
            
            moveset_t get_moves() const;
            vla<unsigned int> get_move_PPs() const;
            void set_move(unsigned int move_id, unsigned int pos);
            void set_move_PP(unsigned int new_PP, unsigned int pos);
            
            unsigned int get_status() const;
            void set_status(unsigned int status);
            
            unsigned int get_personality() const;
            virtual void set_personality(unsigned int new_personality);
            
            unsigned int get_generation() const;
            unsigned int get_held_item() const;
            void set_held_item(unsigned int item);
            unsigned int get_ball() const;
            void set_ball(unsigned int new_ball);
            std::string get_icon_path() const;
            std::string get_sprite_path() const;
            
			int get_attribute(std::string attribute) const;
            dict<std::string, int> get_attributes() const;
            bool has_attribute(std::string attribute) const;
			void set_attribute(std::string attribute, int value);
            void set_hidden_ability(bool val);

            unsigned int get_trainer_id() const;
            unsigned short get_public_trainer_id() const;
            unsigned short get_secret_trainer_id() const;
            void set_trainer_id(unsigned int id);
            void set_public_trainer_id(unsigned short id);
            void set_secret_trainer_id(unsigned short id);

            std::vector<std::string> get_egg_group_names() const;
            std::string get_game_name() const;
            std::string get_species_name() const;
            
            std::vector<unsigned int> get_egg_group_ids() const;
            unsigned int get_game_id() const;
            unsigned int get_pokemon_id() const;
            unsigned int get_species_id() const;
            
            dict<unsigned int, unsigned int> get_types() const;
            dict<unsigned int, unsigned int> get_base_stats() const;
            dict<unsigned int, unsigned int> get_ev_yields() const;
            bool is_fully_evolved() const;
    };
}

#endif /* INCLUDED_TEAM_POKEMON_IMPL_HPP */
