/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
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
        
            trainer_impl(unsigned int game, pokemon_text name, unsigned int gndr);

            unsigned int get_game_id() const;

            unsigned int get_money() const;
            void set_money(unsigned int new_money);

            pokemon_team_t get_party();
            team_pokemon::sptr get_pokemon(unsigned int pos, bool copy = true);

            void set_party(pokemon_team_t &team);
            void set_pokemon(unsigned int pos, team_pokemon::sptr t_pkmn);
            void remove_pokemon(unsigned int pos);
            
            const bag::sptr get_bag() const;

            pokemon_text get_name() const;
            void set_name(pokemon_text new_name);

            unsigned int get_gender() const;
            void set_gender(unsigned int new_gender);
            
            unsigned int get_id() const;
            unsigned short get_public_id() const;
            unsigned short get_secret_id() const;
            void set_id(unsigned int id);
            void set_public_id(unsigned short id);
            void set_secret_id(unsigned short id);

        protected:
        
            unsigned int _money, _game_id, _gender;

            union
            {
                struct
                {
                    unsigned short public_id;
                    unsigned short secret_id;
                } _tid;
                unsigned int _trainer_id;
            };

            bag::sptr _item_bag;
            pokemon_text _trainer_name;
            pokemon_team_t _party;
            
            std::string _sprite_path;

            static prng _rand_gen;
    };
}

#endif /* INCLUDED_TRAINER_IMPL_HPP */
