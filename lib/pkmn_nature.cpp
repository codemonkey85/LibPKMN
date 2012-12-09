#include <iostream>
#include <string>
#include <map>
#include <boost/format.hpp>
#include <sstream>
#include <stdexcept>
#include <pkmnsim/pkmn_nature.hpp>
#include <sqlitecpp/SQLiteCPP.h>

using namespace std;

pkmn_nature::pkmn_nature(string nm, double atk, double def, double spd, double satk, double sdef)
{
    name = nm;
    ATKmod = atk;
    DEFmod = def;
    SPDmod = spd;
    SATKmod = satk;
    SDEFmod = sdef;
}

pkmn_nature::pkmn_nature(map<string,string> from_database)
{
    //Already a string
    name = from_database["display_name"];

    //Need to be doubles
    stringstream sin_atk(from_database["atk_mod"]);
    if(not (sin_atk >> ATKmod)) throw runtime_error("Invalid input.");
    stringstream sin_def(from_database["def_mod"]);
    if(not (sin_def >> DEFmod)) throw runtime_error("Invalid input.");
    stringstream sin_spd(from_database["spd_mod"]);
    if(not (sin_spd >> SPDmod)) throw runtime_error("Invalid input.");
    stringstream sin_satk(from_database["satk_mod"]);
    if(not (sin_satk >> SATKmod)) throw runtime_error("Invalid input.");
    stringstream sin_sdef(from_database["sdef_mod"]);
    if(not (sin_sdef >> SDEFmod)) throw runtime_error("Invalid input.");
}

string pkmn_nature::get_name() {return name;}

double* pkmn_nature::get_mods()
{
    double *mod_list = new double[5];
    mod_list[0] = ATKmod;
    mod_list[1] = DEFmod;
    mod_list[2] = SPDmod;
    mod_list[3] = SATKmod;
    mod_list[4] = SDEFmod;
    return mod_list;
}

void pkmn_nature::print()
{
    cout << boost::format( "Nature: %s") % name << endl;
    cout << "Stat Mods:" << endl;
    cout << boost::format( " - Attack: %f") % ATKmod << endl;
    cout << boost::format( " - Defense: %f") % DEFmod << endl;
    cout << boost::format( " - Speed: %f") % SPDmod << endl;
    cout << boost::format( " - Special Attack: %f") % SATKmod << endl;
    cout << boost::format( " - Special Defense: %f") % SDEFmod << endl;
}

pkmn_nature get_nature(string identifier)
{
    string db_fields[] = {"display_name","atk_mod","def_mod","spd_mod","satk_mod","sdef_mod"};
    map<string,string> from_database;
    SQLite::Database db("/home/ncorgan/build/pkmnsim/share/pkmnsim/pkmnsim.db");

    transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);
    for(int i = 0; i < 6; i++)
    {
        string query_string = str(boost::format("SELECT %s FROM natures WHERE display_name='%s'") % db_fields[i] % identifier);
        string result = db.execAndGet(query_string.c_str(), identifier);
        from_database[db_fields[i]] = result;
    }

    return pkmn_nature(from_database);
}
