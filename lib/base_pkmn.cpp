/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <boost/format.hpp>
#include <iostream>
#include <string>
#include "base_pkmn_gen1impl.hpp"
#include "base_pkmn_gen2impl.hpp"
#include "base_pkmn_gen3impl.hpp"
#include <pkmnsim/base_pkmn.hpp>
#include <sqlitecpp/SQLiteCPP.h>
#include <vector>

using namespace std;

namespace pkmnsim
{
    base_pkmn::sptr base_pkmn::make(string identifier, int gen)
    {
        transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);

        if(gen < 1 or gen > 5) throw runtime_error("Gen must be 1-5.");

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

    base_pkmn::sptr get_base_pkmn(string identifier, int gen) //More user-friendly
    {
        return base_pkmn::make(identifier, gen);
    }

    void get_pkmn_of_type(vector<base_pkmn::sptr> &pkmn_vector, string type1, string type2, int gen, bool lax)
    {
        map<int,int> gen_bounds;
        gen_bounds[1] = 151;
        gen_bounds[2] = 251;
        gen_bounds[3] = 386;
        gen_bounds[4] = 493;
        gen_bounds[5] = 649;

        SQLite::Database db("@PKMNSIM_DB@");
        string query_string;
        int max_pokedex_num;

        if(type2 == "None" and lax) query_string = str(boost::format("SELECT identifier FROM pokedex WHERE (type1='%s' OR type2='%s') AND pokedex_num <= %d") %
                                                       type1 % type1 % gen_bounds[gen]);
        else query_string = str(boost::format("SELECT identifier FROM pokedex WHERE ((type1='%s' AND type2='%s') OR (type1='%s' AND type2='%s')) AND pokedex_num <= %d")
                                % type1 % type2 % type2 % type1 % gen_bounds[gen]);

        SQLite::Statement query(db, query_string.c_str());

        while(query.executeStep())
        {
            string identifier = query.getColumn(0);
            pkmn_vector.push_back(base_pkmn::make(identifier,gen));
        }
    }
}
