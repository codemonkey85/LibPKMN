/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TRAINER_IMPL_HPP
#define INCLUDED_TRAINER_IMPL_HPP

#include <string>
#include <vector>

#include <pkmn/trainer.hpp>

#include <pkmn/types/prng.hpp>

namespace pkmn
{
    class trainer_impl: public trainer
    {
        public:

            trainer_impl(unsigned int game, pokemon_text name, unsigned int gender);

            //Game-specific Info
            pokemon_text get_game() const;
            unsigned int get_generation() const;

            //Get attributes
            pokemon_text get_name() const;
            unsigned int get_money() const;
            pokemon_text get_gender() const;
            unsigned int get_id() const;
            unsigned short get_public_id() const;
            unsigned short get_secret_id() const;

            //Set attributes
            void set_name(pokemon_text name);
            void set_money(unsigned int money);
            void set_gender(unsigned int gender);
            void set_id(unsigned int id);
            void set_public_id(unsigned short id);
            void set_secret_id(unsigned short id);

            //Pokemon
            team_pokemon::sptr get_pokemon(unsigned int pos, bool copy = false);
            void set_pokemon(unsigned int pos, team_pokemon::sptr t_pkmn);
            void remove_pokemon(unsigned int pos);
            void get_party(pokemon_team_t& party);
            void set_party(pokemon_team_t& party);

            //Bag
            bag::sptr get_bag(bool copy = false) const;
            
            //Database Info
            unsigned int get_game_id() const;

        protected:
        
            unsigned int _money, _game_id, _gender_id, _generation;

            union
            {
                struct
                {
                    unsigned short public_id;
                    unsigned short secret_id;
                } _tid;
                unsigned int _trainer_id;
            };

            bag::sptr _bag;
            pokemon_text _trainer_name;
            pokemon_team_t _party;
            
            std::string _sprite_path;

            static prng _rand_gen;
    };
}

#endif /* INCLUDED_TRAINER_IMPL_HPP */
