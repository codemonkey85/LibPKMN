#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import python as LibPKMN

#
# This test's LibPKMN's internal functionality for copying shared pointers,
# which comes into place in custom copy constructors and assignment operators.
#
if __name__ == "__main__":

    t_pkmn = LibPKMN.team_pokemon("Darmanitan", "X", 70, "None", "None", "None", "None")

    b_pkmn1 = t_pkmn.get_base_pokemon(True)
    b_pkmn2 = t_pkmn.get_base_pokemon(False)

    b_pkmn1.set_form("Standard")
    b_pkmn2.set_form("Zen")

    assert(t_pkmn.get_pokemon_id() == b_pkmn2.get_pokemon_id())
    assert(t_pkmn.get_pokemon_id() != b_pkmn1.get_pokemon_id())
