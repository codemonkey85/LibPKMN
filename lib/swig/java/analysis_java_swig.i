/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("analysis_java_swig");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load. \n" + e);
      System.exit(1);
    }
  }
%}

%include "exception.i"
%include "CamelCase.i"
%import "libpkmn_java_swig.i"

%{
    #include "pkmn/analysis/damage.hpp"
    #include "pkmn/analysis/stats.hpp"
%}

%include "pkmn/analysis/damage.hpp"
%include "pkmn/analysis/stats.hpp"
