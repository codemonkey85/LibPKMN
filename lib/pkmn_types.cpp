#include <boost/format.hpp>
#include <iostream>
#include <sqlitecpp/SQLiteCPP.h>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

double get_type_damage_mod(string type1, string type2)
{
    double damage_mod;

    if(type1 != "None" and type1 != "???" and type2 != "None" and type2 != "???")
    {
        SQLite::Database db("/home/ncorgan/build/pkmnsim/share/pkmnsim/pkmnsim.db");

        string query_string = str(boost::format("SELECT %s_mod from types WHERE display_name='%s'") % type2 % type1);
        string result = db.execAndGet(query_string.c_str(), type2);

        stringstream sin(result);
        if(not (sin >> damage_mod)) throw runtime_error("Error acquiring damage mod.");

        return damage_mod;
    }
    else return 1.0;

}
