#ifndef PKMN_NATURES_HPP
#define PKMN_NATURES_HPP

#include <ciso646>
#include <iostream>
#include <string>
#include <map>

namespace pkmnsim
{
    class pkmn_nature
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
