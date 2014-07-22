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
    #include "AboutMessageBoxProxy.hpp"
    #include "NaturesComboBoxProxy.hpp"
    #include "PokemonListComboBoxProxy.hpp"
    #include "TypesComboBoxProxy.hpp"
    #include "VersionsComboBoxProxy.hpp"
%}

%include "AbilitiesComboBoxProxy.hpp"
%include "AboutMessageBoxProxy.hpp"
%include "NaturesComboBoxProxy.hpp"
%include "PokemonListComboBoxProxy.hpp"
%include "TypesComboBoxProxy.hpp"
%include "VersionsComboBoxProxy.hpp"
