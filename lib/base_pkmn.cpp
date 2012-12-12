#include <iostream>
#include <map>
#include <stdexcept>
#include <stdio.h>
#include <sstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <sqlitecpp/SQLiteCPP.h>

using namespace std;
typedef vector<string>::iterator str_iter;

base_pkmn::base_pkmn(string name, int num, string spec, string t1, string t2, string a1, string a2, string a3,
                        double ht, double wt, double cm, double cf,
                        int bhp, int batk, int bdef, int bsatk, int bsdef, int bspd,
                        int ehp, int eatk, int edef, int esatk, int esdef, int espd, int exp)
{
    display_name = name;
    nat_pokedex_num = num;
    species = spec;
    type1 = t1;
    type2 = t2;
    ability1 = a1;
    ability2 = a2;
    ability3 = a3;
    height = ht;
    weight = wt;
    chance_male = cm;
    chance_female = cf;
    baseHP = bhp;
    baseATK = batk;
    baseDEF = bdef;
    baseSATK = bsatk;
    baseSDEF = bsdef;
    baseSPD = bspd;
    evHP = ehp;
    evATK = eatk;
    evDEF = edef;
    evSATK = esatk;
    evSDEF = esdef;
    evSPD = espd;
    exp_yield = exp;
}

//Used when grabbing from databases
base_pkmn::base_pkmn(map<string,string> from_database)
{
    //Already strings
    display_name = from_database["display_name"];
    species = from_database["species"];
    type1 = from_database["type1"];
    type2 = from_database["type2"];
    ability1 = from_database["ability1"];
    ability2 = from_database["ability2"];
    ability3 = from_database["ability3"];

    //Need to be ints
    stringstream sin_num(from_database["pokedex_num"]);
    if(not (sin_num >> nat_pokedex_num)) throw runtime_error("Invalid input.");
    stringstream sin_bhp(from_database["base_hp"]);
    if(not (sin_bhp >> baseHP)) throw runtime_error("Invalid input.");
    stringstream sin_batk(from_database["base_atk"]);
    if(not (sin_batk >> baseATK)) throw runtime_error("Invalid input.");
    stringstream sin_bdef(from_database["base_def"]);
    if(not (sin_bdef >> baseDEF)) throw runtime_error("Invalid input.");
    stringstream sin_bsatk(from_database["base_satk"]);
    if(not (sin_bsatk >> baseSATK)) throw runtime_error("Invalid input.");
    stringstream sin_bsdef(from_database["base_sdef"]);
    if(not (sin_bsdef >> baseSDEF)) throw runtime_error("Invalid input.");
    stringstream sin_bspd(from_database["base_spd"]);
    if(not (sin_bspd >> baseSPD)) throw runtime_error("Invalid input.");
    stringstream sin_ehp(from_database["ev_hp"]);
    if(not (sin_ehp >> evHP)) throw runtime_error("Invalid input.");
    stringstream sin_eatk(from_database["ev_atk"]);
    if(not (sin_eatk >> evATK)) throw runtime_error("Invalid input.");
    stringstream sin_edef(from_database["ev_def"]);
    if(not (sin_edef >> evDEF)) throw runtime_error("Invalid input.");
    stringstream sin_esatk(from_database["ev_satk"]);
    if(not (sin_esatk >> evSATK)) throw runtime_error("Invalid input.");
    stringstream sin_esdef(from_database["ev_sdef"]);
    if(not (sin_esdef >> evSDEF)) throw runtime_error("Invalid input.");
    stringstream sin_espd(from_database["ev_spd"]);
    if(not (sin_espd >> evSPD)) throw runtime_error("Invalid input.");
    stringstream sin_exp(from_database["exp_yield"]);
    if(not (sin_exp >> exp_yield)) throw runtime_error("Invalid input.");

    //Need to be doubles
    stringstream sin_ht(from_database["height"]);
    if(not (sin_ht >> height)) throw runtime_error("Invalid input.");
    stringstream sin_wt(from_database["weight"]);
    if(not (sin_wt >> weight)) throw runtime_error("Invalid input.");
    stringstream sin_cm(from_database["chance_male"]);
    if(not (sin_cm >> chance_male)) throw runtime_error("Invalid input.");
    stringstream sin_cf(from_database["chance_female"]);
    if(not (sin_cf >> chance_female)) throw runtime_error("Invalid input.");
}

void base_pkmn::print()
{
    cout << boost::format("%s (#%d)") % display_name % nat_pokedex_num << endl;
    if(type2 == "None") cout << boost::format("Type: %s") % type1 << endl;
    else cout << boost::format("Type: %s/%s") % type1 % type2 << endl;
    cout << boost::format("Stats: %d,%d,%d,%d,%d,%d") %
                               baseHP % baseATK % baseDEF % baseSPD % baseSATK % baseSDEF << endl;
}

void base_pkmn::print_verbose()
{
    cout << boost::format("%s (#%d)") % display_name % nat_pokedex_num << endl;
    cout << boost::format("%s Pokemon") % species << endl;

    if(type2.empty()) cout << boost::format("Type: %s") % type1 << endl;
    else cout << boost::format("Type: %s/%s") % type1 % type2 << endl;

    if((ability2.empty()) && (ability3.empty())) cout << boost::format("Ability: %s") % ability1 << endl;
    else
    {
        cout << "Potential Abilities:" << endl;
        cout << boost::format(" - %s") % ability1 << endl;
        if(!ability2.empty()) cout << boost::format(" - %s") % ability2 << endl;
        if(!ability3.empty()) cout << boost::format(" - %s") % ability3 << endl;
    }

    cout << boost::format("%d m, %d kg") % height % weight << endl;
    if((chance_male + chance_female) == 0.0) cout << "Genderless" << endl;
    else cout << boost::format("%2.1f%s Male, %2.1f%s Female") % (chance_male*100) % "%" % (chance_female*100) % "%" << endl;

    cout << "Base Stats:" << endl;
    cout << boost::format(" - HP: %d") % baseHP << endl;
    cout << boost::format(" - Attack: %d") % baseATK << endl;
    cout << boost::format(" - Defense: %d") % baseDEF << endl;
    cout << boost::format(" - Speed: %d") % baseSPD << endl;
    cout << boost::format(" - Special Attack: %d") % baseSATK << endl;
    cout << boost::format(" - Special Defense: %d") % baseSDEF << endl;

    cout << "Effort Value Yields:" << endl;
    cout << boost::format(" - HP: %d") % evHP << endl;
    cout << boost::format(" - Attack: %d") % evATK << endl;
    cout << boost::format(" - Defense: %d") % evDEF << endl;
    cout << boost::format(" - Speed: %d") % evSPD << endl;
    cout << boost::format(" - Special Attack: %d") % evSATK << endl;
    cout << boost::format(" - Special Defense: %d") % evSDEF << endl;

    cout << boost::format("Experience Yield: %d") % exp_yield << endl;
}

string base_pkmn::get_display_name() {return display_name;}
int base_pkmn::get_nat_pokedex_num() {return nat_pokedex_num;}
string base_pkmn::get_species() {return species;}

string* base_pkmn::get_types()
{
    string *types = new string[2];
    types[0] = type1;
    types[1] = type2;
    return types;
}

string* base_pkmn::get_abilities()
{
    string *abilities = new string[3];
    abilities[0] = ability1;
    abilities[1] = ability2;
    abilities[2] = ability3;
    return abilities;
}

double base_pkmn::get_height() {return height;}
double base_pkmn::get_weight() {return weight;}
double base_pkmn::get_chance_male() {return chance_male;}
double base_pkmn::get_chance_female() {return chance_female;}

int* base_pkmn::get_base_stats()
{
    int *stats = new int[6];

    stats[0] = baseHP;
    stats[1] = baseATK;
    stats[2] = baseDEF;
    stats[3] = baseSPD;
    stats[4] = baseSATK;
    stats[5] = baseSDEF;

    return stats;
}

int* base_pkmn::get_ev_yields()
{
    int *yields = new int[7];

    yields[0] = evHP;
    yields[1] = evATK;
    yields[2] = evDEF;
    yields[3] = evSPD;
    yields[4] = evSATK;
    yields[5] = evSDEF;
    yields[6] = exp_yield;

    return yields;
}
//get_move_list

base_pkmn get_pokemon(string identifier)
{
    string db_fields[] = {"display_name","pokedex_num","species","type1","type2","ability1",
                          "ability2","ability3","height","weight","chance_male","chance_female",
                          "base_hp","base_atk","base_def","base_satk","base_sdef","base_spd",
                          "ev_hp","ev_atk","ev_def","ev_satk","ev_sdef","ev_spd","exp_yield"};
    map<string,string> from_database;
    SQLite::Database db("@PKMNSIM_PKG_DATA_PATH@/pkmnsim.db");

    transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);
    for(int i = 0; i < 25; i++)
    {
        string query_string = str(boost::format("SELECT %s FROM pokedex WHERE identifier='%s'") % db_fields[i] % identifier);
        string result = db.execAndGet(query_string.c_str(), identifier);
        from_database[db_fields[i]] = result;
    }

    return base_pkmn(from_database);
}

vector<base_pkmn> get_pkmn_of_type(string type1, string type2, bool lax)
{
    vector<base_pkmn> pkmn_vector;
    SQLite::Database db("@PKMNSIM_PKG_DATA_PATH@/pkmnsim.db");
    string query_string;

    if(type2 == "None" and lax) query_string = str(boost::format("SELECT identifier FROM pokedex WHERE type1='%s'") % type1);
    else query_string = str(boost::format("SELECT identifier FROM pokedex WHERE type1='%s' AND type2='%s'") % type1 % type2);

    SQLite::Statement query(db, query_string.c_str());

    while(query.executeStep())
    {
        string identifier = query.getColumn(0);
        pkmn_vector.push_back(get_pokemon(identifier));
    }

    return pkmn_vector;
}
