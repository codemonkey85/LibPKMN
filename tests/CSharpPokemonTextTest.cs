/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using LibPKMN;
using System;

public class PokemonTextTest
{
    public static int Main(string[] args)
    {
        LibPKMN.pokemon_text test_text = new LibPKMN.pokemon_text("Test");
        System.Console.WriteLine(test_text); //If this compiles and prints, test works
        return 0;
    }    
}
