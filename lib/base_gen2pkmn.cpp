#include <iostream>
#include <map>
#include <stdexcept>
#include <stdio.h>
#include <sstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <pkmnsim/base_gen2pkmn.hpp>
#include <sqlitecpp/SQLiteCPP.h>

using namespace std;

base_gen2pkmn::base_gen2pkmn(string name, int num, string spec, string t1, string t2,
                        double ht, double wt, double cm, double cf,
                        int bhp, int batk, int bdef, int bsatk, int bsdef, int bspd,
                        int exp)
{
    display_name = name;
    nat_pokedex_num = num;
    species = spec;
    type1 = t1;
    type2 = t2;
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
    exp_yield = exp;
}

//Used when grabbing from databases
base_gen2pkmn::base_gen2pkmn(map<string,string> from_database)
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
    stringstream sin_bsatk(from_database["base_satk"]);
    if(not (sin_bsatk >> baseSATK)) throw runtime_error("Invalid input.");
    stringstream sin_bsdef(from_database["base_sdef"]);
    if(not (sin_bsdef >> baseSDEF)) throw runtime_error("Invalid input.");
    stringstream sin_bspd(from_database["base_spd"]);
    if(not (sin_bspd >> baseSPD)) throw runtime_error("Invalid input.");
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

void base_gen2pkmn::print_verbose()
{
    cout << boost::format("%s (#%d)") % display_name % nat_pokedex_num << endl;
    cout << boost::format("%s Pokemon") % species << endl;

    if(type2.empty()) cout << boost::format("Type: %s") % type1 << endl;
    else cout << boost::format("Type: %s/%s") % type1 % type2 << endl;

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

    cout << boost::format("Experience Yield: %d") % exp_yield << endl;
}

double base_gen2pkmn::get_height() {return height;}
double base_gen2pkmn::get_weight() {return weight;}
double base_gen2pkmn::get_chance_male() {return chance_male;}
double base_gen2pkmn::get_chance_female() {return chance_female;}

int* base_gen2pkmn::get_base_stats()
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

int* base_gen2pkmn::get_ev_yields() {return get_base_stats();}

base_gen2pkmn get_gen2_pokemon(string identifier)
{
    string db_fields[] = {"display_name","pokedex_num","species","type1","type2","height",
                          "weight","chance_male","chance_female","base_hp","base_atk",
                          "base_def","base_satk","base_sdef","base_spd","exp_yield"};
    map<string,string> from_database;
    SQLite::Database db("@PKMNSIM_PKG_DATA_PATH@/pkmnsim.db");
    string query_string;

    transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);

    for(int i = 0; i < 16; i++)
    {
        query_string = str(boost::format("SELECT %s FROM gen2_pokedex WHERE identifier='%s'") % db_fields[i] % identifier);
        string result = db.execAndGet(query_string.c_str(), identifier);
        from_database[db_fields[i]] = result;
    }

    return base_gen2pkmn(from_database);
}

vector<base_gen2pkmn> get_pkmn_of_type(string type1, string type2, bool lax)
{
    vector<base_gen2pkmn> pkmn_vector;
    SQLite::Database db("@PKMNSIM_PKG_DATA_PATH@/pkmnsim.db");
    string query_string;
    int max_pokedex_num;

    if(type2 == "None" and lax) query_string = str(boost::format("SELECT identifier FROM gen2_pokedex WHERE (type1='%s' OR type2='%s') AND pokedex_num <= 251") %
                                                   type1 % type1);
    else query_string = str(boost::format("SELECT identifier FROM gen2_pokedex WHERE ((type1='%s' AND type2='%s') OR (type1='%s' AND type2='%s')) AND pokedex_num <= 251")
                            % type1 % type2 % type2 % type1);

    SQLite::Statement query(db, query_string.c_str());

    while(query.executeStep())
    {
        string identifier = query.getColumn(0);
        pkmn_vector.push_back(get_gen2_pokemon(identifier));
    }

    return pkmn_vector;
}
