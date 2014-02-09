/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
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

#include <pkmn/base_pokemon.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/lists.hpp>
#include <pkmn/analysis/damage.hpp>
#include <pkmn/analysis/stats.hpp>
#include <pkmn/database/queries.hpp>

namespace po = boost::program_options;
using namespace pkmn;
using namespace pkmn::analysis;

int main(int argc, char *argv[])
{
    //Taking in and processing user options
    std::string attacker_name, defender_name, move_name;
    unsigned int attacker_level, defender_level, gen;

    po::options_description desc("Allowed Options");
    desc.add_options()
        ("help", "Display this help message.")
        ("attacker", po::value<std::string>(&attacker_name)->default_value(""), "Name of attacking Pokemon.")
        ("defender", po::value<std::string>(&defender_name)->default_value(""), "Name of defending Pokemon.")
        ("move", po::value<std::string>(&move_name)->default_value(""), "Name of move")
        ("attacker-level", po::value<unsigned int>(&attacker_level)->default_value(50), "Level of attacking Pokemon.")
        ("defender-level", po::value<unsigned int>(&defender_level)->default_value(50), "Level of defending Pokemon.")
        ("gen", po::value<unsigned int>(&gen)->default_value(6), "Specify a generation (1-6).")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help") or attacker_name == "" or defender_name == "")
    {
        std::cout << "\nDamage Calculator - " << desc << std::endl;
        return EXIT_FAILURE;
    }

    if(gen < 1 or gen > 6)
    {
        std::cerr << "Generation must be 1-6." << std::endl;
        return EXIT_FAILURE;
    }

    /*
     * base_pokemon takes a game ID in its constructor instead of a generation, but this
     * application doesn't discriminate between games in the same generation, so this array
     * guarantees that the given generation will use a game in that generation.
     */
    int game_id_from_gen[] = {0,1,4,7,13,17,23};

    std::string damage_class_string, type_mod_string;
    double type_mod = 1;

    //Generate relevant data structures
    base_pokemon::sptr attacker = base_pokemon::make(database::get_species_id(attacker_name), game_id_from_gen[gen]);
    base_pokemon::sptr defender = base_pokemon::make(database::get_species_id(defender_name), game_id_from_gen[gen]);
    move::sptr attack = move::make(database::get_move_id(move_name), gen);

    //Get defender's HP range for checking possibility of fainting
    std::pair<unsigned int, unsigned int> stat_range = get_stat_range(defender, "HP", defender_level, gen);
    std::pair<unsigned int, unsigned int> damage_range = get_damage_range(attacker, defender,
                                                         attack, attacker_level, defender_level);

    //Getting proper stat std::strings
    damage_class_string = attack->get_move_damage_class();

    //Get type mod
    type_mod = get_type_damage_mod(attack->get_type(), defender->get_types().first, gen);
    type_mod *= get_type_damage_mod(attack->get_type(), defender->get_types().second, gen);

    if(attack->get_move_damage_class_id() != Move_Classes::NON_DAMAGING)
    {
        if(type_mod == 0.0) type_mod_string = "It has no effect.";
        else if(type_mod < 1.0) type_mod_string = "It's not very effective...";
        else if(type_mod > 1.0) type_mod_string = "It's super effective!";
        else type_mod_string = "";
    }
    else type_mod_string = "";

    //Output result
    std::cout << std::endl << "Analyzing attack scenario." << std::endl;
    std::cout << boost::format(" - Generation %d\n") % gen;
    std::cout << boost::format(" - Attacker: %s (Level %d)\n") % attacker->get_species() % attacker_level;
    std::cout << boost::format(" - Defender: %s (Level %d)\n") % defender->get_species() % defender_level;
    std::cout << boost::format(" - Move: %s (%s, Base Power %d)\n") % attack->get_name() % damage_class_string % attack->get_base_power();
    if(attack->get_move_damage_class_id() != Move_Classes::NON_DAMAGING and type_mod != 1.0) std::cout << std::endl << type_mod_string << std::endl;
    std::cout << std::endl << "Damage Range:" << std::endl;
    std::cout << boost::format(" - Minimum: %d\n") % damage_range.first;
    std::cout << boost::format(" - Maximum: %d\n") % damage_range.second;
    std::cout << std::endl << "Defender HP Range:" << std::endl;
    std::cout << boost::format(" - Minimum: %d\n") % stat_range.first;
    std::cout << boost::format(" - Maximum: %d\n") % stat_range.second;

    if(stat_range.second < damage_range.first) std::cout << std::endl << "The defender is guaranteed to faint in this scenario." << std::endl;
    else if(stat_range.first > damage_range.second)
    {
        double min_damage_percent = (double(damage_range.first) / double(stat_range.first)) * 100.0;
        double max_damage_percent = (double(damage_range.second) / double(stat_range.first)) * 100.0;
        std::cout << std::endl << "The defender is guaranteed to survive this attack." << std::endl;
        std::cout << boost::format("Best case scenario: %d/%d HP (%2.2f%%) lost.\n") % damage_range.first % stat_range.second % min_damage_percent;
        std::cout << boost::format("Worst case scenario: %d/%d HP (%2.2f%%) lost.\n") % damage_range.second % stat_range.second % min_damage_percent;
    }
    else
    {
        double min_damage_worst_case_percent = (double(damage_range.first) / double(stat_range.first)) * 100.0;
        double min_damage_best_case_percent = (double(damage_range.first) / double(stat_range.second)) * 100.0;
        std::cout << std::endl << "Minimum damage:" << std::endl;
        if(min_damage_best_case_percent < 100.0)
            std::cout << boost::format(" - Best case scenario: %d/%d HP (%2.2f%%) lost.\n") % damage_range.first % stat_range.second % min_damage_best_case_percent;
        else std::cout << " - Best case scenario: The defender faints." << std::endl;
        if(min_damage_worst_case_percent < 100.0)
            std::cout << boost::format(" - Worst case scenario: %d/%d HP (%2.2f%%) lost.\n") % damage_range.first % stat_range.first % min_damage_worst_case_percent;
        else std::cout << " - Worst case scenario: The defender faints." << std::endl;

        double max_damage_worst_case_percent = (double(damage_range.second) / double(stat_range.first)) * 100.0;
        double max_damage_best_case_percent = (double(damage_range.second) / double(stat_range.second)) * 100.0;
        std::cout << std::endl << "Maximum damage:" << std::endl;
        if(max_damage_best_case_percent < 100.0)
            std::cout << boost::format(" - Best case scenario: %d/%d HP (%2.2f%%) lost.\n") % damage_range.second % stat_range.second % max_damage_best_case_percent;
        else std::cout << " - Best case scenario: The defender faints." << std::endl;
        if(max_damage_worst_case_percent < 100.0)
            std::cout << boost::format(" - Worst case scenario: %d/%d HP (%2.2f%%) lost.\n") % damage_range.second % stat_range.first % max_damage_worst_case_percent;
        else std::cout << " - Worst case scenario: The defender faints." << std::endl;
    }

    return EXIT_SUCCESS;
}
