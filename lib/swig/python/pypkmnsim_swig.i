/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%feature("autodoc", "1");
 
/*
 * An interface template needs to be created for each std::shared_ptr used.
 *
 * To make Python development easier, the make function is rendered unnecessary.
 */

%define PKMNSIM_PYTHON_FIX(CLASS_NAME)
    %template(CLASS_NAME ## _sptr) std::shared_ptr<pkmnsim :: ## CLASS_NAME>;
    %pythoncode %{
        CLASS_NAME = CLASS_NAME.make;
    %}
%enddef

//Extend dict and VLA to allow use of [] operator
%extend pkmnsim::dict<unsigned int, unsigned int>{
    unsigned int __getitem__(unsigned int i) {return (*self)[i];}
    void __setitem__(unsigned int i, unsigned int val) {(*self)[i] = val;}
};
%extend pkmnsim::dict<std::string, int>{
    int __getitem__(std::string i) {return (*self)[i];}
    void __setitem__(std::string i, int val) {(*self)[i] = val;}
};
%extend pkmnsim::vla<pkmnsim::move::sptr>{
    pkmnsim::move::sptr __getitem__(int i) {return (*self)[i];}
    void __setitem__(int i, pkmnsim::move::sptr val) {(*self)[i] = val;}
};
%extend pkmnsim::vla<pkmnsim::team_pokemon::sptr>{
    pkmnsim::team_pokemon::sptr __getitem__(int i) {return (*self)[i];}
    void __setitem__(int i, pkmnsim::team_pokemon::sptr val) {(*self)[i] = val;}
};

//Make pokemon_text act more like a string when printed
%extend pkmnsim::pokemon_text{
    char* __repr__() {return (char*)((*self).const_char());}
    char* __str__() {return (char*)((*self).const_char());}
};

%include "pkmnsim.i"

%{
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
    #include "pkmnsim/paths.hpp"
%}

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
%include "pkmnsim/paths.hpp"

//Templates for common uses of dict and vla
%template(dict_uint_uint) pkmnsim::dict<unsigned int, unsigned int>;
%template(dict_string_int) pkmnsim::dict<std::string, int>;
%template(vla_uint) pkmnsim::vla<unsigned int>;

%template(b_pkmn_vec_t) std::vector<pkmnsim::base_pokemon::sptr>;
%template(b_pkmn_vla_t) pkmnsim::vla<pkmnsim::base_pokemon::sptr>;
%template(move_vec_t) std::vector<pkmnsim::move::sptr>;
%template(moveset_t) pkmnsim::vla<pkmnsim::move::sptr>;
%template(team_pkmn_vec_t) std::vector<pkmnsim::team_pokemon::sptr>;
%template(pokemon_team_t) pkmnsim::vla<pkmnsim::team_pokemon::sptr>;

//Python str -> pkmnsim::pokemon_text
%typemap(in) pkmnsim::pokemon_text {
    $1 = pkmnsim::pokemon_text(PyString_AsString($input));
}

//pkmnsim::pokemon_text -> Python str
%typemap(out) PyString {
    $result = PyString_FromString($1);
}

PKMNSIM_PYTHON_FIX(base_pokemon)
PKMNSIM_PYTHON_FIX(item)
PKMNSIM_PYTHON_FIX(pocket)
PKMNSIM_PYTHON_FIX(bag)
PKMNSIM_PYTHON_FIX(move)
PKMNSIM_PYTHON_FIX(team_pokemon)
PKMNSIM_PYTHON_FIX(trainer)
PKMNSIM_PYTHON_FIX(game_save)
