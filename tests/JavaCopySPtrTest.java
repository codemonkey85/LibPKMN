/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

import org.nc.*;

public class JavaCopySPtrTest
{
    public static void main(String[] args)
    {
        String path = LibPKMN.getDatabasePath();
        long entry = Database.getItemID("Bicycle");
        System.out.println(path + " " + entry);
    }
}