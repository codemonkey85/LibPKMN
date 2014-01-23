/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/assign.hpp>
#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>
#include <pkmn/types/prng.hpp>

#include "SQLiteCpp/src/SQLiteC++.h"

#include "team_pokemon_modernimpl.hpp"

using namespace std;

namespace pkmn
{
    team_pokemon_modernimpl::team_pokemon_modernimpl(base_pokemon::sptr base, unsigned int game, unsigned int level,
                                                     unsigned int move1, unsigned int move2,
                                                     unsigned int move3, unsigned int move4): team_pokemon_impl(base,game,level,
                                                                                              move1,move2,move3,move4)
    {
        _generation = database::get_generation(game);

        //Random individual values
        _ivHP = _rand_gen.lcrng_next(_generation) % 32;
        _ivATK = _rand_gen.lcrng_next(_generation) % 32;
        _ivDEF = _rand_gen.lcrng_next(_generation) % 32;
        _ivSPD = _rand_gen.lcrng_next(_generation) % 32;
        _ivSATK = _rand_gen.lcrng_next(_generation) % 32;
        _ivSDEF = _rand_gen.lcrng_next(_generation) % 32;

        /*
         * Random effort values within following rules:
         *  - Sum <= 510
         *  - EV <= 255
         */
        _evHP = 256;
        _evATK = 256;
        _evDEF = 256;
        _evSPD = 256;
        _evSATK = 256;
        _evSDEF = 256;
        while((_evHP+_evATK+_evDEF+_evSPD+_evSATK+_evSDEF)>510 || _evHP>255 || _evATK>255 || _evDEF>255 || _evSPD>255 || _evSATK>255 || _evSDEF>255)
        {
            _evHP = _rand_gen.lcrng_next(_generation) % 256;
            _evATK = _rand_gen.lcrng_next(_generation) % 256;
            _evDEF = _rand_gen.lcrng_next(_generation) % 256;
            _evSPD = _rand_gen.lcrng_next(_generation) % 256;
            _evSATK = _rand_gen.lcrng_next(_generation) % 256;
            _evSDEF = _rand_gen.lcrng_next(_generation) % 256;
        }

        _gender = _determine_gender();
        _nature = _determine_nature();
        _ability = _determine_ability();

        if(_base_pkmn->get_species_id() == Species::SHEDINJA) _HP = 1;
        else _HP = _get_hp();
        _ATK = _get_stat("Attack", _evATK, _ivATK);
        _DEF = _get_stat("Defense", _evDEF, _ivDEF);
        _SPD = _get_stat("Speed", _evSPD, _ivSPD);
        _SATK = _get_stat("Special Attack", _evSATK, _ivSATK);
        _SDEF = _get_stat("Special Defense", _evSDEF, _ivSDEF);

        _icon_path = _base_pkmn->get_icon_path((_gender != Genders::FEMALE));
        _sprite_path = _base_pkmn->get_sprite_path((_gender != Genders::FEMALE), is_shiny());
    }

    void team_pokemon_modernimpl::set_personality(unsigned int personality)
    {
        _personality = personality;
        _ability = _determine_ability();
        _gender = _determine_gender();
        _nature = _determine_nature();
    }
    
    unsigned int team_pokemon_modernimpl::get_ability() const {return _ability;}
    
    unsigned int team_pokemon_modernimpl::get_gender() const {return _gender;}

    unsigned int team_pokemon_modernimpl::get_nature() const {return _nature;}
    
    bool team_pokemon_modernimpl::is_shiny() const
    {
        int p1, p2, E, F;
        p1 = (_personality & 0xFFFF0000) >> 16;
        p2 = _personality & 0xFFFF;
        E = _tid.public_id ^ _tid.secret_id;
        F = p1 ^ p2;
        return (E ^ F) < 8;
    }
    
    void team_pokemon_modernimpl::set_hidden_ability(bool val)
    {
        _has_hidden_ability = val;
        _ability = _determine_ability();
    }
    
    dict<std::string, unsigned int> team_pokemon_modernimpl::get_stats() const
    {
        dict<std::string, unsigned int> stats;
        stats["HP"] = _HP;
        stats["Attack"] = _ATK;
        stats["Defense"] = _DEF;
        stats["Special Attack"] = _SATK;
        stats["Special Defense"] = _SDEF;
        stats["Speed"] = _SPD;

        return stats;
    }

    dict<std::string, unsigned int> team_pokemon_modernimpl::get_EVs() const
    {
        dict<std::string, unsigned int> EVs;
        EVs["HP"] = _evHP;
        EVs["Attack"] = _evATK;
        EVs["Defense"] = _evDEF;
        EVs["Special Attack"] = _evSATK;
        EVs["Special Defense"] = _evSDEF;
        EVs["Speed"] = _evSPD;
        
        return EVs;
    }
    
    dict<std::string, unsigned int> team_pokemon_modernimpl::get_IVs() const
    {
        dict<std::string, unsigned int> IVs;
        IVs["HP"] = _ivHP;
        IVs["Attack"] = _ivATK;
        IVs["Defense"] = _ivDEF;
        IVs["Special Attack"] = _ivSATK;
        IVs["Special Defense"] = _ivSDEF;
        IVs["Speed"] = _ivSPD;

        return IVs;
    }

    void team_pokemon_modernimpl::set_EV(std::string EV, unsigned int val)
    {
        if(val > 255)
        {
            cerr << "Gen 3-5 EV's must be 0-255." << endl;
            exit(EXIT_FAILURE);
        }

        if(EV == "HP")
        {
            _evHP = val;
            _HP = _get_hp();
        }
        else if(EV == "Attack")
        {
            _evATK = val;
            _ATK = _get_stat("Attack", _evATK, _ivATK);
        }
        else if(EV == "Defense")
        {
            _evDEF = val;
            _DEF = _get_stat("Defense", _evDEF, _ivDEF);
        }
        else if(EV == "Speed")
        {
            _evSPD = val;
            _SPD = _get_stat("Speed", _evSPD, _ivSPD);
        }
        else if(EV == "Special Attack")
        {
            _evSATK = val;
            _SATK = _get_stat("Special Attack", _evSATK, _ivSATK);
        }
        else if(EV == "Special Defense")
        {
            _evSDEF = val;
            _SDEF = _get_stat("Special Defense", _evSDEF, _ivSDEF);
        }
    }
    
    void team_pokemon_modernimpl::set_IV(std::string IV, unsigned int val)
    {
        if(val > 31)
        {
            cerr << "Gen 3-5 IV's must be 0-31." << endl;
            exit(EXIT_FAILURE);
        }

        if(IV == "HP")
        {   
            _ivHP = val;
            _HP = _get_hp();
        }   
        else if(IV == "Attack")
        {   
            _ivATK = val;
            _ATK = _get_stat("Attack", _evATK, _ivATK);
        }   
        else if(IV == "Defense")
        {   
            _ivDEF = val;
            _DEF = _get_stat("Defense", _evDEF, _ivDEF);
        }   
        else if(IV == "Speed")
        {   
            _ivSPD = val;
            _SPD = _get_stat("Speed", _evSPD, _ivSPD);
        }   
        else if(IV == "Special Attack")
        {   
            _ivSATK = val;
            _SATK = _get_stat("Special Attack", _evSATK, _ivSATK);
        }   
        else if(IV == "Special Defense")
        {   
            _ivSDEF = val;
            _SDEF = _get_stat("Special Defense", _evSDEF, _ivSDEF);
        }   
    }

    unsigned int team_pokemon_modernimpl::get_form_id() const {return _base_pkmn->get_pokemon_id();}
    
    void team_pokemon_modernimpl::set_form(unsigned int form)
    {
        _base_pkmn->set_form(form);
        _HP = _get_hp();
        _ATK = _get_stat("Attack", _evATK, _ivATK);
        _DEF = _get_stat("Defense", _evDEF, _ivDEF);
        _SPD = _get_stat("Speed", _evSPD, _ivSPD);
        _SATK = _get_stat("Special Attack", _evSATK, _ivSATK);
        _SDEF = _get_stat("Special Defense", _evSDEF, _ivSDEF);
        _icon_path = _base_pkmn->get_icon_path((_gender != Genders::FEMALE));
        _sprite_path = _base_pkmn->get_sprite_path((_gender != Genders::FEMALE), is_shiny());
        _ability = _determine_ability();
    }

    void team_pokemon_modernimpl::set_form(std::string form)
    {
        _base_pkmn->set_form(form);
        _HP = _get_hp();
        _ATK = _get_stat("Attack", _evATK, _ivATK);
        _DEF = _get_stat("Defense", _evDEF, _ivDEF);
        _SPD = _get_stat("Speed", _evSPD, _ivSPD);
        _SATK = _get_stat("Special Attack", _evSATK, _ivSATK);
        _SDEF = _get_stat("Special Defense", _evSDEF, _ivSDEF);
        _icon_path = _base_pkmn->get_icon_path((_gender != Genders::FEMALE));
        _sprite_path = _base_pkmn->get_sprite_path((_gender != Genders::FEMALE), is_shiny());
        _ability = _determine_ability();
    }

    unsigned int team_pokemon_modernimpl::_get_hp() const
    {
        dict<std::string, unsigned int> stats = _base_pkmn->get_base_stats();

        unsigned int hp_val = int(floor(((double(_ivHP) + (2.0*double(stats["HP"])) + (0.25*double(_evHP)) + 100.0)
                            * double(_level))/100.0 + 10.0));
        return hp_val;
    }

    unsigned int team_pokemon_modernimpl::_get_stat(std::string stat, unsigned int EV, unsigned int IV) const
    {
        //TODO: better solution
        dict<std::string, unsigned int> stat_enums = boost::assign::map_list_of
                                                     ("HP", Stats::HP)
                                                     ("Attack", Stats::ATTACK)
                                                     ("Defense", Stats::DEFENSE)
                                                     ("Special Attack", Stats::SPECIAL_ATTACK)
                                                     ("Special Defense", Stats::SPECIAL_DEFENSE)
                                                     ("Speed", Stats::SPEED)
        ;

        dict<std::string, unsigned int> stats = _base_pkmn->get_base_stats();
        double nature_mod = database::get_nature_stat_effect(_nature, stat_enums[stat]);

        unsigned int stat_val = int(ceil(((((double(IV) + 2.0*double(stats[stat]) + 0.25*double(EV))
                              * double(_level))/100.0) + 5.0) * nature_mod));
        return stat_val;
    }

    unsigned int team_pokemon_modernimpl::_determine_ability() const
    {
        string query_string;
    
        if(_base_pkmn->get_species_id() == Species::NONE or _base_pkmn->get_species_id() == Species::INVALID) return Abilities::NONE;
        else if(_has_hidden_ability and _generation >= 5)
        {
            query_string = "SELECT ability_id FROM pokemon_abilities WHERE is_hidden=1 AND pokemon_id=" + to_string(_base_pkmn->get_pokemon_id());
            return int(_db.execAndGet(query_string.c_str()));
        }
        else
        {
            unsigned int ability_num = _personality % 2;
            query_string = "SELECT ability_id FROM pokemon_abilities WHERE slot=2 AND pokemon_id=" + to_string(_base_pkmn->get_pokemon_id());
            SQLite::Statement query(_db, query_string.c_str());
            
            if(query.executeStep() and ability_num == 1) return int(query.getColumn(0));
            else
            {
                query_string = "SELECT ability_id FROM pokemon_abilities WHERE slot=1 AND pokemon_id=" + to_string(_base_pkmn->get_pokemon_id());
                return int(_db.execAndGet(query_string.c_str()));
            }
        }
    }
    
    unsigned int team_pokemon_modernimpl::_determine_gender() const
    {
        if(_base_pkmn->get_chance_male() + _base_pkmn->get_chance_female() == 0
        or _base_pkmn->get_species_id() == Species::NONE
        or _base_pkmn->get_species_id() == Species::INVALID) return Genders::GENDERLESS;
        else if(_base_pkmn->get_chance_male() == 1.0) return Genders::MALE;
        else if(_base_pkmn->get_chance_female() == 1.0) return Genders::FEMALE;
        else
        {
            if((_personality % 256) > int(floor(255*(1-_base_pkmn->get_chance_male())))) return Genders::MALE;
            else return Genders::FEMALE;
        }

        //Should never get here, this stops Clang from complaining
        return Genders::MALE;
    }
    
    unsigned int team_pokemon_modernimpl::_determine_nature() const {return (_personality % 24);}
} /* namespace pkmn */
