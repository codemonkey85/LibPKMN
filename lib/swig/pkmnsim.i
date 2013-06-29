/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "std_string.i"
%include "std_vector.i"
%include "stl.i"
%include "typemaps.i"

namespace std
{
    /*
     * SWIG's std_shared_ptr.i files simply include boost_shared_ptr.i, but for
     * for ease of development, PKMNsim uses std::shared_ptr. Thus, this SWIG
     * file creates a dummy std::shared_ptr for SWIG to use instead.
     */

    template<class T> class shared_ptr
    {
        public:
            shared_ptr() {}
            shared_ptr(T * p) {}
            T * operator-> () {return px;}

        private:
            T * px;
            int pn;
    };

    %template(vector_i) vector<int>;
    %template(vector_d) vector<double>;
    %template(vector_s) vector<string>;
};

//Should not be exposed
%ignore pkmnsim::get_pkmn_id;
%ignore pkmnsim::get_species_id;

%{
    #include "pkmnsim/config.hpp"
    #include "pkmnsim/dict.hpp"
    #include "pkmnsim/lists.hpp"
    #include "pkmnsim/paths.hpp"
    #include "pkmnsim/vla.hpp"
    
    #include "pkmnsim/pkmn_nature.hpp"
    #include "pkmnsim/pkmn_types.hpp"
    #include "pkmnsim/base_move.hpp"
    #include "pkmnsim/base_pkmn.hpp"
    #include "pkmnsim/spec_pkmn.hpp"
    #include "pkmnsim/game_save.hpp"
%}

%include "pkmnsim/config.hpp"
%include "pkmnsim/dict.hpp"
%include "pkmnsim/lists.hpp"
%include "pkmnsim/paths.hpp"
%include "pkmnsim/vla.hpp"

%include "pkmnsim/pkmn_nature.hpp"
%include "pkmnsim/pkmn_types.hpp"
%include "pkmnsim/base_move.hpp"
%include "pkmnsim/base_pkmn.hpp"
%include "pkmnsim/spec_pkmn.hpp"
%include "pkmnsim/game_save.hpp"

//Templates for different uses of dict and vla
%template(dict_int_std_string) pkmnsim::dict<int, std::string>;
%template(dict_std_string_int) pkmnsim::dict<std::string, int>;
%template(dict_std_string_double) pkmnsim::dict<std::string, double>;
%template(vla_pkmnsim_base_move_sptr) pkmnsim::vla<std::shared_ptr<pkmnsim::base_move> >;
%template(vector_base_pkmn) std::vector<std::shared_ptr<pkmnsim::base_pkmn> >;
%template(vector_spec_pkmn) std::vector<std::shared_ptr<pkmnsim::spec_pkmn> >;
