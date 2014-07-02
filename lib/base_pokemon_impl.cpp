/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "base_pokemon_impl.hpp"
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

#include "base_pokemon_impl.hpp"
#include "base_pokemon_gen1impl.hpp"
#include "base_pokemon_gen2impl.hpp"
#include "base_pokemon_modernimpl.hpp"

namespace fs = boost::filesystem;

namespace pkmn
{
    base_pokemon::sptr base_pokemon::make(unsigned int species_id, unsigned int game_id)
    {
        //Get generation from game enum
        unsigned int gen = database::get_generation(game_id);

        if(gen < 1 or gen > 6) throw std::runtime_error("Gen must be 1-6.");

        switch(gen)
        {
            case 1:
                return sptr(new base_pokemon_gen1impl(species_id, game_id));

            case 2:
                return sptr(new base_pokemon_gen2impl(species_id, game_id));

            default:
                return sptr(new base_pokemon_modernimpl(species_id, game_id));
        }
    }

    base_pokemon::sptr base_pokemon::make(std::string species, std::string game)
    {
        return make(database::get_species_id(species), database::get_game_id(game));
    }

    pkmn::shared_ptr<SQLite::Database> base_pokemon_impl::_db = NULL;

    base_pokemon_impl::base_pokemon_impl(unsigned int id, unsigned int game_id): base_pokemon()
    {
        //The first initialization will stick for all others
        if(!_db) _db = pkmn::shared_ptr<SQLite::Database>(new SQLite::Database(get_database_path().c_str()));
        _game_id = game_id;
        _generation = database::get_generation(_game_id);

        switch(id)
        {
            case Species::NONE: //None
                _pokemon_id = Species::NONE;
                _species_id = Species::NONE;
                _form_id = 0;
                _pokemon_id = 0;
                _species_id = 0;
                _type1_id = 0;
                _type2_id = 0;
                _hp = 0;
                _attack = 0;
                _defense = 0;
                _speed = 0;
                _special = 0;
                _special_attack = 0;
                _special_defense = 0;
                break;

            case Species::INVALID: //Invalid
                _pokemon_id = Species::INVALID;
                _species_id = Species::INVALID;
                _form_id = 0;
                _pokemon_id = 0;
                _species_id = 0;
                _type1_id = 0;
                _type2_id = 0;
                _hp = 0;
                _attack = 0;
                _defense = 0;
                _speed = 0;
                _special = 0;
                _special_attack = 0;
                _special_defense = 0;
                break;

            default:
                _species_id = id;

                //Get generation and name from specified game enum
                std::string query_string = "SELECT name FROM version_names WHERE version_id=" + to_string(_game_id);

                //Fail if Pokemon's generation_id > specified gen
                query_string = "SELECT generation_id FROM pokemon_species WHERE id=" + to_string(_species_id);
                unsigned int gen_id = _db->execAndGet(query_string.c_str());
                if(gen_id > _generation)
                {
                    std::string error_message = to_string(gen_id) + " > " + to_string(_generation);
                    throw std::runtime_error(error_message.c_str());
                }

                //Get Pokemon ID from database
                query_string = "SELECT id FROM pokemon WHERE species_id=" + to_string(_species_id);
                _pokemon_id = _db->execAndGet(query_string.c_str());

                //Even though most attributes are queried from the database when called, stats take a long time when
                //doing a lot at once, so grab these upon instantiation
                query_string = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" + to_string(_pokemon_id) +
                               " AND stat_id IN (1,2,3,6)";
                SQLite::Statement stats_query(*_db, query_string.c_str());

                stats_query.executeStep();
                _hp = stats_query.getColumn(0);
                stats_query.executeStep();
                _attack = stats_query.getColumn(0);
                stats_query.executeStep();
                _defense = stats_query.getColumn(0);
                stats_query.executeStep();
                _speed = stats_query.getColumn(0);

                query_string = "SELECT type_id FROM pokemon_types WHERE slot=1 and pokemon_id=" + to_string(_pokemon_id);
                _type1_id = _db->execAndGet(query_string.c_str());

                //If there's only one type, this won't return anything
                query_string = "SELECT type_id FROM pokemon_types WHERE slot=2 and pokemon_id=" + to_string(_pokemon_id);
                SQLite::Statement query(*_db, query_string.c_str());
                _type2_id = (query.executeStep()) ? int(query.getColumn(0)) : Types::NONE;
                break;
        }

        _images_dir = fs::path(get_images_dir());
        _icon_dir = fs::path(_images_dir / "pokemon-icons");
        _images_default_basename = to_string(_species_id) + ".png";
        _images_gen_string = "generation-" + to_string(_generation);
    }

    std::string base_pokemon_impl::get_game() const
    {
        std::string query_string = "SELECT name FROM version_names WHERE local_language_id=9 AND version_id="
                                 + to_string(_game_id);
        return (const char*)(_db->execAndGet(query_string.c_str()));
    }

    unsigned int base_pokemon_impl::get_generation() const {return _generation;}

    std::string base_pokemon_impl::get_name() const
    {
        switch(_species_id)
        {
            case Species::NONE:
                return "None";

            case Species::INVALID:
                return "Invalid";

            default:
                return database::get_species_name(_species_id);
        }
    }

    std::string base_pokemon_impl::get_species() const
    {
        switch(_species_id)
        {
            case Species::NONE:
                return "None";

            case Species::INVALID:
                return "Invalid";

            default:
            {
                std::string query_string = "SELECT genus FROM pokemon_species_names WHERE local_language_id=9 AND pokemon_species_id="
                                         + to_string(_species_id);
                return (const char*)(_db->execAndGet(query_string.c_str()));
            }
        }
    }

    unsigned int base_pokemon_impl::get_pokedex_num() const {return _species_id;}
    std::string base_pokemon_impl::get_pokedex_entry() const {return database::get_pokedex_entry(_species_id, _game_id);}

    string_pair_t base_pokemon_impl::get_types() const
    {
        string_pair_t types;
        types.first = database::get_type_name(_type1_id);
        types.second = database::get_type_name(_type2_id);

        return types;
    }

    double base_pokemon_impl::get_height() const
    {
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return 0.0;

            default:
                std::string query_string = "SELECT height FROM pokemon WHERE id=" + to_string(_pokemon_id);
                return (double(_db->execAndGet(query_string.c_str())) / 10.0);
        }
    }

    double base_pokemon_impl::get_weight() const
    {
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return 0.0;

            default:
                std::string query_string = "SELECT weight FROM pokemon WHERE id=" + to_string(_pokemon_id);
                return (double(_db->execAndGet(query_string.c_str())) / 10.0);
        }
    }

    void base_pokemon_impl::get_evolutions(base_pokemon_vector &evolution_vec) const
    {
        evolution_vec.clear();

        std::vector<unsigned int> id_vec;
        _get_evolution_ids(id_vec);

        if(id_vec.begin() == id_vec.end()) evolution_vec.push_back(make(pkmn::Species::NONE, _game_id));
        else
        {
            for(size_t i = 0; i < id_vec.size(); i++) evolution_vec.push_back(make(id_vec[i], _game_id));
        }
    }

    bool base_pokemon_impl::is_fully_evolved() const
    {
        std::vector<unsigned int> id_vec;
        _get_evolution_ids(id_vec);

        return (id_vec.begin() == id_vec.end());
    }

    unsigned int base_pokemon_impl::get_exp_yield() const
    {
        //Check for exceptions not covered in database
        if(_generation == 4 and _species_id == Species::ABRA) return 75;
        else if(_generation == 4 and _species_id == Species::MACHOP) return 75;
        else if(_generation == 4 and _species_id == Species::GEODUDE) return 73;
        else if(_generation == 4 and _species_id == Species::OMANYTE) return 99;
        else if(_generation == 4 and _species_id == Species::KABUTO) return 99;
        else if(_generation == 4 and _species_id == Species::KABUTOPS) return 199;
        else if(_generation == 4 and _species_id == Species::DUNSPARCE) return 125;
        else if(_generation == 4 and _species_id == Species::LILEEP) return 99;
        else if(_generation == 4 and _species_id == Species::CRADILY) return 199;
        else if(_generation == 4 and _species_id == Species::ANORITH) return 99;
        else if(_generation == 4 and _species_id == Species::ARMALDO) return 199;
        else
        {
            std::string query_string;
            if(_generation < 4 and _species_id <= 439) query_string = "SELECT exp_yield FROM old_exp_yields WHERE species_id=" + to_string(_species_id);
            else query_string = "SELECT base_experience FROM pokemon WHERE species_id=" + to_string(_species_id);
            return int(_db->execAndGet(query_string.c_str()));
        }
    }

    std::string base_pokemon_impl::get_form() const
    {
        if(_form_id == _species_id) return "Standard";
        else
        {
            std::string query_string = "SELECT form_name FROM pokemon_form_names WHERE local_language_id=9 AND pokemon_form_id="
                                     + to_string(_form_id);
            return (const char*)(_db->execAndGet(query_string.c_str()));
        }
    }

    unsigned int base_pokemon_impl::get_pokemon_id() const {return _pokemon_id;}

    unsigned int base_pokemon_impl::get_species_id() const {return _species_id;}

    unsigned int base_pokemon_impl::get_game_id() const {return _game_id;}

    unsigned int base_pokemon_impl::get_form_id() const {return _form_id;}

    void base_pokemon_impl::_get_evolution_ids(std::vector<unsigned int>& id_vec) const
    {
        id_vec.clear();

        if(_species_id != Species::NONE and _species_id != Species::INVALID)
        {
            std::string query_string;

            std::vector<int> to_erase;
            query_string = "SELECT id FROM pokemon_species WHERE evolves_from_species_id=" + to_string(_species_id);
            SQLite::Statement query(*_db, query_string.c_str());
            while(query.executeStep())
            {
                int evol_id = query.getColumn(0); //id
                id_vec.push_back(evol_id);
            }

            //Evolutions may not be present in specified generation
            for(unsigned int i = 0; i < id_vec.size(); i++)
            {
                query_string = "SELECT generation_id FROM pokemon_species WHERE id=" + to_string(id_vec[i]);
                unsigned int generation_id = _db->execAndGet(query_string.c_str());
                if(generation_id > _generation) to_erase.push_back(i);
            }
            for(int j = to_erase.size()-1; j >= 0; j--) id_vec.erase(id_vec.begin() + to_erase[j]);
        }
    }

    /*
     * There are several Pokémon whose stats changed in Generation VI. This function sets the old stats
     * if the Pokémon is from a prior generation.
     */
    void base_pokemon_impl::_use_old_stats()
    {
        //TODO: better way without unnecessary database queries?
        if(_species_id == Species::BUTTERFREE) _special_attack = 80;
        else if(_species_id == Species::BEEDRILL) _attack = 80;
        else if(_species_id == Species::PIDGEOT) _speed = 91;
        else if(_species_id == Species::PIKACHU)
        {
            _defense = 30;
            _special_defense = 40;
        }
        else if(_species_id == Species::RAICHU) _speed = 100;
        else if(_species_id == Species::NIDOQUEEN) _attack = 82;
        else if(_species_id == Species::NIDOKING) _attack = 92;
        else if(_species_id == Species::CLEFABLE) _special_attack = 85;
        else if(_species_id == Species::WIGGLYTUFF) _special_attack = 75;
        else if(_species_id == Species::VILEPLUME) _special_attack = 100;
        else if(_species_id == Species::POLIWRATH) _attack = 85;
        else if(_species_id == Species::ALAKAZAM) _special_defense = 85;
        else if(_species_id == Species::VICTREEBEL) _special_defense = 60;
        else if(_species_id == Species::GOLEM) _attack = 210;
        else if(_species_id == Species::AMPHAROS) _defense = 75;
        else if(_species_id == Species::BELLOSSOM) _defense = 85;
        else if(_species_id == Species::AZUMARILL) _special_attack = 50;
        else if(_species_id == Species::JUMPLUFF) _special_defense = 85;
        else if(_species_id == Species::BEAUTIFLY) _special_attack = 90;
        else if(_species_id == Species::EXPLOUD) _special_defense = 63;
        else if(_species_id == Species::STARAPTOR) _special_defense = 50;
        else if(_species_id == Species::ROSERADE) _defense = 55;
        else if(_species_id == Species::STOUTLAND) _attack = 100;
        else if(_species_id == Species::UNFEZANT) _attack = 105;
        else if(_species_id == Species::GIGALITH) _special_defense = 70;
        else if(_species_id == Species::SEISMITOAD) _attack = 85;
        else if(_species_id == Species::LEAVANNY) _special_defense = 70;
        else if(_species_id == Species::SCOLIPEDE) _attack = 90;
        else if(_species_id == Species::KROOKODILE) _defense = 70;
    }
} /* namespace pkmn */
