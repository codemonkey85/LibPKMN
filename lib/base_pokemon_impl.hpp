/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_POKEMON_IMPL_HPP
#define INCLUDED_BASE_POKEMON_IMPL_HPP

#include <pkmnsim/base_pokemon.hpp>

namespace pkmnsim
{
    class base_pokemon_impl: public base_pokemon
    {
        public:

            base_pokemon_impl(unsigned int species_id, unsigned int game_id);

            unsigned int get_pokedex_num() const;
            std::string get_pokedex_entry() const;
            dict<unsigned int, unsigned int> get_types() const;
            double get_height() const;
            double get_weight() const;
			void get_evolutions(std::vector<sptr> &evolution_vec) const;
			bool is_fully_evolved() const;
            unsigned int get_generation() const;

            std::string get_species_name() const;
            
            unsigned int get_game_id() const;
            unsigned int get_pokemon_id() const;
            unsigned int get_species_id() const;
    };
}

#endif /* INCLUDED_BASE_POKEMON_IMPL_HPP */
