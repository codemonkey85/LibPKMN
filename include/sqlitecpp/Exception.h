/**
 * @file  Exception.h
 * @brief Encapsulation of the error message from SQLite3 on a std::runtime_error.
 *
 * Copyright (c) 2012 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 * Copyright (c) 2012 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef EXCEPTION_H
#define EXCEPTION_H

#pragma once

#include <stdexcept>

#include <sqlitecpp/Config.h>

#ifdef _WIN32
#pragma warning(disable:4290) // Disable warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#endif

namespace SQLite
{

/**
 * @brief Encapsulation of the error message from SQLite3, based on std::runtime_error.
 */
class SQLITECPP_API Exception : public std::runtime_error
{
public:
    Exception(const std::string& aErrorMessage) :
        std::runtime_error(aErrorMessage)
    {
    }
};


}  // namespace SQLite
#endif /*EXCEPTION_H*/
