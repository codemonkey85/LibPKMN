/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmnsim/lists.hpp>
#include <pkmnsim/analysis/damage.hpp>
#include <pkmnsim/analysis/stats.hpp>
#include <pkmnsim/database/lists.hpp>

#include "pkmnsim_swig_functions.hpp"

using namespace std;

namespace pkmnsim
{
    vector<string> get_game_vec()
    {
        vector<string> game_vec;
        get_game_list(game_vec);
        return game_vec;
    }
    
    vector<string> get_game_group_vec()
    {
        vector<string> game_group_vec;
        get_game_group_list(game_group_vec);
        return game_group_vec;
    }
    
    vector<string> get_item_vec(unsigned int game)
    {
        vector<string> item_vec;
        get_item_list(item_vec, game);
        return item_vec;
    }
    
    vector<string> get_pokemon_vec(unsigned int game)
    {
        vector<string> pokemon_vec;
        get_pokemon_list(pokemon_vec, game);
        return pokemon_vec;
    }
    
    vector<string> get_type_vec(unsigned int gen)
    {
        vector<string> type_vec;
        get_type_list(type_vec, gen);
        return type_vec;
    }
    
    vector<string> get_ability_vec(unsigned int gen)
    {
        vector<string> ability_vec;
        get_type_list(ability_vec, gen);
        return ability_vec;
    }

    vector<string> get_nature_vec()
    {
        vector<string> nature_vec;
        get_game_list(nature_vec);
        return nature_vec;
    }
    
    b_pkmn_vec_t get_pokemon_vec_of_type(string type1, string type2, unsigned int gen, bool lax)
    {
        b_pkmn_vec_t b_pkmn_vec;
        get_pokemon_of_type(b_pkmn_vec, type1, type2, gen, lax);
        return b_pkmn_vec;
    }
    
    vector<unsigned int> get_stat_range_vec(base_pokemon::sptr b_pkmn, unsigned int stat, unsigned int level, unsigned int gen)
    {
        vector<unsigned int> stat_vec;
        analysis::get_stat_range(b_pkmn, stat, level, gen, stat_vec);
        return stat_vec;
    }
    
    vector<unsigned int> get_damage_range_vec(base_pokemon::sptr attacker, base_pokemon::sptr defender,
                              move::sptr attack, unsigned int attacker_level, unsigned int defender_level)
    {
        vector<unsigned int> damage_range_vec;
        analysis::get_damage_range(attacker, defender, attack, attacker_level, defender_level, damage_range_vec);
        return damage_range_vec;
    }
    
    vector<unsigned int> get_damage_range_vec(team_pokemon::sptr attacker, team_pokemon::sptr defender, move::sptr attack)
    {
        vector<unsigned int> damage_range_vec;
        analysis::get_damage_range(attacker, defender, attack, damage_range_vec);
        return damage_range_vec;
    }

    vector<string> get_table_vec(void)
    {
        vector<string> table_list;
        database::get_table_list(table_list);
        return table_list;
    }

    vector<string> get_column_vec(string table_name)
    {
        vector<string> column_list;
        database::get_column_list(column_list, table_name);
        return column_list;
    }
}
