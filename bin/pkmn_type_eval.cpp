#include <iostream>
#include <string>
#include <map>
#include <pkmnsim/globals.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include <boost/format.hpp>

int main(int argc, char *argv[])
{
    std::string type_list[18] = {"Normal","Fire","Fighting","Water","Flying",
                                 "Grass","Poison","Electric","Ground","Psychic",
                                 "Rock","Ice","Bug","Dragon","Ghost",
                                 "Dark","Steel","???"};

    std::string type_to_eval = "Grass";

    std::map<std::string,base_pkmn> pkmn_list;

    for(std::map<std::string,base_pkmn>::iterator iter = nat_pokedex.begin(); iter != nat_pokedex.end(); ++iter)
    {
        if(iter->second.get_types()[0] == type_to_eval || iter->second.get_types()[1] == type_to_eval)
        {
            pkmn_list[iter->first] = iter->second;
        }
    }
    for(std::map<std::string,base_pkmn>::iterator iter = pkmn_list.begin(); iter != pkmn_list.end(); ++iter)
    {
        iter->second.print();
    }

    return 0;
}
