/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%ignore pkmnsim::pokemon_text::pokemon_text(const wchar_t*);
%ignore pkmnsim::pokemon_text::pokemon_text(std::string);
%ignore pkmnsim::pokemon_text::pokemon_text(std::wstring);
%ignore pkmnsim::pokemon_text::set(const wchar_t*);
%ignore pkmnsim::pokemon_text::set(std::string);
%ignore pkmnsim::pokemon_text::set(std::wstring);

%include "CamelCase.i"
%include "pkmnsim.i"

//TODO: make C++ -> C# class name conversion function
%template(BagSPtr)         std::shared_ptr<pkmnsim::bag>;
%template(BasePokemonSPtr) std::shared_ptr<pkmnsim::base_pokemon>;
%template(GameSaveSPtr)    std::shared_ptr<pkmnsim::game_save>;
%template(ItemSPtr)        std::shared_ptr<pkmnsim::item>;
%template(MoveSPtr)        std::shared_ptr<pkmnsim::move>;
%template(PocketSPtr)      std::shared_ptr<pkmnsim::pocket>;
%template(TeamPokemonSPtr) std::shared_ptr<pkmnsim::team_pokemon>;
%template(TrainerSPtr)     std::shared_ptr<pkmnsim::trainer>;
