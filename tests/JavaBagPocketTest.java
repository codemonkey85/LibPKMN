/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

import org.nc.*;

/*
 * This unit test confirms that the SWIG'd shared_ptr returned from
 * the SWIG'd bag points to the same memory underneath it all by
 * changing something from the PocketSPtr and making sure the change
 * can be detected from the BagSPtr.
 */
public class JavaBagPocketTest
{
    public static void main(String[] args)
    {
        BagSPtr itemBag = Bag.make("Platinum");

        //Add items directly to pocket
        PocketSPtr medicinePocket = itemBag.getPocket("Medicine");
        medicinePocket.addItem("Potion", 10);

        //Check item amount from both bag and pocket
        if(itemBag.getPocket("Medicine").getItemAmount("Potion") != 10 || itemBag.getItemAmount("Potion") != 10)
        {
            throw new RuntimeException("Invalid pointer returned from Bag.getPocket()!");
        }
    }
}
