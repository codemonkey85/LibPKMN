#!/usr/bin/env python

import LibPKMN

if __name__ == "__main__":

    #########################################
    # pkmn::dict<unsigned int, unsigned int>
    #########################################
    dict1 = LibPKMN.dict_uint_uint()

    # Test SWIG'd __setitem__
    dict1[5] = 76
    dict1[18] = 3

    # Test SWIG'd __getitem__
    print dict1[5]
    print dict1[18]

    #########################################
    # pkmn::dict<std::string, int>
    #########################################
    dict2 = LibPKMN.dict_string_int()

    # Test SWIG'd __setitem__
    dict2["string1"] = -58
    dict2["string2"] = 234

    # Test SWIG'd __getitem__
    print dict2["string1"]
    print dict2["string2"]
