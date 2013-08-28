/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "std_pair.i"
%include "std_string.i"
%include "std_vector.i"
%include "stl.i"

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

    %template(pair_us) pair<unsigned short, unsigned short>;
    %template(vector_b) vector<bool>;
};

%ignore pkmnsim::PokeLib::Pokemon::make(::PokeLib::Pokemon*);
%ignore pkmnsim::PokeLib::Trainer::make(::PokeLib::Save*);
%ignore pkmnsim::PokeLib::Trainer::make(::PokeLib::Trainer*);
%ignore pkmnsim::PokeLib::Save::make(::PokeLib::Save*);
%ignore pkmnsim::PokeLib::Party::make(::PokeLib::Party*);
%ignore pkmnsim::PokeLib::Box::make(::PokeLib::Box*);

%{
    #include "pkmnsim/config.hpp"

    #include "pkmnsim/pokelib/Pokemon.hpp"
    #include "pkmnsim/pokelib/Trainer.hpp"
    #include "pkmnsim/pokelib/Party.hpp"
    #include "pkmnsim/pokelib/Box.hpp"
    #include "pkmnsim/pokelib/Save.hpp"
%}

%include "pkmnsim/config.hpp"

%include "pkmnsim/pokelib/Pokemon.hpp"
%include "pkmnsim/pokelib/Trainer.hpp"
%include "pkmnsim/pokelib/Party.hpp"
%include "pkmnsim/pokelib/Box.hpp"
%include "pkmnsim/pokelib/Save.hpp"
