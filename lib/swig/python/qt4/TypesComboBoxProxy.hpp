/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TYPESCOMBOBOXPROXY_HPP
#define INCLUDED_TYPESCOMBOBOXPROXY_HPP

#include <Python.h>

#include <pkmn/qt4/TypesComboBox.hpp>

namespace pkmn
{
    class TypesComboBoxProxy
    {
        public:
            TypesComboBoxProxy(unsigned int gen, QWidget* parent = NULL);

            PyObject* get();

        private:
            pkmn::qt4::TypesComboBox* _qwidget;
    };
}
#endif /* INCLUDED_TYPESCOMBOBOXPROXY_HPP */
