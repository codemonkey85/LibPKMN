#ifndef BASE_PKMN_HPP
#define BASE_PKMN_HPP

#include <ciso646>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class base_pkmn
{
    public:
        base_pkmn() {};
        base_pkmn(std::string, int, std::string, std::string, std::string, std::string, std::string, std::string,
                  double, double, double, double,
                  int, int, int, int, int, int, int, int, int, int, int, int, int);
        base_pkmn(std::map<std::string,std::string>);
        virtual void print();
        virtual void print_verbose();
        std::string get_display_name();
        int get_nat_pokedex_num();
        std::string get_species();
        virtual std::string* get_types();
        std::string* get_abilities();
        double get_height();
        double get_weight();
        double get_chance_male();
        double get_chance_female();
        virtual int* get_base_stats();
        virtual int* get_ev_yields();
        //virtual std::string * get_move_list();
    protected:
        std::string display_name;
        int nat_pokedex_num;
        std::string species;
        std::string type1, type2;
        std::string ability1, ability2, ability3;
        double height; //meters
        double weight; //kilograms
        double chance_male, chance_female; //Gender proportions (both are 0.0 if Pokemon is ungendered)
        int baseHP, baseATK, baseDEF, baseSPD, baseSATK, baseSDEF; //Base stats
        int evHP, evATK, evDEF, evSPD, evSATK, evSDEF, exp_yield; //Effort value yield
        //std::string * move_list;
};

base_pkmn get_pokemon(std::string,int);
std::vector<base_pkmn> get_pkmn_of_type(std::string,std::string,int,bool);

#endif /*BASE_PKMN_HPP*/
