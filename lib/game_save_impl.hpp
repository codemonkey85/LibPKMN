/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_IMPL_HPP
#define INCLUDED_GAME_SAVE_IMPL_HPP

#include <pkmn/game_save.hpp>

#include "conversions/trainer.hpp"

namespace pkmn
{
    class game_save_impl: public game_save
    {
        public:
        
            game_save_impl() {};
            
            unsigned int get_game_id() const;
            
            trainer::sptr get_trainer() const;
            
            bag::sptr get_trainer_bag() const;
            
            pokemon_team_t get_trainer_party() const;
            void set_trainer_party(pokemon_team_t party);
            
            pokemon_text get_trainer_name() const;
            void set_trainer_name(pokemon_text trainer_name);
            
        protected:
        
            unsigned int _game_id;
            trainer::sptr _trainer;
    };
}
#endif /* INCLUDED_GAME_SAVE_IMPL_HPP */