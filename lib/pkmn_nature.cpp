/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <iostream>
#include <string>
#include <map>
#include <boost/format.hpp>
#include <sstream>
#include <stdexcept>
#include <pkmnsim/pkmn_nature.hpp>
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    pkmn_nature::pkmn_nature(string nm, double atk, double def, double satk, double sdef, double spd)
    {
        name = nm;
        ATKmod = atk;
        DEFmod = def;
        SATKmod = satk;
        SDEFmod = sdef;
        SPDmod = spd;
    }

    string pkmn_nature::get_name() {return name;}

    map<string, double> pkmn_nature::get_mods()
    {
        map<string, double> mod_map;
        mod_map["ATK"] = ATKmod;
        mod_map["DEF"] = DEFmod;
        mod_map["SPD"] = SPDmod;
        mod_map["SATK"] = SATKmod;
        mod_map["SDEF"] = SDEFmod;
        return mod_map;
    }

    std::string pkmn_nature::get_info()
    {
        std::string info_string = str(boost::format( "Nature: %s\n") % name.c_str());
        info_string += "Stat Mods:\n";
        info_string += str(boost::format( " - Attack: %f") % ATKmod);
        info_string += str(boost::format( " - Defense: %f") % DEFmod);
        info_string += str(boost::format( " - Speed: %f") % SPDmod);
        info_string += str(boost::format( " - Special Attack: %f") % SATKmod);
        info_string += str(boost::format( " - Special Defense: %f") % SDEFmod);
        
        return info_string;
    }

    pkmn_nature get_nature(string identifier)
    {
        double atk = 1.0;
        double def = 1.0;
        double satk = 1.0;
        double sdef = 1.0;
        double spd = 1.0;

        SQLite::Database db("@PKMNSIM_DB@");

        transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);

        string query_string = str(boost::format("SELECT id FROM natures WHERE identifier=%d")
                                         % identifier);
        int nature_id = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT name FROM nature_names WHERE nature_id=%d")
                                         % nature_id);
        string name = db.execAndGetStr(query_string.c_str(), identifier);

        //Getting positive mod
        query_string = str(boost::format("SELECT increased_stat_id FROM natures WHERE identifier=%d")
                                         % identifier);
        int pos_id = db.execAndGet(query_string.c_str(), identifier);
        switch(pos_id)
        {
            case 2:
                atk += 0.1;
                break;
            case 3:
                def += 0.1;
                break;
            case 4:
                satk += 0.1;
                break;
            case 5:
                sdef += 0.1;
                break;
            case 6:
                spd += 0.1;
                break;
            default:
                break;
        }

        //Getting positive mod
        query_string = str(boost::format("SELECT decreased_stat_id FROM natures WHERE identifier=%d")
                                         % identifier);
        int neg_id = db.execAndGet(query_string.c_str(), identifier);
        switch(neg_id)
        {
            case 2:
                atk -= 0.1;
                break;
            case 3:
                def -= 0.1;
                break;
            case 4:
                satk -= 0.1;
                break;
            case 5:
                sdef -= 0.1;
                break;
            case 6:
                spd -= 0.1;
                break;
            default:
                break;
        }

        return pkmn_nature(name, atk, def, satk, sdef, spd);
    }
}
