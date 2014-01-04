/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_TYPES_POKEMON_TEXT_HPP
#define INCLUDED_PKMNSIM_TYPES_POKEMON_TEXT_HPP

#include <string>

#include <pkmn/config.hpp>

namespace pkmn
{
    class PKMNSIM_API pokemon_text
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
            
            //Typecasts
            inline operator std::string() const {return stdstring;}
            inline operator std::wstring() const {return stdwstring;}

        private:
        
            std::string stdstring;
            std::wstring stdwstring;
    };
}

#endif /* INCLUDED_PKMNSIM_TYPES_POKEMON_TEXT_HPP */
