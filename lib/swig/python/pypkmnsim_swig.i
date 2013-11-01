/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * An interface template needs to be created for each std::shared_ptr used.
 *
 * To make Python development easier, the make function is rendered unnecessary.
 */

%define PKMNSIM_PYTHON_FIX(CLASS_NAME)
    %template(CLASS_NAME ## _sptr) std::shared_ptr<pkmnsim :: ## CLASS_NAME>;
    %pythoncode %{
        CLASS_NAME = CLASS_NAME.make;
    %}
%enddef

%ignore pkmnsim::dict::operator[];
%ignore pkmnsim::vla::operator[];

//Extend dict and VLA to allow use of [] operator
%extend pkmnsim::dict<unsigned int, unsigned int>{
    unsigned int __getitem__(unsigned int i) {return (*self)[i];}
    void __setitem__(unsigned int i, unsigned int val) {(*self)[i] = val;}
};
%extend pkmnsim::dict<std::string, int>{
    int __getitem__(std::string i) {return (*self)[i];}
    void __setitem__(std::string i, int val) {(*self)[i] = val;}
};
%extend pkmnsim::vla<pkmnsim::move::sptr>{
    pkmnsim::move::sptr __getitem__(int i) {return (*self)[i];}
    void __setitem__(int i, pkmnsim::move::sptr val) {(*self)[i] = val;}
};
%extend pkmnsim::vla<pkmnsim::team_pokemon::sptr>{
    pkmnsim::team_pokemon::sptr __getitem__(int i) {return (*self)[i];}
    void __setitem__(int i, pkmnsim::team_pokemon::sptr val) {(*self)[i] = val;}
};

%include "pkmnsim.i"

%{
    #include "pkmnsim_swig_functions.hpp"
%};

%include "pkmnsim_swig_functions.hpp"

PKMNSIM_PYTHON_FIX(base_pokemon)
PKMNSIM_PYTHON_FIX(item)
PKMNSIM_PYTHON_FIX(move)
PKMNSIM_PYTHON_FIX(team_pokemon)
PKMNSIM_PYTHON_FIX(trainer)