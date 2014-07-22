/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_VERSIONSCOMBOBOXPROXY_HPP
#define INCLUDED_VERSIONSCOMBOBOXPROXY_HPP

#include <Python.h>

#include <pkmn/qt4/VersionsComboBox.hpp>

namespace pkmn
{
    class VersionsComboBoxProxy
    {
        public:
            VersionsComboBoxProxy(unsigned int gen, QWidget* parent = NULL);

            PyObject* get();

        private:
            pkmn::qt4::VersionsComboBox* _qwidget;
    };
}
#endif /* INCLUDED_VERSIONSCOMBOBOXPROXY_HPP */
