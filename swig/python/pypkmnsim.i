/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "pkmnsim.i"

%ignore get_game_list;
%ignore get_game_group_list;
%ignore get_type_list;

%{
#include "pkmnsim/config.hpp"
#include "pkmnsim/lists.hpp"
%}

%include "pkmnsim/config.hpp"
%include "pkmnsim/lists.hpp"
