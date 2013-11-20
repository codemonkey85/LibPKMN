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

%ignore pkmnsim::dict::operator[];
%ignore pkmnsim::vla::operator[];

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

%ignore pkmnsim::get_game_list;
%ignore pkmnsim::get_game_group_list;
%ignore pkmnsim::get_item_list;
%ignore pkmnsim::get_pokemon_list;
%ignore pkmnsim::get_type_list;
%ignore pkmnsim::get_ability_list;
%ignore pkmnsim::get_nature_list;
%ignore pkmnsim::get_pokemon_of_type;
%ignore pkmnsim::analysis::get_damage_range;
%ignore pkmnsim::analysis::get_stat_range;
%ignore pkmnsim::database::get_table_list;
%ignore pkmnsim::database::get_column_list;
%rename(get_game_list) pkmnsim::get_game_vec;
%rename(get_game_group_list) pkmnsim::get_game_group_vec;
%rename(get_item_list) pkmnsim::get_item_vec;
%rename(get_pokemon_list) pkmnsim::get_pokemon_vec;
%rename(get_type_list) pkmnsim::get_type_vec;
%rename(get_ability_list) pkmnsim::get_ability_vec;
%rename(get_nature_list) pkmnsim::get_nature_vec;
%rename(get_pokemon_of_type) pkmnsim::get_pokemon_vec_of_type;
%rename(get_damage_range) pkmnsim::get_damage_range_vec;
%rename(get_stat_range) pkmnsim::get_stat_range_vec;
%rename(get_table_list) pkmnsim::get_table_vec;
%rename(get_column_list) pkmnsim::get_column_vec;

%include "pkmnsim.i"

//Python str -> pkmnsim::pokemon_text
%typemap(in) pkmnsim::pokemon_text {
    $1 = pkmnsim::pokemon_text(PyString_AsString($input));
}

//pkmnsim::pokemon_text -> Python str
%typemap(out) PyString {
    $result = PyString_FromString($1);
}

%{
    #include "pkmnsim_swig_functions.hpp"
%};

%include "pkmnsim_swig_functions.hpp"

PKMNSIM_PYTHON_FIX(base_pokemon)
PKMNSIM_PYTHON_FIX(item)
PKMNSIM_PYTHON_FIX(pocket)
PKMNSIM_PYTHON_FIX(bag)
PKMNSIM_PYTHON_FIX(move)
PKMNSIM_PYTHON_FIX(team_pokemon)
PKMNSIM_PYTHON_FIX(trainer)
PKMNSIM_PYTHON_FIX(game_save)
