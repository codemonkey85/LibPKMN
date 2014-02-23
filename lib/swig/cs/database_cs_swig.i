/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "exception.i"
%include "CamelCase.i"
%import "libpkmn.i"

%{
    #include "pkmn/database/lists.hpp"
    #include "pkmn/database/queries.hpp"
%}

%include "pkmn/database/lists.hpp"
%include "pkmn/database/queries.hpp"
