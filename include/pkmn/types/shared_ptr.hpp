/*
 * Copyright (c) 2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_SHARED_PTR_HPP
#define INCLUDED_PKMN_TYPES_SHARED_PTR_HPP

/// Compatibility with non-clang compilers.
#ifndef __has_feature
  #define __has_feature(x) 0
#endif

//
// Try to detect the better shared_ptr to use, and then imports the symbol in the current namespace
// => if you include this "shared_ptr.hpp" file inside your own namespace you will
//    get a kind of universal easy to use "shared_ptr" type
//
#ifdef LIBPKMN_USE_BOOST_SHARED_PTR
    // Use Boost only if explicitly told
    #include <boost/shared_ptr.hpp>
    #include <boost/make_shared.hpp>
    namespace pkmn {
    using boost::shared_ptr;
    using boost::make_shared;
    using boost::dynamic_pointer_cast;
    } // namespace pkmn
// Detect whether the compiler supports C++11 shared_ptr or its TR1 pre-version.
#elif (defined(__GNUC__) && \
       (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 2)) && \
       defined(__GXX_EXPERIMENTAL_CXX0X__))
    // GCC 4.3 and following have std::shared_ptr support when called with -std=c++0x (or -std=c++11 starting with GCC 4.7)
    #include <memory>
    namespace pkmn {
        using std::shared_ptr;
        using std::make_shared;
        using std::dynamic_pointer_cast;
    } // namespace pkmn
#elif (defined(__GNUC__) && (__GNUC__ == 4) && defined(__GXX_EXPERIMENTAL_CXX0X__))
    // GCC 4.0/4.1/4.2 have std::shared_ptr support when when called with -std=c++0x
    #include <tr1/memory>
    namespace pkmn {
        using std::tr1::shared_ptr;
        using std::tr1::make_shared;
        using std::tr1::dynamic_pointer_cast;
    } // namespace pkmn
#elif defined(_MSC_VER) && (_MSC_VER >= 1600)
    // Visual Studio 2010 compile by default in C++11 mode
    #include <memory>
    namespace pkmn {
        using std::shared_ptr;
        using std::make_shared;
        using std::dynamic_pointer_cast;
    } // namespace pkmn
#elif defined(_MSC_VER) && (_MSC_VER >= 1500)
    // Visual Studio 2008 : beware, TR1 is provided with the Service Pack 1 only !
    #include <memory>
    namespace pkmn {
        using std::tr1::shared_ptr;
        using std::tr1::make_shared;
        using std::tr1::dynamic_pointer_cast;
    } // namespace pkmn
#else
    using std::shared_ptr;
    using std::make_shared;
    using std::dynamic_pointer_cast;
#endif

#endif /* INCLUDED_PKMN_TYPES_SHARED_PTR_HPP */
