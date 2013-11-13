/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "std_string.i"
%include "std_vector.i"
%include "std_wstring.i"
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

    %template(int_vec) vector<int>;
    %template(double_vec) vector<double>;
    %template(string_vec) vector<string>;
    %template(uint_vec) vector<unsigned int>;
};

%{
    #include "pkmnsim/config.hpp"
    #include "pkmnsim/paths.hpp"

    #include "pkmnsim/types/dict.hpp"
    #include "pkmnsim/types/pokemon_text.hpp"
    #include "pkmnsim/types/vla.hpp"
    
    #include "pkmnsim/base_pokemon.hpp"
    #include "pkmnsim/item.hpp"
    #include "pkmnsim/pocket.hpp"
    #include "pkmnsim/bag.hpp"
    #include "pkmnsim/move.hpp"
    #include "pkmnsim/team_pokemon.hpp"
    #include "pkmnsim/trainer.hpp"
    #include "pkmnsim/game_save.hpp"
    
    #include "pkmnsim/analysis/damage.hpp"
    #include "pkmnsim/analysis/stats.hpp"
    
    #include "pkmnsim/database/lists.hpp"
    #include "pkmnsim/database/queries.hpp"
    
    #include "pkmnsim/lists.hpp"
%}

%include "pkmnsim/config.hpp"
%include "pkmnsim/paths.hpp"

%include "pkmnsim/types/dict.hpp"
%include "pkmnsim/types/pokemon_text.hpp"
%include "pkmnsim/types/vla.hpp"

%include "pkmnsim/base_pokemon.hpp"
%include "pkmnsim/item.hpp"
%include "pkmnsim/pocket.hpp"
%include "pkmnsim/bag.hpp"
%include "pkmnsim/move.hpp"
%include "pkmnsim/team_pokemon.hpp"
%include "pkmnsim/trainer.hpp"
%include "pkmnsim/game_save.hpp"

%include "pkmnsim/analysis/damage.hpp"
%include "pkmnsim/analysis/stats.hpp"

%include "pkmnsim/database/lists.hpp"
%include "pkmnsim/database/queries.hpp"

%include "pkmnsim/lists.hpp"

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