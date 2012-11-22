#include <iostream>
#include <string>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <pkmnsim/pkmn_nature.hpp>

pkmn_nature::pkmn_nature(std::string nm, double atk, double def, double spd, double satk, double sdef)
{
    name = nm;
    ATKmod = atk;
    DEFmod = def;
    SPDmod = spd;
    SATKmod = satk;
    SDEFmod = sdef;
}

std::string pkmn_nature::get_name() {return name;}

double* pkmn_nature::get_mods()
{
    double *mod_list = new double[5];
    mod_list[0] = ATKmod;
    mod_list[1] = DEFmod;
    mod_list[2] = SPDmod;
    mod_list[3] = SATKmod;
    mod_list[4] = SDEFmod;
    return mod_list;
}

void pkmn_nature::print()
{
    std::cout << boost::format( "Nature: %s") % name << std::endl;
    std::cout << "Stat Mods:" << std::endl;
    std::cout << boost::format( " - Attack: %f") % ATKmod << std::endl;
    std::cout << boost::format( " - Defense: %f") % DEFmod << std::endl;
    std::cout << boost::format( " - Speed: %f") % SPDmod << std::endl;
    std::cout << boost::format( " - Special Attack: %f") % SATKmod << std::endl;
    std::cout << boost::format( " - Special Defense: %f") % SDEFmod << std::endl;
}

pkmn_nature get_nature(std::string nature_name)
{
    boost::algorithm::to_lower(nature_name);

    std::map<std::string, pkmn_nature> pkmn_nature_map = boost::assign::map_list_of
        ("hardy", pkmn_nature("Hardy",     1.0,1.0,1.0,1.0,1.0))
        ("lonely", pkmn_nature("Lonely",   1.1,0.9,1.0,1.0,1.0))
        ("brave", pkmn_nature("Brave",     1.1,1.0,0.9,1.0,1.0))
        ("adamant", pkmn_nature("Adamant", 1.1,1.0,1.0,0.9,1.0))
        ("naughty", pkmn_nature("Naughty", 1.1,1.0,1.0,1.0,0.9))
        ("bold", pkmn_nature("Bold",       0.9,1.1,1.0,1.0,1.0))
        ("docile", pkmn_nature("Docile",   1.0,1.0,1.0,1.0,1.0))
        ("relaxed", pkmn_nature("Relaxed", 1.0,1.1,0.9,1.0,1.0))
        ("impish", pkmn_nature("Impish",   1.0,1.1,1.0,0.9,1.0))
        ("lax", pkmn_nature("Lax",         1.0,1.1,1.0,1.0,0.9))
        ("timid", pkmn_nature("Timid",     0.9,1.0,1.1,1.0,1.0))
        ("hasty", pkmn_nature("Hasty",     1.0,0.9,1.1,1.0,1.0))
        ("serious", pkmn_nature("Serious", 1.0,1.0,1.0,1.0,1.0))
        ("jolly", pkmn_nature("Jolly",     1.0,1.0,1.1,0.9,1.0))
        ("naive", pkmn_nature("Naive",     1.0,1.0,1.1,1.0,0.9))
        ("modest", pkmn_nature("Modest",   0.9,1.0,1.0,1.1,1.0))
        ("mild", pkmn_nature("Mild",       1.0,0.9,1.0,1.0,1.1))
        ("quiet", pkmn_nature("Quiet",     1.0,1.0,0.9,1.1,1.0))
        ("bashful", pkmn_nature("Bashful", 1.0,1.0,1.0,1.0,1.0))
        ("rash", pkmn_nature("Rash",       1.0,1.0,1.0,1.1,0.9))
        ("calm", pkmn_nature("Calm",       0.9,1.0,1.0,1.0,1.1))
        ("gentle", pkmn_nature("Gentle",   1.0,0.9,1.0,1.0,1.1))
        ("sassy", pkmn_nature("Sassy",     1.0,1.0,0.9,1.0,1.1))
        ("careful", pkmn_nature("Careful", 1.0,1.0,1.0,0.9,1.1))
        ("quirky", pkmn_nature("Quirky",   1.0,1.0,1.0,1.0,1.0))
    ;

    return pkmn_nature_map[nature_name];
}
