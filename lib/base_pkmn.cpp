#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <sqlitecpp/SQLiteCPP.h>

using namespace pkmnsim;
using namespace std;
typedef vector<string>::iterator str_iter;

/*********************************************
 * Base Pokemon - Generation 1 Implementation
 *********************************************/
class base_pkmn_gen1impl: public base_pkmn
{
    public:

        base_pkmn_gen1impl(string identifier)
        {
            SQLite::Database db("@PKMNSIM_DB@"); //Filepath filled by CMake
            string query_string;

            query_string = str(boost::format("SELECT string display_name FROM gen1_pokedex WHERE identifier='%s'") % identifier.c_str());
            string display_name = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT pokedex_num FROM gen1_pokedex WHERE identifier='%s'") % identifier.c_str());
            nat_pokedex_num = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT species FROM gen1_pokedex WHERE identifier='%s'") % identifier.c_str());
            string species = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT type1 FROM gen1_pokedex WHERE identifier='%s'") % identifier.c_str());
            string type1 = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT type2 FROM gen1_pokedex WHERE identifier='%s'") % identifier.c_str());
            string type2 = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT base_hp FROM gen1_pokedex WHERE identifier='%s'") % identifier.c_str());
            baseHP = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT base_atk FROM gen1_pokedex WHERE identifier='%s'") % identifier.c_str());
            baseATK = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT base_def FROM gen1_pokedex WHERE identifier='%s'") % identifier.c_str());
            baseDEF = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT base_spcl FROM gen1_pokedex WHERE identifier='%s'") % identifier.c_str());
            baseSPCL = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT base_spd FROM gen1_pokedex WHERE identifier='%s'") % identifier.c_str());
            baseSPD = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT exp_yield FROM gen1_pokedex WHERE identifier='%s'") % identifier.c_str());
            exp_yield = db.execAndGet(query_string.c_str(), identifier);
        }

        string get_info()
        {
            string types_str;
            if(type2 == "None") types_str = type1;
            else types_str = str(boost::format("%s/%s") % type1 % type2);

            return str(boost::format(
                "%s (#%d)\n"
                "Type: %s\n"
                "Stats: %d, %d, %d, %d, %d"
                ) % display_name % nat_pokedex_num
                  % types_str
                  % baseHP % baseATK % baseDEF % baseSPD % baseSPCL
            );
        }

        string get_info_verbose()
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
                " - Speed: %d\n"
                " - Special: %d\n"
                " - Experience Yield: %d\n"
                ) % display_name % nat_pokedex_num
                  % species
                  % types_str
                  % height % weight
                  % baseHP
                  % baseATK
                  % baseDEF
                  % baseSPD
                  % baseSPCL
                  % exp_yield
            );
        }

        map<string,int> get_base_stats()
        {
            map<string,int> stats;
            stats["HP"] = baseHP;
            stats["ATK"] = baseATK;
            stats["DEF"] = baseDEF;
            stats["SPD"] = baseSPD;
            stats["SPCL"] = baseSPCL;
            return stats;
        }

        map<string,int> get_ev_yields()
        {
            //In Generation 1, EV yields were the same as the corresponding base stat
            return get_base_stats();
        }

    private:
        int baseSPCL;
};

/*********************************************
 * Base Pokemon - Generation 2 Implementation
 *********************************************/
class base_pkmn_gen2impl: public base_pkmn
{
    public:
        base_pkmn_gen2impl(string identifier)
        {
            SQLite::Database db("@PKMNSIM_DB@"); //Filepath filled by CMake
            string query_string;

            query_string = str(boost::format("SELECT string display_name FROM pokedex WHERE identifier='%s'") % identifier.c_str());
            string display_name = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT pokedex_num FROM pokedex WHERE identifier='%s'") % identifier.c_str());
            nat_pokedex_num = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT species FROM pokedex WHERE identifier='%s'") % identifier.c_str());
            string species = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT type1 FROM pokedex WHERE identifier='%s'") % identifier.c_str());
            string type1 = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT type2 FROM pokedex WHERE identifier='%s'") % identifier.c_str());
            string type2 = db.execAndGet(query_string.c_str(), identifier);

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

        string get_info()
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

        string get_info_verbose()
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

        map<string,int> get_base_stats()
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

        map<string,int> get_ev_yields()
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

    private:
        int baseSATK, baseSDEF;
        int evHP, evATK, evDEF, evSATK, evSDEF, evSPD;
        double chance_male, chance_female;
};


/***********************************************
 * Base Pokemon - Generation 3-5 Implementation
 ***********************************************/
class base_pkmn_gen3impl: public base_pkmn
{
    public:
        base_pkmn_gen3impl(string identifier, int gen)
        {
            SQLite::Database db("@PKMNSIM_DB@"); //Filepath filled by CMake
            string query_string;

            query_string = str(boost::format("SELECT string display_name FROM pokedex WHERE identifier='%s'") % identifier.c_str());
            string display_name = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT pokedex_num FROM pokedex WHERE identifier='%s'") % identifier.c_str());
            nat_pokedex_num = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT species FROM pokedex WHERE identifier='%s'") % identifier.c_str());
            string species = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT type1 FROM pokedex WHERE identifier='%s'") % identifier.c_str());
            string type1 = db.execAndGet(query_string.c_str(), identifier);

            query_string = str(boost::format("SELECT type2 FROM pokedex WHERE identifier='%s'") % identifier.c_str());
            string type2 = db.execAndGet(query_string.c_str(), identifier);

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

            query_string = str(boost::format("SELECT ability1 FROM pokedex WHERE identifier='%s'") % identifier.c_str());
            string ability1 = db.execAndGet(query_string.c_str(), identifier); //First ability guaranteed to be available in Gen 3

            query_string = str(boost::format("SELECT ability2 FROM pokedex WHERE identifier='%s' AND first_gen<=%d") % identifier.c_str() % gen);
            string ability2 = db.execAndGet(query_string.c_str(), identifier); //Second ability may or may not be in selected generation

            if(gen == 5)
            {
                query_string = str(boost::format("SELECT ability3 FROM pokedex WHERE identifier='%s'") % identifier.c_str());
                string ability3 = db.execAndGet(query_string.c_str(), identifier); //Third/hidden ability only in Gen 5
            }
            else string ability3 = "None";
        }

        string get_info()
        {
            string types_str;
            if(type2 == "None") types_str = type1;
            else types_str = str(boost::format("%s/%s") % type1 % type2);

            return str(boost::format(
                "%s (#%d)\n"
                "Type: %s\n"
                "Stats: %d, %d, %d, %d, %d, %d"
                ) % display_name.c_str() % nat_pokedex_num
                  % types_str.c_str()
                  % baseHP % baseATK % baseDEF % baseSATK % baseSDEF % baseSPD
            );
        }

        string get_info_verbose()
        {
            string types_str;
            if(type2 == "None") types_str = type1;
            else types_str = str(boost::format("%s/%s") % type1 % type2);

            string abilities_str;
            if(ability2 == "None") abilities_str = ability1; //1 ability
            else if(ability3 == "None") abilities_str = str(boost::format("%s, %s")
                                                            % ability1 % ability2); //2 abilities
            else abilities_str = str(boost::format("%s, %s, %s") 
                                     % ability1 % ability2 % ability3);

            return str(boost::format(
                "%s (#%d)\n"
                "%s Pokemon\n"
                "Type: %s\n"
                "%d m, %d kg\n"
                "Ability: %s\n"
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
                  % abilities_str.c_str()
                  % baseHP
                  % baseATK
                  % baseDEF
                  % baseSATK
                  % baseSDEF
                  % baseSPD
                  % exp_yield
            );
        }


        map<string,int> get_base_stats()
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

        map<string,int> get_ev_yields()
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

    private:
        string ability1, ability2, ability3;
        int baseSATK, baseSDEF;
        int evHP, evATK, evDEF, evSATK, evSDEF, evSPD;
        double chance_male, chance_female;

};

base_pkmn::sptr base_pkmn::make(const string identifier, const int gen)
{
    if(gen <= 1 or gen >= 5) throw runtime_error("Gen must be 1-5.");

    switch(gen)
    {
        case 1:
            return sptr(new base_pkmn_gen1impl(identifier));

        case 2:
            return sptr(new base_pkmn_gen2impl(identifier));

        default:
            return sptr(new base_pkmn_gen3impl(identifier, gen));
    }
}
