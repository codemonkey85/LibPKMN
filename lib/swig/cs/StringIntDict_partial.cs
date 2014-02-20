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
    public partial class StringIntDict : IDisposable
    {
        public static implicit operator Dictionary<string, int>(StringIntDict input)
        {
            Dictionary<string, int> output = new Dictionary<string, int>();
            string_vec keys = input.keys();
            int_vec vals = input.vals();

            for(int i = 0; i < keys.Count; i++)
            {
                output.Add(keys[i], vals[i]);
            }

            return output;
        }

        public static implicit operator StringIntDict(Dictionary<string, int> input)
        {
            StringIntDict output = new StringIntDict();
            foreach(var pair in input)
            {
                output.insert(pair.Key, pair.Value);
            }
            return output;
        }
    }
}
