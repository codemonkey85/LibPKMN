/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%feature("autodoc", "1");

%include "std_vector.i"
%import "libpkmn_python_swig.i"

%{
    #include "pkmn/analysis/damage.hpp"
    #include "pkmn/analysis/stats.hpp"
%} 

%include "pkmn/analysis/damage.hpp"
%include "pkmn/analysis/stats.hpp"
