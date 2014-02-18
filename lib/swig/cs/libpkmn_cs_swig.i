/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%rename(tostring) pkmn::pokemon_text::const_char;
%typemap(csclassmodifiers) pkmn::pokemon_text "public partial class"

%include "libpkmn.i"

%{
    #include "pkmn/paths.hpp"

    #include "pkmn/types/array.hpp"
    #include "pkmn/types/dict.hpp"
    #include "pkmn/types/pokemon_text.hpp"
    #include "pkmn/types/prng.hpp"

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

%include "pkmn/types/array.hpp"
%include "pkmn/types/dict.hpp"
%include "pkmn/types/pokemon_text.hpp"
%include "pkmn/types/prng.hpp"

%include "pkmn/base_pokemon.hpp"
%include "pkmn/item.hpp"
%include "pkmn/pocket.hpp"
%include "pkmn/bag.hpp"
%include "pkmn/move.hpp"
%include "pkmn/team_pokemon.hpp"
%include "pkmn/trainer.hpp"
%include "pkmn/game_save.hpp"

%include "pkmn/lists.hpp"

%template(StringArray) pkmn::array<std::string>;
%template(UIntArray) pkmn::array<unsigned int>;
%template(BasePokemonArray) pkmn::array<pkmn::base_pokemon::sptr>;
%template(Moveset) pkmn::array<pkmn::move::sptr>;
%template(PokemonTeam) pkmn::array<pkmn::team_pokemon::sptr>;
%template(StringIntDict) pkmn::dict<std::string, int>;
%template(StringStringDict) pkmn::dict<std::string, std::string>;
%template(StringUIntDict) pkmn::dict<std::string, unsigned int>;

//Factory functions need to specifically be associated with newobject
%newobject pkmn::bag::make;
%newobject pkmn::base_pokemon::make;
%newobject pkmn::game_save::make;
%newobject pkmn::item::make;
%newobject pkmn::move::make;
%newobject pkmn::pocket::make;
%newobject pkmn::team_pokemon::make;
%newobject pkmn::trainer::make;

//TODO: make C++ -> C# class name conversion function
%template(BagSPtr)         pkmn::shared_ptr<pkmn::bag>;
%template(BasePokemonSPtr) pkmn::shared_ptr<pkmn::base_pokemon>;
%template(GameSaveSPtr)    pkmn::shared_ptr<pkmn::game_save>;
%template(ItemSPtr)        pkmn::shared_ptr<pkmn::item>;
%template(MoveSPtr)        pkmn::shared_ptr<pkmn::move>;
%template(PocketSPtr)      pkmn::shared_ptr<pkmn::pocket>;
%template(TeamPokemonSPtr) pkmn::shared_ptr<pkmn::team_pokemon>;
%template(TrainerSPtr)     pkmn::shared_ptr<pkmn::trainer>;