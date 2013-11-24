#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT USED_AS_SUBMODULE) #Let parent project decide these settings
    #Set CPack Generator
    IF(CPACK_GENERATOR)
        #already set
    ELSEIF(APPLE)
        SET(CPACK_GENERATOR PackageMaker)
    ELSEIF(WIN32)
        SET(CPACK_GENERATOR NSIS)
    ELSEIF(LINUX AND EXISTS "/etc/debian_version")
        SET(CPACK_GENERATOR DEB)
    ELSEIF(LINUX AND EXISTS "/etc/redhat-release")
        SET(CPACK_GENERATOR RPM)
    ELSE()
        SET(CPACK_GENERATOR TGZ)
    ENDIF()

    ########################################################################
    # Setup CPack General
    ########################################################################
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Pokémon Simulator")
    SET(CPACK_PACKAGE_VENDOR              "Nicholas Corgan")
    SET(CPACK_PACKAGE_CONTACT             "Nicholas Corgan <n.corgan@gmail.com>")
    SET(CPACK_PACKAGE_VERSION "0.0.1")
    SET(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_SOURCE_DIR}/README.txt)
    SET(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE.txt)

    ########################################################################
    # Setup Windows-specific settings
    ########################################################################
    IF(WIN7_PKG)
        #Install MSVC runtime
        INCLUDE(InstallRequiredSystemLibraries)
    ENDIF(WIN7_PKG)

    IF(${CPACK_GENERATOR} STREQUAL NSIS)
        ENABLE_LANGUAGE(C)

        include(CheckTypeSize)
        check_type_size("void*[8]" BIT_WIDTH BUILTIN_TYPES_ONLY)
        SET(CPACK_PACKAGE_FILE_NAME "PKMNsim_Win${BIT_WIDTH}")

        SET(CPACK_PACKAGE_INSTALL_DIRECTORY "PKMNsim")
    ENDIF()
ENDIF(NOT USED_AS_SUBMODULE)

SET(CPACK_NSIS_MODIFY_PATH ON)

SET(HLKM_ENV "\\\"SYSTEM\\\\CurrentControlSet\\\\Control\\\\Session Manager\\\\Environment\\\"")

SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
    WriteRegStr HKLM ${HLKM_ENV} \\\"PKMNSIM_DATABASE_DIR\\\" \\\"$INSTDIR\\\\share\\\\pkmnsim\\\"
    WriteRegStr HKLM ${HLKM_ENV} \\\"PKMNSIM_IMAGES_DIR\\\" \\\"$INSTDIR\\\\share\\\\pkmnsim\\\\images\\\"
")

SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
    DeleteRegValue HKLM ${HLKM_ENV} \\\"PKMNSIM_DATABASE_DIR\\\"
    DeleteRegValue HKLM ${HLKM_ENV} \\\"PKMNSIM_IMAGES_DIR\\\"
")

INCLUDE(CPack)