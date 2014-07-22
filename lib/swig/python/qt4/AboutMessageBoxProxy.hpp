/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ABOUTMESSAGEBOXPROXY_HPP
#define INCLUDED_ABOUTMESSAGEBOXPROXY_HPP

#include <Python.h>

#include <pkmn/qt4/AboutMessageBox.hpp>

namespace pkmn
{
    class AboutMessageBoxProxy
    {
        public:
            AboutMessageBoxProxy(QWidget* parent = NULL);

            PyObject* get();

        private:
            pkmn::qt4::AboutMessageBox* _qwidget;
    };
}
#endif /* INCLUDED_ABOUTMESSAGEBOXPROXY_HPP */
