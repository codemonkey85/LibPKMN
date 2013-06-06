#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FILE(TO_NATIVE_PATH "${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR}/images" IMAGES_DIR)
SET(PKMNSIM_IMAGES_DIR ${IMAGES_DIR} CACHE FILEPATH "Images directory")

ADD_SUBDIRECTORY(icons)
