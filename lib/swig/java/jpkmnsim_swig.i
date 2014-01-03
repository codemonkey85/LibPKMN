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

%include "stdint.i"
%include "std_pair.i"
%include "CamelCase.i"
%include "pkmnsim.i"

%{
    #include "pkmnsim/config.hpp"

    #include "pkmnsim/calculations.hpp"
    #include "pkmnsim/paths.hpp"

    #include "pkmnsim/types/dict.hpp"
    #include "pkmnsim/types/pokemon_text.hpp"
    #include "pkmnsim/types/prng.hpp"
    #include "pkmnsim/types/vla.hpp"

    #include "pkmnsim/base_pokemon.hpp"
    #include "pkmnsim/item.hpp"
    #include "pkmnsim/pocket.hpp"
    #include "pkmnsim/bag.hpp"
    #include "pkmnsim/move.hpp"
    #include "pkmnsim/team_pokemon.hpp"
    #include "pkmnsim/trainer.hpp"
    #include "pkmnsim/game_save.hpp"

    #include "pkmnsim/lists.hpp"

%}

%include "pkmnsim/calculations.hpp"
%include "pkmnsim/paths.hpp"

%include "pkmnsim/types/dict.hpp"
%include "pkmnsim/types/pokemon_text.hpp"
%include "pkmnsim/types/prng.hpp"
%include "pkmnsim/types/vla.hpp"

%include "pkmnsim/base_pokemon.hpp"
%include "pkmnsim/item.hpp"
%include "pkmnsim/pocket.hpp"
%include "pkmnsim/bag.hpp"
%include "pkmnsim/move.hpp"
%include "pkmnsim/team_pokemon.hpp"
%include "pkmnsim/trainer.hpp"
%include "pkmnsim/game_save.hpp"

%include "pkmnsim/lists.hpp"

//TODO: make C++ -> Java class name conversion function
%template(BagSPtr)         std::shared_ptr<pkmn::bag>;
%template(BasePokemonSPtr) std::shared_ptr<pkmn::base_pokemon>;
%template(GameSaveSPtr)    std::shared_ptr<pkmn::game_save>;
%template(ItemSPtr)        std::shared_ptr<pkmn::item>;
%template(MoveSPtr)        std::shared_ptr<pkmn::move>;
%template(PocketSPtr)      std::shared_ptr<pkmn::pocket>;
%template(TeamPokemonSPtr) std::shared_ptr<pkmn::team_pokemon>;
%template(TrainerSPtr)     std::shared_ptr<pkmn::trainer>;

%template(UIntUIntDict) pkmn::dict<unsigned int, unsigned int>;
%template(StringIntDict) pkmn::dict<std::string, int>;
%template(UIntVLA) pkmn::vla<unsigned int>;

%template(BasePokemonVector) std::vector<pkmn::base_pokemon::sptr>;
%template(BasePokemonVLA) pkmn::vla<pkmn::base_pokemon::sptr>;
%template(MoveVector) std::vector<pkmn::move::sptr>;
%template(Moveset) pkmn::vla<pkmn::move::sptr>;
%template(TeamPokemonVector) std::vector<pkmn::team_pokemon::sptr>;
%template(PokemonTeam) pkmn::vla<pkmn::team_pokemon::sptr>;
