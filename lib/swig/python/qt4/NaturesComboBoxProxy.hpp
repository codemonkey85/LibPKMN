/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_NATURESCOMBOBOXPROXY_HPP
#define INCLUDED_NATURESCOMBOBOXPROXY_HPP

#include <Python.h>

#include <pkmn/qt4/NaturesComboBox.hpp>

namespace pkmn
{
    class NaturesComboBoxProxy
    {
        public:
            NaturesComboBoxProxy(QWidget* parent = NULL);

            PyObject* get();

        private:
            pkmn::qt4::NaturesComboBox* _qwidget;
    };
}
#endif /* INCLUDED_NATURESCOMBOBOXPROXY_HPP */
