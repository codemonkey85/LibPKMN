#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/pkmn_types.hpp>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

struct effectiveness_arr {int arr[4];};
/*
 * [0] = No effect
 * [1] = Not very effective
 * [2] = No type mod
 * [3] = Super effective
 */
typedef vector<base_pkmn>::iterator team_iter;

string type_list[17] = {"Normal","Fighting","Flying","Poison","Ground","Rock","Bug","Ghost","Steel",
                        "Fire","Water","Grass","Electric","Psychic","Ice","Dragon","Dark"};
vector<base_pkmn> pkmn_team;
map<string,effectiveness_arr> earr_map;

int main(int argc, char *argv[])
{

    if(argc > 1)
    {
        ifstream team_file(argv[1], ifstream::in);
        string pkmn_name;

        //Populate earr_map
        effectiveness_arr temp_earr = {0,0,0,0};
        for(int i = 0; i < 17; i++) earr_map[type_list[i]] = temp_earr;

        //Generate vector of Pokemon team to analyze
        int count = 1;
        while(getline(team_file,pkmn_name))
        {
            if(count > 6) break;
            pkmn_team.push_back(get_pokemon(pkmn_name));
            count++;
        }

        team_file.close();

        //Calculations
        for(team_iter ti = pkmn_team.begin(); ti != pkmn_team.end(); ++ti) //For each Pokemon
        {
            effectiveness_arr temp_earr;

            for(int i = 0; i < 17; i++) //For each type
            {
                //Determine type damage mod
                double mod = 1.0;
                string *types = ti->get_types();
                mod *= get_type_damage_mod(type_list[i], types[0]);
                if(types[1] != "") mod *= get_type_damage_mod(type_list[i], types[1]);

                //Increment appropriate number
                if(mod == 0.0) earr_map[type_list[i]].arr[0]++;
                else if(mod > 0.0 and mod < 1.0) earr_map[type_list[i]].arr[1]++;
                else if(mod == 1.0) earr_map[type_list[i]].arr[2]++;
                else earr_map[type_list[i]].arr[3]++;
            } 
        }

        //Print output
        for(int i = 0; i < 17; i++)
        {
            int *earr = earr_map[type_list[i]].arr;
            cout << boost::format("%s (%d,%d,%d,%d)\n") % type_list[i] % earr[0] % earr[1] % earr[2] % earr[3];
        }
    }
    else throw runtime_error("You must supply a file with one Pokemon name per line (max 6).");
}
