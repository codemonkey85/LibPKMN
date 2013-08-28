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
    %template(CLASS_NAME ## _sptr) std::shared_ptr<pkmnsim::PokeLib :: ## CLASS_NAME>;
    %pythoncode %{
        CLASS_NAME = CLASS_NAME.make;
    %}
%enddef

%ignore pkmnsim::dict::operator[];

%include "pokelib.i"

PKMNSIM_PYTHON_FIX(Trainer)
PKMNSIM_PYTHON_FIX(Save)
PKMNSIM_PYTHON_FIX(Party)
PKMNSIM_PYTHON_FIX(Box)
PKMNSIM_PYTHON_FIX(Pokemon)
