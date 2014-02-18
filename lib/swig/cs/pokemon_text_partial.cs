/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;

namespace LibPKMN
{
    public partial class pokemon_text : IDisposable
    {
        public override string ToString()
        {
            string ret = tostring();
            return ret;
        }

        public static implicit operator string(pokemon_text input)
        {
            return input.tostring();
        }

        public static implicit operator pokemon_text(string input)
        {
            pokemon_text temp = new pokemon_text(input);
            return temp;
        }
    }
}
