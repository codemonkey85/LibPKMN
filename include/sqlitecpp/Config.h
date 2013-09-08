/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_SQLITECPP_CONFIG_HPP
#define INCLUDED_SQLITECPP_CONFIG_HPP

#if defined(_MSC_VER)
    #define SQLITECPP_EXPORT         __declspec(dllexport)
    #define SQLITECPP_IMPORT         __declspec(dllimport)
#elif defined(__GNUG__) && __GNUG__ >= 4
    #define SQLITECPP_EXPORT         __attribute__((visibility("default")))
    #define SQLITECPP_IMPORT         __attribute__((visibility("default")))
#else
    #define SQLITECPP_EXPORT
    #define SQLITECPP_IMPORT
#endif

#ifdef SQLITECPP_DLL_EXPORTS
    #define SQLITECPP_API SQLITECPP_EXPORT
#else
    #define SQLITECPP_API SQLITECPP_IMPORT
#endif

#endif /* INCLUDED_SQLITECPP_CONFIG_HPP */
