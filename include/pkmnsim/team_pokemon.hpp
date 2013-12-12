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

#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/config.hpp>
#include <pkmnsim/item.hpp>
#include <pkmnsim/move.hpp>
#include <pkmnsim/types/dict.hpp>
#include <pkmnsim/types/pokemon_text.hpp>
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
            
            virtual base_pokemon::sptr get_base_pokemon(bool copy = true) const = 0;
            virtual pokemon_text get_nickname() const = 0;
            virtual void set_nickname(pokemon_text name) = 0;
            virtual pokemon_text get_trainer_name() const = 0;
            virtual void set_trainer_name(pokemon_text name) = 0;
            virtual unsigned int get_level() const = 0;
            virtual void set_level(unsigned int level) = 0;
            virtual unsigned int get_met_level() const = 0;
            virtual void set_met_level(unsigned int level) = 0;

            virtual moveset_t get_moves() const = 0;
            virtual vla<unsigned int> get_move_PPs() const = 0;
            virtual void set_move(unsigned int move_id, unsigned int pos) = 0;
            virtual void set_move_PP(unsigned int PP, unsigned int pos) = 0;
            
            virtual unsigned int get_status() const = 0;
            virtual void set_status(unsigned int status) = 0;
            
            virtual unsigned int get_personality() const = 0;
            virtual unsigned int get_ability() const = 0;
            virtual unsigned int get_gender() const = 0;
            virtual unsigned int get_nature() const = 0;
            virtual bool is_shiny() const = 0;
            
            virtual void set_ability(unsigned int ability) = 0;
            virtual void set_nature(unsigned int nature) = 0;
            virtual void set_gender(unsigned int gender) = 0;

            virtual dict<unsigned int, unsigned int> get_stats() const = 0;
            virtual dict<unsigned int, unsigned int> get_IVs() const = 0;
            virtual dict<unsigned int, unsigned int> get_EVs() const = 0;
            
            virtual void set_personality(unsigned int personality) = 0;
            
            virtual void set_EV(unsigned int EV, unsigned int val) = 0;
            virtual void set_IV(unsigned int IV, unsigned int val) = 0;
            
            virtual unsigned int get_generation() const = 0;
            virtual item::sptr get_held_item(bool copy = true) const = 0;
            virtual void set_held_item(item::sptr item) = 0;
            virtual void set_held_item(unsigned int item_id) = 0;
            virtual unsigned int get_ball() const = 0;
            virtual void set_ball(unsigned int ball) = 0;
            virtual std::string get_icon_path() const = 0;
            virtual std::string get_sprite_path() const = 0;
            
			virtual int get_attribute(std::string attribute) const = 0;
            virtual dict<std::string, int> get_attributes() const = 0;
            virtual bool has_attribute(std::string attribute) const = 0;
			virtual void set_attribute(std::string attribute, int value) = 0;
            virtual void set_hidden_ability(bool val) = 0;

            virtual unsigned int get_trainer_gender() const = 0;
            virtual void set_trainer_gender(unsigned int gender) = 0;
            virtual unsigned int get_trainer_id() const = 0;
            virtual unsigned short get_public_trainer_id() const = 0;
            virtual unsigned short get_secret_trainer_id() const = 0;
            virtual void set_trainer_id(unsigned int id) = 0;
            virtual void set_public_trainer_id(unsigned short id) = 0;
            virtual void set_secret_trainer_id(unsigned short id) = 0;

            virtual void get_egg_group_names(std::vector<std::string>
                                             &egg_group_name_vec) const = 0;
            virtual std::string get_form_name() const = 0;
            virtual std::string get_species_name() const = 0;
            
            virtual void get_egg_group_ids(std::vector<unsigned int>
                                           &egg_group_id_vec) const = 0;
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
        
            base_pokemon::sptr _base_pkmn;
            pokemon_text _nickname, _trainer_name;
            item::sptr _held_item;
            unsigned int _ball;
            unsigned int _game_id, _generation;
            unsigned int _level, _met_level;
            unsigned int _personality;
            
            union
            {
                struct
                {
                    unsigned short public_id;
                    unsigned short secret_id;
                } _tid;
                unsigned int _trainer_id;
            };

            unsigned int _HP, _ATK, _DEF, _SPD;
            unsigned int _evHP, _evATK, _evDEF, _evSPD;
            unsigned int _ivHP, _ivATK, _ivDEF, _ivSPD;
            unsigned int _nonvolatile_status;
            moveset_t _moves;
            vla<unsigned int> _move_PPs;
            unsigned int _num_moves;
            std::string _icon_path, _sprite_path;
			dict<std::string, int> _attributes;

            virtual unsigned int _get_hp() const = 0;
            virtual unsigned int _get_stat(unsigned int stat, unsigned int EV, unsigned int IV) const = 0;

            unsigned int _SPCL, _SATK, _SDEF;
            unsigned int _ivSPCL, _ivSATK, _ivSDEF;
            unsigned int _evSPCL, _evSATK, _evSDEF;
            unsigned int _gender, _otgender;
            unsigned int _ability, _nature;
            bool _has_hidden_ability;
    };

    //Related typedefs
    typedef std::vector<team_pokemon::sptr> t_pkmn_vec_t;
    typedef vla<team_pokemon::sptr> t_pkmn_vla_t;
    typedef vla<team_pokemon::sptr> pokemon_team_t;
}

#endif /* INCLUDED_PKMNSIM_TEAM_POKEMON_HPP */
