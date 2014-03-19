#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#Extract Git commits from LibPKMN and all submodules

INCLUDE(CheckCXXSourceCompiles)
INCLUDE(CheckIncludeFileCXX)

IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(CMAKE_REQUIRED_FLAGS "-std=c++0x")
ENDIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

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

#
# It's harder to check the compiler with CMake and test for the
# appropriate include, so use the same code.
#
CHECK_CXX_SOURCE_COMPILES("
    #ifndef __has_feature
        #define __has_feature(x) 0
    #endif

    #if (defined(__GNUC__) && \\\\
           (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 2)) && \\\\
           defined(__GXX_EXPERIMENTAL_CXX0X__))
        #include <memory>
        namespace pkmn {
            using std::shared_ptr;
        } // namespace pkmn
    #elif (defined(__GNUC__) && (__GNUC__ == 4) && defined(__GXX_EXPERIMENTAL_CXX0X__))
        #include <tr1/memory>
        namespace pkmn {
            using std::tr1::shared_ptr;
        } // namespace pkmn
    #elif defined(__clang__) && __has_feature(cxx_nullptr)
        #include <memory>
        namespace pkmn {
            using std::shared_ptr;
        } // namespace pkmn
    #elif defined(_MSC_VER) && (_MSC_VER >= 1600)
        #include <memory>
        namespace pkmn {
            using std::shared_ptr;
        } // namespace pkmn
    #elif defined(_MSC_VER) && (_MSC_VER >= 1500)
        #include <memory>
        namespace pkmn {
            using std::tr1::shared_ptr;
        } // namespace pkmn
    #endif

    int main()
    {
        return 0;
    }
    " HAVE_SHARED_PTR
)

IF(NOT HAVE_SHARED_PTR)
    MESSAGE(FATAL_ERROR "Shared pointers not supported!")
ENDIF(NOT HAVE_SHARED_PTR)

#
# By this point, any non-supported compiler setup should have errored out, but if
# not, print a warning.
#
IF(NOT MSVC AND NOT CMAKE_COMPILER_IS_GNUCXX AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    MESSAGE(WARNING "Your compiler passed all necessary tests, but this configuration is untested.")
ENDIF(NOT MSVC AND NOT CMAKE_COMPILER_IS_GNUCXX AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

MESSAGE(STATUS "")
