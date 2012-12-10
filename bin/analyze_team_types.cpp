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
using namespace std;

typedef vector<base_pkmn>::iterator team_iter;
struct effectiveness_arr {int arr[4];};
/*
 * [0] = No effect
 * [1] = Not very effective
 * [2] = No type mod
 * [3] = Super effective
 */

string type_list[17] = {"Normal","Fighting","Flying","Poison","Ground","Rock","Bug","Ghost","Steel",
                        "Fire","Water","Grass","Electric","Psychic","Ice","Dragon","Dark"};
vector<base_pkmn> pkmn_team;
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

vector<string> get_max(string *type_list, map<string, effectiveness_arr> earr_map, int index)
{
    int current_max = 0;
    vector<string> max_types;

    for(int i = 0; i < 17; i++)
    {
        int *earr = earr_map[type_list[i]].arr; 

        if(earr[index] > current_max)
        {
            current_max = earr[index];
            max_types.clear();
            max_types.push_back(str(boost::format("%s (%d)") % type_list[i] % current_max));
        }
        else if(earr[index] == current_max) max_types.push_back(str(boost::format("%s (%d)") % type_list[i] % current_max));
    }
    if(current_max == 0) max_types.clear();
    return max_types;
}

map<string, int> get_type_overlaps(vector<base_pkmn> pkmn_team, string *type_list)
{
    map<string, int> nums; //Key = type, val = number of Pokemon with that type
    for(int i = 0; i < 17; i++) nums[type_list[i]] = 0;

    for(team_iter ti = pkmn_team.begin(); ti != pkmn_team.end(); ++ti)
    {
        string *types = ti->get_types();
        nums[types[0]]++;
        if(types[1] != "") nums[types[1]]++;
    }

    return nums;
}

int main(int argc, char *argv[])
{
    //Taking in and processing user options

    string team_file;

    po::options_description desc("Allowed Options");
    desc.add_options()
        ("help", "Display this help message.")
        ("team_file", po::value<string>(&team_file), "Specify a file with a Pokemon team.")
        ("early_gens", "Use type chart from Gen. 1-2. (unimplemented)")
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

    if(vm.count("team_file") == 0) throw runtime_error("Specify a team file. Run \"analyze_team_types --help\" for information.");
    bool verbose = (vm.count("verbose") > 0);

    ifstream team_file_input(team_file.c_str(), ifstream::in);
    if(team_file_input.fail()) throw runtime_error("Specified file doesn't exist.");
    string pkmn_name;

    //Populate earr_map
    effectiveness_arr temp_earr = {0,0,0,0};
    for(int i = 0; i < 17; i++) earr_map[type_list[i]] = temp_earr;

    if(verbose) cout << boost::format("\nFound team in file \"%s\"\n\n") % team_file;

    //Generate vector of Pokemon team to analyze
    int count = 1;
    while(getline(team_file_input,pkmn_name))
    {
        if(count > 6) break;
        pkmn_team.push_back(get_pokemon(pkmn_name));
        if(verbose) cout << "Successfully added Pokemon: " << pkmn_name << endl;
        count++;
    }
    if(verbose) cout << endl;

    team_file_input.close();

    //Calculations
    for(team_iter ti = pkmn_team.begin(); ti != pkmn_team.end(); ++ti) //For each Pokemon
    {
        effectiveness_arr temp_earr;
        
        for(int i = 0; i < 17; i++) //For each type
        {
            //Determine type damage mod
            double mod = 1.0;
            string *types = ti->get_types();
            mod *= get_type_damage_mod(type_list[i], types[0], false);
            if(types[1] != "") mod *= get_type_damage_mod(type_list[i], types[1], false);

            //Increment appropriate number
            if(mod == 0.0)
            {
                earr_map[type_list[i]].arr[0]++;
                if(verbose) cout << boost::format("%s has no effect on %s\n") % type_list[i] % ti->get_display_name();
            }
            else if(mod > 0.0 and mod < 1.0)
            {
                earr_map[type_list[i]].arr[1]++;
                if(verbose) cout << boost::format("%s is not very effective against %s\n") % type_list[i] % ti->get_display_name();
            }
            else if(mod == 1.0)
            {
                earr_map[type_list[i]].arr[2]++;
                if(verbose) cout << boost::format("%s has normal effect on %s\n") % type_list[i] % ti->get_display_name();
            }
            else
            {
                earr_map[type_list[i]].arr[3]++;
                if(verbose) cout << boost::format("%s is super effective against %s\n") % type_list[i] % ti->get_display_name();
            }
        } 
        if(verbose) cout << endl;
    }

    //Find maxes
    vector<string> no_effect = get_max(type_list, earr_map, 0); //Type(s) with no effect
    vector<string> not_very_effective = get_max(type_list, earr_map, 1); //Type(s) with most "not very effective"
    vector<string> super_effective = get_max(type_list, earr_map, 3); //Type(s) with most "super effective"

    //Print output
    if(verbose)
    {
        cout << "Type (No Effect, Not Very Effective, No Mod, Super Effective)" << endl;
        for(int i = 0; i < 17; i++)
        {
            int *earr = earr_map[type_list[i]].arr;
            if(verbose) cout << boost::format("%s (%d,%d,%d,%d)\n") % type_list[i] % earr[0] % earr[1] % earr[2] % earr[3];
        }
    }
    else
    {
        cout << endl << "Team:" << endl;
        for(team_iter ti = pkmn_team.begin(); ti != pkmn_team.end(); ++ti)
        {
            string pkmn_output;
            pkmn_output = str(boost::format(" * %s") % ti->get_display_name());
            string *types = ti->get_types();
            if(types[1] != "") cout << (boost::format("%s (%s/%s)\n") % pkmn_output % types[0] % types[1]);
            else cout << (boost::format("%s (%s)\n") % pkmn_output % types[0]);
        }
    }

    map<string, int> overlaps = get_type_overlaps(pkmn_team, type_list);
    cout << endl << "Type Overlaps:" << endl;
    for(int i = 0; i < 17; i++)
        if(overlaps[type_list[i]] > 1) cout << (boost::format(" * %s (%d)\n") % type_list[i] % overlaps[type_list[i]]);

    cout << endl << "Biggest risk:" << endl;
    for(vector<string>::iterator vit = super_effective.begin(); vit != super_effective.end(); ++vit)
        cout << boost::format("* %s\n") % *vit;

    cout << endl << "Little risk: " << endl;
    for(vector<string>::iterator vit = not_very_effective.begin(); vit != not_very_effective.end(); ++vit)
        cout << boost::format("* %s\n") % *vit;

    cout << endl << "Almost no risk: " << endl;
    for(vector<string>::iterator vit = no_effect.begin(); vit != no_effect.end(); ++vit)
        cout << boost::format("* %s\n") % *vit;
    cout << endl;

    return EXIT_SUCCESS;
}
