/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "PokemonListComboBoxProxy.hpp"

namespace pkmn
{
    PokemonListComboBoxProxy::PokemonListComboBoxProxy(unsigned int version_id, QWidget* parent)
    {
        _qwidget = new qt4::PokemonListComboBox(version_id, parent);
    }

    PokemonListComboBoxProxy::PokemonListComboBoxProxy(std::string &version_name, QWidget* parent)
    {
        _qwidget = new qt4::PokemonListComboBox(version_name, parent);
    }

    PyObject* PokemonListComboBoxProxy::get()
    {
        PyObject* obj = PyLong_FromVoidPtr((void*)_qwidget);
        PyObject* retarg = Py_BuildValue("N", obj);
        return retarg;
    }
}
