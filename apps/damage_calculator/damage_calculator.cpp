/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include <pkmnsim/analysis.hpp>
#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/lists.hpp>
#include <pkmnsim/pkmn_types.hpp>

namespace po = boost::program_options;
using namespace pkmnsim;
using namespace std;

int main(int argc, char *argv[])
{
    //Taking in and processing user options
    string attacker_name, defender_name, move_name;
    int attacker_level, defender_level, gen;

    po::options_description desc("Allowed Options");
    desc.add_options()
        ("help", "Display this help message.")
        ("attacker", po::value<string>(&attacker_name)->default_value(""), "Name of attacking Pokemon.")
        ("defender", po::value<string>(&defender_name)->default_value(""), "Name of defending Pokemon.")
        ("move", po::value<string>(&move_name)->default_value(""), "Name of move")
        ("attacker-level", po::value<int>(&attacker_level)->default_value(50), "Level of attacking Pokemon.")
        ("defender-level", po::value<int>(&defender_level)->default_value(50), "Level of defending Pokemon.")
        ("gen", po::value<int>(&gen)->default_value(5), "Specify a generation (1-5).")
        ("verbose", "Enable verbosity.")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help") or attacker_name == "" or defender_name == "")
    {
        cout << "\nDamage Calculator - " << desc << endl;
        return EXIT_FAILURE;
    }

    if(gen < 1 or gen > 5)
    {
        cerr << "Generation must be 1-5." << endl;
        return EXIT_FAILURE;
    }

    /*
     * base_pokemon now takes a game ID in its constructor instead of a generation, but this
     * application doesn't discriminate between games in the same generation, so this array
     * guarantees that the given generation will use a game in that generation.
     */
    int game_id_from_gen[] = {0,1,4,7,13,17};

    vector<int> damage_range, stat_range;
    string damage_class_string, type_mod_string;
    double type_mod = 1;

    //Generate relevant data structures
    base_pokemon::sptr attacker = base_pokemon::make(attacker_name, game_id_from_gen[gen]);
    base_pokemon::sptr defender = base_pokemon::make(defender_name, game_id_from_gen[gen]);
    base_move::sptr move = base_move::make(move_name, gen);

    //Get defender's HP range for checking possibility of fainting
    get_stat_range(defender, "HP", defender_level, gen, stat_range);
    get_damage_range(attacker, defender, move, attacker_level, defender_level, damage_range);

    //Getting proper stat strings
    if(move->get_move_damage_class() == Move_Classes::PHYSICAL) damage_class_string = "Physical";
    else if(move->get_move_damage_class() == Move_Classes::SPECIAL) damage_class_string = "Special";
    else damage_class_string = "Non-damaging";

    //Get type mod
    type_mod = get_type_damage_mod(move->get_type(), defender->get_types()[0], (gen != 1));
    type_mod *= get_type_damage_mod(move->get_type(), defender->get_types()[1], (gen != 1));

    if(move->get_move_damage_class() != Move_Classes::NON_DAMAGING)
    {
        if(type_mod == 0.0) type_mod_string = "It has no effect.";
        else if(type_mod < 1.0) type_mod_string = "It's not very effective...";
        else if(type_mod > 1.0) type_mod_string = "It's super effective!";
        else type_mod_string = "";
    }
    else type_mod_string = "";

    //Output result
    cout << endl << "Analyzing attack scenario." << endl;
    cout << boost::format(" - Generation %d\n") % gen;
    cout << boost::format(" - Attacker: %s (Level %d)\n") % attacker->get_species_name() % attacker_level;
    cout << boost::format(" - Defender: %s (Level %d)\n") % defender->get_species_name() % defender_level;
    cout << boost::format(" - Move: %s (%s, Base Power %d)\n") % move->get_name() % damage_class_string % move->get_base_power();
    if(move->get_move_damage_class() != Move_Classes::NON_DAMAGING and type_mod != 1.0) cout << endl << type_mod_string << endl;
    cout << endl << "Damage Range:" << endl;
    cout << boost::format(" - Minimum: %d\n") % damage_range[0];
    cout << boost::format(" - Maximum: %d\n") % damage_range[1];
    cout << endl << "Defender HP Range:" << endl;
    cout << boost::format(" - Minimum: %d\n") % stat_range[0];
    cout << boost::format(" - Maximum: %d\n") % stat_range[1];

    if(stat_range[1] < damage_range[0]) cout << endl << "The defender is guaranteed to faint in this scenario." << endl;
    else if(stat_range[0] > damage_range[1])
    {
        double min_damage_percent = (double(damage_range[0]) / double(stat_range[0])) * 100.0;
        double max_damage_percent = (double(damage_range[1]) / double(stat_range[0])) * 100.0;
        cout << endl << "The defender is guaranteed to survive this attack." << endl;
        cout << boost::format("Best case scenario: %d/%d HP (%2.2f%%) lost.\n") % damage_range[0] % stat_range[1] % min_damage_percent;
        cout << boost::format("Worst case scenario: %d/%d HP (%2.2f%%) lost.\n") % damage_range[1] % stat_range[1] % min_damage_percent;
    }
    else
    {
        double min_damage_worst_case_percent = (double(damage_range[0]) / double(stat_range[0])) * 100.0;
        double min_damage_best_case_percent = (double(damage_range[0]) / double(stat_range[1])) * 100.0;
        cout << endl << "Minimum damage:" << endl;
        if(min_damage_best_case_percent < 100.0)
            cout << boost::format(" - Best case scenario: %d/%d HP (%2.2f%%) lost.\n") % damage_range[0] % stat_range[1] % min_damage_best_case_percent;
        else cout << " - Best case scenario: The defender faints." << endl;
        if(min_damage_worst_case_percent < 100.0)
            cout << boost::format(" - Worst case scenario: %d/%d HP (%2.2f%%) lost.\n") % damage_range[0] % stat_range[0] % min_damage_worst_case_percent;
        else cout << " - Worst case scenario: The defender faints." << endl;

        double max_damage_worst_case_percent = (double(damage_range[1]) / double(stat_range[0])) * 100.0;
        double max_damage_best_case_percent = (double(damage_range[1]) / double(stat_range[1])) * 100.0;
        cout << endl << "Maximum damage:" << endl;
        if(max_damage_best_case_percent < 100.0)
            cout << boost::format(" - Best case scenario: %d/%d HP (%2.2f%%) lost.\n") % damage_range[1] % stat_range[1] % max_damage_best_case_percent;
        else cout << " - Best case scenario: The defender faints." << endl;
        if(max_damage_worst_case_percent < 100.0)
            cout << boost::format(" - Worst case scenario: %d/%d HP (%2.2f%%) lost.\n") % damage_range[1] % stat_range[0] % max_damage_worst_case_percent;
        else cout << " - Worst case scenario: The defender faints." << endl;
    }

    return EXIT_SUCCESS;
}
