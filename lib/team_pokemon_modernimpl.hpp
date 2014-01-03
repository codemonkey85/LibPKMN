/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TEAM_POKEMON_MODERNIMPL_HPP
#define INCLUDED_TEAM_POKEMON_MODERNIMPL_HPP

#include "team_pokemon_impl.hpp"

namespace pkmn
{
    class team_pokemon_modernimpl: public team_pokemon_impl
    {
        public:

            team_pokemon_modernimpl(base_pokemon::sptr base, unsigned int game, unsigned int lvl,
                                    unsigned int move1, unsigned int move2,
                                    unsigned int move3, unsigned int move4);

            void set_personality(unsigned int new_personality);
            
            unsigned int get_ability() const;
            unsigned int get_gender() const;
            unsigned int get_nature() const;
            bool is_shiny() const;
            void set_hidden_ability(bool val);
            
            dict<unsigned int, unsigned int> get_stats() const;
            dict<unsigned int, unsigned int> get_IVs() const;
            dict<unsigned int, unsigned int> get_EVs() const;
            
            void set_EV(unsigned int EV, unsigned int val);
            void set_IV(unsigned int IV, unsigned int val);
          
            std::string get_form_name() const;
            unsigned int get_form_id() const;
          
            void set_form(unsigned int form);
            void set_form(std::string form);

        private:
        
            unsigned int _get_hp() const;
            unsigned int _get_stat(unsigned int stat, unsigned int EV, unsigned int IV) const;
            
            unsigned int _determine_ability() const;
            unsigned int _determine_gender() const;
            unsigned int _determine_nature() const;
    };
}

#endif /* INCLUDED_TEAM_POKEMON_MODERNIMPL_HPP */
