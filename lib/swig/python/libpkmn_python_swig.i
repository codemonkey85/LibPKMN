/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%feature("autodoc", "1");
 
/*
 * An interface template needs to be created for each pkmn::shared_ptr used.
 *
 * To make Python development easier, the make function is rendered unnecessary.
 */

%define LIBPKMN_PYTHON_FIX(CLASS_NAME)
    %template(CLASS_NAME ## _sptr) pkmn::shared_ptr<pkmn :: ## CLASS_NAME>;
    %pythoncode %{
        CLASS_NAME = CLASS_NAME.make;
    %}
%enddef

/*
 * Extend array, dict, and pokemon_text to act more like native Python
 * arrays, dicts, and strings
 */
%extend pkmn::array<pkmn::move::sptr>{
    pkmn::move::sptr __getitem__(int i) {return (*self)[i];}
    void __setitem__(int i, pkmn::move::sptr val) {(*self)[i] = val;}
};
%extend pkmn::array<pkmn::team_pokemon::sptr>{
    pkmn::team_pokemon::sptr __getitem__(int i) {return (*self)[i];}
    void __setitem__(int i, pkmn::team_pokemon::sptr val) {(*self)[i] = val;}
};
%extend pkmn::dict<unsigned int, unsigned int>{
    unsigned int __getitem__(unsigned int i) {return (*self)[i];}
    void __setitem__(unsigned int i, unsigned int val) {(*self)[i] = val;}
};
%extend pkmn::dict<std::string, int>{
    int __getitem__(std::string i) {return (*self)[i];}
    void __setitem__(std::string i, int val) {(*self)[i] = val;}
};
%extend pkmn::pokemon_text{
    char* __repr__() {return (char*)((*self).const_char());}
    char* __str__() {return (char*)((*self).const_char());}
};

%include "libpkmn.i"

%{
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
    #include "pkmn/paths.hpp"
%}

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
%include "pkmn/paths.hpp"

/*
 * Templates for specific uses of pkmn::array and pkmn::dict
 */
%template(string_array) pkmn::array<std::string>;
%template(uint_array) pkmn::array<unsigned int>;
%template(base_pokemon_array) pkmn::array<pkmn::base_pokemon::sptr>;
%template(moveset_t) pkmn::array<pkmn::move::sptr>;
%template(pokemon_team_t) pkmn::array<pkmn::team_pokemon::sptr>;
%template(string_int_dict) pkmn::dict<std::string, int>;
%template(string_string_dict) pkmn::dict<std::string, std::string>;
%template(string_uint_dict) pkmn::dict<std::string, unsigned int>;

//Python str -> pkmn::pokemon_text
%typemap(in) pkmn::pokemon_text {
    $1 = pkmn::pokemon_text(PyString_AsString($input));
}

//pkmn::pokemon_text -> Python str
%typemap(out) PyString {
    $result = PyString_FromString($1);
}

LIBPKMN_PYTHON_FIX(base_pokemon)
LIBPKMN_PYTHON_FIX(item)
LIBPKMN_PYTHON_FIX(pocket)
LIBPKMN_PYTHON_FIX(bag)
LIBPKMN_PYTHON_FIX(move)
LIBPKMN_PYTHON_FIX(team_pokemon)
LIBPKMN_PYTHON_FIX(trainer)
LIBPKMN_PYTHON_FIX(game_save)
