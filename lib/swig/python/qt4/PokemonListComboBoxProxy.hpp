/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_POKEMONLISTCOMBOBOXPROXY_HPP
#define INCLUDED_POKEMONLISTCOMBOBOXPROXY_HPP

#include <Python.h>

#include <pkmn/qt4/PokemonListComboBox.hpp>

namespace pkmn
{
    class PokemonListComboBoxProxy
    {
        public:
            PokemonListComboBoxProxy(unsigned int version_id, QWidget* parent = NULL);
            PokemonListComboBoxProxy(std::string &version_name, QWidget* parent = NULL);

            PyObject* get();

        private:
            pkmn::qt4::PokemonListComboBox* _qwidget;
    };
}
#endif /* INCLUDED_POKEMONLISTCOMBOBOXPROXY_HPP */
