#include <iostream>
#include <string>

class base_pkmn
{
    public:
        base_pkmn(std::string, int, std::string, std::string, std::string, double, double, double, double, int, int, int, int, int, int, int, int, int, int, int, int);
        std::string get_display_name();
        int get_nat_pokedex_num();
        std::string get_species();
        //std::string * get_types();
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
        double height; //meters
        double weight; //kilograms
        double chance_male, chance_female; //Gender proportions (both are 0.0 if Pokemon is ungendered)
        int baseHP, baseATK, baseDEF, baseSPD, baseSATK, baseSDEF; //Base stats
        int evHP, evATK, evDEF, evSPD, evSATK, evSDEF; //Effort value yield
        //std::string * move_list;
};

base_pkmn::base_pkmn(std::string name, int num, std::string spec, std::string t1, std::string t2, double ht, double wt, double cm, double cf,
                        int bhp, int batk, int bdef, int bspd, int bsatk, int bsdef, int ehp, int eatk, int edef, int espd, int esatk, int esdef)
{
    display_name = name;
    nat_pokedex_num = num;
    species = spec;
    type1 = t1;
    type2 = t2;
    height = ht;
    weight = wt;
    chance_male = cm;
    chance_female = cf;
    baseHP = bhp;
    baseATK = batk;
    baseDEF = bdef;
    baseSPD = bspd;
    baseSATK = bsatk;
    baseSDEF = bsdef;
    evHP = ehp;
    evATK = eatk;
    evDEF = edef;
    evSPD = espd;
    evSATK = esatk;
    evSDEF = esdef;
}

std::string base_pkmn::get_display_name() {return display_name;}
int base_pkmn::get_nat_pokedex_num() {return nat_pokedex_num;}
std::string base_pkmn::get_species() {return species;}
//get_types
double base_pkmn::get_height() {return height;}
double base_pkmn::get_weight() {return weight;}
double base_pkmn::get_chance_male() {return chance_male;}
double base_pkmn::get_chance_female() {return chance_female;}
//get_base_stats
//get_ev_yields
//get_move_list

int main()
{
    std::cout << "Yay, successful compile!" << std::endl;
    return 0;
}
