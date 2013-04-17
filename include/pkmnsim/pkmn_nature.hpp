/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_NATURES_HPP
#define INCLUDED_PKMN_NATURES_HPP

#include <boost/shared_ptr.hpp>
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
            pkmn_nature() {};
            pkmn_nature(std::string, double, double, double, double, double);

            typedef boost::shared_ptr<pkmn_nature> sptr;
            static sptr make(std::string);

            /*
             * Get the nature's name.
             * \return Nature's name
             */
            std::string get_name();

            /*
             * Get the mods for each stat type.
             * \return a dict whose keys=stat names and vals=mods
             */
            dict<std::string, double> get_mods();

            /*
             * Get a string with all information on this nature.
             * \return a string with all nature fields
             */
            std::string get_info();
        private:
            std::string name;
            double ATKmod, DEFmod, SPDmod, SATKmod, SDEFmod;
    };

    /*
     * User-friendly function to get a Pokémon nature.
     * \param identifier Nature's name in the database.
     * \return Pokémon nature
     */
    pkmn_nature PKMNSIM_API get_nature(std::string identifier);
}

#endif /*INCLUDED_PKMN_NATURES_HPP*/
