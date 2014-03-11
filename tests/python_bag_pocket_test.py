#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#
# This unit test confirms that the SWIG'd shared_ptr returned from
# the SWIG'd bag points to the same memory underneath it all by
# changing something from the PocketSPtr and making sure the change
# can be detected from the BagSPtr.
#

import python as LibPKMN

if __name__ == "__main__":

    item_bag = LibPKMN.bag("Platinum")
    
    # Add items directly to pocket
    medicine_pocket = item_bag.get_pocket("Medicine")
    medicine_pocket.add_item(LibPKMN.Items.POTION, 10) # TODO: fix str <-> LibPKMN.pokemon_text typemap
    
    # Check item amount from both bag and pocket
    assert item_bag.get_pocket("Medicine").get_item_amount(LibPKMN.Items.POTION) == 10
    assert item_bag.get_item_amount(LibPKMN.Items.POTION) == 10