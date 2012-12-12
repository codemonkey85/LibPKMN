#ifndef PKMN_NATURES_HPP
#define PKMN_NATURES_HPP
#include <iostream>
#include <string>
#include <map>

using namespace std;

class pkmn_nature
{
    public:
        pkmn_nature() {};
        pkmn_nature(string,double,double,double,double,double);
        pkmn_nature(map<string,string>);
        string get_name();
        double* get_mods();
        void print();
    private:
        string name;
        double ATKmod, DEFmod, SPDmod, SATKmod, SDEFmod;
};

pkmn_nature get_nature(string);

#endif /*PKMN_NATURES_HPP*/
