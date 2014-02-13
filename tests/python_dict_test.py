#!/usr/bin/env python
#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import python as LibPKMN

if __name__ == "__main__":

    #########################################
    # pkmn::dict<std::string, int>
    #########################################
    dict1 = LibPKMN.string_int_dict()
    dict1["abc"] = -1
    dict1["def"] = 5
    dict1["ghi"] = 57
    assert(dict1["abc"] == -1)
    assert(dict1["def"] == 5)
    assert(dict1["ghi"] == 57)
    assert(dict1.at("abc",5) == -1)
    assert(dict1.at("ade",-10) == -10)
    dict1.swap("abc","ghi")
    assert(dict1["abc"] == 57)
    assert(dict1["ghi"] == -1)

    #########################################
    # pkmn::dict<std::string, std::string>
    #########################################
    dict2 = LibPKMN.string_string_dict()
    dict2["abc"] = "def"
    dict2["ghi"] = "jkl"
    dict2["mno"] = "pqr"
    assert(dict2["abc"] == "def")
    assert(dict2["ghi"] == "jkl")
    assert(dict2["mno"] == "pqr")
    assert(dict2.at("abc","stu") == "def")
    assert(dict2.at("stu","vwx") == "vwx")
    dict2.swap("abc","mno")
    assert(dict2["abc"] == "pqr")
    assert(dict2["mno"] == "def")

    #########################################
    # pkmn::dict<std::string, unsigned int>
    #########################################
    dict3 = LibPKMN.string_uint_dict()
    dict1 = LibPKMN.string_int_dict()
    dict1["abc"] = 10
    dict1["def"] = 5
    dict1["ghi"] = 57
    assert(dict1["abc"] == 10)
    assert(dict1["def"] == 5)
    assert(dict1["ghi"] == 57)
    assert(dict1.at("abc",5) == 10)
    assert(dict1.at("ade",-10) == -10)
    dict1.swap("abc","ghi")
    assert(dict1["abc"] == 57)
    assert(dict1["ghi"] == 10)
