#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/pkmn_types.hpp>
#include <stdexcept>
#include <string>
#include <vector>

//TODO: take move types into account
//TODO: confirm "Almost no risk" types, Electric should be classified as effective against Misty

namespace po = boost::program_options;
using namespace pkmnsim;
using namespace std;

typedef map<string, int>::iterator iter;

struct effectiveness_arr {int arr[4];};
/*
 * [0] = No effect
 * [1] = Not very effective
 * [2] = No type mod
 * [3] = Super effective
 */

map<string, effectiveness_arr> earr_map;

void print_help(po::variables_map vm, po::options_description desc)
{
    cout << endl << "Analyze Pokemon Team - " << desc << endl;

    cout << "Point the --team_file option at a file with" << endl
         << "one Pokemon name on each line. Using Lance's" << endl
         << "first team from Fire Red/Leaf Green as an" << endl
         << "example:" << endl << endl

         << "Gyarados" << endl
         << "Dragonair" << endl
         << "Dragonair" << endl
         << "Aerodactyl" << endl
         << "Dragonite" << endl << endl;
}

map<string, int> get_type_overlaps(vector<base_pkmn::sptr> pkmn_team, vector<string> type_list)
{
    map<string, int> nums; //Key = type, val = number of Pokemon with that type
    for(int i = 0; i < type_list.size(); i++) nums[type_list[i]] = 0;

    //Generate vector with number of occurrences of each type
    for(int i = 0; i < pkmn_team.size(); i++)
    {
        string * types = pkmn_team[i]->get_types();
        nums[types[0]]++;
        if(types[1] != "None") nums[types[1]]++;
    }

    //Delete map entries for non-overlapping types
    for(int i = 0; i < type_list.size(); i++)
    {
        if(nums[type_list[i]] < 2) nums.erase(type_list[i]);
    }

    return nums;
}

int main(int argc, char *argv[])
{
    //Taking in and processing user options

    string team_file;
    int gen;

    po::options_description desc("Allowed Options");
    desc.add_options()
        ("help", "Display this help message.")
        ("team_file", po::value<string>(&team_file), "Specify a file with a Pokemon team.")
        ("gen", po::value<int>(&gen)->default_value(5), "Specify a generation (1-5).")
        ("verbose", "Enable verbosity.")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

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

    //Get Pokemon team and output
    vector<base_pkmn::sptr> pkmn_team;
    int count = 0;
    while(getline(team_file_input,pkmn_name))
    {
        if(count > 6) break;
        pkmn_team.push_back(base_pkmn::make(pkmn_name,gen));
        if(verbose) cout << "Successfully added Pokemon: " << pkmn_name << endl;
        count++;
    }
    if(verbose) cout << endl;
    team_file_input.close();

    cout << endl << "Team:" << endl;
    for(int i = 0; i < pkmn_team.size(); i++)
    {
        string * types = pkmn_team[i]->get_types();
        if(types[1] == "None") cout << boost::format(" * %s (%s)\n") % pkmn_team[i]->get_display_name() % types[0];
        else cout << boost::format(" * %s (%s/%s)\n") % pkmn_team[i]->get_display_name() % types[0] % types[1];
    }
    cout << endl;

    //Get type overlaps and output
    vector<string> type_list = get_type_names(gen);
    map<string, int> type_overlaps = get_type_overlaps(pkmn_team, type_list);

    cout << "Type overlaps:" << endl;
    for(iter i = type_overlaps.begin(); i != type_overlaps.end(); ++i)
    {
        cout << boost::format(" * %s (%d)\n") % i->first % i->second;
    }

    //Get type mods for each Pokemon
    map<string, map<string, double> > team_mod_map;
    for(int i = 0; i < pkmn_team.size(); i++) //Iterate over Pokemon team
    {
        string pkmn_name = pkmn_team[i]->get_display_name();

        map<string, double> mod_map;
        for(int j = 0; j < type_list.size(); j++) //Iterate over type list
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

    //Get number of super effective types, normally effective types, and not very effective types, and output all
    map<string, int> super_effective_map;
    map<string, int> normal_effective_map;
    map<string, int> not_very_effective_map;
    map<string, int> useless_map;
    for(int i = 0; i < pkmn_team.size(); i++)
    {
        string pkmn_name = pkmn_team[i]->get_display_name();

        for(int j = 0; j < type_list.size(); j++)
        {
            string type_name = type_list[j];

            if(type_name != "???" and type_name != "None" and type_name != "???") //Don't use nonstandard types
            {
                if(team_mod_map[pkmn_name][type_name] == 0.0) useless_map[type_name]++; //Immune to this type
                else if(team_mod_map[pkmn_name][type_name] < 1.0) not_very_effective_map[type_name]++; //Not very effective
                else if(team_mod_map[pkmn_name][type_name] > 1.0) super_effective_map[type_name]++;
                else normal_effective_map[type_name]++;
            }
        }
    }
    cout << endl << "Super Effective:" << endl;
    for(int i = 0; i < type_list.size(); i++)
    {
        string type_name = type_list[i];

        if(super_effective_map[type_name] > 0) cout << boost::format(" * %s (%d)\n")
                                                           % type_name % super_effective_map[type_name];
    }
    cout << endl << "Not Very Effective:" << endl;
    for(int i = 0; i < type_list.size(); i++)
    {
        string type_name = type_list[i];

        if(not_very_effective_map[type_name] > 0) cout << boost::format(" * %s (%d)\n")
                                                           % type_name % not_very_effective_map[type_name];
    } 
    cout << endl << "Useless:" << endl;
    for(int i = 0; i < type_list.size(); i++)
    {
        string type_name = type_list[i];

        if(useless_map[type_name] > 0) cout << boost::format(" * %s (%d)\n")
                                                           % type_name % useless_map[type_name];
    }
    cout << endl; 
}
