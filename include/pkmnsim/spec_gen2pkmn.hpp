#ifndef SPEC_GEN2PKMN_HPP
#define SPEC_GEN2PKMN_HPP

#include <iostream>
#include <string>
#include <map>
#include <pkmnsim/base_gen2pkmn.hpp>
#include <pkmnsim/spec_pkmn.hpp>

class spec_gen2pkmn: public spec_pkmn
{
    public:
        spec_gen2pkmn() {};
        spec_gen2pkmn(base_gen2pkmn, int, int);
        spec_gen2pkmn(base_gen2pkmn, std::string, int, char, std::string, int, int, int, int,
                  int, int, std::string, std::string, std::string, std::string);
        base_gen2pkmn get_base_gen2pkmn();
        void reset_volatile_status_map();
        void print();
        void print_verbose();
        
    private:
        base_gen2pkmn base;
        int get_hp_from_iv_ev();
        int get_stat_from_iv_ev(int,int,int); //for others besides HP
};

#endif /*SPEC_GEN2PKMN_HPP*/
