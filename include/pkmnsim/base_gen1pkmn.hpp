#ifndef BASE_GEN1PKMN_HPP
#define BASE_GEN1PKMN_HPP

#include <pkmnsim/base_pkmn.hpp>
#include <boost/format.hpp>
#include <ciso646>
#include <iostream>
#include <map>
#include <string>


class base_gen1pkmn: public base_pkmn
{
    public:
        base_gen1pkmn() {};
        base_gen1pkmn(std::string, int, std::string, std::string, std::string, double, double,
                      int, int, int, int, int, int);
        base_gen1pkmn(std::map<std::string, std::string> from_database);
        void print();
        void print_verbose();
        std::string get_display_name();        
        int get_nat_pokedex_num();
        std::string get_species();
        double get_height();
        double get_weight();
        int* get_base_stats();
        int* get_ev_yields();

    private:
        string display_name;
        int nat_pokedex_num;
        string species;
        string type1, type2;
        double height; //meters
        double weight; //kilograms
        int baseHP, baseATK, baseDEF, baseSPD, baseSPCL; //Base stats
        int exp_yield;

};

base_gen1pkmn get_gen1_pkmn(std::string);
vector<base_gen1pkmn> get_gen1_pkmn_of_type(std::string, std::string, bool);

#endif /*BASE_GEN1PKMN_HPP*/
