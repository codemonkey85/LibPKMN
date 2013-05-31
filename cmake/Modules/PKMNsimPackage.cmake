#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

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
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PKMNsim - Pokémon Simulator")
SET(CPACK_PACKAGE_VENDOR              "Nicholas Corgan")
SET(CPACK_PACKAGE_CONTACT             "Nicholas Corgan <n.corgan@gmail.com>")
SET(CPACK_PACKAGE_VERSION "0.0.1")
SET(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_SOURCE_DIR}/README.txt)
SET(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE.txt)

INCLUDE(CPack)