#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import python as LibPKMN

if __name__ == "__main__":

    test_text1 = LibPKMN.pokemon_text("foo")
    print test_text1
    print len(test_text1)
