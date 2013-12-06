/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "CamelCase.i"
%import "pkmnsimsharp_swig.i"

%{
    #include "pkmnsim/analysis/damage.hpp"
    #include "pkmnsim/analysis/stats.hpp"
%}

%include "pkmnsim/analysis/damage.hpp"
%include "pkmnsim/analysis/stats.hpp"
