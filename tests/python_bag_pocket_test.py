#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#
# When instantiating a bag, it automatically populates with the appropriate
# pockets. This test confirms that this is done correctly for each version group.
#

import python as LibPKMN

if __name__ == "__main__":

    # Gen 1
    item_bag = LibPKMN.bag("Blue")
    pocket_dict = item_bag.get_pockets()
    assert pocket_dict.has_key("Items")
    assert pocket_dict["Items"].size() == 20
    assert pocket_dict.size() == 1
    
    # Gen 2
    item_bag = LibPKMN.bag("Silver")
    pocket_dict = item_bag.get_pockets()
    assert pocket_dict.has_key("Items")
    assert pocket_dict["Items"].size() == 20
    assert pocket_dict.has_key("Balls")
    assert pocket_dict["Balls"].size() == 12
    assert pocket_dict.has_key("KeyItems")
    assert pocket_dict["KeyItems"].size() == 26
    assert pocket_dict.has_key("TM/HM")
    assert pocket_dict["TM/HM"].size() == 99
    assert pocket_dict.size() == 4
    
    # Ruby/Sapphire
    item_bag = LibPKMN.bag("Sapphire")
    pocket_dict = item_bag.get_pockets()
    assert pocket_dict.has_key("Items")
    assert pocket_dict["Items"].size() == 20
    assert pocket_dict.has_key("Poke Balls")
    assert pocket_dict["Poke Balls"].size() == 16
    assert pocket_dict.has_key("TMs and HMs")
    assert pocket_dict["TMs and HMs"].size() == 64
    assert pocket_dict.has_key("Berries")
    assert pocket_dict["Berries"].size() == 46
    assert pocket_dict.has_key("Key Items")
    assert pocket_dict["Key Items"].size() == 20
    assert pocket_dict.size() == 5
    
    # Emerald
    item_bag = LibPKMN.bag("Emerald")
    pocket_dict = item_bag.get_pockets()
    assert pocket_dict.has_key("Items")
    assert pocket_dict["Items"].size() == 20
    assert pocket_dict.has_key("Poke Balls")
    assert pocket_dict["Poke Balls"].size() == 16
    assert pocket_dict.has_key("TMs and HMs")
    assert pocket_dict["TMs and HMs"].size() == 64
    assert pocket_dict.has_key("Berries")
    assert pocket_dict["Berries"].size() == 46
    assert pocket_dict.has_key("Key Items")
    assert pocket_dict["Key Items"].size() == 30
    assert pocket_dict.size() == 5
    
    # Fire Red/Leaf Green
    item_bag = LibPKMN.bag("Leaf Green")
    pocket_dict = item_bag.get_pockets()
    assert pocket_dict.has_key("Items")
    assert pocket_dict["Items"].size() == 42
    assert pocket_dict.has_key("Key Items")
    assert pocket_dict["Key Items"].size() == 30
    assert pocket_dict.has_key("Poke Balls")
    assert pocket_dict["Poke Balls"].size() == 13
    assert pocket_dict.has_key("TM Case")
    assert pocket_dict["TM Case"].size() == 58
    assert pocket_dict.has_key("Berry Pouch")
    assert pocket_dict["Berry Pouch"].size() == 43
    assert pocket_dict.size() == 5
    
    # Diamond/Pearl/Platinum
    item_bag = LibPKMN.bag("Pearl")
    pocket_dict = item_bag.get_pockets()
    assert pocket_dict.has_key("Items")
    assert pocket_dict["Items"].size() == 165
    assert pocket_dict.has_key("Medicine")
    assert pocket_dict["Medicine"].size() == 40
    assert pocket_dict.has_key("Poke Balls")
    assert pocket_dict["Poke Balls"].size() == 15
    assert pocket_dict.has_key("TMs and HMs")
    assert pocket_dict["TMs and HMs"].size() == 100
    assert pocket_dict.has_key("Berries")
    assert pocket_dict["Berries"].size() == 64
    assert pocket_dict.has_key("Mail")
    assert pocket_dict["Mail"].size() == 12
    assert pocket_dict.has_key("Battle Items")
    assert pocket_dict["Battle Items"].size() == 30
    assert pocket_dict.has_key("Key Items")
    assert pocket_dict["Key Items"].size() == 50
    assert pocket_dict.size() == 8
    
    # Heart Gold/Soul Silver
    item_bag = LibPKMN.bag("Heart Gold")
    pocket_dict = item_bag.get_pockets()
    assert pocket_dict.has_key("Items")
    assert pocket_dict["Items"].size() == 165
    assert pocket_dict.has_key("Medicine")
    assert pocket_dict["Medicine"].size() == 40
    assert pocket_dict.has_key("Poke Balls")
    assert pocket_dict["Poke Balls"].size() == 24
    assert pocket_dict.has_key("TMs and HMs")
    assert pocket_dict["TMs and HMs"].size() == 100
    assert pocket_dict.has_key("Berries")
    assert pocket_dict["Berries"].size() == 64
    assert pocket_dict.has_key("Mail")
    assert pocket_dict["Mail"].size() == 12
    assert pocket_dict.has_key("Battle Items")
    assert pocket_dict["Battle Items"].size() == 30
    assert pocket_dict.has_key("Key Items")
    assert pocket_dict["Key Items"].size() == 43
    assert pocket_dict.size() == 8
    
    # Gen 5/6
    item_bag = LibPKMN.bag("Black 2")
    pocket_dict = item_bag.get_pockets()
    assert pocket_dict.has_key("Items")
    assert pocket_dict["Items"].size() == 310
    assert pocket_dict.has_key("Medicine")
    assert pocket_dict["Medicine"].size() == 48
    assert pocket_dict["TMs and HMs"].size() == 109
    assert pocket_dict.has_key("Berries")
    assert pocket_dict["Berries"].size() == 64
    assert pocket_dict.has_key("Key Items")
    assert pocket_dict["Key Items"].size() == 83
    assert pocket_dict.size() == 5