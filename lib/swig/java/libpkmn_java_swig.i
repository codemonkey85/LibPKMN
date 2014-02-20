/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%typemap(javacode) pkmn::pokemon_text %{
    @Override public string toString() {
        return const_char();
    }
%}

/*
 * Macro for dict templates to avoid individual javacode declarations for HashMaps
 */
%define LIBPKMN_JAVA_DICT(dict_name, ctype1, ctype2, javatype1, javatype2, vec1, vec2)
    %typemap(javacode) pkmn::dict<ctype1, ctype2> %{
        public java.util.HashMap<javatype1, javatype2> toHashMap() {
            java.util.HashMap<javatype1, javatype2> output = new java.util.HashMap<javatype1, javatype2>();

            vec1 key_vec = keys();
            vec2 val_vec = vals();

            for(int i = 0; i < key_vec.size(); i++)
            {
                output.put(key_vec.get(i), val_vec.get(i));
            }

            return output;
        }
    %}

    %template(dict_name) pkmn::dict<ctype1, ctype2>;
%enddef

%include "CamelCase.i"
%include "stdint.i"
%include "std_pair.i"
%include "libpkmn.i"

%{
    #include "pkmn/config.hpp"

    #include "pkmn/calculations.hpp"
    #include "pkmn/paths.hpp"

    #include "pkmn/types/dict.hpp"
    #include "pkmn/types/pokemon_text.hpp"
    #include "pkmn/types/prng.hpp"
    #include "pkmn/types/array.hpp"

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

%include "pkmn/calculations.hpp"
%include "pkmn/paths.hpp"

%include "pkmn/types/dict.hpp"
%include "pkmn/types/pokemon_text.hpp"
%include "pkmn/types/prng.hpp"
%include "pkmn/types/array.hpp"

%include "pkmn/base_pokemon.hpp"
%include "pkmn/item.hpp"
%include "pkmn/pocket.hpp"
%include "pkmn/bag.hpp"
%include "pkmn/move.hpp"
%include "pkmn/team_pokemon.hpp"
%include "pkmn/trainer.hpp"
%include "pkmn/game_save.hpp"

%include "pkmn/lists.hpp"

//TODO: make C++ -> Java class name conversion function
%template(BagSPtr)         pkmn::shared_ptr<pkmn::bag>;
%template(BasePokemonSPtr) pkmn::shared_ptr<pkmn::base_pokemon>;
%template(GameSaveSPtr)    pkmn::shared_ptr<pkmn::game_save>;
%template(ItemSPtr)        pkmn::shared_ptr<pkmn::item>;
%template(MoveSPtr)        pkmn::shared_ptr<pkmn::move>;
%template(PocketSPtr)      pkmn::shared_ptr<pkmn::pocket>;
%template(TeamPokemonSPtr) pkmn::shared_ptr<pkmn::team_pokemon>;
%template(TrainerSPtr)     pkmn::shared_ptr<pkmn::trainer>;

LIBPKMN_JAVA_DICT(StringIntDict, int, int, int, int, int_vec, int_vec)
LIBPKMN_JAVA_DICT(StringStringDict, std::string, std::string, string, string, string_vec, string_vec)
LIBPKMN_JAVA_DICT(StringUIntDict, std::string, unsigned int, string, long, string_vec, uint_vec)

%template(UIntArray) pkmn::array<unsigned int>;

%template(BasePokemonVector) std::vector<pkmn::base_pokemon::sptr>;
%template(BasePokemonArray) pkmn::array<pkmn::base_pokemon::sptr>;
%template(MoveVector) std::vector<pkmn::move::sptr>;
%template(Moveset) pkmn::array<pkmn::move::sptr>;
%template(TeamPokemonVector) std::vector<pkmn::team_pokemon::sptr>;
%template(PokemonTeam) pkmn::array<pkmn::team_pokemon::sptr>;
