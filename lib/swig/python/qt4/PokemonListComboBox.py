#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
# This file was generated: 2014-07-09 20:44:30.941573
#

from PyQt4 import QtGui
import qt4_swig
import sip

class PokemonListComboBox(object):
    def __new__(cls, version_id, parent = 0):
        if isinstance(parent, QtGui.QWidget):
            widget_proxy = qt4_swig.PokemonListComboBoxProxy(version_id, sip.unwrapinstance(parent))
        else:
            widget_proxy = qt4_swig.PokemonListComboBoxProxy(version_id)

        return sip.wrapinstance(widget_proxy.get(), QtGui.QWidget)
