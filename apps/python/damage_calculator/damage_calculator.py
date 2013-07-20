#!/usr/bin/env python
#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
from optparse import OptionParser
from PyPKMNsim import *

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--attacker", type="string", default="", help="Name of attacking Pokemon")
    parser.add_option("--defender", type="string", default="", help="Name of defending Pokemon")
    parser.add_option("--move", type="string", default="", help="Name of the move being used")
    parser.add_option("--attacker-level", type="int", default=50, help="Level of attacking Pokemon")
    parser.add_option("--defender-level", type="int", default=50, help="Level of defending Pokemon")
    parser.add_option("--gen", type="int", default=5, help="Generation for analysis")
    (options,args) = parser.parse_args()

    #Generate relevant data structures
    attacker = base_pkmn(options.attacker, options.gen, False)
    defender = base_pkmn(options.defender, options.gen, False)
    move = base_move(options.move, options.gen)

    #Get defender's HP range for checking possibility of fainting
    defender_min_HP = get_min_possible_stat(defender, "HP", options.defender_level, options.gen)
    defender_max_HP = get_max_possible_stat(defender, "HP", options.defender_level, options.gen)

    damage_range = get_damage_range_from_base_pkmn(attacker, defender, move, options.attacker_level,
                                                   options.defender_level)

    #Getting proper stat strings
    if move.get_move_damage_class() == Move_Classes.PHYSICAL:
        damage_class_string = "Physical"
    elif move.get_move_damage_class() == Move_Classes.SPECIAL:
        damage_class_string = "Special"
    else:
        damage_class_string = "Non-damaging"

    #Get type mod
    type_mod = get_type_damage_mod(move.get_type(), defender.get_types().get(0), (options.gen != 1))
    type_mod *= get_type_damage_mod(move.get_type(), defender.get_types().get(1), (options.gen != 1))

    if move.get_move_damage_class() != Move_Classes.NON_DAMAGING:
        if type_mod == 0.0:
            type_mod_string = "It is not effective..."
        elif type_mod < 1.0:
            type_mod_string = "It's not very effective..."
        elif type_mod > 1.0:
            type_mod_string = "It's super effective!"
        else:
            type_mod_string = ""
    else:
        type_mod_string = ""
    
    #Get defender's max HP
    min_hp = get_min_possible_stat(defender, "HP", options.defender_level, options.gen)
    max_hp = get_max_possible_stat(defender, "HP", options.defender_level, options.gen)

    #Output result
    print "\nAnalyzing attack scenario."
    print " - Generation %d" % options.gen
    print " - Attacker: %s (Level %d)" % (attacker.get_species_name(), options.attacker_level)
    print " - Defender: %s (Level %d)" % (defender.get_species_name(), options.defender_level)
    print " - Move: %s (%s, Base Power %d)" % (move.get_name(), damage_class_string, move.get_base_power())
    if move.get_move_damage_class() != Move_Classes.NON_DAMAGING and type_mod != 1.0:
        print "\n%s" % type_mod_string
    print "\nDamage Range:"
    print " - Minimum: %d" % damage_range[0]
    print " - Maximum: %d" % damage_range[1]
    print "\nDefender HP Range:"
    print " - Minimum: %d" % min_hp
    print " - Maximum: %d" % max_hp
    if max_hp < damage_range[0]:
        print "\nThe defender is guaranteed to faint in this scenario."
    elif min_hp > damage_range[1]:
        min_damage_percent = (float(damage_range[0]) / float(min_hp)) * 100.0
        max_damage_percent = (float(damage_range[1]) / float(min_hp)) * 100.0
        print "\nThe defender is guaranteed to survive this attack.\n"
        print "Best case scenario: %d HP (%2.2f%%) lost." % (damage_range[0], min_damage_percent)
        print "Worst case scenario: %d HP (%2.2f%%) lost." % (damage_range[1], max_damage_percent)
    else:
        min_damage_worst_case_percent = (float(damage_range[0]) / float(min_hp)) * 100.0
        min_damage_best_case_percent = (float(damage_range[0]) / float(max_hp)) * 100.0
        print "\nMinimum damage:"
        if min_damage_best_case_percent < 100.0:
            print " - Best case scenario: the defender loses %2.2f%% of its HP." % min_damage_best_case_percent
        else:
            print " - Worst case secnario: the defender faints."
        if min_damage_worst_case_percent < 100.0:
            print " - Best case scenario: the defender loses %2.2f%% of its HP." % min_damage_worst_case_percent
        else:
            print " - Worst case scenario: the defender faints."

        max_damage_worst_case_percent = (float(damage_range[1]) / float(min_hp)) * 100.0
        max_damage_best_case_percent = (float(damage_range[1]) / float(max_hp)) * 100.0
        print "\nMaximum damage:"
        if max_damage_best_case_percent < 100.0:
            print " - Best case scenario: the defender loses %2.2f%% of its HP." % max_damage_best_case_percent
        else:
            print " - Worst case secnario: the defender faints."
        if max_damage_worst_case_percent < 100.0:
            print " - Best case scenario: the defender loses %2.2f%% of its HP." % max_damage_worst_case_percent
        else:
            print " - Worst case scenario: the defender faints."