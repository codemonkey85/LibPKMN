/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

import org.nc.*;

public class JavaDictTest
{
    public static void main(String[] args)
    {
        BasePokemonSPtr bPKMN = BasePokemon.make("Mewtwo", "Emerald");
        StringUIntDict stats = bPKMN.getBaseStats();
        string_vec keys = stats.KeySet();

        java.util.HashMap<String, Long> statsHashMap = stats.toHashMap();

        for(int i = 0; i < stats.size(); i++)
        {
            if(stats.at(keys.get(i)) != statsHashMap.get(keys.get(i)))
            {
                throw new RuntimeException("StringUintDict -> HashMap conversion failed!");
            }
        }
    }
}
