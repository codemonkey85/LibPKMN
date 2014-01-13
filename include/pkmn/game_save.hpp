/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_GAME_SAVE_HPP
#define INCLUDED_PKMN_GAME_SAVE_HPP

#include <string>
#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/trainer.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    class PKMN_API game_save
    {
        public:

            typedef pkmn::shared_ptr<game_save> sptr;
            static sptr make(std::string filename);

            //Class constructors (should never be called directly)
            game_save() {};
            virtual ~game_save() {};
            
            virtual void load() = 0;
            virtual void save() = 0;

            virtual unsigned int get_game_id() const = 0;
            
            virtual trainer::sptr get_trainer() const = 0;
            
            virtual pokemon_team_t get_trainer_party() const = 0;
            virtual void set_trainer_party(pokemon_team_t party) = 0;
            
            virtual pokemon_text get_trainer_name() const = 0;
            virtual void set_trainer_name(pokemon_text trainer_name) = 0;
    };
}
#endif /* INCLUDED_PKMN_GAME_SAVE_HPP */

