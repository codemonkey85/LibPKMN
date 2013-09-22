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
#include <vector>

#include <pkmnsim/config.hpp>
#include <pkmnsim/team_pokemon.hpp>

namespace pkmnsim
{
    class PKMNSIM_API trainer
    {
        public:

            typedef std::shared_ptr<trainer> sptr;
            static sptr make(std::string filename, unsigned int game);
            
            trainer() {};
            virtual ~trainer() {};

            virtual unsigned int get_game_id() const = 0;
            virtual unsigned int get_money() const = 0;
            virtual void get_party(std::vector<team_pokemon::sptr> &party_vec) = 0;
            virtual std::string get_trainer_name() = 0;
            virtual unsigned short get_trainer_id() = 0;
            virtual void export_to_file(std::string filename) = 0;
            virtual void import_from_file(std::string filename) = 0;

        protected:
            unsigned int money, game_id, gender;
            unsigned short trainer_id;
            std::string trainer_name;
            std::vector<spec_pokemon::sptr> party;
    };
}

#endif /* INCLUDED_PKMNSIM_TRAINER_HPP */
