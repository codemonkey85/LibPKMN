/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_TEAM_POKEMON_HPP
#define INCLUDED_PKMNSIM_TEAM_POKEMON_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <string>
#include <vector>

#include <pkmnsim/move.hpp>
#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/config.hpp>
#include <pkmnsim/types/dict.hpp>
#include <pkmnsim/types/vla.hpp>

namespace pkmnsim
{
    class PKMNSIM_API team_pokemon
    {
        public:

            typedef std::shared_ptr<team_pokemon> sptr;
            static sptr make(unsigned int id, unsigned int game, unsigned int level,
                             unsigned int move1, unsigned int move2,
                             unsigned int move3, unsigned int move4);

            team_pokemon() {};
            virtual ~team_pokemon() {};

            virtual std::string get_info() const = 0;
            virtual std::string get_info_verbose() const = 0;
            
            virtual base_pokemon::sptr get_base_pokemon() const = 0;
            virtual std::string get_nickname() const = 0;
            virtual unsigned int get_level() const = 0;

            virtual vla<move::sptr> get_moves() const = 0;
            virtual vla<unsigned int> get_move_PPs() const = 0;
            virtual void set_move(unsigned int move_id, unsigned int pos) = 0;
            virtual void set_move_PP(unsigned int new_PP, unsigned int pos) = 0;
            
            virtual unsigned int get_status() const = 0;
            virtual void set_status(unsigned int status) = 0;
            
            virtual unsigned int get_personality() const = 0;
            virtual unsigned int get_ability() const = 0;
            virtual unsigned int get_gender() const = 0;
            virtual unsigned int get_nature() const = 0;
            virtual bool is_shiny() const = 0;
            
            virtual dict<unsigned int, unsigned int> get_stats() const = 0;
            virtual dict<unsigned int, unsigned int> get_IVs() const = 0;
            virtual dict<unsigned int, unsigned int> get_EVs() const = 0;
            
            virtual void set_personality(unsigned int personality) = 0;
            
            virtual void set_EV(unsigned int EV, unsigned int val) = 0;
            virtual void set_IV(unsigned int IV, unsigned int val) = 0;
            
            virtual unsigned int get_generation() const = 0;
            virtual unsigned int get_held_item() const = 0;
            virtual void set_held_item(unsigned int item) = 0;
            virtual std::string get_icon_path() const = 0;
            virtual std::string get_sprite_path() const = 0;
            
			virtual int get_attribute(std::string attribute) const = 0;
            virtual dict<std::string, int> get_attributes() const = 0;
            virtual bool has_attribute(std::string attribute) const = 0;
			virtual void set_attribute(std::string attribute, int value) = 0;
            virtual void set_hidden_ability(bool val) = 0;

            virtual unsigned int get_trainer_id() const = 0;
            virtual unsigned short get_public_trainer_id() const = 0;
            virtual unsigned short get_secret_trainer_id() const = 0;
            virtual void set_trainer_id(unsigned int id) = 0;
            virtual void set_public_trainer_id(unsigned short id) = 0;
            virtual void set_secret_trainer_id(unsigned short id) = 0;

            virtual std::vector<std::string> get_egg_group_names() const = 0;
            virtual std::string get_form_name() const = 0;
            virtual std::string get_game_name() const = 0;
            virtual std::string get_species_name() const = 0;
            
            virtual std::vector<unsigned int> get_egg_group_ids() const = 0;
            virtual unsigned int get_form_id() const = 0;
            virtual unsigned int get_game_id() const = 0;
            virtual unsigned int get_pokemon_id() const = 0;
            virtual unsigned int get_species_id() const = 0;
            
            virtual dict<unsigned int, unsigned int> get_types() const = 0;
            virtual dict<unsigned int, unsigned int> get_base_stats() const = 0;
            virtual dict<unsigned int, unsigned int> get_ev_yields() const = 0;
            virtual bool is_fully_evolved() const = 0;
            virtual void set_form(unsigned int form) = 0;
            virtual void set_form(std::string form) = 0;

        protected:
            base_pokemon::sptr base_pkmn;
            std::string nickname;
            unsigned int held_item;
            unsigned int from_game, from_gen;
            unsigned int level;
            unsigned int personality;
            
            union
            {
                struct
                {
                    unsigned short public_id;
                    unsigned short secret_id;
                } tid;
                unsigned int trainer_id;
            };

            unsigned int HP, ATK, DEF, SPD;
            unsigned int evHP, evATK, evDEF, evSPD;
            unsigned int ivHP, ivATK, ivDEF, ivSPD;
            unsigned int nonvolatile_status;
            b_move_vla_t moves;
            vla<unsigned int> move_PPs;
            unsigned int num_moves;
            std::string icon_path, sprite_path;
			dict<std::string, int> attributes;

            virtual unsigned int get_hp_from_ev_iv() const = 0;
            virtual unsigned int get_stat_from_ev_iv(unsigned int stat, unsigned int EV, unsigned int IV) const = 0;

            unsigned int SPCL, SATK, SDEF;
            unsigned int ivSPCL, ivSATK, ivSDEF;
            unsigned int evSPCL, evSATK, evSDEF;
            unsigned int gender;
            unsigned int ability, nature;
            bool has_hidden_ability;
    };

    //Related typedefs
    typedef std::vector<team_pokemon::sptr> s_pkmn_vec_t;
    typedef vla<team_pokemon::sptr> s_pkmn_vla_t;
    typedef vla<team_pokemon::sptr> pokemon_team_t;
}

#endif /* INCLUDED_PKMNSIM_TEAM_POKEMON_HPP */