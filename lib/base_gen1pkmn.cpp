#include <boost/format.hpp>
#include <pkmnsim/base_gen1pkmn.hpp>
#include <map>
#include <sqlitecpp/SQLiteCPP.h>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

base_gen1pkmn::base_gen1pkmn(string name, int num, string spec, string t1, string t2, double ht,
                             double wt, int bhp, int batk, int bdef, int bspcl, int bspd, int exp)
{
    display_name = name;
    nat_pokedex_num = num;
    species = spec;
    type1 = t1;
    type2 = t2;
    height = ht;
    weight = wt;
    baseHP = bhp;
    baseATK = batk;
    baseDEF = bdef;
    baseSPCL = bspcl;
    baseSPD = bspd;
    exp_yield = exp;
}

base_gen1pkmn::base_gen1pkmn(std::map<std::string,std::string> from_database)
{
    //Already strings
    display_name = from_database["display_name"];
    species = from_database["species"];
    type1 = from_database["type1"];
    type2 = from_database["type2"];

    //Need to be ints
    stringstream sin_num(from_database["pokedex_num"]);
    if(not (sin_num >> nat_pokedex_num)) throw runtime_error("Invalid input.");
    stringstream sin_bhp(from_database["base_hp"]);
    if(not (sin_bhp >> baseHP)) throw runtime_error("Invalid input.");
    stringstream sin_batk(from_database["base_atk"]);
    if(not (sin_batk >> baseATK)) throw runtime_error("Invalid input.");
    stringstream sin_bdef(from_database["base_def"]);
    if(not (sin_bdef >> baseDEF)) throw runtime_error("Invalid input.");
    stringstream sin_bspcl(from_database["base_spcl"]);
    if(not (sin_bspcl >> baseSPCL)) throw runtime_error("Invalid input.");
    stringstream sin_exp(from_database["exp_yield"]);
    if(not (sin_exp >> exp_yield)) throw runtime_error("Invalid input.");

    //Need to be doubles
    stringstream sin_ht(from_database["height"]);
    if(not (sin_ht >> height)) throw runtime_error("Invalid input.");
    stringstream sin_wt(from_database["weight"]);
    if(not (sin_wt >> weight)) throw runtime_error("Invalid input.");
}

void base_gen1pkmn::print()
{
    cout << boost::format("%s (#%d)") % display_name % nat_pokedex_num << endl;
    if(type2 == "None") cout << boost::format("Type: %s") % type1 << endl;
    else cout << boost::format("Type: %s/%s") % type1 % type2 << endl;
    cout << boost::format("Stats: %d,%d,%d,%d,%d") %
                               baseHP % baseATK % baseDEF % baseSPD % baseSPCL << endl;
}

void base_gen1pkmn::print_verbose()
{
    cout << boost::format("%s (#%d)") % display_name % nat_pokedex_num << endl;
    cout << boost::format("%s Pokemon") % species << endl;

    if(type2.empty()) cout << boost::format("Type: %s") % type1 << endl;
    else cout << boost::format("Type: %s/%s") % type1 % type2 << endl;

    cout << boost::format("%d m, %d kg") % height % weight << endl;

    cout << "Base Stats:" << endl;
    cout << boost::format(" - HP: %d") % baseHP << endl;
    cout << boost::format(" - Attack: %d") % baseATK << endl;
    cout << boost::format(" - Defense: %d") % baseDEF << endl;
    cout << boost::format(" - Speed: %d") % baseSPD << endl;
    cout << boost::format(" - Special: %d") % baseSPCL << endl;

    cout << boost::format("Experience Yield: %d") % exp_yield << endl;
}

int* base_gen1pkmn::get_base_stats()
{
    int *stats = new int[5];

    stats[0] = baseHP;
    stats[1] = baseATK;
    stats[2] = baseDEF;
    stats[3] = baseSPD;
    stats[4] = baseSPCL;

    return stats;
}

int* base_gen1pkmn::get_ev_yields() {return get_base_stats();}

base_gen1pkmn get_gen1_pokemon(string identifier)
{
    string db_fields[] = {"pokedex_num","display_name","species","type1","type2","height",
                          "weight","base_hp","base_atk","base_def","base_spcl","base_spd",
                          "exp_yield"};
    map<string,string> from_database;
    SQLite::Database db("@PKMNSIM_PKG_DATA_PATH@/pkmnsim.db");

    transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);

    for(int i = 0; i < 13; i++)
    {
        string query_string = str(boost::format("SELECT %s FROM gen1_pokedex WHERE identifier='%s'") % db_fields[i] % identifier);
        string result = db.execAndGet(query_string.c_str(), identifier);
        from_database[db_fields[i]] = result;
    }

    return base_gen1pkmn(from_database);
}

vector<base_gen1pkmn> get_gen1_pkmn_of_type(string type1, string type2, bool lax)
{
    vector<base_gen1pkmn> pkmn_vector;
    SQLite::Database db("@PKMNSIM_PKG_DATA_PATH@/pkmnsim.db");
    string query_string;
    int max_pokedex_num;

    if(type2 == "None" and lax) query_string = str(boost::format("SELECT identifier FROM gen1_pokedex WHERE (type1='%s' OR type2='%s') AND pokedex_num <= 151") %
                                                   type1 % type1);
    else query_string = str(boost::format("SELECT identifier FROM gen1_pokedex WHERE ((type1='%s' AND type2='%s') OR (type1='%s' AND type2='%s')) AND pokedex_num <= 151")
                            % type1 % type2 % type2 % type1);

    SQLite::Statement query(db, query_string.c_str());

    while(query.executeStep())
    {
        string identifier = query.getColumn(0);
        pkmn_vector.push_back(get_gen1_pokemon(identifier));
    }

    return pkmn_vector;
}
