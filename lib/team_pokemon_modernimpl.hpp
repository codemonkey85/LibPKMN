/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
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

            team_pokemon_modernimpl();
            team_pokemon_modernimpl(base_pokemon::sptr base, unsigned int game, unsigned int lvl,
                                    unsigned int move1, unsigned int move2,
                                    unsigned int move3, unsigned int move4);
            team_pokemon_modernimpl(const team_pokemon_modernimpl &other);
            team_pokemon_modernimpl& operator=(const team_pokemon_modernimpl &other);
            ~team_pokemon_modernimpl() {};

            //Getting Individual Stat Info
            std::string get_gender() const;
            std::string get_nature() const;
            std::string get_ability() const;
            bool is_shiny() const;
            pkmn::dict<std::string, unsigned int> get_stats() const;
            pkmn::dict<std::string, unsigned int> get_EVs() const;
            pkmn::dict<std::string, unsigned int> get_IVs() const;

            //Setting Individual Stat Info
            void set_personality(unsigned int personality);
            void set_gender(std::string gender);
            void set_nature(std::string nature);
            void set_ability(std::string ability);
            virtual void set_using_hidden_ability(bool value);
            void set_EV(std::string stat_name, unsigned int stat);
            void set_IV(std::string stat_name, unsigned int stat);

            //Misc
            void set_form(std::string form);
            void set_form(unsigned int form);

        private:

            unsigned int _SATK, _SDEF;
            unsigned int _evSATK, _evSDEF;
            unsigned int _ivSATK, _ivSDEF;

            unsigned int _get_hp() const;
            unsigned int _get_stat(std::string stat, unsigned int EV, unsigned int IV) const;
            void _set_stats();

            std::string _determine_ability() const;
            std::string _determine_gender() const;
            std::string _determine_nature() const;
    };
}

#endif /* INCLUDED_TEAM_POKEMON_MODERNIMPL_HPP */
