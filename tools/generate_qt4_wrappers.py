#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import datetime
from optparse import OptionParser
import os

import CppHeaderParser

def generate_cpp(header, license):
    class_name = header.classes[header.classes.keys()[0]]["name"]

    f = open("%sProxy.cpp" % class_name, 'w')
    f.write(license + "\n")

    f.write("""
#include \"%sProxy.hpp\"

namespace pkmn
{
    %sProxy::%sProxy(""" % (class_name, class_name, class_name))

    for i,parameter in enumerate(header.classes[class_name]["methods"]["public"][0]["parameters"]):
        if i != 0:
            f.write(", ")
        f.write("%s %s" % (parameter["type"], parameter["name"]))

    f.write(""")
    {
        _qwidget = new pkmn::qt4::%s(""" % class_name)

    for i,parameter in enumerate(header.classes[class_name]["methods"]["public"][0]["parameters"]):
        if i != 0:
            f.write(", ")
        f.write(parameter["name"])

    f.write(""");
    }

    PyObject* %sProxy::get()
    {
        PyObject* obj = PyLong_FromVoidPtr((void*)_qwidget);
        PyObject* retarg = Py_BuildValue("N", obj);
        return retarg;
    }
}""" % class_name)

    f.close()

def generate_hpp(header, license):
    class_name = header.classes[header.classes.keys()[0]]["name"]
    includeguard_name = "%sPROXY" % class_name.upper()

    f = open("%sProxy.hpp" % class_name, 'w')
    f.write(license)

    f.write("""
#ifndef INCLUDED_%s_HPP
#define INCLUDED_%s_HPP

#include <Python.h>
#include <pkmn/qt4/%s.hpp>

namespace pkmn
{
    class %sProxy
    {
        public:
            %sProxy(""" % (includeguard_name, includeguard_name, class_name, class_name, class_name))

    for i,parameter in enumerate(header.classes[class_name]["methods"]["public"][0]["parameters"]):
        if i != 0:
            f.write(", ")
        f.write("%s %s" % (parameter["type"], parameter["name"]))

    f.write(""" = NULL);
            PyObject* get();

        private:
            pkmn::qt4::%s* _qwidget;
    };
}
#endif /* INCLUDED_%s_HPP */""" % (class_name, includeguard_name))

    f.close()

def generate_py(header, license):
    class_name = header.classes[header.classes.keys()[0]]["name"]

    f = open("%s.py" % class_name, 'w')
    f.write(license + "\n")

    f.write("""
from PyQt4 import QtGui
import qt4_swig
import sip

class %s(object):
    def __new__(cls, """ % class_name)

    for i,parameter in enumerate(header.classes[class_name]["methods"]["public"][0]["parameters"]):
        if i != 0:
            f.write(", ")
        f.write(parameter["name"])

    f.write(""" = 0):
        if isinstance(parent, QtGui.QWidget):
            widget_proxy = qt4_swig.%sProxy(""" % class_name)

    for i,parameter in enumerate(header.classes[class_name]["methods"]["public"][0]["parameters"][:-1]):
        if i != 0:
            f.write(", ")
        f.write(parameter["name"])

    f.write(""", sip.unwrapinstance(parent))
        else:
            widget_proxy = qt4_swig.%sProxy(""" % class_name)

    for i,parameter in enumerate(header.classes[class_name]["methods"]["public"][0]["parameters"][:-1]):
        if i != 0:
            f.write(", ")
        f.write(parameter["name"])

    f.write(""")

        return sip.wrapinstance(widget_proxy.get(), QtGui.QWidget)""")

    f.close()

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--qt4-include-dir", type="string", help="Location of Qt4 subclasses")
    parser.add_option("--output-dir", type="string", help="Output directory")
    (options,args) = parser.parse_args()

    time = datetime.datetime.now()

    cpp_license = """/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 *
 * This file was generated: %s
 */""" % time

    python_license = """#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
# This file was generated: %s
#""" % time

    # Process options and parse headers
    CppHeaderParser.ignoreSymbols += ["PKMN_API"]
    headers = []

    os.chdir(options.qt4_include_dir)
    for header in os.listdir(os.getcwd()):
        if header != "CMakeLists.txt":
            headers += [header]

    for header in headers:
        os.chdir(options.qt4_include_dir)
        cppheader = CppHeaderParser.CppHeader(header)
        os.chdir(options.output_dir)
        generate_cpp(cppheader, cpp_license)
        generate_hpp(cppheader, cpp_license)
        generate_py(cppheader, python_license)
