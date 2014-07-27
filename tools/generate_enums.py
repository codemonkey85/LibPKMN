#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

###############################################################
# This script is called at build-time to generate the database
# enums for the SWIG-wrapped libraries. Values are taken
# directly from include/pkmn/enums.hpp so as to guarantee all
# values match.
###############################################################

import CppHeaderParser
import datetime
from optparse import OptionParser
import os

abilities = []
egg_groups = []
forms = []
genders = []
items = []
markings = []
moves = []
move_damage_classes = []
natures = []
pokeballs = []
ribbons = []
species = []
stats = []
types = []
versions = []
version_groups = []

def get_abilities(e):
    global abilities

    for value in e["values"]:
        abilities += [(value["name"], value["value"])]

def get_egg_groups(e):
    global egg_groups

    for value in e["values"]:
        egg_groups += [(value["name"], value["value"])]

def get_forms(e):
    global forms

    form_index = []
    form_index += [e["namespace"].split("::")[-2]]

    for value in e["values"]:
        form_index += [(value["name"], value["value"])]

    forms += [form_index]

def get_genders():
    global genders

    genders = [("FEMALE",0),("MALE",1),("GENDERLESS",2)]

def get_items(c):
    global items

    for value in e["values"]:
        items += [(value["name"], value["value"])]

def get_markings():
    global markings

    markings = [("CIRCLE",0),("TRIANGLE",1),("SQUARE",2),("HEART",3),("STAR",4),("DIAMOND",5)]

def get_moves(c):
    global moves

    for value in e["values"]:
        moves += [(value["name"], value["value"])]

def get_move_damage_classes(c):
    global move_damage_classes

    for value in e["values"]:
        move_damage_classes += [(value["name"], value["value"])]

def get_natures(c):
    global natures

    for value in e["values"]:
        natures += [(value["name"], value["value"])]

def get_pokeballs():
    global pokeballs

    pokeballs = [("UNKNOWN",0),("POKE_BALL",1),("GREAT_BALL",2),("ULTRA_BALL",3),("MASTER_BALL",4),("SAFARI_BALL",5),
                 ("LEVEL_BALL",6),("LURE_BALL",7),("MOON_BALL",8),("FRIEND_BALL",8),("LOVE_BALL",9),("HEAVY_BALL",10),
                 ("FAST_BALL",11),("SPORT_BALL",12),("PREMIER_BALL",13),("REPEAT_BALL",14),("TIMER_BALL",15),
                 ("NEST_BALL",16),("NET_BALL",17),("DIVE_BALL",18),("LUXURY_BALL",19),("HEAL_BALL",20),
                 ("QUICK_BALL",21),("DUSK_BALL",22),("CHERISH_BALL",23),("PARK_BALL",24),("DREAM_BALL",25)]

def get_ribbons():
    global ribbons

    hoenn_ribbons = ["WORLD","EARTH","NATIONAL","COUNTRY","SKY","LAND","MARINE","EFFORT","ARTIST","VICTORY","WINNING","CHAMPION",
                     "TOUGH_MASTER","TOUGH_HYPER","TOUGH_SUPER","TOUGH",
                     "SMART_MASTER","SMART_HYPER","SMART_SUPER","SMART",
                     "CUTE_MASTER","CUTE_HYPER","CUTE_SUPER","CUTE",
                     "BEAUTY_MASTER","BEAUTY_HYPER","BEAUTY_SUPER","BEAUTY",
                     "COOL_MASTER","COOL_HYPER","COOL_SUPER","COOL"]

    sinnoh_ribbons = ["","","","",
                      "TOUGH_MASTER","TOUGH_HYPER","TOUGH_SUPER","TOUGH",
                      "SMART_MASTER","SMART_HYPER","SMART_SUPER","SMART",
                      "CUTE_MASTER","CUTE_HYPER","CUTE_SUPER","CUTE",
                      "BEAUTY_MASTER","BEAUTY_HYPER","BEAUTY_SUPER","BEAUTY",
                      "COOL_MASTER","COOL_HYPER","COOL_SUPER","COOL",
                      "","","","",
                      "PREMIER","CLASSIC","CARNIVAL","FESTIVAL","BLUE","GREEN","RED","LEGEND","HISTORY","RECORD",
                      "FOOTPRINT","GORGEOUS_ROYAL","ROYAL","GORGEOUS","SMILE","SNOOZE","RELAX","CARELESS",
                      "DOWNCAST","SHOCK","ALERT","WORLD_ABILITY","PAIR_ABILITY","MULTI_ABILITY","DOUBLE_ABILITY",
                      "GREAT_ABILITY","ABILITY","CHAMPION"]


    unova_ribbons = ["","","","",
                     "TOUGH_MASTER","TOUGH_HYPER","TOUGH_SUPER","TOUGH",
                     "SMART_MASTER","SMART_HYPER","SMART_SUPER","SMART",
                     "CUTE_MASTER","CUTE_HYPER","CUTE_SUPER","CUTE",
                     "BEAUTY_MASTER","BEAUTY_HYPER","BEAUTY_SUPER","BEAUTY",
                     "COOL_MASTER","COOL_HYPER","COOL_SUPER","COOL",
                     "","","","",
                     "PREMIER","CLASSIC","WISHING","SOUVENIR","SPECIAL","BIRTHDAY","WORLD_CHAMPION","LEGEND",
                     "EVENT","RECORD","FOOTPRINT","GORGEOUS_ROYAL","ROYAL","GORGEOUS","SMILE","SNOOZE","RELAX","CARELESS",
                     "DOWNCAST","SHOCK","ALERT","WORLD_ABILITY","PAIR_ABILITY","MULTI_ABILITY","DOUBLE_ABILITY",
                     "GREAT_ABILITY","ABILITY","CHAMPION"]

    kalos_ribbons = ["","","","WORLD_CHAMPION","NATIONAL_CHAMPION","REGIONAL_CHAMPION","BATTLE_CHAMPION",
                     "WISHING","FESTIVAL","SPECIAL","BIRTHDAY","EVENT","PREMIER","CLASSIC","WORLD",
                     "EARTH","NATIONAL","COUNTRY","LEGEND","RECORD","FOOTPRINT","ARTIST","GORGEOUS_ROYAL",
                     "ROYAL","GORGEOUS","SMILE","SNOOZE","RELAX","CARELESS","DOWNCAST","SHOCK","ALERT",
                     "EFFORT","","SKILLFUL_BATTLER","TRAINING","BEST_FRIENDS","SINNOH_CHAMPION","CHAMPION",
                     "KALOS_CHAMPION"]

    ribbons = [hoenn_ribbons, sinnoh_ribbons, unova_ribbons, kalos_ribbons]

def get_species(c):
    global species

    for value in e["values"]:
        species += [(value["name"], value["value"])]

def get_stats(c):
    global stats

    for value in e["values"]:
        stats += [(value["name"], value["value"])]

def get_types(c):
    global types

    for value in e["values"]:
        types += [(value["name"], value["value"])]

def get_versions(c):
    global versions

    for value in e["values"]:
        versions += [(value["name"], value["value"])]

def get_version_groups(c):
    global version_groups

    for value in e["values"]:
        version_groups += [(value["name"], value["value"])]

def generate_python_files(output_dir, python_license):
    os.chdir(output_dir)

    f = open("Abilities.py",'w')
    f.write(python_license + "\n\n")

    for ability in abilities:
        f.write("%s = %d\n" % (ability[0],ability[1]))

    f.close()

    f = open("Egg_Groups.py",'w')
    f.write(python_license + "\n\n")

    for egg_group in egg_groups:
        f.write("%s = %d\n" % (egg_group[0],egg_group[1]))

    f.close()

    f = open("Forms.py",'w')
    f.write(python_license + "\n")

    for i in range(len(forms)):
        f.write("\nclass %s:" % forms[i][0])

        for j in range(1,len(forms[i])):
            f.write("\n    %s = %d" % (forms[i][j][0], forms[i][j][1]))

        f.write("\n")

    f.close()

    f = open("Genders.py",'w')
    f.write(python_license + "\n\n")

    for gender in genders:
        f.write("%s = %d\n" % (gender[0],gender[1]))

    f.close()

    f = open("Items.py",'w')
    f.write(python_license + "\n\n")

    for item in items:
        f.write("%s = %d\n" % (item[0],item[1]))

    f.close()

    f = open("Markings.py",'w')
    f.write(python_license + "\n\n")

    for marking in markings:
        f.write("%s = %d\n" % (marking[0],marking[1]))

    f.close()

    f = open("Moves.py",'w')
    f.write(python_license + "\n\n")

    for move in moves:
        f.write("%s = %d\n" % (move[0],move[1]))

    f.close()

    f = open("Move_Classes.py",'w')
    f.write(python_license + "\n\n")

    for move_damage_class in move_damage_classes:
        f.write("%s = %d\n" % (move_damage_class[0],move_damage_class[1]))

    f.close()

    f = open("PokeBalls.py",'w')
    f.write(python_license + "\n\n")

    for pokeball in pokeballs:
        f.write("%s = %d\n" % (pokeball[0],pokeball[1]))

    f.close()

    f = open("Natures.py",'w')
    f.write(python_license + "\n\n")

    for nature in natures:
        f.write("%s = %d\n" % (nature[0],nature[1]))

    f.close()

    f = open("Ribbons.py",'w')
    f.write(python_license + "\n")

    f.write("\nclass Hoenn:")
    for i in range(len(ribbons[0])):
        if ribbons[0][i] != "":
            f.write("\n    %s = %d" % (ribbons[0][i], i))
    f.write("\n")

    f.write("\nclass Sinnoh:")
    for i in range(len(ribbons[1])):
        if ribbons[1][i] != "":
            f.write("\n    %s = %d" % (ribbons[1][i], i))
    f.write("\n")

    f.write("\nclass Unova:")
    for i in range(len(ribbons[2])):
        if ribbons[2][i] != "":
            f.write("\n    %s = %d" % (ribbons[2][i], i))
    f.write("\n")

    f.write("\nclass Kalos:")
    for i in range(len(ribbons[3])):
        if ribbons[3][i] != "":
            f.write("\n    %s = %d" % (ribbons[3][i], i))

    f.close()

    f = open("Species.py",'w')
    f.write(python_license + "\n\n")

    for pokemon in species:
        f.write("%s = %d\n" % (pokemon[0],pokemon[1]))

    f.close()

    f = open("Stats.py",'w')
    f.write(python_license + "\n\n")

    for stat in stats:
        f.write("%s = %d\n" % (stat[0],stat[1]))

    f.close()

    f = open("Types.py",'w')
    f.write(python_license + "\n\n")

    for _type in types:
        f.write("%s = %d\n" % (_type[0],_type[1]))

    f.close()

    f = open("Versions.py",'w')
    f.write(python_license + "\n\n")

    for version in versions:
        f.write("%s = %d\n" % (version[0],version[1]))

    f.close()

    f = open("Version_Groups.py",'w')
    f.write(python_license + "\n\n")

    for version_group in version_groups:
        f.write("%s = %d\n" % (version_group[0],version_group[1]))

    f.close()

def generate_cs_files(output_dir, license):
    os.chdir(output_dir)

    #Non-nested enums can be done in a loop
    enums = dict()
    enums["Abilities"] = abilities
    enums["Egg_Groups"] = egg_groups
    enums["Genders"] = genders
    enums["Items"] = items
    enums["Markings"] = markings
    enums["Moves"] = moves
    enums["Move_Classes"] = move_damage_classes
    enums["Natures"] = natures
    enums["Species"] = species
    enums["Stats"] = stats
    enums["PokeBalls"] = pokeballs
    enums["Types"] = types
    enums["Versions"] = versions
    enums["Version_Groups"] = version_groups

    for key in enums:
        f = open("%s.cs" % key,'w')
        f.write(license + "\n\n")

        f.write("""namespace LibPKMN
{
    public static class %s
    {""" % key)

        for i in range(len(enums[key])):
            f.write("\n        public const uint %s = %d;" % (enums[key][i][0],enums[key][i][1]))

        f.write("""
    }
}""")

        f.close()

    f = open("Forms.cs",'w')
    f.write(license + "\n\n")

    f.write("""namespace LibPKMN
{
    namespace Forms
    {""")

    for i in range(len(forms)):
        f.write("""
        public static class %s
        {""" % forms[i][0])

        for j in range(1,len(forms[i])):
            f.write("""
            public const uint %s = %d;""" % (forms[i][j][0],forms[i][j][1]))

        f.write("""
        }
""")

    f.write("""
    }
}""")

    f.close()

    f = open("Ribbons.cs",'w')
    f.write(license + "\n\n")

    f.write("""namespace LibPKMN
{
    namespace Ribbons
    {
        public static class Hoenn
        {""")

    for i in range(len(ribbons[0])):
        if ribbons[0][i] != "":
            f.write("""
                public const uint %s = %d;""" % (ribbons[0][i],i))

    f.write("""
        }

        public static class Sinnoh
        {""")

    for i in range(len(ribbons[1])):
        if ribbons[1][i] != "":
            f.write("""
                public const uint %s = %d;""" % (ribbons[1][i],i))

    f.write("""
        }

        public static class Unova
        {""")

    for i in range(len(ribbons[2])):
        if ribbons[2][i] != "":
            f.write("""
                public const uint %s = %d;""" % (ribbons[2][i],i))

    f.write("""
        }

        public static class Kalos
        {""")

    for i in range(len(ribbons[3])):
        if ribbons[3][i] != "":
            f.write("""
                public const uint %s = %d;""" % (ribbons[3][i],i))

    f.write("""
        }
    }
}""")

    f.close()

def generate_java_files(output_dir, license):
    os.chdir(output_dir)

    #Non-nested enums can be done in a loop
    enums = dict()
    enums["Abilities"] = abilities
    enums["Egg_Groups"] = egg_groups
    enums["Genders"] = genders
    enums["Items"] = items
    enums["Markings"] = markings
    enums["Moves"] = moves
    enums["Move_Classes"] = move_damage_classes
    enums["Natures"] = natures
    enums["Species"] = species
    enums["Stats"] = stats
    enums["PokeBalls"] = pokeballs
    enums["Types"] = types
    enums["Versions"] = versions
    enums["Version_Groups"] = version_groups

    for key in enums:
        f = open("%s.java" % key,'w')
        f.write(license + "\n\n")

        f.write("package org.nc;\n\n");

        f.write("""public class %s
{""" % key)

        for i in range(len(enums[key])):
            f.write("""
    public static final long %s = %d;""" % (enums[key][i][0],enums[key][i][1]))

        f.write("""
}""")

        f.close()

    for i in range(len(forms)):
        f = open("%s_Forms.java" % forms[i][0],'w')
        f.write(license + "\n\n")

        f.write("package org.nc;\n\n");

        f.write("""public class %s_Forms
{""" % forms[i][0])

        for j in range(1,len(forms[i])):
            f.write("""
    public static final long %s = %d;""" % (forms[i][j][0],forms[i][j][1]))

        f.write("""
}""")

        f.close()

    f = open("Hoenn_Ribbons.java",'w')
    f.write(license + "\n\n")

    f.write("package org.nc;\n\n")

    f.write("""public class Hoenn_Ribbons
{""")

    for i in range(len(ribbons[0])):
        f.write("""
    public static final long %s = %d;""" % (ribbons[0][i],i))

    f.write("""
}""")

    f.close()

    f = open("Sinnoh_Ribbons.java",'w')
    f.write(license + "\n\n")

    f.write("package org.nc;\n\n")

    f.write("""public class Sinnoh_Ribbons
{""")

    for i in range(len(ribbons[1])):
        if ribbons[1][i] != "":
            f.write("""
    public static final long %s = %d;""" % (ribbons[1][i],i))

    f.write("""
}""")

    f.close()

    f = open("Unova_Ribbons.java",'w')
    f.write(license + "\n\n")

    f.write("package org.nc;\n\n")

    f.write("""public class Unova_Ribbons
{""")

    for i in range(len(ribbons[2])):
        if ribbons[2][i] != "":
            f.write("""
    public static final long %s = %d;""" % (ribbons[2][i],i))

    f.write("""
}""")

    f.close()

    f = open("Kalos_Ribbons.java",'w')
    f.write(license + "\n\n")

    f.write("package org.nc;\n\n")

    f.write("""public class Kalos_Ribbons
{""")

    for i in range(len(ribbons[3])):
        if ribbons[3][i] != "":
            f.write("""
    public static final long %s = %d;""" % (ribbons[3][i],i))

    f.write("""
}""")

    f.close()

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--include-path", type="string", help="enums.hpp location")
    parser.add_option("--output-dir", type="string", help="Output directory")
    parser.add_option("--language", type="string", help="cpp, python, cs, or java")
    (options,args) = parser.parse_args()

    time = datetime.datetime.now()

    license = """/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 *
 * This file was generated: %s
 */""" % time

    python_license = """#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
# This file was generated: %s
#""" % time

    header = CppHeaderParser.CppHeader(options.include_path)
    enums = header.__dict__["enums"]
    for e in enums:
        if e["namespace"] == "pkmn::Abilities::":
            get_abilities(e)
        elif e["namespace"] == "pkmn::Egg_Groups::":
            get_egg_groups(e)
        elif "Forms" in e["namespace"]:
            get_forms(e)
        elif e["namespace"] == "pkmn::Items::":
            get_items(e)
        elif e["namespace"] == "pkmn::Moves::":
            get_moves(e)
        elif e["namespace"] == "pkmn::Move_Classes::":
            get_move_damage_classes(e)
        elif e["namespace"] == "pkmn::Natures::":
            get_natures(e)
        elif e["namespace"] == "pkmn::Species::":
            get_species(e)
        elif e["namespace"] == "pkmn::Stats::":
            get_stats(e)
        elif e["namespace"] == "pkmn::Types::":
            get_types(e)
        elif e["namespace"] == "pkmn::Versions::":
            get_versions(e)
        elif e["namespace"] == "pkmn::Version_Groups::":
            get_version_groups(e)

    #These don't need the header file
    get_genders()
    get_markings()
    get_pokeballs()
    get_ribbons()

    if options.language == "python":
        generate_python_files(options.output_dir, python_license)
    elif options.language == "cs":
        generate_cs_files(options.output_dir, license)
    elif options.language == "java":
        generate_java_files(options.output_dir, license)
