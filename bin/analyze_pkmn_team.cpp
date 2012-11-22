#include <fstream>
#include <iostream>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/pkmn_types.hpp>
#include <stdexcept>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    vector<base_pkmn> pkmn_team;
    string type_list[] = {"Normal","Fighting","Flying","Poison","Ground","Rock","Bug","Ghost",
                        "Steel","Fire","Water","Grass","Electric","Psychic","Ice","Dragon","Dark"};

    if(argc > 1)
    {
        ifstream team_file(argv[1]);
        string pkmn_name;
       //TODO: Ask for differentiations when necessary, such as for different Rotom variations

        for(int i = 0; i < 6; i++)
        {
            getline(team_file,pkmn_name);
            pkmn_team.push_back(get_pokemon(pkmn_name));
        }

        team_file.close();
    }
    else throw runtime_error("You must supply a file with one Pokemon name per line (max 6).");
}
