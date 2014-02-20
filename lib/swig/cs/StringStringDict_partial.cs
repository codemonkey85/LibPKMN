/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using System.Collections.Generic;

namespace LibPKMN
{
    public partial class StringStringDict : IDisposable
    {
        public static implicit operator Dictionary<string, string>(StringStringDict input)
        {
            Dictionary<string, string> output = new Dictionary<string, string>();
            string_vec keys = input.keys();
            string_vec vals = input.vals();

            for(int i = 0; i < keys.Count; i++)
            {
                output.Add(keys[i], vals[i]);
            }

            return output;
        }

        public static implicit operator StringStringDict(Dictionary<string, string> input)
        {
            StringStringDict output = new StringStringDict();
            foreach(var pair in input)
            {
                output.insert(pair.Key, pair.Value);
            }
            return output;
        }
    }
}
