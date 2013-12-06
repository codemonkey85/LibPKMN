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

%{
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

//TODO: make C++ -> C% class name conversion function
%template(BagSPtr)         std::shared_ptr<pkmnsim::bag>;
%template(BasePokemonSPtr) std::shared_ptr<pkmnsim::base_pokemon>;
%template(GameSaveSPtr)    std::shared_ptr<pkmnsim::game_save>;
%template(ItemSPtr)        std::shared_ptr<pkmnsim::item>;
%template(MoveSPtr)        std::shared_ptr<pkmnsim::move>;
%template(PocketSPtr)      std::shared_ptr<pkmnsim::pocket>;
%template(TeamPokemonSPtr) std::shared_ptr<pkmnsim::team_pokemon>;
%template(TrainerSPtr)     std::shared_ptr<pkmnsim::trainer>;

%template(UIntUIntDict) pkmnsim::dict<unsigned int, unsigned int>;
%template(StringIntDict) pkmnsim::dict<std::string, int>;
%template(UIntVLA) pkmnsim::vla<unsigned int>;

%template(BasePokemonVector) std::vector<pkmnsim::base_pokemon::sptr>;
%template(BasePokemonVLA) pkmnsim::vla<pkmnsim::base_pokemon::sptr>;
%template(MoveVector) std::vector<pkmnsim::move::sptr>;
%template(Moveset) pkmnsim::vla<pkmnsim::move::sptr>;
%template(TeamPokemonVector) std::vector<pkmnsim::team_pokemon::sptr>;
%template(PokemonTeam) pkmnsim::vla<pkmnsim::team_pokemon::sptr>;
