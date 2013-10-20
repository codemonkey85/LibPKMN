/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_TRAINER_HPP
#define INCLUDED_PKMNSIM_TRAINER_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

#include <string>

#include <pkmnsim/config.hpp>
#include <pkmnsim/team_pokemon.hpp>
#include <pkmnsim/types/pokemon_text.hpp>
#include <pkmnsim/types/vla.hpp>

namespace pkmnsim
{
    class PKMNSIM_API trainer
    {
        public:

            typedef std::shared_ptr<trainer> sptr;
            static sptr make(unsigned int game, std::string name, unsigned int gender);
            
            trainer(unsigned int game, std::string name, unsigned int gender) {};
            trainer() {};
            virtual ~trainer() {};

            virtual unsigned int get_game_id() const = 0;

            virtual unsigned int get_money() const = 0;
            virtual void set_money(unsigned int new_money) = 0;

            virtual pokemon_team_t get_party() = 0;
            virtual team_pokemon::sptr get_pokemon(unsigned int pos) = 0;

            virtual void set_party(pokemon_team_t &team) = 0;
            virtual void set_pokemon(unsigned int pos, team_pokemon::sptr t_pkmn) = 0;

            virtual std::string get_trainer_name() const = 0;
            virtual void set_trainer_name(std::string new_name) = 0;

            virtual unsigned int get_trainer_id() const = 0;
            virtual unsigned short get_public_trainer_id() const = 0;
            virtual unsigned short get_secret_trainer_id() const = 0;
            virtual void set_trainer_id(unsigned int id) = 0;
            virtual void set_public_trainer_id(unsigned short id) = 0;
            virtual void set_secret_trainer_id(unsigned short id) = 0;

        protected:
            unsigned int money, game_id, gender;

            union
            {
                struct
                {
                    unsigned short public_id;
                    unsigned short secret_id;
                } tid;
                unsigned int trainer_id;
            };

            pokemon_text trainer_name;
            pokemon_team_t party;
    };
}

#endif /* INCLUDED_PKMNSIM_TRAINER_HPP */
