/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%feature("autodoc", "1");

%include "stl.i"
%import "libpkmn_python_swig.i"

%{
    #include "AbilitiesComboBoxProxy.hpp"
%}

%include "AbilitiesComboBoxProxy.hpp"
