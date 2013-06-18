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

/*
 * To make development in other languages easier, the make function is
 * rendered unnecessary. (Python only so far)
 */

#ifdef SWIG_PYTHON
    %define PKMNSIM_PYTHON_UNMAKE(CLASS_NAME)
        %template(CLASS_NAME ## _sptr std::shared_ptr<pkmnsim:: ## CLASS_NAME>;
        %pythoncode %{
            CLASS_NAME = CLASS_NAME.make;
        %}
    %enddef
#endif
