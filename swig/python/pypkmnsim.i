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

%ignore pkmnsim::get_game_list;
%ignore pkmnsim::get_game_group_list;
%ignore pkmnsim::get_type_list;

%include "pkmnsim.i"

PKMNSIM_PYTHON_FIX(base_move)
PKMNSIM_PYTHON_FIX(base_pkmn)
PKMNSIM_PYTHON_FIX(pkmn_nature)
PKMNSIM_PYTHON_FIX(spec_pkmn)
