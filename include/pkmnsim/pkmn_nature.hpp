/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_NATURES_HPP
#define PKMN_NATURES_HPP

#include <iostream>
#include <string>
#include <map>
#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    class PKMNSIM_API pkmn_nature
    {
        public:
            pkmn_nature() {};
            pkmn_nature(std::string, double, double, double, double, double);
            pkmn_nature(std::map<std::string, std::string>);
            std::string get_name();
            std::map<std::string, double> get_mods();
            void print();
        private:
            std::string name;
            double ATKmod, DEFmod, SPDmod, SATKmod, SDEFmod;
    };

    pkmn_nature get_nature(std::string);
}

#endif /*PKMN_NATURES_HPP*/
