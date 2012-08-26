#include <iostream>
#include <string>

class base_pkmn
{
    public:
        base_pkmn() {};
        base_pkmn(std::string, int, std::string, std::string, std::string, std::string, std::string, std::string,
                  double, double, double, double,
                  int, int, int, int, int, int, int, int, int, int, int, int);
        void print();
        std::string get_display_name();
        int get_nat_pokedex_num();
        std::string get_species();
        //std::string * get_types();
        //std::string * get_abilities();
        double get_height();
        double get_weight();
        double get_chance_male();
        double get_chance_female();
        //int * get_base_stats();
        //int * get_ev_yields();
        //std::string * get_move_list();
    private:
        std::string display_name;
        int nat_pokedex_num;
        std::string species;
        std::string type1, type2;
        std::string ability1, ability2, ability3;
        double height; //meters
        double weight; //kilograms
        double chance_male, chance_female; //Gender proportions (both are 0.0 if Pokemon is ungendered)
        int baseHP, baseATK, baseDEF, baseSPD, baseSATK, baseSDEF; //Base stats
        int evHP, evATK, evDEF, evSPD, evSATK, evSDEF; //Effort value yield
        //std::string * move_list;
};
