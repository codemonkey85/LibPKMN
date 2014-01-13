/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%ignore pkmn::pokemon_text::pokemon_text(const wchar_t*);
%ignore pkmn::pokemon_text::pokemon_text(std::string);
%ignore pkmn::pokemon_text::pokemon_text(std::wstring);
%ignore pkmn::pokemon_text::set(const wchar_t*);
%ignore pkmn::pokemon_text::set(std::string);
%ignore pkmn::pokemon_text::set(std::wstring);

%include "CamelCase.i"
%include "libpkmn.i"

%{
    #include "pkmn/paths.hpp"

    #include "pkmn/types/dict.hpp"
    #include "pkmn/types/pokemon_text.hpp"
    #include "pkmn/types/prng.hpp"
    #include "pkmn/types/vla.hpp"

    #include "pkmn/base_pokemon.hpp"
    #include "pkmn/item.hpp"
    #include "pkmn/pocket.hpp"
    #include "pkmn/bag.hpp"
    #include "pkmn/move.hpp"
    #include "pkmn/team_pokemon.hpp"
    #include "pkmn/trainer.hpp"
    #include "pkmn/game_save.hpp"

    #include "pkmn/lists.hpp"

%}

%include "pkmn/paths.hpp"

%include "pkmn/types/dict.hpp"
%include "pkmn/types/pokemon_text.hpp"
%include "pkmn/types/prng.hpp"
%include "pkmn/types/vla.hpp"

%include "pkmn/base_pokemon.hpp"
%include "pkmn/item.hpp"
%include "pkmn/pocket.hpp"
%include "pkmn/bag.hpp"
%include "pkmn/move.hpp"
%include "pkmn/team_pokemon.hpp"
%include "pkmn/trainer.hpp"
%include "pkmn/game_save.hpp"

%include "pkmn/lists.hpp"

//TODO: make C++ -> C# class name conversion function
%template(BagSPtr)         pkmn::shared_ptr<pkmn::bag>;
%template(BasePokemonSPtr) pkmn::shared_ptr<pkmn::base_pokemon>;
%template(GameSaveSPtr)    pkmn::shared_ptr<pkmn::game_save>;
%template(ItemSPtr)        pkmn::shared_ptr<pkmn::item>;
%template(MoveSPtr)        pkmn::shared_ptr<pkmn::move>;
%template(PocketSPtr)      pkmn::shared_ptr<pkmn::pocket>;
%template(TeamPokemonSPtr) pkmn::shared_ptr<pkmn::team_pokemon>;
%template(TrainerSPtr)     pkmn::shared_ptr<pkmn::trainer>;

%template(UIntUIntDict) pkmn::dict<unsigned int, unsigned int>;
%template(StringIntDict) pkmn::dict<std::string, int>;
%template(UIntVLA) pkmn::vla<unsigned int>;

%template(BasePokemonVector) std::vector<pkmn::base_pokemon::sptr>;
%template(BasePokemonVLA) pkmn::vla<pkmn::base_pokemon::sptr>;
%template(MoveVector) std::vector<pkmn::move::sptr>;
%template(Moveset) pkmn::vla<pkmn::move::sptr>;
%template(TeamPokemonVector) std::vector<pkmn::team_pokemon::sptr>;
%template(PokemonTeam) pkmn::vla<pkmn::team_pokemon::sptr>;
