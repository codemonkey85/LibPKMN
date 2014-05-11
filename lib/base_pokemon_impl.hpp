/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_POKEMON_IMPL_HPP
#define INCLUDED_BASE_POKEMON_IMPL_HPP

//For code cleanliness in source file
#define ICON_PATH(name) fs::path(fs::path(_icon_dir) / name)
#define FEMALE_ICON_PATH(name) fs::path(fs::path(_icon_dir) / "female" / name)
#define SPRITE_PATH(name) fs::path(fs::path(_sprite_dir) / name)
#define SHINY_SPRITE_PATH(name) fs::path(fs::path(_shiny_sprite_dir) / name)
#define FEMALE_SPRITE_PATH(name) fs::path(fs::path(_sprite_dir) / "female" / name)
#define FEMALE_SHINY_SPRITE_PATH(name) fs::path(fs::path(_shiny_sprite_dir) / "female" / name)

#define SET_IMAGES_PATHS(filename) _male_icon_path = ICON_PATH(filename); \
                                   _female_icon_path = _male_icon_path; \
                                   _male_sprite_path = SPRITE_PATH(filename); \
                                   _female_sprite_path = _male_sprite_path; \
                                   _male_shiny_sprite_path = SHINY_SPRITE_PATH(filename); \
                                   _female_shiny_sprite_path = _male_sprite_path;

#define SET_POKEBALL_IMAGE() _male_icon_path = fs::path(_images_dir / "misc" / "pokeball.png"); \
                             _female_icon_path = _male_icon_path; \
                             _male_sprite_path = fs::path(_images_dir / "misc" / "pokeball.png"); \
                             _female_sprite_path = _female_icon_path; \
                             _male_shiny_sprite_path = _male_sprite_path; \
                             _female_shiny_sprite_path = _female_sprite_path;

#define SET_SUBSTITUTE_IMAGE() _male_icon_path = _male_icon_path = SPRITE_PATH("substitute.png"); \
                                                 _female_icon_path = _male_icon_path; \
                                                 _male_sprite_path = SPRITE_PATH("substitute.png"); \
                                                 _female_sprite_path = _female_icon_path; \
                                                 _male_shiny_sprite_path = _male_sprite_path; \
                                                 _female_shiny_sprite_path = _female_sprite_path;

#include <boost/filesystem/path.hpp>

#include <pkmn/base_pokemon.hpp>

#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    class base_pokemon_impl: public base_pokemon
    {
        public:

            base_pokemon_impl(unsigned int species_id, unsigned int game_id);

            //Game-specific info
            std::string get_game() const;
            unsigned int get_generation() const;

            //Non-Battle Attributes
            std::string get_name() const;
            std::string get_species() const;
            unsigned int get_pokedex_num() const;
            std::string get_pokedex_entry() const;
            string_pair_t get_types() const;
            double get_height() const;
            double get_weight() const;
            void get_evolutions(base_pokemon_vector& evolution_vec) const;
			bool is_fully_evolved() const;

            //Battle Stats
            unsigned int get_exp_yield() const;

            //Forms
            std::string get_form() const;

            //Database Info
            unsigned int get_pokemon_id() const;
            unsigned int get_species_id() const;
            unsigned int get_game_id() const;
            unsigned int get_form_id() const;

        protected:

            void _get_evolution_ids(std::vector<unsigned int>& id_vec) const;
            void _use_old_stats();

            unsigned int _generation, _form_id, _pokemon_id, _species_id, _type1_id, _type2_id;
            unsigned int _hp, _attack, _defense, _speed, _special, _special_attack, _special_defense;

            unsigned int _game_id;
            std::string _images_default_basename, _images_game_string, _images_gen_string;
            boost::filesystem::path _sprite_dir, _shiny_sprite_dir;
            boost::filesystem::path _male_icon_path, _female_icon_path;
            boost::filesystem::path _male_sprite_path, _female_sprite_path;
            boost::filesystem::path _male_shiny_sprite_path, _female_shiny_sprite_path;

            static SQLite::Database _db;
            static boost::filesystem::path _images_dir, _icon_dir;
    };
}

#endif /* INCLUDED_BASE_POKEMON_IMPL_HPP */
