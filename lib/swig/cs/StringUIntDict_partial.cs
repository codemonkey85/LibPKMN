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
    public partial class StringUIntDict : IDisposable
    {
        public static implicit operator Dictionary<string, uint>(StringUIntDict input)
        {
            Dictionary<string, uint> output = new Dictionary<string, uint>();
            string_vec keys = input.keys();
            uint_vec vals = input.vals();

            for(int i = 0; i < keys.Count; i++)
            {
                output.Add(keys[i], vals[i]);
            }

            return output;
        }

        public static implicit operator StringUIntDict(Dictionary<string, uint> input)
        {
            StringUIntDict output = new StringUIntDict();
            foreach(var pair in input)
            {
                output.insert(pair.Key, pair.Value);
            }
            return output;
        }
    }
}
