#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import datetime
from optparse import OptionParser
import sqlite3
from unidecode import unidecode

def get_abilities(c):
    c.execute("SELECT ability_id,name FROM ability_names WHERE local_language_id=9 AND ability_id<10000")
    from_db = c.fetchall()
    abilities = [(0,"NONE")]

    for i in range(len(from_db)):
        ability_name = str(from_db[i][1]).replace("-","_").replace(" ","_").upper()
        abilities += [(from_db[i][0], ability_name)]

    return abilities

def get_egg_groups(c):
    c.execute("SELECT * FROM egg_groups")
    from_db = c.fetchall()
    egg_groups = [(0,"NONE")]

    for i in range(len(from_db)):
        egg_group_name = str(from_db[i][1]).replace("-","_").replace(" ","_").upper()
        egg_groups += [(from_db[i][0], egg_group_name)]

    return egg_groups

def get_items(c):
    c.execute("SELECT item_id,name FROM item_names WHERE local_language_id=9")
    from_db = c.fetchall()
    items = [(0,"NONE")]

    for i in range(len(from_db)):
        item_name = str(unidecode(from_db[i][1])).replace("-","_").replace(" ","_").upper()
        items += [(from_db[i][0], item_name)]

    return items

def get_markings():
    markings = [(0,"CIRCLE"),(1,"TRIANGLE"),(2,"SQUARE"),(3,"HEART"),(4,"STAR"),(5,"DIAMOND")]
    return markings

def get_moves(c):
    c.execute("SELECT move_id,name FROM move_names WHERE local_language_id=9")
    from_db = c.fetchall()
    moves = [(0,"NONE")]

    for i in range(len(from_db)):
        move_name = str(from_db[i][1]).replace("-","_").replace(" ","_").upper().replace("SING","__SING")
        moves += [(from_db[i][0], move_name)]

    return moves

def get_move_damage_classes(c):
    c.execute("SELECT * FROM move_damage_classes")
    from_db = c.fetchall()
    move_damage_classes = [(0,"NONE")]

    for i in range(len(from_db)):
        move_damage_class_name = str(from_db[i][1]).upper()
        move_damage_classes += [(from_db[i][0], move_damage_class_name)]

    return move_damage_classes

def get_natures(c):
    c.execute("SELECT nature_id,name FROM nature_names WHERE local_language_id=9")
    from_db = c.fetchall()
    natures = [(0,"NONE")]

    for i in range(len(from_db)):
        nature_name = str(from_db[i][1]).upper()
        natures += [(from_db[i][0], nature_name)]

    return natures

def get_stats(c):
    c.execute("SELECT stat_id,name FROM stat_names WHERE local_language_id=9")
    from_db = c.fetchall()
    stats = [(0,"NONE")]

    for i in range(len(from_db)):
        stat_name = str(from_db[i][1]).replace(" ","_").upper()
        stats += [(from_db[i][0], stat_name)]

    return stats

def get_types(c):
    c.execute("SELECT type_id,name FROM type_names WHERE local_language_id=9")
    from_db = c.fetchall()
    types = [(0,"NONE")]

    for i in range(len(from_db)):
        type_name = str(from_db[i][1]).replace("-","_").replace(" ","_").replace("???","QUESTION_MARK").upper()
        types += [(from_db[i][0], type_name)]

    return types

def get_versions(c):
    c.execute("SELECT version_id,name FROM version_names WHERE local_language_id=9")
    from_db = c.fetchall()
    versions = [(0,"NONE")]

    for i in range(len(from_db)):
        version_name = str(from_db[i][1]).replace("-","_").replace(" ","_").replace("???","QUESTION_MARK").upper()
        versions += [(from_db[i][0], version_name)]

    return versions

def get_version_groups(c):
    c.execute("SELECT id,identifier FROM version_groups")
    from_db = c.fetchall()
    version_groups = [(0,"NONE")]

    for i in range(len(from_db)):
        version_group_name = str(from_db[i][1]).replace("-","_").replace(" ","_").upper()
        version_groups += [(from_db[i][0], version_group_name)]

    return version_groups

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--database-path", type="string", help="LibPKMN database location")
    parser.add_option("--top-level-dir", type="string", help="Top-level LibPKMN source directory")
    (options,args) = parser.parse_args()

    conn = sqlite3.connect(options.database_path)
    c = conn.cursor()

    time = datetime.datetime.now()

    cpp_cs_header = """/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 *
 * This file was last generated: %s
 */""" % time

    python_header = """#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
# This file was generated: %s
#""" % time

    abilities = get_abilities(c)
    egg_groups = get_egg_groups(c)
    items = get_items(c)
    markings = get_markings()
    moves = get_moves(c)
    move_damage_classes = get_move_damage_classes(c)
    natures = get_natures(c)
    stats = get_stats(c)
    types = get_types(c)
    versions = get_versions(c)
    version_groups = get_version_groups(c)
