/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using LibPKMN;
using System;
using System.Collections.Generic;

public class DictTest
{
    public static int Main(string[] args)
    {
        /*
         * pkmn::dict<std::string, int> tests
         */
        StringIntDict test_dict1 = new StringIntDict();
        test_dict1.Add("TestKey1", -10);
        test_dict1["TestKey2"] = -5;
        test_dict1.Add("TestKey3", 0);
        test_dict1["TestKey4"] = 5;
        test_dict1.Add("TestKey5", 10);

        //StringIntDict -> Dictionary<string, int>
        Dictionary<string, int> test_cs_dict1 = test_dict1;
        foreach(System.Collections.Generic.KeyValuePair<string, int> pair in test_cs_dict1)
        {
            if(pair.Value != test_dict1[pair.Key])
            {
                throw new System.Exception("StringIntDict -> Dictionary<string, int> conversion failed!");
            }
        }

        //Dictionary<string, int> -> StringIntDict
        Dictionary<string, int> test_cs_dict2 = new Dictionary<string, int>();
        test_cs_dict2.Add("TestKey1", -10);
        test_cs_dict2["TestKey2"] = -5;
        test_cs_dict2.Add("TestKey3", 0);
        test_cs_dict2["TestKey4"] = 5;
        test_cs_dict2.Add("TestKey5", 10);
        StringIntDict test_dict2 = test_cs_dict2;
        string_vec test_dict2_keys = test_dict2.Keys;
        for(int i = 0; i < test_dict2_keys.Count; i++)
        {
            if(test_dict2[test_dict2_keys[i]] != test_cs_dict2[test_dict2_keys[i]])
            {
                throw new System.Exception("Dictionary<string, int> -> StringIntDict conversion failed!");
            }
        }

        /*
         * pkmn::dict<std::string, std::string> tests
         */
        StringStringDict test_dict3 = new StringStringDict();
        test_dict3.Add("TestKey1", "TestValue1");
        test_dict3["TestKey2"] = "TestValue2";
        test_dict3.Add("TestKey3", "TestValue3");
        test_dict3["TestKey4"] = "TestValue4";
        test_dict3.Add("TestKey5", "TestValue5");

        //StringStringDict -> Dictionary<string, string>
        Dictionary<string, string> test_cs_dict3 = test_dict3;
        foreach(System.Collections.Generic.KeyValuePair<string, string> pair in test_cs_dict3)
        {
            if(pair.Value != test_dict3[pair.Key])
            {
                throw new System.Exception("StringStringDict -> Dictionary<string, string> conversion failed!");
            }
        }

        //Dictionary<string, string> -> StringStringDict
        Dictionary<string, string> test_cs_dict4 = new Dictionary<string, string>();
        test_cs_dict4.Add("TestKey1", "TestValue1");
        test_cs_dict4["TestKey2"] = "TestValue2";
        test_cs_dict4.Add("TestKey3", "TestValue3");
        test_cs_dict4["TestKey4"] = "TestValue4";
        test_cs_dict4.Add("TestKey5", "TestValue5");
        StringStringDict test_dict4 = test_cs_dict4;
        string_vec test_dict4_keys = test_dict4.Keys;
        for(int i = 0; i < test_dict4_keys.Count; i++)
        {
            if(test_dict4.at(test_dict4_keys[i]) != test_cs_dict4[test_dict4_keys[i]])
            {
                throw new System.Exception("Dictionary<string, string> -> StringStringDict conversion failed!");
            }
        }

        /*
         * pkmn::dict<std::string, uint> tests
         */
        StringUIntDict test_dict5 = new StringUIntDict();
        test_dict5.Add("TestKey1", 0);
        test_dict5["TestKey2"] = 5;
        test_dict5.Add("TestKey3", 10);
        test_dict5["TestKey4"] = 15;
        test_dict5.Add("TestKey5", 20);

        //StringUIntDict -> Dictionary<string, uint>
        Dictionary<string, uint> test_cs_dict5 = test_dict5;
        foreach(System.Collections.Generic.KeyValuePair<string, uint> pair in test_cs_dict5)
        {
            if(pair.Value != test_dict5[pair.Key])
            {
                throw new System.Exception("StringUIntDict -> Dictionary<string, uint> conversion failed!");
            }
        }

        //Dictionary<string, uint> -> StringUIntDict
        Dictionary<string, uint> test_cs_dict6 = new Dictionary<string, uint>();
        test_cs_dict6.Add("TestKey1", 0);
        test_cs_dict6["TestKey2"] = 5;
        test_cs_dict6.Add("TestKey3", 10);
        test_cs_dict6["TestKey4"] = 15;
        test_cs_dict6.Add("TestKey5", 20);
        StringUIntDict test_dict6 = test_cs_dict6;
        string_vec test_dict6_keys = test_dict6.Keys;
        for(int i = 0; i < test_dict6_keys.Count; i++)
        {
            if(test_dict6[test_dict6_keys[i]] != test_cs_dict6[test_dict6_keys[i]])
            {
                throw new System.Exception("Dictionary<string, uint> -> StringUIntDict conversion failed!");
            }
        }

        return 0;
    }    
}
