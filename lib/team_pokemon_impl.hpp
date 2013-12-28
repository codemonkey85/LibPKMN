/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TEAM_POKEMON_IMPL_HPP
#define INCLUDED_TEAM_POKEMON_IMPL_HPP

#include <pkmnsim/team_pokemon.hpp>

#include <pkmnsim/types/prng.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

namespace pkmnsim
{
    class team_pokemon_impl: public team_pokemon
    {
        public:

            team_pokemon_impl(base_pokemon::sptr base_pkmn, unsigned int g, unsigned int level,
                              unsigned int move1, unsigned int move2,
                              unsigned int move3, unsigned int move4);

            base_pokemon::sptr get_base_pokemon(bool copy = true) const;
            pokemon_text get_nickname() const;
            void set_nickname(pokemon_text name);
            pokemon_text get_trainer_name() const;
            void set_trainer_name(pokemon_text name);
            unsigned int get_level() const;
            void set_level(unsigned int level);
            unsigned int get_met_level() const;
            void set_met_level(unsigned int level);
            
            moveset_t get_moves() const;
            vla<unsigned int> get_move_PPs() const;
            void set_move(unsigned int move_id, unsigned int pos);
            void set_move_PP(unsigned int PP, unsigned int pos);
            
            unsigned int get_status() const;
            void set_status(unsigned int status);
            
            unsigned int get_personality() const;
            virtual void set_personality(unsigned int personality);

            void set_ability(unsigned int ability);
            void set_nature(unsigned int nature);
            void set_gender(unsigned int gender);
            
            unsigned int get_generation() const;
            item::sptr get_held_item(bool copy = true) const;
            void set_held_item(item::sptr item);
            void set_held_item(unsigned int item_id);
            unsigned int get_ball() const;
            void set_ball(unsigned int ball);
            std::string get_icon_path() const;
            std::string get_sprite_path() const;
            
			int get_attribute(std::string attribute) const;
            dict<std::string, int> get_attributes() const;
            bool has_attribute(std::string attribute) const;
			void set_attribute(std::string attribute, int value);
            void set_hidden_ability(bool val);

            unsigned int get_trainer_gender() const;
            void set_trainer_gender(unsigned int gender);
            unsigned int get_trainer_id() const;
            unsigned short get_public_trainer_id() const;
            unsigned short get_secret_trainer_id() const;
            void set_trainer_id(unsigned int id);
            void set_public_trainer_id(unsigned short id);
            void set_secret_trainer_id(unsigned short id);

            void get_egg_group_names(std::vector<std::string>
                                     &egg_group_name_vec) const;
            void get_egg_group_ids(std::vector<unsigned int>
                                   &egg_group_id_vec) const;
            
            unsigned int get_game_id() const;
            unsigned int get_pokemon_id() const;
            unsigned int get_species_id() const;
            std::string get_species_name() const;
            
            dict<unsigned int, unsigned int> get_types() const;
            dict<unsigned int, unsigned int> get_base_stats() const;
            dict<unsigned int, unsigned int> get_ev_yields() const;
            bool is_fully_evolved() const;

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
            
            static SQLite::Database _db;
            static prng _rand_gen;
    };
}

#endif /* INCLUDED_TEAM_POKEMON_IMPL_HPP */
