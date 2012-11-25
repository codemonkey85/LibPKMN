#ifndef BASE_PKMN_HPP
#define BASE_PKMN_HPP
#include <iostream>
#include <map>
#include <string>

using namespace std;

class base_pkmn
{
    public:
        base_pkmn() {};
        base_pkmn(string, int, string, string, string, string, string, string,
                  double, double, double, double,
                  int, int, int, int, int, int, int, int, int, int, int, int, int);
        base_pkmn(map<string,string>);
        void print();
        void print_verbose();
        string get_display_name();
        int get_nat_pokedex_num();
        string get_species();
        string* get_types();
        string* get_abilities();
        double get_height();
        double get_weight();
        double get_chance_male();
        double get_chance_female();
        int* get_base_stats();
        int* get_ev_yields();
        //string * get_move_list();
    private:
        string display_name;
        int nat_pokedex_num;
        string species;
        string type1, type2;
        string ability1, ability2, ability3;
        double height; //meters
        double weight; //kilograms
        double chance_male, chance_female; //Gender proportions (both are 0.0 if Pokemon is ungendered)
        int baseHP, baseATK, baseDEF, baseSPD, baseSATK, baseSDEF; //Base stats
        int evHP, evATK, evDEF, evSPD, evSATK, evSDEF, exp_yield; //Effort value yield
        //string * move_list;
};

base_pkmn get_pokemon(string);
#endif /*BASE_PKMN_HPP*/
