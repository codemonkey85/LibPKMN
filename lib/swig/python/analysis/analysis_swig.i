/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%feature("autodoc", "1");

/*
 * Needs access to wrapped PKMNsim classes
 */
%import "pypkmnsim_swig.i"

%{
    #include "pkmnsim/analysis/damage.hpp"
    #include "pkmnsim/analysis/stats.hpp"
%} 

%include "pkmnsim/analysis/damage.hpp"
%include "pkmnsim/analysis/stats.hpp"
