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
        PokemonText test_text1 = new PokemonText("Test1");
        System.Console.WriteLine(test_text1);

        //Testing string -> PokemonText
        string test_string1 = "Test2";
        PokemonText test_text2 = test_string1; //implicit
        if(test_text2 != "Test2")
        {
            throw new System.Exception("C# string -> PokemonText implicit conversion failed!");
        }
        PokemonText test_text3 = (PokemonText)test_string1; //explicit
        if(test_text3 != "Test2")
        {
            throw new System.Exception("C# string -> PokemonText explicit conversion failed!");
        }

        //Testing PokemonText -> string
        PokemonText test_text4 = new PokemonText("Test3");
        string test_string2 = test_text4.ToString();
        if(test_string2 != "Test3")
        {
            throw new System.Exception("PokemonText.ToString() failed!");
        }
        string test_string3 = test_text4;
        if(test_string3 != "Test3")
        {
            throw new System.Exception("PokemonText -> C# string implicit conversion failed!");
        }
        string test_string4 = (string)test_text4;
        if(test_string4 != "Test3")
        {
            throw new System.Exception("PokemonText -> C# string explicit conversion failed!");
        }

        return 0;
    }    
}
