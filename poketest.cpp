#include <iostream>
#include <pkmnsim/globals.hpp>
#include <string>
#include <map>
#include <pkmnsim/spec_pkmn.hpp>
#include <boost/program_options.hpp>

int main(int argc, char *argv[])
{
    namespace po = boost::program_options;

    std::string use_pkmn;
    int level;
    int force_vals;
    bool verbose = false;

    po::options_description desc("Options");
    desc.add_options()
        ("help", "Print this help message.")
        ("pkmn", po::value<std::string>(&use_pkmn)->default_value("bulbasaur"), "Choose nat_pokedex key.")
        ("level", po::value<int>(&level)->default_value(100), "Choose level.")
        ("force_vals", po::value<int>(&force_vals)->default_value(-1), "-1 for random IV's, 0-31 for forced IV's.")
        ("verbose", "Enables verbosity.")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    //Help and options
    if(vm.count("help") > 0)
    {
        std::cout << desc << std::endl;
        return ~0;
    }
    if(vm.count("verbose") > 0) verbose = true;
    if(force_vals < -1 || force_vals > 31)
    {
        std::cout << "-1 <= force_vals <= 31" << std::endl;
        return ~0;
    }
    if(level < 0 || level > 100)
    {
        std::cout << "0 <= level <= 100" << std::endl;
        return ~0;
    }


    std::map<std::string,base_pkmn>::iterator iter = nat_pokedex.find(use_pkmn);
    if(iter != nat_pokedex.end())
    {
        spec_pkmn pkmn1 = spec_pkmn(nat_pokedex[use_pkmn],level,force_vals);
        std::cout << "Base Pokemon info" << std::endl << std::endl;
        if(verbose) pkmn1.get_base_pkmn().print_verbose();
        else pkmn1.get_base_pkmn().print();
        std::cout << std::endl << "Specific Pokemon info" << std::endl << std::endl;
        if(verbose) pkmn1.print_verbose();
        else pkmn1.print();
    }
    else
    {
        std::cout << "Pokemon not in National Pokedex." << std::endl;
    }

}
