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

#include <pkmnsim/move.hpp>
#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/config.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/types/dict.hpp>
#include <pkmnsim/types/vla.hpp>

namespace pkmnsim
{
    class PKMNSIM_API team_pokemon
    {
        public:

            typedef std::shared_ptr<team_pokemon> sptr;
            static sptr make(int identifier, int gen, int level,
                             int move1, int move2, int move3, int move4);

            //Class constructors (should never be called directly)
            team_pokemon() {};
            virtual ~team_pokemon() {};

            virtual std::string get_info(void) = 0;
            virtual std::string get_info_verbose(void) = 0;
            
            virtual base_pokemon::sptr get_base_pokemon(void) const = 0;
            virtual std::string get_nickname(void) const = 0;
            virtual unsigned int get_level(void) const = 0;
            virtual vla<move::sptr> get_moves(void) = 0;
            
            virtual unsigned int get_personality(void) const = 0;
            virtual unsigned int get_ability(void) const = 0;
            virtual unsigned int get_gender(void) const = 0;
            virtual unsigned int get_nature(void) const = 0;
            virtual bool is_shiny(void) const = 0;
            
            virtual dict<unsigned int, unsigned int> get_stats(void) const = 0;
            virtual dict<unsigned int, unsigned int> get_IVs(void) const = 0;
            virtual dict<unsigned int, unsigned int> get_EVs(void) const = 0;
            
            virtual void set_personality(void) = 0;
            
            virtual void set_EV(unsigned int EV, unsigned int val) = 0;
            virtual void set_IV(unsigned int IV, unsigned int val) = 0;
            virtual void set_move(move::sptr, unsigned int pos) = 0;
            
            virtual unsigned int get_generation(void) = 0;
            virtual unsigned int get_held_item(void) = 0;
            virtual void set_held_item(int item) = 0;
            virtual std::string get_icon_path(void) = 0;
            virtual std::string get_sprite_path(void) = 0;
            
			virtual int get_attribute(std::string attribute) = 0;
            virtual dict<std::string, int> get_attributes(void) = 0;
            virtual bool has_attribute(std::string attribute) = 0;
			virtual void set_attribute(std::string attribute, int value) = 0;

            virtual unsigned int get_game_id(void) = 0;
            virtual std::string get_species_name(void) = 0;
            virtual dict<unsigned int, std::string> get_types(void) = 0;
            virtual dict<std::string, unsigned int> get_base_stats(void) = 0;
            virtual dict<std::string, unsigned int> get_ev_yields(void) = 0;
            virtual bool is_fully_evolved(void) = 0;
            virtual int get_pokemon_id(void) = 0;
            virtual int get_species_id(void) = 0;
            virtual void set_form(int form) = 0;
            virtual void set_form(std::string form) = 0;

        protected:
            base_pokemon::sptr base;
            std::string nickname;
            std::string held_item;
            int from_game, from_gen;
            unsigned int level;
            unsigned int personality;
            unsigned short trainer_id, secret_id;

            unsigned int HP, ATK, DEF, SPD;
            unsigned int ivHP, ivATK, ivDEF, ivSPD;
            unsigned int evHP, evATK, evDEF, evSPD;
            dict<unsigned int, unsigned int> volatile_status_map;
            unsigned int nonvolatile_status;
            vla<base_move::sptr> moves;
            unsigned int num_moves;
            std::string icon_path, sprite_path;
			dict<std::string, int> attributes;

            virtual int get_hp_from_iv_ev() = 0;
            virtual int get_stat_from_iv_ev(std::string, int, int) = 0; //Others share common algorithm

            /*
             * Reset map of volatile statuses (confusion, infatuation, etc.).
             * This map varies with the generation.
             */
            virtual void reset_volatile_status_map(void) = 0;

            //Used by sub-classes, but needed for friend classes to access values
            //These are otherwise only used by appropriate subclasses
            int SPCL, SATK, SDEF;
            int ivSPCL, ivSATK, ivSDEF;
            int evSPCL, evSATK, evSDEF;
            int gender;
            pkmn_nature::sptr nature;
            std::string ability;
    };

    //Related typedefs
    typedef std::vector<team_pokemon::sptr> s_pkmn_vec_t;
    typedef vla<team_pokemon::sptr> s_pkmn_vla_t;
}
#endif /* INCLUDED_PKMNSIM_TEAM_POKEMON_HPP */