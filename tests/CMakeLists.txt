#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

SET(cpp_test_sources
    boostless_dict_test.cpp
)

FOREACH(cpp_test ${cpp_test_sources})
    GET_FILENAME_COMPONENT(cpp_test_name ${cpp_test} NAME_WE)
    ADD_EXECUTABLE(${cpp_test_name} ${cpp_test})
    TARGET_LINK_LIBRARIES(${cpp_test_name} pkmnsim)

    IF(CMAKE_COMPILER_IS_GNUCXX)
        IF(LINUX)
            SET_TARGET_PROPERTIES(boostless_dict_test PROPERTIES COMPILE_FLAGS "-std=c++0x -fPIC")
        ELSE() #Cygwin
            SET_TARGET_PROPERTIES(boostless_dict_test PROPERTIES COMPILE_FLAGS "-std=c++0x")
        ENDIF(LINUX)
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        SET_TARGET_PROPERTIES(pkmnsim PROPERTIES COMPILE_FLAGS "-std=c++11 -fPIC")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)

    ADD_TEST(${cpp_test_name} ${cpp_test_name})
ENDFOREACH(cpp_test ${cpp_test_sources})

