#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#Extract Git commits from LibPKMN and all submodules

INCLUDE(CheckCXXSourceCompiles)
INCLUDE(CheckIncludeFileCXX)

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_REQUIRED_FLAGS "-std=c++0x")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(CMAKE_REQUIRED_FLAGS "-std=c++11")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

CHECK_CXX_SOURCE_COMPILES("
    enum testenum: short
    {
        foo,
        bar,
        baz
    };

    int main()
    {
        int a = bar;

        return 0;
    }
    " HAVE_STRONGLY_TYPED_ENUMS
)

IF(NOT HAVE_STRONGLY_TYPED_ENUMS)
    MESSAGE(FATAL_ERROR "Strongly typed enums not supported!")
ENDIF(NOT HAVE_STRONGLY_TYPED_ENUMS)

CHECK_CXX_SOURCE_COMPILES("
    namespace testnamespace
    {
        enum testenum
        {
            foo,
            bar,
            baz
        };
    }

    int main()
    {
        int a = testnamespace::bar;

        return 0;
    }
    " HAVE_SCOPED_ENUMS
)

IF(NOT HAVE_SCOPED_ENUMS)
    MESSAGE(FATAL_ERROR "Scoped enums not supported!")
ENDIF(NOT HAVE_SCOPED_ENUMS)

#
# If the user doesn't specifically say to use boost::shared_ptr,
# manually check to see if it's necessary.
#
IF(NOT USE_BOOST_SHARED_PTR)
    CHECK_CXX_SOURCE_COMPILES("
        #if (defined(__GNUC__) && \\\\
               (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 2)) && \\\\
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
            #error no std::shared_ptr
        #endif

        int main()
        {
            return 0;
        }
        " HAVE_SHARED_PTR
    )

    IF(NOT HAVE_SHARED_PTR)
        ADD_DEFINITIONS(-DLIBPKMN_USE_BOOST_SHARED_PTR)
        MESSAGE(STATUS "STL shared_ptr not found. Defaulting to boost::shared_ptr.")
    ENDIF(NOT HAVE_SHARED_PTR)
ENDIF(NOT USE_BOOST_SHARED_PTR)

#
# By this point, any non-supported compiler setup should have errored out, but if
# not, print a warning.
#
IF(NOT MSVC AND NOT CMAKE_COMPILER_IS_GNUCXX AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    MESSAGE(WARNING "Your compiler passed all necessary tests, but this configuration is untested.")
ENDIF(NOT MSVC AND NOT CMAKE_COMPILER_IS_GNUCXX AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

MESSAGE(STATUS "")
