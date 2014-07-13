#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(${SWIG_USE_FILE})

FILE(TO_NATIVE_PATH "${LIBPKMN_SOURCE_DIR}/lib/swig" LIBPKMN_SWIG_SOURCE_DIR)
SET(LIBPKMN_SWIG_SOURCE_DIR ${LIBPKMN_SWIG_SOURCE_DIR} CACHE FILEPATH "LibPKMN SWIG source directory")

SET(LIBPKMN_JAVA_INCLUDE_DIRS
    ${LIBPKMN_SOURCE_DIR}/include
    ${LIBPKMN_SWIG_SOURCE_DIR}
    ${JAVA_INCLUDE_PATH}
    ${JNI_INCLUDE_DIRS}
)

SET(LIBPKMN_JAVA_LIBRARIES
    pkmn
)

INCLUDE_DIRECTORIES(${LIBPKMN_JAVA_INCLUDE_DIRS})
SET(JAVA_BUILD_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
SET(JAVA_INSTALL_DIRECTORY share/java)
FILE(MAKE_DIRECTORY ${JAVA_BUILD_DIRECTORY})
FILE(MAKE_DIRECTORY ${JAVA_BUILD_DIRECTORY})

SET(java_sources
    Abilities.java
    Abomasnow_Forms.java
    Absol_Forms.java
    Aegislash_Forms.java
    Aerodactyl_Forms.java
    Aggron_Forms.java
    Alakazam_Forms.java
    Ampharos_Forms.java
    Arceus_Forms.java
    Banette_Forms.java
    Basculin_Forms.java
    Blastoise_Forms.java
    Blaziken_Forms.java
    Burmy_Forms.java
    Castform_Forms.java
    Charizard_Forms.java
    Cherrim_Forms.java
    Darmanitan_Forms.java
    Deerling_Forms.java
    Deoxys_Forms.java
    Egg_Groups.java
    Flabebe_Forms.java
    Floette_Forms.java
    Florges_Forms.java
    Furfrou_Forms.java
    Garchomp_Forms.java
    Gardevoir_Forms.java
    Gastrodon_Forms.java
    Genders.java
    Genesect_Forms.java
    Gengar_Forms.java
    Giratina_Forms.java
    Gourgeist_Forms.java
    Gyarados_Forms.java
    Heracross_Forms.java
    Hoenn_Ribbons.java
    Houndoom_Forms.java
    Items.java
    Kalos_Ribbons.java
    Kangaskhan_Forms.java
    Keldeo_Forms.java
    Kyurem_Forms.java
    Landorus_Forms.java
    Lucario_Forms.java
    Manectric_Forms.java
    Markings.java
    Mawile_Forms.java
    Medicham_Forms.java
    Meloetta_Forms.java
    Meowstic_Forms.java
    Mewtwo_Forms.java
    Move_Classes.java
    Moves.java
    Natures.java
    Pichu_Forms.java
    Pinsir_Forms.java
    PokeBalls.java
    Pumpkaboo_Forms.java
    Rotom_Forms.java
    Sawsbuck_Forms.java
    Scizor_Forms.java
    Shaymin_Forms.java
    Shellos_Forms.java
    Sinnoh_Ribbons.java
    Species.java
    Stats.java
    Thundurus_Forms.java
    Tornadus_Forms.java
    Types.java
    Tyranitar_Forms.java
    Unova_Ribbons.java
    Unown_Forms.java
    Venusaur_Forms.java
    Version_Groups.java
    Versions.java
    Vivillon_Forms.java
    Wormadam_Forms.java
    Xerneas_Forms.java
)
FOREACH(src ${java_sources})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${src}
        ${CMAKE_CURRENT_BINARY_DIR}/org/nc/LibPKMN/${src}
    COPYONLY)
ENDFOREACH(src ${java_sources})

INCLUDE(UseSWIG)
MACRO(JAVA_GENERATE_FILES swig_source java_module_name depend_module_name do_build)
    FOREACH(dir ${LIBPKMN_JAVA_INCLUDE_DIRS})
        LIST(APPEND CMAKE_SWIG_FLAGS "-I${dir}")
    ENDFOREACH(dir ${LIBPKMN_JAVA_INCLUDE_DIRS})
    SET_SOURCE_FILES_PROPERTIES(${swig_source}.i PROPERTIES CPLUSPLUS ON)

    SET(CMAKE_SWIG_OUTDIR ${JAVA_BUILD_DIRECTORY}/org/nc/LibPKMN)
    SET(CMAKE_SWIG_FLAGS -module ${java_module_name} -package "org.nc" ${CMAKE_GLOBAL_SWIG_FLAGS})
    SWIG_ADD_MODULE(${swig_source} java ${swig_source}.i)
    IF(NOT ${depend_module_name} STREQUAL "none")
        ADD_DEPENDENCIES(${SWIG_MODULE_${swig_source}_REAL_NAME} ${SWIG_MODULE_${depend_module_name}_REAL_NAME})
    ENDIF(NOT ${depend_module_name} STREQUAL "none")
    IF(UNIX)
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES PREFIX "lib")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES SUFFIX ".so")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++0x")
    ELSE()
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES PREFIX "")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES SUFFIX ".dll")
    ENDIF(UNIX)
    SWIG_LINK_LIBRARIES(${swig_source} ${LIBPKMN_JAVA_LIBRARIES})

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

    IF(${do_build} STREQUAL "true")
        ADD_CUSTOM_COMMAND(
            OUTPUT ${JAVA_BUILD_DIRECTORY}/LibPKMN.jar
            COMMENT "Creating LibPKMN.jar"
            COMMAND ${Java_JAVAC_EXECUTABLE} -d ${JAVA_BUILD_DIRECTORY} ${JAVA_BUILD_DIRECTORY}/org/nc/LibPKMN/*.java
            COMMAND ${Java_JAR_EXECUTABLE} cf ${JAVA_BUILD_DIRECTORY}/LibPKMN.jar -C ${JAVA_BUILD_DIRECTORY} org
            DEPENDS ${SWIG_MODULE_${swig_source}_REAL_NAME}
        )
        ADD_CUSTOM_TARGET(
            LibPKMN_jar ALL DEPENDS ${JAVA_BUILD_DIRECTORY}/LibPKMN.jar
        )
        INSTALL(
            FILES ${JAVA_BUILD_DIRECTORY}/LibPKMN.jar
            DESTINATION ${JAVA_INSTALL_DIRECTORY}
            COMPONENT "LibPKMN_Java"
        )
    ENDIF(${do_build} STREQUAL "true")
ENDMACRO(JAVA_GENERATE_FILES)

JAVA_GENERATE_FILES(libpkmn_java_swig Core none false)
JAVA_GENERATE_FILES(calculations_java_swig Calculations libpkmn_java_swig false)
JAVA_GENERATE_FILES(database_java_swig Database calculations_java_swig false)
JAVA_GENERATE_FILES(io_java_swig IO database_java_swig true)
