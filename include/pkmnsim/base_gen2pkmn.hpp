#ifndef BASE_GEN2PKMN_HPP
#define BASE_GEN2PKMN_HPP
#include <ciso646>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class base_gen2pkmn
{
    public:
        base_gen2pkmn() {};
        base_gen2pkmn(std::string, int, std::string, std::string, std::string,
                      double, double, double, double,
                      int, int, int, int, int, int, int);
        base_gen2pkmn(std::map<std::string,std::string>);
        void print_verbose();
        std::string get_display_name();
        int get_nat_pokedex_num();
        std::string get_species();
        std::string* get_types();
        double get_height();
        double get_weight();
        double get_chance_male();
        double get_chance_female();
        int* get_base_stats();
        int* get_ev_yields();
        //std::string * get_move_list();
    private:
        std::string display_name;
        int nat_pokedex_num;
        std::string species;
        std::string type1, type2;
        double height; //meters
        double weight; //kilograms
        double chance_male, chance_female; //Gender proportions (both are 0.0 if Pokemon is ungendered)
        int baseHP, baseATK, baseDEF, baseSPD, baseSATK, baseSDEF; //Base stats
        int exp_yield; //Effort value yield
        //std::string * move_list;
};

base_gen2pkmn get_pokemon(std::string,int);
std::vector<base_gen2pkmn> get_pkmn_of_type(std::string,std::string,int,bool);
#endif /*BASE_GEN2PKMN_HPP*/
