/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%rename(tostring) pkmn::pokemon_text::const_char;

%typemap(cscode) pkmn::pokemon_text %{
    public override string ToString()
    {
        return tostring();
    }

    public static implicit operator string(pokemon_text input)
    {
        return input.tostring();
    }

    public static implicit operator pokemon_text(string input)
    {
        pokemon_text temp = new pokemon_text(input);
        return temp;
    }
%}

/*
 * Macro for dict templates to avoid using partial classes for every type
 */
%define LIBPKMN_CS_DICT(dict_name, ctype1, ctype2, cstype1, cstype2)
    %typemap(cscode) pkmn::dict<ctype1, ctype2> %{
        public static implicit operator System.Collections.Generic.Dictionary<cstype1, cstype2>(dict_name input)
        {
            System.Collections.Generic.Dictionary<cstype1, cstype2> output = new System.Collections.Generic.Dictionary<cstype1, cstype2>();
            cstype1 ## _vec keys = input.keys();
            cstype2 ## _vec vals = input.vals();

            for(int i = 0; i < keys.Count; i++) output.Add(keys[i], vals[i]);

            return output;
        }

        public static implicit operator dict_name(System.Collections.Generic.Dictionary<cstype1, cstype2> input)
        {
            dict_name output = new dict_name();
            foreach(System.Collections.Generic.KeyValuePair<cstype1, cstype2> pair in input) output.insert(pair.Key, pair.Value);
            return output;
        }
    %}

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
