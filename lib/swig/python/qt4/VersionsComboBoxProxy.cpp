/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "VersionsComboBoxProxy.hpp"

namespace pkmn
{
    VersionsComboBoxProxy::VersionsComboBoxProxy(unsigned int gen, QWidget* parent)
    {
        _qwidget = new qt4::VersionsComboBox(gen, parent);
    }

    PyObject* VersionsComboBoxProxy::get()
    {
        PyObject* obj = PyLong_FromVoidPtr((void*)_qwidget);
        PyObject* retarg = Py_BuildValue("N", obj);
        return retarg;
    }
}
