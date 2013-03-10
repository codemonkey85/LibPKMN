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
	
	bool base_pkmn::is_fully_evolved()
	{
		map<int,int> gen_bounds;
        gen_bounds[1] = 151;
        gen_bounds[2] = 251;
        gen_bounds[3] = 386;
        gen_bounds[4] = 493;
        gen_bounds[5] = 649;
	
		SQLite::Database db("@PKMNSIM_DB@");
		string query_string;
		
		try
		{
			vector<string> evolution_names;
			vector<int> to_erase;
			query_string = str(boost::format("SELECT pkmn2_identifier FROM evolutions WHERE pkmn1_identifier='%s'") % database_identifier.c_str());
			SQLite::Statement query(db, query_string.c_str());
			while(query.executeStep())
			{
				string identifier = query.getColumn(0); //If fully evolved, this will fail
				evolution_names.push_back(identifier);
			}
			
			//Even if evolutions exist, they may not in specified generation
			for(int i = 0; i < evolution_names.size(); i++)
			{
				query_string = str(boost::format("SELECT pokedex_num FROM pokedex WHERE identifier='%s'") % evolution_names[i].c_str());
				int num = db.execAndGet(query_string.c_str(), evolution_names[i].c_str());
				if(num > gen_bounds[from_gen]) to_erase.push_back(i);
				
				for(vector<int>::iterator iter = to_erase.end(); iter != to_erase.begin(); --i)
				{
					evolution_names.erase(evolution_names.begin() + *iter);
				}
				if(evolution_names.begin() == evolution_names.end()) return false;
			}
			
			return true;
		}
		catch(SQLite::Exception& e) {return false;}
	}
	
	void base_pkmn::get_evolutions(vector<sptr>& evolution_vec)
	{
		if(is_fully_evolved()) {evolution_vec.clear();}
		else
		{
			SQLite::Database db("@PKMNSIM_DB@");
			string query_string;
			
			query_string = str(boost::format("SELECT pkmn2_identifier FROM evolutions WHERE pkmn1_identifier='%s'") % database_identifier.c_str());
			SQLite::Statement query(db, query_string.c_str());
			
			while(query.executeStep())
			{
				string identifier = query.getColumn(0);
				evolution_vec.push_back(make(identifier, from_gen));
			}
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
