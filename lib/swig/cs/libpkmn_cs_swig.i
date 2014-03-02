/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%rename(tostring) pkmn::pokemon_text::const_char;

%typemap(cscode) pkmn::pokemon_text %{
    public char this[int index] {
        get {
            return ToString()[index];
        }
        set {
            char[] arr = ToString().ToCharArray();
            arr[index] = value;
            set(new string(arr));
        }
    }

    public int Length {
        get {
            return ToString().Length;
        }
    }

    public override string ToString() {
        return tostring();
    }

    public static implicit operator string(pokemon_text input) {
        return input.tostring();
    }

    public static implicit operator pokemon_text(string input) {
        pokemon_text temp = new pokemon_text(input);
        return temp;
    }
%}

/*
 * Macro for dict templates to avoid using partial classes for every type
 */
%define LIBPKMN_CS_DICT(dict_name, ctype1, ctype2, cstype1, cstype2)
    %typemap(cscode) pkmn::dict<ctype1, ctype2> %{
        public cstype1 ## _vec Keys {
            get {
                return keys();
            }
        }

        public cstype2 ## _vec Values {
            get {
                return vals();
            }
        }
        
        public cstype2 this[cstype1 index] {
            get {
                return at(index);
            }
            set {
                Add(index, value);
            }
        }

        public static implicit operator System.Collections.Generic.Dictionary<cstype1, cstype2>(dict_name input) {
            System.Collections.Generic.Dictionary<cstype1, cstype2> output = new System.Collections.Generic.Dictionary<cstype1, cstype2>();
            cstype1 ## _vec keys = input.keys();
            cstype2 ## _vec vals = input.vals();

            for(int i = 0; i < keys.Count; i++) output.Add(keys[i], vals[i]);

            return output;
        }

        public static implicit operator dict_name(System.Collections.Generic.Dictionary<cstype1, cstype2> input) {
            dict_name output = new dict_name();
            foreach(System.Collections.Generic.KeyValuePair<cstype1, cstype2> pair in input) output.Add(pair.Key, pair.Value);
            return output;
        }
    %}

    %rename(Add) pkmn::dict<ctype1, ctype2>::insert;
    %rename(Clear) pkmn::dict<ctype1, ctype2>::clear;
    %rename(ContainsKey) pkmn::dict<ctype1, ctype2>::has_key;
    %rename(Remove) pkmn::dict<ctype1, ctype2>::erase;

    %template(dict_name) pkmn::dict<ctype1, ctype2>;
%enddef

%include "libpkmn.i"

%{
    #include "pkmn/paths.hpp"

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

LIBPKMN_CS_DICT(StringIntDict, std::string, int, string, int)
LIBPKMN_CS_DICT(StringStringDict, std::string, std::string, string, string)
LIBPKMN_CS_DICT(StringUIntDict, std::string, unsigned int, string, uint)

//Factory functions need to specifically be associated with newobject
%newobject pkmn::bag::make;
%newobject pkmn::base_pokemon::make;
%newobject pkmn::game_save::make;
%newobject pkmn::item::make;
%newobject pkmn::move::make;
%newobject pkmn::pocket::make;
%newobject pkmn::prng::get;
%newobject pkmn::team_pokemon::make;
%newobject pkmn::trainer::make;

//TODO: make C++ -> C# class name conversion function
%template(BagSPtr)         pkmn::shared_ptr<pkmn::bag>;
%template(BasePokemonSPtr) pkmn::shared_ptr<pkmn::base_pokemon>;
%template(GameSaveSPtr)    pkmn::shared_ptr<pkmn::game_save>;
%template(ItemSPtr)        pkmn::shared_ptr<pkmn::item>;
%template(MoveSPtr)        pkmn::shared_ptr<pkmn::move>;
%template(PocketSPtr)      pkmn::shared_ptr<pkmn::pocket>;
%template(PRNGSPtr)        pkmn::shared_ptr<pkmn::prng>;
%template(TeamPokemonSPtr) pkmn::shared_ptr<pkmn::team_pokemon>;
%template(TrainerSPtr)     pkmn::shared_ptr<pkmn::trainer>;

%template(BasePokemonVector) std::vector<pkmn::base_pokemon::sptr>;
%template(Moveset) std::vector<pkmn::move::sptr>;
%template(PokemonTeam) std::vector<pkmn::team_pokemon::sptr>;