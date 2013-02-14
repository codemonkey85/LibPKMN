#include "base_pkmn_gen2impl.hpp"
#include <boost/format.hpp>
#include <sqlitecpp/SQLiteCPP.h>
#include <stdio.h>

using namespace std;

namespace pkmnsim
{
    base_pkmn_gen2impl::base_pkmn_gen2impl(string identifier)
    {
        SQLite::Database db("@PKMNSIM_DB@"); //Filepath filled by CMake
        string query_string;

        query_string = str(boost::format("SELECT display_name FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        display_name = db.execAndGetStr(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT pokedex_num FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        nat_pokedex_num = db.execAndGet(query_string.c_str(), identifier);

        if(nat_pokedex_num > 251)
        {
            string error_message = str(boost::format("%s not present in Generation 2.") % identifier.c_str());
            throw runtime_error(error_message.c_str());
        }

        query_string = str(boost::format("SELECT species FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        species = db.execAndGetStr(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT height FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        height = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT weight FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        weight = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT type1 FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        type1 = db.execAndGetStr(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT type2 FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        type2 = db.execAndGetStr(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT base_hp FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        baseHP = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT base_atk FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        baseATK = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT base_def FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        baseDEF = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT base_satk FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        baseSATK = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT base_sdef FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        baseSDEF = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT base_spd FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        baseSPD = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT exp_yield FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        exp_yield = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT ev_hp FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        evHP = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT ev_atk FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        evATK = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT ev_def FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        evDEF = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT ev_satk FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        evSATK = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT ev_sdef FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        evSDEF = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT ev_spd FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        evSPD = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT chance_male FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        chance_male = atof(db.execAndGet(query_string.c_str(), identifier));

        query_string = str(boost::format("SELECT chance_female FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        chance_female = atof(db.execAndGet(query_string.c_str(), identifier));
    }

    string base_pkmn_gen2impl::get_info()
    {
        string types_str;
        if(type2 == "None") types_str = type1;
        else types_str = str(boost::format("%s/%s") % type1.c_str() % type2.c_str());
        
        return str(boost::format(
            "%s (#%d)\n"
            "Type: %s\n"
            "Stats: %d, %d, %d, %d, %d, %d"
            ) % display_name.c_str() % nat_pokedex_num
              % types_str.c_str()
              % baseHP % baseATK % baseDEF % baseSATK % baseSDEF % baseSPD
        );
    }

    string base_pkmn_gen2impl::get_info_verbose()
    {
        string types_str;
        if(type2 == "None") types_str = type1;
        else types_str = str(boost::format("%s/%s") % type1 % type2);

        return str(boost::format(
            "%s (#%d)\n"
            "%s Pokemon\n"
            "Type: %s\n"
            "%d m, %d kg\n"
            "Base Stats:\n"
            " - HP: %d\n"
            " - Attack: %d\n"
            " - Defense: %d\n"
            " - Special Attack: %d\n"
            " - Special Defense: %d\n"
            " - Speed: %d\n"
            " - Experience Yield: %d\n"
            ) % display_name.c_str() % nat_pokedex_num
              % species.c_str()
              % types_str.c_str()
              % height % weight
              % baseHP
              % baseATK
              % baseDEF
              % baseSATK
              % baseSDEF
              % baseSPD
              % exp_yield
        );
    }

    map<string,int> base_pkmn_gen2impl::get_base_stats()
    {
        map<string,int> stats;
        stats["HP"] = baseHP;
        stats["ATK"] = baseATK;
        stats["DEF"] = baseDEF;
        stats["SATK"] = baseSATK;
        stats["SDEF"] = baseSDEF;
        stats["SPD"] = baseSPD;
        return stats;
    }

    map<string,int> base_pkmn_gen2impl::get_ev_yields()
    {
        map<string,int> stats;
        stats["HP"] = evHP;
        stats["ATK"] = evATK;
        stats["DEF"] = evDEF;
        stats["SATK"] = evSATK;
        stats["SDEF"] = evSDEF;
        stats["SPD"] = evSPD;
        return stats;
    }

}
