/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <QString>

#include <pkmn/types/pokemon_text.hpp>

namespace pkmnsim
{
    pokemon_text::pokemon_text(const char* input) {set(input);}
    pokemon_text::pokemon_text(const wchar_t* input) {set(input);}
    pokemon_text::pokemon_text(std::string input) {set(input);}
    pokemon_text::pokemon_text(std::wstring input) {set(input);}
    
    const char* pokemon_text::const_char() const {return stdstring.c_str();}
    const wchar_t* pokemon_text::const_wchar_t() const {return stdwstring.c_str();}
    std::string pokemon_text::std_string() const {return stdstring;}
    std::wstring pokemon_text::std_wstring() const {return stdwstring;}
    
    void pokemon_text::set(const char* input)
    {
        QString intermediary = QString(input);
        
        stdstring = intermediary.toStdString();
        
        #ifdef _MSC_VER
            stdwstring = std::wstring((const wchar_t*)intermediary.utf16());
        #else
            stdwstring = intermediary.toStdWString();
        #endif
    }
    
    void pokemon_text::set(const wchar_t* input)
    {
        QString intermediary;
    
        #ifdef _MSC_VER
            intermediary = QString::fromUtf16((const ushort*)input);
            stdwstring = std::wstring((const wchar_t*)intermediary.utf16());
        #else
            intermediary = QString::fromWCharArray(input);
            stdwstring = intermediary.toStdWString();
        #endif
        
        stdstring = intermediary.toStdString();
    }
    
    void pokemon_text::set(std::string input)
    {
        QString intermediary = QString::fromStdString(input);
        
        stdstring = intermediary.toStdString();
        
        #ifdef _MSC_VER
            stdwstring = std::wstring((const wchar_t*)intermediary.utf16());
        #else
            stdwstring = intermediary.toStdWString();
        #endif
    }
    
    void pokemon_text::set(std::wstring input)
    {
        QString intermediary;
        
        #ifdef _MSC_VER
            intermediary = QString::fromUtf16((const ushort*)input.c_str());
            stdwstring = std::wstring((const wchar_t*)intermediary.utf16());
        #else
            intermediary = QString::fromStdWString(input);
            stdwstring = intermediary.toStdWString();
        #endif
        
        stdstring = intermediary.toStdString();
    }
} /* namespace pkmnsim */