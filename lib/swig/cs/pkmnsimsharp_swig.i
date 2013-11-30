/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%define PKMNSIM_CS_FIX(CLASS_NAME)
    %template(CLASS_NAME ## _sptr) std::shared_ptr<pkmnsim :: ## CLASS_NAME>;
%enddef

%ignore pkmnsim::pokemon_text::pokemon_text(const wchar_t*);
%ignore pkmnsim::pokemon_text::pokemon_text(std::string);
%ignore pkmnsim::pokemon_text::pokemon_text(std::wstring);
%ignore pkmnsim::pokemon_text::set(const wchar_t*);
%ignore pkmnsim::pokemon_text::set(std::string);
%ignore pkmnsim::pokemon_text::set(std::wstring);

%include "CamelCase.i"
%include "pkmnsim.i"

PKMNSIM_CS_FIX(base_pokemon)
PKMNSIM_CS_FIX(item)
PKMNSIM_CS_FIX(pocket)
PKMNSIM_CS_FIX(bag)
PKMNSIM_CS_FIX(move)
PKMNSIM_CS_FIX(team_pokemon)
PKMNSIM_CS_FIX(trainer)
PKMNSIM_CS_FIX(game_save)

