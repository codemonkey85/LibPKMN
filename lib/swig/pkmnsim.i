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

%ignore pkmnsim::dict::operator[];
%ignore pkmnsim::vla::operator[];
%ignore pkmnsim::pokemon_text::operator std::string;
%ignore pkmnsim::pokemon_text::operator std::wstring;

%{
    #include "pkmnsim/config.hpp"
%}

%include "pkmnsim/config.hpp"
