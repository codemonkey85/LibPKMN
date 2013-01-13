#ifndef BASE_GEN2PKMN_HPP
#define BASE_GEN2PKMN_HPP

#include <pkmnsim/base_pkmn.hpp>
#include <ciso646>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class base_gen2pkmn: public base_pkmn
{
    public:
        base_gen2pkmn() {};
        base_gen2pkmn(std::string, int, std::string, std::string, std::string,
                      double, double, double, double,
                      int, int, int, int, int, int, int);
        base_gen2pkmn(std::map<std::string,std::string>);
        void print_verbose();
        int* get_base_stats();
        int* get_ev_yields();
        //std::string * get_move_list();
};

base_gen2pkmn get_gen2_pokemon(std::string,int);
std::vector<base_gen2pkmn> get_gen2_pkmn_of_type(std::string,std::string,bool);

#endif /*BASE_GEN2PKMN_HPP*/
