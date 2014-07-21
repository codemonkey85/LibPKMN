/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ABILITIESCOMBOBOXPROXY_HPP
#define INCLUDED_ABILITIESCOMBOBOXPROXY_HPP

#include <Python.h>

#include <pkmn/qt4/AbilitiesComboBox.hpp>

namespace pkmn
{
    class AbilitiesComboBoxProxy
    {
        public:
            AbilitiesComboBoxProxy(unsigned int gen, QWidget* parent = NULL);

            PyObject* get();

        private:
            pkmn::qt4::AbilitiesComboBox* _qwidget;
    };
}
#endif /* INCLUDED_ABILITIESCOMBOBOXPROXY_HPP */
