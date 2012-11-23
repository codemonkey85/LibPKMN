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

int main(int argc, char *argv[])
{
    vector<base_pkmn> pkmn_team;
    map<string, double> base_effectiveness_map = boost::assign::map_list_of
        ("Normal",1.0)
        ("Fighting",1.0)
        ("Flying",1.0)
        ("Poison",1.0)
        ("Ground",1.0)
        ("Rock",1.0)
        ("Bug",1.0)
        ("Ghost",1.0)
        ("Steel",1.0)
        ("Fire",1.0)
        ("Water",1.0)
        ("Grass",1.0)
        ("Electric",1.0)
        ("Psychic",1.0)
        ("Ice",1.0)
        ("Dragon",1.0)
        ("Dark",1.0);
    map<string, int> base_num_each = boost::assign::map_list_of
        ("Normal",0)
        ("Fighting",0)
        ("Flying",0)
        ("Poison",0)
        ("Ground",0)
        ("Rock",0)
        ("Bug",0)
        ("Ghost",0)
        ("Steel",0)
        ("Fire",0)
        ("Water",0)
        ("Grass",0)
        ("Electric",0)
        ("Psychic",0)
        ("Ice",0)
        ("Dragon",0)
        ("Dark",0);
    map<string, map<string, double> > pkmn_type_mods;

    if(argc > 1)
    {
        ifstream team_file(argv[1], ifstream::in);
        string pkmn_name;

        //Generate vector of Pokemon team to analyze
        int count = 1;
        while(getline(team_file,pkmn_name))
        {
            if(count > 6) break;
            pkmn_team.push_back(get_pokemon(pkmn_name));
            count++;
        }

        team_file.close();

        //Generate maps of type effectiveness against each Pokemon
        for(vector<base_pkmn>::iterator vit = pkmn_team.begin(); vit != pkmn_team.end(); ++vit)
        {
            string *types = vit->get_types();
            map<string, double> temp_map = base_effectiveness_map;
            for(map<string, double>::iterator mit = temp_map.begin(); mit != temp_map.end(); ++mit)
            {
                mit->second *= get_type_damage_mod(mit->first, types[0]);
                if(types[1] != "") mit->second *= get_type_damage_mod(mit->first, types[1]);
                pkmn_type_mods[vit->get_display_name()] = temp_map;
            }
        }

        //Consolidate results into numbers of each effectiveness
        map<string, int> num_super_effective = base_num_each;
        map<string, int> num_normal = base_num_each;
        map<string, int> num_not_very_effective = base_num_each;
        map<string, int> num_zero = base_num_each;
        for(map<string, map<string,double> >::iterator mit1 = pkmn_type_mods.begin(); mit1 != pkmn_type_mods.end(); ++mit1)
        {
            for(map<string,double>::iterator mit2 = mit1->second.begin(); mit2 != mit1->second.end(); ++mit2)
            {
                if(mit2->second == 0.0) num_zero[mit2->first]++;
                else if(mit2->second > 0.0 && mit2->second < 1.0) num_not_very_effective[mit2->first]++;
                else if(mit2->second == 1.0) num_normal[mit2->first]++;
                else num_super_effective[mit2->first]++;
            }
        }

        //Print results
        cout << "Super effective:" << endl;
        for(map<string, int>::iterator mit = num_super_effective.begin(); mit != num_super_effective.end(); ++mit)
        {
            if(mit->second > 1) cout << boost::format(" * %s (%d)\n") % mit->first % mit->second;
        }
        cout << "Normal effectiveness:" << endl;
        for(map<string, int>::iterator mit = num_normal.begin(); mit != num_normal.end(); ++mit)
        {
            if(mit->second > 1) cout << boost::format(" * %s (%d)\n") % mit->first % mit->second;
        }
        cout << "Not very effective:" << endl;
        for(map<string, int>::iterator mit = num_not_very_effective.begin(); mit != num_not_very_effective.end(); ++mit)
        {
            if(mit->second > 1) cout << boost::format(" * %s (%d)\n") % mit->first % mit->second;
        }
        cout << "Has no effect: " << endl;
        for(map<string, int>::iterator mit = num_zero.begin(); mit != num_zero.end(); ++mit)
        {
            if(mit->second > 1) cout << boost::format(" * %s (%d)\n") % mit->first % mit->second;
        }
    }
    else throw runtime_error("You must supply a file with one Pokemon name per line (max 6).");
}
