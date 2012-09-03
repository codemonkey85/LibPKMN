#ifndef PKMN_NATURE_HPP
#define PKMN_NATURE_HPP
#include <iostream>
#include <string>
#include <map>

class pkmn_nature
{
    public:
        pkmn_nature() {};
        pkmn_nature(std::string,double,double,double,double,double);
        std::string get_name();
        double* get_mods();
        void print();
    private:
        std::string name;
        double ATKmod, DEFmod, SPDmod, SATKmod, SDEFmod;
};

#endif /*PKMN_NATURE_HPP*/
