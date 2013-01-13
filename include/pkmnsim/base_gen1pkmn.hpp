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
        int* get_base_stats();
        int* get_ev_yields();

    private:
        int baseSPCL; //Generation 1 has Special stat instead of Special Attack/Defense

};

base_gen1pkmn get_gen1_pkmn(std::string);
void get_gen1_pkmn_of_type(std::vector<base_pkmn>&, std::string, std::string, bool);

#endif /*BASE_GEN1PKMN_HPP*/
