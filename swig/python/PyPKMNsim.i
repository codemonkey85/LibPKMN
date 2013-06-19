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
%ignore pkmnsim::get_pkmn_of_type;
%ignore pkmnsim::get_type_list;

%rename(get_game_list) pkmnsim::get_game_vec;
%rename(get_game_group_list) pkmnsim::get_game_group_vec;
%rename(get_pkmn_of_type) pkmnsim::get_pkmn_list_of_type;
%rename(get_type_list) pkmnsim::get_type_vec;

%include "pkmnsim.i"

PKMNSIM_PYTHON_FIX(base_move)
PKMNSIM_PYTHON_FIX(base_pkmn)
PKMNSIM_PYTHON_FIX(pkmn_nature)
PKMNSIM_PYTHON_FIX(spec_pkmn)
