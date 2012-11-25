#include <iostream>
#include <map>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <sqlite3.h>

base_pkmn::base_pkmn(std::string name, int num, std::string spec, std::string t1, std::string t2, std::string a1, std::string a2, std::string a3,
                        double ht, double wt, double cm, double cf,
                        int bhp, int batk, int bdef, int bsatk, int bsdef, int bspd,
                        int ehp, int eatk, int edef, int esatk, int esdef, int espd, int exp)
{
    display_name = name;
    nat_pokedex_num = num;
    species = spec;
    type1 = t1;
    type2 = t2;
    ability1 = a1;
    ability2 = a2;
    ability3 = a3;
    height = ht;
    weight = wt;
    chance_male = cm;
    chance_female = cf;
    baseHP = bhp;
    baseATK = batk;
    baseDEF = bdef;
    baseSATK = bsatk;
    baseSDEF = bsdef;
    baseSPD = bspd;
    evHP = ehp;
    evATK = eatk;
    evDEF = edef;
    evSATK = esatk;
    evSDEF = esdef;
    evSPD = espd;
    exp_yield = exp;
}

void base_pkmn::print()
{
    std::cout << boost::format("%s (#%d)") % display_name % nat_pokedex_num << std::endl;
    if(type2 != "") std::cout << boost::format("Type: %s") % type1 << std::endl;
    else std::cout << boost::format("Type: %s/%s") % type1 % type2 << std::endl;
    std::cout << boost::format("Stats: %d,%d,%d,%d,%d,%d") %
                               baseHP % baseATK % baseDEF % baseSPD % baseSATK % baseSDEF << std::endl;
}

void base_pkmn::print_verbose()
{
    std::cout << boost::format("%s (#%d)") % display_name % nat_pokedex_num << std::endl;
    std::cout << boost::format("%s Pokemon") % species << std::endl;

    if(type2.empty()) std::cout << boost::format("Type: %s") % type1 << std::endl;
    else std::cout << boost::format("Type: %s/%s") % type1 % type2 << std::endl;

    if((ability2.empty()) && (ability3.empty())) std::cout << boost::format("Ability: %s") % ability1 << std::endl;
    else
    {
        std::cout << "Potential Abilities:" << std::endl;
        std::cout << boost::format(" - %s") % ability1 << std::endl;
        if(!ability2.empty()) std::cout << boost::format(" - %s") % ability2 << std::endl;
        if(!ability3.empty()) std::cout << boost::format(" - %s") % ability3 << std::endl;
    }

    std::cout << boost::format("%d m, %d kg") % height % weight << std::endl;
    if((chance_male + chance_female) == 0.0) std::cout << "Genderless" << std::endl;
    else std::cout << boost::format("%2.1f%s Male, %2.1f%s Female") % (chance_male*100) % "%" % (chance_female*100) % "%" << std::endl;

    std::cout << "Base Stats:" << std::endl;
    std::cout << boost::format(" - HP: %d") % baseHP << std::endl;
    std::cout << boost::format(" - Attack: %d") % baseATK << std::endl;
    std::cout << boost::format(" - Defense: %d") % baseDEF << std::endl;
    std::cout << boost::format(" - Speed: %d") % baseSPD << std::endl;
    std::cout << boost::format(" - Special Attack: %d") % baseSATK << std::endl;
    std::cout << boost::format(" - Special Defense: %d") % baseSDEF << std::endl;

    std::cout << "Effort Value Yields:" << std::endl;
    std::cout << boost::format(" - HP: %d") % evHP << std::endl;
    std::cout << boost::format(" - Attack: %d") % evATK << std::endl;
    std::cout << boost::format(" - Defense: %d") % evDEF << std::endl;
    std::cout << boost::format(" - Speed: %d") % evSPD << std::endl;
    std::cout << boost::format(" - Special Attack: %d") % evSATK << std::endl;
    std::cout << boost::format(" - Special Defense: %d") % evSDEF << std::endl;

    std::cout << boost::format("Experience Yield: %d") % exp_yield << std::endl;
}

std::string base_pkmn::get_display_name() {return display_name;}
int base_pkmn::get_nat_pokedex_num() {return nat_pokedex_num;}
std::string base_pkmn::get_species() {return species;}

std::string* base_pkmn::get_types()
{
    std::string *types = new std::string[2];
    types[0] = type1;
    types[1] = type2;
    return types;
}

std::string* base_pkmn::get_abilities()
{
    std::string *abilities = new std::string[3];
    abilities[0] = ability1;
    abilities[1] = ability2;
    abilities[2] = ability3;
    return abilities;
}

double base_pkmn::get_height() {return height;}
double base_pkmn::get_weight() {return weight;}
double base_pkmn::get_chance_male() {return chance_male;}
double base_pkmn::get_chance_female() {return chance_female;}

int* base_pkmn::get_base_stats()
{
    int *stats = new int[6];

    stats[0] = baseHP;
    stats[1] = baseATK;
    stats[2] = baseDEF;
    stats[3] = baseSPD;
    stats[4] = baseSATK;
    stats[5] = baseSDEF;

    return stats;
}

int* base_pkmn::get_ev_yields()
{
    int *yields = new int[7];

    yields[0] = evHP;
    yields[1] = evATK;
    yields[2] = evDEF;
    yields[3] = evSPD;
    yields[4] = evSATK;
    yields[5] = evSDEF;
    yields[6] = exp_yield;

    return yields;
}
//get_move_list

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for(int i = 0; i < argc; i++) printf("%s\n", argv[i] ? argv[i] : "NULL");
    return 0;
}

base_pkmn get_pokemon(std::string pkmn_name)
{
    std::string pkmn_fields[] = {"display_name","pokedex_num","species","type1","type2","ability1",
                               "ability2","ability3","height","weight","chance_male","chance_female",
                               "base_hp","base_atk","base_def","base_satk","base_sdef","base_spd",
                               "ev_hp","ev_atk","ev_def","ev_satk","ev_sdef","ev_spd","exp_yield"};
    sqlite3 *db = 0;
    int rc;
    char *db_err = 0;
    std::string select;


    rc = sqlite3_open_v2("/home/ncorgan/build/pkmnsim/share/pkmnsim/pokedex.db", &db, SQLITE_OPEN_READONLY, NULL);
    if(rc != SQLITE_OK) fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));

    for(int i = 0; i < 25; i++)
    {
        select = str(boost::format("SELECT %s FROM pokedex WHERE display_name='%s'") % pkmn_fields[i] % pkmn_name);
        sqlite3_exec(db, select.c_str(), callback, 0, &db_err);
    }

    sqlite3_close(db);

    return base_pkmn("Turtwig",387,"Tiny Leaf", "Grass","", "Overgrow","Shell Armor","",
                              0.4,10.2, 0.875,0.125, 55,68,64,45,55,31, 0,1,0,0,0,0, 64);
}
