/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_CONFIG_HPP
#define INCLUDED_PKMNSIM_CONFIG_HPP

//Alternative to BOOST_FOREACH
#define FOREACH(VAR, RANGE) \
  for (auto _foreach_range = (RANGE); !_foreach_range.empty(); _foreach_range.pop_front()) \
    if (bool _foreach_inner = false) {} else \
      for (VAR = _foreach_range.front(); !_foreach_inner; _foreach_inner = true)

//Macro to convert number to std::string
#include <sstream>
template <class T>
inline std::string to_string (const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

// define logical operators
#include <ciso646>

//define cross platform attribute macros
#if defined(_MSC_VER)
    #define PKMNSIM_EXPORT         __declspec(dllexport)
    #define PKMNSIM_IMPORT         __declspec(dllimport)
    #define PKMNSIM_INLINE         __forceinline
    #define PKMNSIM_DEPRECATED     __declspec(deprecated)
    #define PKMNSIM_ALIGNED(x)     __declspec(align(x))
    # pragma warning(disable: 4251) // class 'A<T>' needs to have dll-interface to be used by clients of class 'B'
#elif defined(__GNUG__) && __GNUG__ >= 4
    #define PKMNSIM_EXPORT         __attribute__((visibility("default")))
    #define PKMNSIM_IMPORT         __attribute__((visibility("default")))
    #define PKMNSIM_INLINE         inline __attribute__((always_inline))
    #define PKMNSIM_DEPRECATED     __attribute__((deprecated))
    #define PKMNSIM_ALIGNED(x)     __attribute__((aligned(x)))
#else
    #define PKMNSIM_EXPORT
    #define PKMNSIM_IMPORT
    #define PKMNSIM_INLINE         inline
    #define PKMNSIM_DEPRECATED
    #define PKMNSIM_ALIGNED(x)
#endif

// Define API declaration macro
#ifdef PKMNSIM_DLL_EXPORTS
    #define PKMNSIM_API PKMNSIM_EXPORT
#else
    #define PKMNSIM_API PKMNSIM_IMPORT
#endif // PKMNSIM_DLL_EXPORTS

// Platform defines for conditional parts of headers:
#if defined(linux) || defined(__linux) || defined(__linux__)
    #define PKMNSIM_PLATFORM_LINUX
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #define PKMNSIM_PLATFORM_WIN32
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
    #define PKMNSIM_PLATFORM_MACOS
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    #define PKMNSIM_PLATFORM_BSD
#endif

#endif /* INCLUDED_PKMNSIM_CONFIG_HPP */
