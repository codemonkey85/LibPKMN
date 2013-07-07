#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
IF(ENABLE_WIN_PYTHON_PKG)
    FILE(TO_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/damage_calculator.py" DAMAGE_CALCULATOR_PY)
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
        ${CMAKE_CURRENT_BINARY_DIR}/setup.py
    @ONLY)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dist/damage_calculator.exe
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/PyPKMNsim
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/lib/swig/python/pypkmnsim_swig.py ${CMAKE_CURRENT_BINARY_DIR}/PyPKMNsim/pypkmnsim_swig.py
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/lib/swig/python/${CMAKE_BUILD_TYPE}/_pypkmnsim_swig.pyd ${CMAKE_CURRENT_BINARY_DIR}/PyPKMNsim/_pypkmnsim_swig.pyd
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/swig/python/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/PyPKMNsim/__init__.py
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/swig/python/Forms.py ${CMAKE_CURRENT_BINARY_DIR}/PyPKMNsim/Forms.py
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/swig/python/Games.py ${CMAKE_CURRENT_BINARY_DIR}/PyPKMNsim/Games.py
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/swig/python/Genders.py ${CMAKE_CURRENT_BINARY_DIR}/PyPKMNsim/Genders.py
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/swig/python/Move_Classes.py ${CMAKE_CURRENT_BINARY_DIR}/PyPKMNsim/Move_Classes.py
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/swig/python/Statuses.py ${CMAKE_CURRENT_BINARY_DIR}/PyPKMNsim/Statuses.py
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/pkmnsim.dll ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/pokehack/lib/${CMAKE_BUILD_TYPE}/pokehack.dll ${CMAKE_CURRENT_BINARY_DIR}/pokehack.dll
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/setup.py py2exe
        DEPENDS ${PYPKMNSIM_REAL_SWIG_NAME}
        COMMENT "Building standalone Python program for damage_calculator"
    )
    ADD_CUSTOM_TARGET(
        damage_calculator ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/dist/damage_calculator.exe
    )
    LIST(APPEND damage_calculator_outputs
        ${CMAKE_CURRENT_BINARY_DIR}/dist/_hashlib.pyd
        ${CMAKE_CURRENT_BINARY_DIR}/dist/bz2.pyd
        ${CMAKE_CURRENT_BINARY_DIR}/dist/damage_calculator.exe
        ${CMAKE_CURRENT_BINARY_DIR}/dist/library.zip
        ${CMAKE_CURRENT_BINARY_DIR}/dist/PyPKMNsim._pypkmnsim_swig.pyd
        ${CMAKE_CURRENT_BINARY_DIR}/dist/select.pyd
        ${CMAKE_CURRENT_BINARY_DIR}/dist/unicodedata.pyd
        ${CMAKE_CURRENT_BINARY_DIR}/dist/w9xpopen.exe
        ${CMAKE_CURRENT_BINARY_DIR}/dist/python27.dll
    )
    INSTALL(FILES ${damage_calculator_outputs} DESTINATION ${RUNTIME_DIR} COMPONENT Runtime)
    
    SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/damage_calculator.exe")
    INSTALL(CODE
        "INCLUDE(BundleUtilities)
        fixup_bundle(\"${APPS}\" \"\" \"\")"
    COMPONENT Runtime)
    SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/w9xpopen.exe")
    INSTALL(CODE
        "fixup_bundle(\"${APPS}\" \"\" \"\")"
    COMPONENT Runtime)
ELSE()
    INSTALL(PROGRAMS damage_calculator.py DESTINATION ${RUNTIME_DIR} COMPONENT Runtime)
ENDIF(ENABLE_WIN_PYTHON_PKG)

