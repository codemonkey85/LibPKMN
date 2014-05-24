/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TEAM_POKEMON_IMPL_HPP
#define INCLUDED_TEAM_POKEMON_IMPL_HPP

#include <pkmn/team_pokemon.hpp>

#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    class team_pokemon_impl: public team_pokemon
    {
        public:

            team_pokemon_impl() {};
            team_pokemon_impl(const team_pokemon_impl& other);
            team_pokemon_impl& operator=(const team_pokemon_impl& other);
            team_pokemon_impl(base_pokemon::sptr base_pkmn, unsigned int game, unsigned int level,
                              unsigned int move1, unsigned int move2,
                              unsigned int move3, unsigned int move4);

            //Game-specific Info
            std::string get_game() const;
            unsigned int get_generation() const;

            //Non-battle Stats
            base_pokemon::sptr get_base_pokemon(bool copy = false) const;
            pokemon_text get_species_name() const;
            pokemon_text get_nickname() const;
            string_pair_t get_types() const;
            pkmn::dict<std::string, unsigned int> get_base_stats() const;
            void set_nickname(pokemon_text nickname);

            //Getting Trainer Info
            pokemon_text get_trainer_name() const;
            pokemon_text get_trainer_gender() const;
            unsigned int get_trainer_id() const;
            unsigned short get_trainer_public_id() const;
            unsigned short get_trainer_secret_id() const;
            pokemon_text get_ball() const;
            unsigned int get_met_level() const;

            //Setting Trainer Info
            void set_trainer_name(pokemon_text trainer_name);
            void set_trainer_gender(pokemon_text gender);
            void set_trainer_id(unsigned int id);
            void set_trainer_public_id(unsigned short id);
            void set_trainer_secret_id(unsigned short id);
            void set_ball(pokemon_text ball);
            void set_met_level(unsigned int level);

            //Getting Individual Stat Info
            unsigned int get_personality() const;
            unsigned int get_level() const;
            bool using_hidden_ability() const;

            //Setting Individual Stat Info
            void set_level(unsigned int level);
            virtual void set_using_hidden_ability(bool value);

            //Battle Stat Info
            pokemon_text get_status() const;
            item::sptr get_held_item() const;
            void set_status(pokemon_text status);
            void set_held_item(std::string item_name);
            void set_held_item(item::sptr item_sptr);

            //Getting Move Info
            move::sptr get_move(unsigned int pos) const;
            void get_moves(moveset_t& moves) const;
            unsigned int get_move_PP(unsigned int pos) const;
            void get_move_PPs(std::vector<unsigned int>& move_PPs) const;

            //Setting Move Info
            void set_move(std::string move_name, unsigned int pos);
            void set_move(unsigned int move_id, unsigned int pos);
            void set_move(move::sptr move_sptr, unsigned int pos);
            void set_move_PP(unsigned int PP, unsigned int pos);

            //Misc
            int get_attribute(std::string attribute) const;
            pkmn::dict<std::string, int> get_attributes() const;
            bool has_attribute(std::string attribute) const;
			void set_attribute(std::string attribute, int value);
            std::string get_icon_path() const;
            std::string get_sprite_path() const;
            virtual void set_form(std::string form);
            virtual void set_form(unsigned int form);

            //Database Info
            unsigned int get_form_id() const;
            unsigned int get_game_id() const;
            unsigned int get_pokemon_id() const;
            unsigned int get_species_id() const;
            unsigned int get_ability_id() const;
            unsigned int get_item_id() const;
            unsigned int get_nature_id() const;

        protected:

            base_pokemon::sptr _base_pkmn;
            unsigned int _pokemon_id, _form_id; //Copied from _base_pkmn for _check()
            pokemon_text _nickname, _trainer_name, _ball;
            std::string _trainer_gender;
            unsigned int _held_item;
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
            pokemon_text _nonvolatile_status;
            moveset_t _moves;
            std::vector<unsigned int> _move_PPs;
            std::string _icon_path, _sprite_path;
            pkmn::dict<std::string, int> _attributes;

            virtual unsigned int _get_hp() const = 0;
            virtual unsigned int _get_stat(std::string stat, unsigned int EV, unsigned int IV) const = 0;
            virtual void _set_stats() = 0;

            std::string _ability, _gender, _nature, _otgender;
            bool _has_hidden_ability;

            static SQLite::Database _db;

            void _check() const;
    };
}

#endif /* INCLUDED_TEAM_POKEMON_IMPL_HPP */
