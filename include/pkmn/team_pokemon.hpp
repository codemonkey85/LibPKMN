/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TEAM_POKEMON_HPP
#define INCLUDED_PKMN_TEAM_POKEMON_HPP

#include <string>
#include <vector>

#include <pkmn/base_pokemon.hpp>
#include <pkmn/config.hpp>
#include <pkmn/item.hpp>
#include <pkmn/move.hpp>
#include <pkmn/nature.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/pokemon_text.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    class PKMN_API team_pokemon
    {
        public:

            typedef pkmn::shared_ptr<team_pokemon> sptr;
            static sptr make(unsigned int id, unsigned int game, unsigned int level,
                             unsigned int move1, unsigned int move2,
                             unsigned int move3, unsigned int move4);
            static sptr make(std::string name, std::string game, unsigned int level,
                             std::string move1, std::string move2,
                             std::string move3, std::string move4);

            team_pokemon() {};
            virtual ~team_pokemon() {};

            //Game-specific Info
            virtual std::string get_game() const = 0;
            virtual unsigned int get_generation() const = 0;

            //Non-battle Stats
            virtual base_pokemon::sptr get_base_pokemon(bool copy = false) const = 0;
            virtual pokemon_text get_species_name() const = 0;
            virtual pokemon_text get_nickname() const = 0;
            virtual string_pair_t get_types() const = 0;
            virtual pkmn::dict<std::string, unsigned int> get_base_stats() const = 0;
            virtual void set_nickname(pokemon_text nickname) = 0;

            //Getting Trainer Info
            virtual pokemon_text get_trainer_name() const = 0;
            virtual pokemon_text get_trainer_gender() const = 0;
            virtual unsigned int get_trainer_id() const = 0;
            virtual unsigned short get_trainer_public_id() const = 0;
            virtual unsigned short get_trainer_secret_id() const = 0;
            virtual pokemon_text get_ball() const = 0;
            virtual unsigned int get_met_level() const = 0;

            //Setting Trainer Info
            virtual void set_trainer_name(pokemon_text trainer_name) = 0;
            virtual void set_trainer_gender(pokemon_text gender) = 0;
            virtual void set_trainer_id(unsigned int id) = 0;
            virtual void set_trainer_public_id(unsigned short id) = 0;
            virtual void set_trainer_secret_id(unsigned short id) = 0;
            virtual void set_ball(pokemon_text ball) = 0;
            virtual void set_met_level(unsigned int level) = 0;

            //Getting Individual Stat Info
            virtual unsigned int get_personality() const = 0;
            virtual unsigned int get_level() const = 0;
            virtual std::string get_gender() const = 0;
            virtual nature get_nature() const = 0;
            virtual std::string get_ability() const = 0;
            virtual bool using_hidden_ability() const = 0;
            virtual bool is_shiny() const = 0;
            virtual pkmn::dict<std::string, unsigned int> get_stats() const = 0;
            virtual pkmn::dict<std::string, unsigned int> get_EVs() const = 0;
            virtual pkmn::dict<std::string, unsigned int> get_IVs() const = 0;
            
            //Setting Individual Stat Info
            virtual void set_personality(unsigned int personality) = 0;
            virtual void set_level(unsigned int level) = 0;
            virtual void set_nature(std::string nature_name) = 0;
            virtual void set_ability(std::string ability) = 0;
            virtual void set_using_hidden_ability(bool value) = 0;
            virtual void set_EV(std::string stat_name, unsigned int stat) = 0;
            virtual void set_IV(std::string stat_name, unsigned int stat) = 0;

            //Battle Stat Info
            virtual pokemon_text get_status() const = 0;
            virtual item::sptr get_held_item() const = 0;
            virtual void set_status(pokemon_text status) = 0;
            virtual void set_held_item(std::string item_name) = 0;
            virtual void set_held_item(item::sptr item_sptr) = 0;

            //Getting Move Info
            virtual move::sptr get_move(unsigned int pos) const = 0;
            virtual void get_moves(moveset_t& moves) const = 0;
            virtual unsigned int get_move_PP(unsigned int pos) const = 0;
            virtual void get_move_PPs(std::vector<unsigned int>& move_PPs) const = 0;

            //Setting Move Info
            virtual void set_move(std::string move_name, unsigned int pos) = 0;
            virtual void set_move(unsigned int move_id, unsigned int pos) = 0;
            virtual void set_move(move::sptr move_sptr, unsigned int pos) = 0;
            virtual void set_move_PP(unsigned int PP, unsigned int pos) = 0;

            //Misc
            virtual int get_attribute(std::string attribute) const = 0;
            virtual pkmn::dict<std::string, int> get_attributes() const = 0;
            virtual bool has_attribute(std::string attribute) const = 0;
			virtual void set_attribute(std::string attribute, int value) = 0;
            virtual std::string get_icon_path() const = 0;
            virtual std::string get_sprite_path() const = 0;
            virtual void set_form(std::string form) = 0;
            virtual void set_form(unsigned int form) = 0;

            //Database Info
            virtual unsigned int get_form_id() const = 0;
            virtual unsigned int get_game_id() const = 0;
            virtual unsigned int get_pokemon_id() const = 0;
            virtual unsigned int get_species_id() const = 0;
            virtual unsigned int get_ability_id() const = 0;
            virtual unsigned int get_item_id() const = 0;
            virtual unsigned int get_nature_id() const = 0;
    };

    //Related typedefs
    typedef std::vector<team_pokemon::sptr> pokemon_team_t;
}

#endif /* INCLUDED_PKMN_TEAM_POKEMON_HPP */
