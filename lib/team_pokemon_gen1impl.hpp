/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TEAM_POKEMON_GEN1IMPL_HPP
#define INCLUDED_TEAM_POKEMON_GEN1IMPL_HPP

#include "team_pokemon_impl.hpp"

namespace pkmn
{
    class team_pokemon_gen1impl: public team_pokemon_impl
    {
        public:

            team_pokemon_gen1impl();
            team_pokemon_gen1impl(base_pokemon::sptr base, unsigned int game, unsigned int level,
                                  unsigned int move1, unsigned int move2,
                                  unsigned int move3, unsigned int move4);

            std::string get_ability() const;
            unsigned int get_gender() const;
            unsigned int get_nature() const;
            bool is_shiny() const;
            
            dict<std::string, unsigned int> get_stats() const;
            dict<std::string, unsigned int> get_EVs() const;
            dict<std::string, unsigned int> get_IVs() const;
            
            void set_EV(std::string EV, unsigned int val);
            void set_IV(std::string IV, unsigned int val);
            
            unsigned int get_form_id() const;
            
            void set_form(unsigned int form);
            void set_form(std::string form);

        private:

            unsigned int _SPCL, _evSPCL, _ivSPCL;
        
            unsigned int _get_hp() const;
            unsigned int _get_stat(std::string stat, unsigned int EV, unsigned int IV) const;
    };
}

#endif /* INCLUDED_TEAM_POKEMON_GEN1IMPL_HPP */
