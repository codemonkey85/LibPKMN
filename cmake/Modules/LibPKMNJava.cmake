#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(LIBPKMN_JAVA_INCLUDE_DIRS
    ${LIBPKMN_SOURCE_DIR}/include
    ${LIBPKMN_SWIG_SOURCE_DIR}
    ${JAVA_INCLUDE_PATH}
    ${JNI_INCLUDE_DIRS}
)

INCLUDE_DIRECTORIES(${LIBPKMN_JAVA_INCLUDE_DIRS})

########################################################################
# Build and install Java SWIG modules
# Assumes LIBPKMN_SWIG_SOURCE_DIR is set
########################################################################
MACRO(JAVA_BUILD_SWIG_MODULE swig_source java_module_name)
    FOREACH(dir ${LIBPKMN_JAVA_INCLUDE_DIRS})
        LIST(APPEND CMAKE_SWIG_FLAGS "-I${dir}")
    ENDFOREACH(dir ${LIBPKMN_JAVA_INCLUDE_DIRS})
    SET_SOURCE_FILES_PROPERTIES(${swig_source}.i PROPERTIES CPLUSPLUS ON)

    SET(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/org/nc/LibPKMN)
    SET(CMAKE_SWIG_FLAGS -module ${java_module_name} -package "org.nc" ${CMAKE_GLOBAL_SWIG_FLAGS})
    SWIG_ADD_MODULE(${swig_source} java ${swig_source}.i)
    ADD_DEPENDENCIES(${SWIG_MODULE_${swig_source}_REAL_NAME} java_enums)
    IF(UNIX)
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES PREFIX "lib")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES SUFFIX ".so")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++0x")
    ELSE()
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES PREFIX "")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES SUFFIX ".dll")
    ENDIF(UNIX)
    SWIG_LINK_LIBRARIES(${swig_source} pkmn)

    IF(WIN32)
        INSTALL(
            TARGETS ${SWIG_MODULE_${swig_source}_REAL_NAME}
            DESTINATION ${RUNTIME_DIR}
            COMPONENT "LibPKMN_Java"
        )
    ELSE()
        INSTALL(
            TARGETS ${SWIG_MODULE_${swig_source}_REAL_NAME}
            DESTINATION ${LIBRARY_DIR}
            COMPONENT "LibPKMN_Java"
        )
    ENDIF(WIN32)
ENDMACRO(JAVA_BUILD_SWIG_MODULE)
