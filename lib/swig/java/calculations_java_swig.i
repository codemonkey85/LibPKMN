/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("calculations_java_swig");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load. \n" + e);
      System.exit(1);
    }
  }
%}

%include "exception.i"
%include "stdint.i"
%include "std_pair.i"

%include "CamelCase.i"
%import "libpkmn_java_swig.i"

%{
    #include "pkmn/calculations.hpp"
%}

%include "pkmn/calculations.hpp"
