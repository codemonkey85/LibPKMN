#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import python as LibPKMN

if __name__ == "__main__":

    #########################################
    # pkmn::array<std::string>
    #########################################
    array1 = LibPKMN.string_array(5)
    array1[3] = "def"
    assert(array1[3] == "def")
    assert(array1.size() == 5)

    #########################################
    # pkmn::array<unsigned int>
    #########################################
    array2 = LibPKMN.uint_array(10)
    array2[4] = 5
    assert(array2[4] == 5)
    assert(array2.size() == 10)

    #########################################
    # pkmn::array<pkmn::base_pokemon::sptr>
    #########################################
    array3 = LibPKMN.base_pokemon_array(2)
    array3[1] = LibPKMN.base_pokemon("Mewtwo","Diamond")
    assert(array3[1].get_name() == "Mewtwo")
    assert(array3.size() == 2)
