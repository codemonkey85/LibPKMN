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
        //Testing printing on command line
        LibPKMN.pokemon_text test_text1 = new LibPKMN.pokemon_text("Test1");
        System.Console.WriteLine(test_text1);

        //Testing string -> LibPKMN.pokemon_text
        string test_string1 = "Test2";
        LibPKMN.pokemon_text test_text2 = test_string1; //implicit
        if(test_text2 != "Test2")
        {
            throw new System.Exception("C# string -> LibPKMN.pokemon_text implicit conversion failed!");
        }
        LibPKMN.pokemon_text test_text3 = (LibPKMN.pokemon_text)test_string1; //explicit
        if(test_text3 != "Test2")
        {
            throw new System.Exception("C# string -> LibPKMN.pokemon_text explicit conversion failed!");
        }

        //Testing LibPKMN.pokemon_text -> string
        LibPKMN.pokemon_text test_text4 = new LibPKMN.pokemon_text("Test3");
        string test_string2 = test_text4.ToString();
        if(test_string2 != "Test3")
        {
            throw new System.Exception("LibPKMN.pokemon_text.ToString() failed!");
        }
        string test_string3 = test_text4;
        if(test_string3 != "Test3")
        {
            throw new System.Exception("LibPKMN.pokemon_text -> C# string implicit conversion failed!");
        }
        string test_string4 = (string)test_text4;
        if(test_string4 != "Test3")
        {
            throw new System.Exception("LibPKMN.pokemon_text -> C# string explicit conversion failed!");
        }

        return 0;
    }    
}
