/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <string>

#include <pkmnsim/database/queries.hpp>

using namespace std;

namespace pkmnsim
{
    namespace database
    {
        //Transform identifier strings into proper format for database
        string to_database_format(string identifier)
        {
            //Some manual changes
            if(identifier == "Farfetch'd") identifier = "farfetchd";
            else if(identifier == "Mr. Mime") identifier = "mr-mime";
            else if(identifier == "Mime Jr.") identifier = "mime-jr";
            else if(identifier == "Exp. Share") identifier = "exp-share";

            transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);
            replace(identifier.begin(), identifier.end(), ' ', '-');

            return identifier;
        }
    }
}
