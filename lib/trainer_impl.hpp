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

#include <pkmnsim/trainer.hpp>

namespace pkmnsim
{
    class trainer_impl: public trainer
    {
        public:
            trainer_impl(unsigned int game, std::string name, unsigned int gndr);

            unsigned int get_game_id() const;

            unsigned int get_money() const;
            void set_money(unsigned int new_money);

            pokemon_team_t get_party();
            team_pokemon::sptr get_pokemon(unsigned int pos, bool copy = true);

            void set_party(pokemon_team_t &team);
            void set_pokemon(unsigned int pos, team_pokemon::sptr t_pkmn);
            
            void remove_pokemon(unsigned int pos);

            std::string get_name() const;
            void set_name(std::string new_name);

            unsigned int get_gender() const;
            void set_gender(unsigned int new_gender);
            
            unsigned int get_id() const;
            unsigned short get_public_id() const;
            unsigned short get_secret_id() const;
            void set_id(unsigned int id);
            void set_public_id(unsigned short id);
            void set_secret_id(unsigned short id);
    };
}

#endif /* INCLUDED_TRAINER_IMPL_HPP */
