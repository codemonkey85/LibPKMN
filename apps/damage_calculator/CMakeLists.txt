#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

ADD_EXECUTABLE(damage_calculator damage_calculator.cpp)
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET_TARGET_PROPERTIES(damage_calculator PROPERTIES COMPILE_FLAGS "-std=c++0x")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET_TARGET_PROPERTIES(damage_calculator PROPERTIES COMPILE_FLAGS "-std=c++11")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
TARGET_LINK_LIBRARIES(damage_calculator pkmn)
INSTALL(TARGETS damage_calculator RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime)
