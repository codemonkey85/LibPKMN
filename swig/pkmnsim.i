/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * SWIG's std_shared_ptr.i files simply include boost_shared_ptr.i, but for
 * for ease of development, PKMNsim uses std::shared_ptr. Thus, this SWIG
 * file creates a dummy std::shared_ptr for SWIG to use instead.
 */

%include "std_string.i"
%include "std_vector.i"

namespace std
{
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
