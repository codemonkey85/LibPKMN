/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/assign.hpp>
#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>
#include <map>

#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/lists.hpp>
#include <pkmnsim/pkmn_types.hpp>

#include "team_analysis_common.hpp"


//TODO: take move types into account
//TODO: take counter-effectiveness into account

namespace po = boost::program_options;
using namespace pkmnsim;
using namespace std;

void print_help(po::variables_map vm, po::options_description desc)
{
    cout << endl << "Team Analysis - " << desc << endl;

    cout << "Point the --team_file option at a file with" << endl
         << "one Pokémon name on each line. Using Lance's" << endl
         << "first team from Fire Red/Leaf Green as an" << endl
         << "example:" << endl << endl

         << "Gyarados" << endl
         << "Dragonair" << endl
         << "Dragonair" << endl
         << "Aerodactyl" << endl
         << "Dragonite" << endl << endl;
}

string get_pkmn_effectiveness_string(string pkmn_name, map<string, double> &effectiveness_map)
{
    string output_string = " * " + pkmn_name;
    string good_types_str = "   * Use:";
    string bad_types_str = "   * Don't use:";

    //Iterate over map and add to good/bad_types_str as appropriate
    for(sd_iter i = effectiveness_map.begin(); i != effectiveness_map.end(); ++i)
    {
        if(i->second > 1.0) good_types_str = good_types_str + " " + i->first + ",";
        else if(i->second < 1.0) bad_types_str = bad_types_str + " " + i->first + ",";
    }

    //Cut off final comma from good/bad_types_str
    good_types_str = good_types_str.substr(0, good_types_str.size()-1);
    bad_types_str = bad_types_str.substr(0, bad_types_str.size()-1);

    //Only add good/bad_types_str onto output string if applicable
    if(good_types_str != " * Use") output_string = output_string + "\n" + good_types_str;
    if(bad_types_str != " * Don't use") output_string = output_string + "\n" + bad_types_str;

    return output_string;
}

string get_trends_string(map<string, int> &super_effective_map, map<string, int> &not_very_effective_map)
{
    string output_string = " * Use:";
    for(si_iter i = super_effective_map.begin(); i != super_effective_map.end(); ++i)
    {
        if(i->second > 1) output_string = output_string + " " + i->first + ",";
    }
    output_string = output_string.substr(0, output_string.size()-1);
    output_string = output_string + "\n * Don't use:";
    for(si_iter i = not_very_effective_map.begin(); i != not_very_effective_map.end(); ++i)
    {
        if(i->second > 1) output_string = output_string + " " + i->first + ",";
    }
    output_string = output_string.substr(0, output_string.size()-1);

    return output_string;
}

int main(int argc, char *argv[])
{
    //Taking in and processing user options

    string team_file;
    int gen;

    po::options_description desc("Allowed Options");
    desc.add_options()
        ("help", "Display this help message.")
        ("team_file", po::value<string>(&team_file), "Specify a file with a Pokémon team.")
        ("gen", po::value<int>(&gen)->default_value(5), "Specify a generation (1-5).")
        ("verbose", "Enable verbosity.")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    //Process help or user mistake
    if(vm.count("help") > 0)
    {
        print_help(vm, desc);
        return EXIT_FAILURE;
    }

    if(vm.count("team_file") == 0) throw runtime_error("Specify a team file. Run \"team_analysis --help\" for information.");
    if(gen < 1 or gen > 5) throw runtime_error("Gen must be 1-5.");
    bool verbose = (vm.count("verbose") > 0);

    ifstream team_file_input(team_file.c_str(), ifstream::in);
    if(team_file_input.fail()) throw runtime_error("Specified file doesn't exist.");
    string pkmn_name;

    //Get Pokémon team and output
    vector<base_pkmn::sptr> pkmn_team;
    int count = 0;
    cout << "Analyzing team..." << endl << endl;
    while(getline(team_file_input,pkmn_name))
    {
        //A team can only have six members
        if(count > 6) break;
        pkmn_team.push_back(base_pkmn::make(pkmn_name, gen));
        if(verbose) cout << "Successfully added Pokémon: " << pkmn_name << endl;
        count++;
    }
    if(verbose) cout << endl;
    team_file_input.close();

    cout << "Team:" << endl;
    for(unsigned int i = 0; i < pkmn_team.size(); i++)
    {
        if(i > 5) break;

        string pkmn_name = pkmn_team[i]->get_species_name();
        string type1 = pkmn_team[i]->get_types()[0];
        string type2 = pkmn_team[i]->get_types()[1];
        
        if(type2 == "None") cout << " * " << pkmn_name << " (" << type1 << ")" << endl;
        else cout << " * " << pkmn_name << " (" << type1 << "/" << type2 << ")" << endl;
    }
    cout << endl;

    //Get type overlaps and output
    vector<string> type_list;
    get_type_list(type_list, gen);
    map<string, int> type_overlaps = get_type_overlaps(pkmn_team, type_list);

    cout << "Type overlaps:" << endl;
    for(si_iter i = type_overlaps.begin(); i != type_overlaps.end(); ++i)
    {
        cout << " * " << i->first << " (" << i->second << ")" << endl;
    }

    //Get type mods for each Pokémon and output
    map<string, map<string, double> > team_mod_map;
    for(unsigned int i = 0; i < pkmn_team.size(); i++) //Iterate over Pokémon team
    {
        string pkmn_name = pkmn_team[i]->get_species_name();

        map<string, double> mod_map;
        for(unsigned int j = 0; j < type_list.size(); j++) //Iterate over type list
        {
            string type_name = type_list[j];
            string pkmn_type1 = pkmn_team[i]->get_types()[0];
            string pkmn_type2 = pkmn_team[i]->get_types()[1];
            bool is_gen1 = (gen == 1);

            mod_map[type_name] = get_type_damage_mod(type_name, pkmn_type1, is_gen1);
            if(pkmn_type2 != "None")
            {
                mod_map[type_name] *= get_type_damage_mod(type_name, pkmn_type2, is_gen1);
            }
        }
        team_mod_map[pkmn_name] = mod_map;
    }

    cout << endl << "Individual weaknesses/resistances:" << endl;
    for(unsigned int i = 0; i < pkmn_team.size(); i++)
    {
        string pkmn_name = pkmn_team[i]->get_species_name();
        string output_string = get_pkmn_effectiveness_string(pkmn_name, team_mod_map[pkmn_name]);

        cout << output_string << endl;
    }

    //Get number of super effective types and not very effective types
    map<string, int> super_effective_map;
    map<string, int> not_very_effective_map;
    for(unsigned int i = 0; i < pkmn_team.size(); i++)
    {
        string pkmn_name = pkmn_team[i]->get_species_name();

        for(unsigned int j = 0; j < type_list.size(); j++)
        {
            string type_name = type_list[j];

            if(type_name != "Shadow" and type_name != "None" and type_name != "???") //Don't use nonstandard types
            {
                if(team_mod_map[pkmn_name][type_name] < 1.0) not_very_effective_map[type_name]++; //Not very effective
                else if(team_mod_map[pkmn_name][type_name] > 1.0) super_effective_map[type_name]++;
            }
        }
    }
    trim_effectiveness_maps(super_effective_map, not_very_effective_map);
    string trends_string = get_trends_string(super_effective_map, not_very_effective_map);

    cout << endl << "Trends:" << endl << trends_string << endl << endl;

    return EXIT_SUCCESS;
}
