/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "stdint.i"
%include "std_pair.i"

%include "CamelCase.i"
%import "libpkmn.i"

%{
    #include "pkmn/calculations.hpp"
%}

%include "pkmn/calculations.hpp"
