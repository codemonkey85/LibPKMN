/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include "type_stats_common.hpp"

#include <boost/program_options.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <stdexcept>
#include <vector>

using namespace pkmnsim;
using namespace std;
namespace po = boost::program_options;

int main(int argc, char *argv[])
{
    //Taking in user options
    int gen;
    string type1;
    string type2;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "Display this help message.")
        ("gen", po::value<int>(&gen)->default_value(5), "Which generation to use")
        ("type1", po::value<string>(&type1)->default_value("None"), "Type 1 to search for")
        ("type2", po::value<string>(&type2)->default_value("None"), "Type 2 to search for, default=None")
        ("lax", "If only specifying one type, specify '--lax' to include any type combination with that type")
        ("evolved", "Only check fully evolved Pokémon")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

	bool lax = (vm.count("lax") > 0);
	bool evolved = (vm.count("evolved") > 0);
	
    //Process help or user mistake
    if(vm.count("help") > 0)
    {
        cout << "\nGet Type Stats - " << desc << endl;
        return EXIT_FAILURE;
    }
    if(type1 == "None")
    {
        cout << "\nSpecify a type with type1=(type)." << endl;
        cout << "\nSearch Type Combo - " << desc << endl;
        return EXIT_FAILURE;
    }

    //Preparing stat_st vectors
	vector<stat_st> highest_stats;
	highest_stats.push_back(stat_st("HP"));
	highest_stats.push_back(stat_st("Attack"));
	highest_stats.push_back(stat_st("Defense"));
	highest_stats.push_back(stat_st("Speed"));
	if(gen == 1) highest_stats.push_back(stat_st("Special"));
	else
	{
		highest_stats.push_back(stat_st("Special Attack"));
		highest_stats.push_back(stat_st("Special Defense"));
	}
	vector<stat_st> lowest_stats = highest_stats;
    vector<base_pkmn::sptr> pkmn_vector;

    //Get relevant Pokémon and determine validity of options
    get_pkmn_of_type(pkmn_vector, type1, type2, gen, lax);
    if(pkmn_vector.size() == 0)
    {
        cout << "No Pokémon of specified type combination in Generation " << gen << ".\n";
        return EXIT_FAILURE;
    }
	if(evolved) remove_unevolved_pokemon(pkmn_vector);
	
    for(int i = 0; i < pkmn_vector.size(); i++)
    {
        string pkmn_name = pkmn_vector[i]->get_display_name();
        dict<string, int> stats = pkmn_vector[i]->get_base_stats();

        for(int j = 0; j < highest_stats.size(); j++)
        {
            string stat_name = get_stat_map()[highest_stats[j].stat_name];

            //TODO: account for case of stats being equal by appending pkmn_name strings
            if(highest_stats[j].pkmn_name == "Missingno." or highest_stats[j].stat_value < stats[stat_name])
            {
                highest_stats[j].pkmn_name = pkmn_name;
                highest_stats[j].stat_value = stats[stat_name];
            }
            if(lowest_stats[j].pkmn_name == "Missingno." or lowest_stats[j].stat_value > stats[stat_name])
            {
                lowest_stats[j].pkmn_name = pkmn_name;
                lowest_stats[j].stat_value = stats[stat_name];
            }
        }
    }

    //Format output
    string type_str;
    if(type2 != "None") type_str = type1 + "/" + type2;
    else
    {
        if(lax) type_str = type1 + "/Any";
        else type_str = type1;
    }

    //Output highest/lowest stats
    cout << "\nHighest/lowest stats for type " << type_str << " (Generation " << gen << "):" << endl;

    for(int i = 0; i < highest_stats.size(); i++)
    {
        cout << endl << highest_stats[i].stat_name << endl;
        cout << " * Highest: " << highest_stats[i].pkmn_name << " (" << highest_stats[i].stat_value << ")\n";
        cout << " * Lowest: " << lowest_stats[i].pkmn_name << " (" << lowest_stats[i].stat_value << ")\n";
    }
    cout << endl;

    return EXIT_SUCCESS;
}
