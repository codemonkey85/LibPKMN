/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "CamelCase.i"
%include "libpkmn_java_swig.i"

%{
    #include "pkmn/analysis/damage.hpp"
    #include "pkmn/analysis/stats.hpp"
%}

%include "pkmn/analysis/damage.hpp"
%include "pkmn/analysis/stats.hpp"
