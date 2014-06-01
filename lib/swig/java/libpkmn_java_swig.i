/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("libpkmn_java_swig");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load. \n" + e);
      System.exit(1);
    }
  }
%}

%typemap(javacode) pkmn::pokemon_text %{
    public char charAt(int index) {
        return toString().charAt(index);
    }

    @Override public boolean equals(Object other) {
        return (toString().equals(((PokemonText)other).toString()));
    }

    public boolean isEmpty() {
        return toString().isEmpty();
    }

    @Override public String toString() {
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

            vec1 key_vec = KeySet();
            vec2 val_vec = values();

            for(int i = 0; i < key_vec.size(); i++)
            {
                output.put(key_vec.get(i), val_vec.get(i));
            }

            return output;
        }
    %}

    %rename(put) pkmn::dict<ctype1, ctype2>::insert;
    %rename(containsKey) pkmn::dict<ctype1, ctype2>::has_key;
    %rename(remove) pkmn::dict<ctype1, ctype2>::erase;
    %rename(KeySet) pkmn::dict<ctype1, ctype2>::keys;
    %rename(values) pkmn::dict<ctype1, ctype2>::vals;

    %template(dict_name) pkmn::dict<ctype1, ctype2>;
%enddef

%extend pkmn::nature {
    double at(std::string key) {return (*self)[key];}
    double at(unsigned int key) {return (*self)[key];}
};

%include "CamelCase.i"
%include "stdint.i"
%include "std_pair.i"
%include "libpkmn.i"

%{
    #include "pkmn/config.hpp"

    #include "pkmn/paths.hpp"

    #include "pkmn/types/dict.hpp"
    #include "pkmn/types/pokemon_text.hpp"
    #include "pkmn/types/prng.hpp"

    #include "pkmn/nature.hpp"
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

%include "pkmn/nature.hpp"
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
%template(PRNGSPtr)        pkmn::shared_ptr<pkmn::prng>;
%template(TeamPokemonSPtr) pkmn::shared_ptr<pkmn::team_pokemon>;
%template(TrainerSPtr)     pkmn::shared_ptr<pkmn::trainer>;

%template(BagSlot) std::pair<pkmn::item::sptr, unsigned int>;
%template(ItemList) std::vector<std::pair<pkmn::item::sptr, unsigned int> >;

%template(UIntPairVector) std::vector<std::pair<unsigned int, unsigned int> >;
%template(BasePokemonVector) std::vector<pkmn::base_pokemon::sptr>;
%template(Moveset) std::vector<pkmn::move::sptr>;
%template(PocketVector) std::vector<pkmn::pocket::sptr>;
%template(PokemonTeam) std::vector<pkmn::team_pokemon::sptr>;

LIBPKMN_JAVA_DICT(StringIntDict, std::string, int, String, Integer, string_vec, int_vec)
LIBPKMN_JAVA_DICT(StringStringDict, std::string, std::string, String, String, string_vec, string_vec)
LIBPKMN_JAVA_DICT(StringUIntDict, std::string, unsigned int, String, Long, string_vec, uint_vec)
LIBPKMN_JAVA_DICT(PocketDict, std::string, pkmn::pocket::sptr, String, PocketSPtr, string_vec, PocketVector)

//Factory functions need to specifically be associated with newobject
%newobject pkmn::bag::make;
%newobject pkmn::base_pokemon::make;
%newobject pkmn::game_save::make;
%newobject pkmn::item::make;
%newobject pkmn::move::make;
%newobject pkmn::pocket::make;
%newobject pkmn::prng::get;
%newobject pkmn::team_pokemon::make;
%newobject pkmn::team_pokemon::get_base_pokemon;
%newobject pkmn::trainer::make;
