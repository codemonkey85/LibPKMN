/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%feature("autodoc", "1");

%include "std_vector.i"
%import "pypkmnsim_swig.i"

%{
    #include "pkmnsim/io.hpp"
%} 

%include "pkmnsim/io.hpp"
