/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_POKEMON_TEXT_HPP
#define INCLUDED_PKMN_TYPES_POKEMON_TEXT_HPP

#include <iostream>
#include <string>

#include <pkmn/config.hpp>

namespace pkmn
{
    class PKMN_API pokemon_text
    {
        public:
        
            pokemon_text() {};
            pokemon_text(const char* input);
            pokemon_text(const wchar_t* input);
            pokemon_text(std::string input);
            pokemon_text(std::wstring input);
            
            const char* const_char() const;
            const wchar_t* const_wchar_t() const;
            std::string std_string() const;
            std::wstring std_wstring() const;
            
            void set(const char* input);
            void set(const wchar_t* input);
            void set(std::string input);
            void set(std::wstring input);

            size_t length() const;

            //Typecasts
            inline operator std::string() const {return stdstring;}
            inline operator std::wstring() const {return stdwstring;}
            
            const char& operator[](size_t pos) const;

        private:

            std::string stdstring;
            std::wstring stdwstring;
    };
    
    PKMN_API bool operator==(const pokemon_text& l, const pokemon_text& r);
#ifndef SWIG
    PKMN_API std::istream& operator>>(std::istream& stream, pokemon_text& text);
    PKMN_API std::wistream& operator>>(std::wistream& wstream, pokemon_text& text);
    PKMN_API std::ostream& operator<<(std::ostream& stream, const pokemon_text& text);
    PKMN_API std::wostream& operator<<(std::wostream& wstream, const pokemon_text& text);
#endif
}

#endif /* INCLUDED_PKMN_TYPES_POKEMON_TEXT_HPP */
