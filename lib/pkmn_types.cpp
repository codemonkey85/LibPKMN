#include <boost/format.hpp>
#include <iostream>
#include <pkmnsim/pkmn_types.hpp>
#include <sqlitecpp/SQLiteCPP.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

double pkmnsim::get_type_damage_mod(string type1, string type2, bool gen1)
{
    if(gen1 and (type1 == "Dark" or type1 == "Steel" or type2 == "Dark" or type2 == "Steel"))
    {
        throw runtime_error("Dark and Steel types invalid for Generation 1.");
    }

    double damage_mod;
    string query_string;

    if(type1 != "None" and type1 != "???" and type2 != "None" and type2 != "???")
    {
        SQLite::Database db("@PKMNSIM_DB@");

        if(gen1) query_string = str(boost::format("SELECT %s_mod from gen1_types WHERE display_name='%s'") % type2 % type1);
        else query_string = str(boost::format("SELECT %s_mod from types WHERE display_name='%s'") % type2 % type1);
        string result = db.execAndGet(query_string.c_str(), type2);

        stringstream sin(result);
        if(not (sin >> damage_mod)) throw runtime_error("Error acquiring damage mod.");

        return damage_mod;
    }
    else return 1.0;
}

vector<string> pkmnsim::get_type_names(int gen)
{
    SQLite::Database db("@PKMNSIM_DB@");
    string table = (gen == 1) ? "gen1_types" : "types";
    string query_string = str(boost::format("SELECT display_name FROM %s") % table);
    SQLite::Statement query(db, query_string.c_str());
    vector<string> types;

    while(query.executeStep())
    {
        string type = query.getColumn(0);
        types.push_back(type);
    }
    return types;
}
