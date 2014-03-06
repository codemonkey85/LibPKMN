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
        System.Console.WriteLine("PokemonText: " + test_text1);

        //Testing string -> PokemonText
        string test_string1 = "Test2";
        PokemonText test_text2 = test_string1; //implicit
        if(test_text2 != "Test2")
        {
            throw new System.Exception("C# string -> PokemonText implicit conversion failed!");
        }
        System.Console.WriteLine("C# string -> PokemonText (implicit): " + test_text2);
        PokemonText test_text3 = (PokemonText)test_string1; //explicit
        if(test_text3 != "Test2")
        {
            throw new System.Exception("C# string -> PokemonText explicit conversion failed!");
        }
        System.Console.WriteLine("C# string -> PokemonText (explicit): " + test_text3);

        //Testing PokemonText -> string
        PokemonText test_text4 = new PokemonText("Test3");
        string test_string2 = test_text4.ToString();
        if(test_string2 != "Test3")
        {
            throw new System.Exception("PokemonText.ToString() failed!");
        }
        System.Console.WriteLine("PokemonText.ToString(): " + test_string2);
        string test_string3 = test_text4;
        if(test_string3 != "Test3")
        {
            throw new System.Exception("PokemonText -> C# string implicit conversion failed!");
        }
        System.Console.WriteLine("PokemonText -> C# string (implicit): " + test_string3);
        string test_string4 = (string)test_text4;
        if(test_string4 != "Test3")
        {
            throw new System.Exception("PokemonText -> C# string explicit conversion failed!");
        }
        System.Console.WriteLine("PokemonText -> C# string (explicit): " + test_string4);

        return 0;
    }    
}
