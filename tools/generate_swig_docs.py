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

def get_javadoc_function_output(cpp_function, class_name=""):
    if cpp_function.has_key("doxygen"):
        return "%" + """javamethodmodifiers %s%s%s() \"
%s
public\";""" % (cpp_function["namespace"], class_name, cpp_function["name"], cpp_function["doxygen"].replace("./*",".\n/*").replace("\"","\\\""))
    else:
        return ""

def get_javadoc_class_output(cpp_class):
    if cpp_class.has_key("doxygen"):
        output_str = ""
        output_str += "%" + """javamethodmodifiers %s::%s \"
%s
public\";""" % (cpp_class["namespace"], cpp_class["name"], cpp_class["doxygen"].replace("./*",".\n/*").replace("\"","\\\""))

        output_str += "\n\n"

        for function in cpp_class["methods"]["public"]:
            if "operator" not in function["name"]:
                output_str += get_javadoc_function_output(function, "%s::" % cpp_class["name"])
                output_str += "\n\n"

        return output_str

    else:
        return ""

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--include-dir", type="string", help="Main C++ include directory")
    parser.add_option("--language", type="string", help="python, cs, or java")
    parser.add_option("--output-dir", type="string", help="Output directory for documentation")
    (options,args) = parser.parse_args()

    # Process options and parse headers
    CppHeaderParser.ignoreSymbols += ["PKMN_API"]
    top_level_dir = os.path.abspath(options.include_dir)
    database_dir = os.path.join(top_level_dir, "database")
    output_dir = os.path.abspath(options.output_dir)
    headers = dict() # CppHeader doesn't store filename, so use dict

    os.chdir(top_level_dir)
    for header in os.listdir(os.getcwd()):
        if header.endswith("hpp") and "enums" not in header and "config" not in header:
            headers[header] = CppHeaderParser.CppHeader(header)

    os.chdir(database_dir)
    for header in os.listdir(os.getcwd()):
        if header.endswith("hpp"):
            headers[header] = CppHeaderParser.CppHeader(header)

    if options.language == "java":
        os.chdir(output_dir)
        f = open("libpkmn_javadocs.i", 'w')

        output_str = ""

        for key in headers:
            for cpp_class in headers[key].classes:
                output_str += get_javadoc_class_output(headers[key].classes[cpp_class])

            for function in headers[key].functions:
                output_str += get_javadoc_function_output(function)

        f.write(output_str)
        f.close()
