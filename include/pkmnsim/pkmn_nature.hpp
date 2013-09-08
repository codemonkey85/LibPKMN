/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_PKMN_NATURE_HPP
#define INCLUDED_PKMNSIM_PKMN_NATURE_HPP

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif
#include <iostream>
#include <string>

#include <pkmnsim/config.hpp>
#include <pkmnsim/dict.hpp>

namespace pkmnsim
{
    /*
     * Pokémon Nature Class
     *
     * This class generates an object representing a specific Pokémon's
     * nature. This affects two of its stats, increasing one by 10%
     * and decreasing another by 10%. A specific Pokémon's nature is
     * randomly chosen upon creation.
     */
    class PKMNSIM_API pkmn_nature
    {
        public:

            typedef std::shared_ptr<pkmn_nature> sptr;

            //Class constructors (should never be called directly)
            pkmn_nature(void) {};
            pkmn_nature(int id);

            /*
             * Returns a std::shared_ptr<pkmn_nature> of specified nature.
             *
             * Parameters:
             *  - id: enum associated with nature ID
             */
            static sptr make(int id);

            /*
             * Returns the nature's name.
             */
            std::string get_name(void);

            /*
             * Returns the damage mods for each stat type.
             * One will be 1.1, one will be 0.9, and the rest will be 1.0.
             */
            dict<std::string, double> get_mods(void);

            /*
             * Return string with all information on this nature.
             */
            std::string get_info(void);
            
            /*
             * Returns natures.id from pkmnsim.db
             */
            int get_nature_id(void);
            
        private:
            int nature_id;
            std::string name;
            double ATKmod, DEFmod, SPDmod, SATKmod, SDEFmod;
    };
}

#endif /* INCLUDED_PKMNSIM_PKMN_NATURE_HPP */
