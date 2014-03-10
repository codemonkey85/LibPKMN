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

%define LIBPKMN_PYTHON_SPTR(CLASS_NAME)
    %template(CLASS_NAME ## _sptr) pkmn::shared_ptr<pkmn :: ## CLASS_NAME>;
    %pythoncode %{
        CLASS_NAME = CLASS_NAME.make;
    %}
%enddef

/*
 * Macro for pkmn::dict templates
 */
%define LIBPKMN_PYTHON_DICT(python_name, type1, type2)
    %extend pkmn::dict<type1, type2>{
        type2 __getitem__(type1 i) {return (*self)[i];}
        void __setitem__(type1 i, type2 val) {(*self)[i] = val;}
    }
    %template(python_name) pkmn::dict<type1, type2>;
%enddef

%ignore pkmn::pokemon_text::const_char();
%extend pkmn::pokemon_text{
    char* __repr__() {return (char*)((*self).const_char());}
    char* __str__() {return (char*)((*self).const_char());}
};

%include "libpkmn.i"

%{
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
 * pkmn::pokemon_text typemaps
 */
//Python str -> pkmn::pokemon_text
%typemap(in) pkmn::pokemon_text {
    $1 = pkmn::pokemon_text(PyString_AsString($input));
}

//pkmn::pokemon_text -> Python str
%typemap(out) PyString {
    $result = PyString_FromString($1);
}

%template(bag_slot) std::pair<pkmn::item::sptr, unsigned int>;

%template(base_pokemon_vec) std::vector<pkmn::base_pokemon::sptr>;
%template(moveset) std::vector<pkmn::move::sptr>;
%rename(pocket_vec) std::vector<pkmn::pocket::sptr>;
%rename(pokemon_team) std::vector<pkmn::team_pokemon::sptr>;

LIBPKMN_PYTHON_DICT(string_int_dict, std::string, int)
LIBPKMN_PYTHON_DICT(string_string_dict, std::string, std::string)
LIBPKMN_PYTHON_DICT(string_uint_dict, std::string, unsigned int)
LIBPKMN_PYTHON_DICT(pocket_dict, std::string, pkmn::pocket::sptr)

LIBPKMN_PYTHON_SPTR(base_pokemon)
LIBPKMN_PYTHON_SPTR(item)
LIBPKMN_PYTHON_SPTR(pocket)
LIBPKMN_PYTHON_SPTR(bag)
LIBPKMN_PYTHON_SPTR(move)
LIBPKMN_PYTHON_SPTR(team_pokemon)
LIBPKMN_PYTHON_SPTR(trainer)
LIBPKMN_PYTHON_SPTR(game_save)

%template(prng_sptr) pkmn::shared_ptr<pkmn::prng>;
%pythoncode %{
    prng = prng.get;
%}