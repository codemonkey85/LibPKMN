/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TRAINER_HPP
#define INCLUDED_PKMN_TRAINER_HPP

#include <string>

#include <pkmn/bag.hpp>
#include <pkmn/config.hpp>
#include <pkmn/team_pokemon.hpp>
#include <pkmn/types/pokemon_text.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    class PKMN_API trainer
    {
        public:

            typedef pkmn::shared_ptr<trainer> sptr;
            static sptr make(unsigned int game, pokemon_text name, unsigned int gender);
            static sptr make(std::string game, pokemon_text name, std::string gender);
            
            trainer() {};
            virtual ~trainer() {};

            //Game-specific Info
            virtual pokemon_text get_game() const = 0;
            virtual unsigned int get_generation() const = 0;

            //Get attributes
            virtual pokemon_text get_name() const = 0;
            virtual unsigned int get_money() const = 0;
            virtual pokemon_text get_gender() const = 0;
            virtual unsigned int get_id() const = 0;
            virtual unsigned short get_public_id() const = 0;
            virtual unsigned short get_secret_id() const = 0;

            //Set attributes
            virtual void set_name(pokemon_text name) = 0;
            virtual void set_money(unsigned int money) = 0;
            virtual void set_gender(pokemon_text gender) = 0;
            virtual void set_id(unsigned int id) = 0;
            virtual void set_public_id(unsigned short id) = 0;
            virtual void set_secret_id(unsigned short id) = 0;

            //Pokemon
            virtual team_pokemon::sptr get_pokemon(unsigned int pos) = 0;
            virtual void set_pokemon(unsigned int pos, team_pokemon::sptr t_pkmn) = 0;
            virtual void remove_pokemon(unsigned int pos) = 0;
            virtual void get_party(pokemon_team_t& party) = 0;
            virtual void set_party(pokemon_team_t& party) = 0;

            //Bag
            virtual bag::sptr get_bag() const = 0;

            //Database Info
            virtual unsigned int get_game_id() const = 0;
    };
}

#endif /* INCLUDED_PKMN_TRAINER_HPP */
