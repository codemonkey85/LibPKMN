/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

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
        _ATK = _get_stat(Stats::ATTACK, _evATK, _ivATK);
        _DEF = _get_stat(Stats::DEFENSE, _evDEF, _ivDEF);
        _SPD = _get_stat(Stats::SPEED, _evSPD, _ivSPD);
        _SATK = _get_stat(Stats::SPECIAL_ATTACK, _evSATK, _ivSATK);
        _SDEF = _get_stat(Stats::SPECIAL_DEFENSE, _evSDEF, _ivSDEF);

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
    
    dict<unsigned int, unsigned int> team_pokemon_modernimpl::get_stats() const
    {
        dict<unsigned int, unsigned int> stats;
        stats[Stats::HP] = _HP;
        stats[Stats::ATTACK] = _ATK;
        stats[Stats::DEFENSE] = _DEF;
        stats[Stats::SPECIAL_ATTACK] = _SATK;
        stats[Stats::SPECIAL_DEFENSE] = _SDEF;
        stats[Stats::SPEED] = _SPD;

        return stats;
    }

    dict<unsigned int, unsigned int> team_pokemon_modernimpl::get_EVs() const
    {
        dict<unsigned int, unsigned int> EVs;
        EVs[Stats::HP] = _evHP;
        EVs[Stats::ATTACK] = _evATK;
        EVs[Stats::DEFENSE] = _evDEF;
        EVs[Stats::SPECIAL_ATTACK] = _evSATK;
        EVs[Stats::SPECIAL_DEFENSE] = _evSDEF;
        EVs[Stats::SPEED] = _evSPD;
        
        return EVs;
    }
    
    dict<unsigned int, unsigned int> team_pokemon_modernimpl::get_IVs() const
    {
        dict<unsigned int, unsigned int> IVs;
        IVs[Stats::HP] = _ivHP;
        IVs[Stats::ATTACK] = _ivATK;
        IVs[Stats::DEFENSE] = _ivDEF;
        IVs[Stats::SPECIAL_ATTACK] = _ivSATK;
        IVs[Stats::SPECIAL_DEFENSE] = _ivSDEF;
        IVs[Stats::SPEED] = _ivSPD;

        return IVs;
    }

    void team_pokemon_modernimpl::set_EV(unsigned int EV, unsigned int val)
    {
        if(val > 255)
        {
            cerr << "Gen 3-5 EV's must be 0-255." << endl;
            exit(EXIT_FAILURE);
        }

        switch(EV)
        {
            case Stats::HP:
                _evHP = val;
                _HP = _get_hp();
                break;
                
            case Stats::ATTACK:
                _evATK = val;
                _ATK = _get_stat(Stats::ATTACK, _evATK, _ivATK);
                break;
                
            case Stats::DEFENSE:
                _evDEF = val;
                _DEF = _get_stat(Stats::DEFENSE, _evDEF, _ivDEF);
                break;
            
            case Stats::SPECIAL_ATTACK:
                _evSATK = val;
                _SATK = _get_stat(Stats::SPECIAL_ATTACK, _evSPCL, _ivSPCL);
                break;
                
            case Stats::SPECIAL_DEFENSE:
                _evSDEF = val;
                _SDEF = _get_stat(Stats::SPECIAL_DEFENSE, _evSPCL, _ivSPCL);
                break;
                
            case Stats::SPEED:
                _evSPD = val;
                _SPD = _get_stat(Stats::SPEED, _evSPD, _ivSPD);
                break;
            
            default:
                break;
        }
    }
    
    void team_pokemon_modernimpl::set_IV(unsigned int IV, unsigned int val)
    {
        if(val > 31)
        {
            cerr << "Gen 3-5 IV's must be 0-31." << endl;
            exit(EXIT_FAILURE);
        }

        switch(IV)
        {
            case Stats::HP:
                _ivHP = val;
                _HP = _get_hp();
                break;
                
            case Stats::ATTACK:
                _ivATK = val;
                _ATK = _get_stat(Stats::ATTACK, _ivATK, _ivATK);
                break;
                
            case Stats::DEFENSE:
                _ivDEF = val;
                _DEF = _get_stat(Stats::DEFENSE, _ivDEF, _ivDEF);
                break;
            
            case Stats::SPECIAL_ATTACK:
                _ivSATK = val;
                _SATK = _get_stat(Stats::SPECIAL_ATTACK, _ivSPCL, _ivSPCL);
                break;
                
            case Stats::SPECIAL_DEFENSE:
                _ivSDEF = val;
                _SDEF = _get_stat(Stats::SPECIAL_DEFENSE, _ivSPCL, _ivSPCL);
                break;
                
            case Stats::SPEED:
                _ivSPD = val;
                _SPD = _get_stat(Stats::SPEED, _ivSPD, _ivSPD);
                break;
            
            default:
                break;
        }
    }

    //TODO: better way of internally dealing with alternate forms
    
    std::string team_pokemon_modernimpl::get_form_name() const
    {
        //Still doesn't account for things like West Sea vs. East Sea Shellos
        //Will need to be done with ugly manual work
        if(_base_pkmn->get_species_id() == _base_pkmn->get_pokemon_id()
        or _base_pkmn->get_species_id() == Species::NONE
        or _base_pkmn->get_species_id() == Species::INVALID) return _base_pkmn->get_species_name();
        else
        {
            string query_string = str(boost::format("SELECT id FROM pokemon_forms WHERE pokemon_id=%d")
                                                    % _base_pkmn->get_pokemon_id());
            int id = _db.execAndGet(query_string.c_str());
            
            query_string = str(boost::format("SELECT pokemon_name FROM pokemon_form_names WHERE pokemon_form_id=%d AND local_language_id=9")
                                             % id);
            return string((const char*)(_db.execAndGet(query_string.c_str())));
        }
    }
    
    unsigned int team_pokemon_modernimpl::get_form_id() const {return _base_pkmn->get_pokemon_id();}
    
    void team_pokemon_modernimpl::set_form(unsigned int form)
    {
        _base_pkmn->set_form(form);
        _HP = _get_hp();
        _ATK = _get_stat(Stats::ATTACK, _evATK, _ivATK);
        _DEF = _get_stat(Stats::DEFENSE, _evDEF, _ivDEF);
        _SPD = _get_stat(Stats::SPEED, _evSPD, _ivSPD);
        _SATK = _get_stat(Stats::SPECIAL_ATTACK, _evSATK, _ivSATK);
        _SDEF = _get_stat(Stats::SPECIAL_DEFENSE, _evSDEF, _ivSDEF);
        _icon_path = _base_pkmn->get_icon_path((_gender != Genders::FEMALE));
        _sprite_path = _base_pkmn->get_sprite_path((_gender != Genders::FEMALE), is_shiny());
        _ability = _determine_ability();
    }

    void team_pokemon_modernimpl::set_form(std::string form)
    {
        _base_pkmn->set_form(form);
        _HP = _get_hp();
        _ATK = _get_stat(Stats::ATTACK, _evATK, _ivATK);
        _DEF = _get_stat(Stats::DEFENSE, _evDEF, _ivDEF);
        _SPD = _get_stat(Stats::SPEED, _evSPD, _ivSPD);
        _SATK = _get_stat(Stats::SPECIAL_ATTACK, _evSATK, _ivSATK);
        _SDEF = _get_stat(Stats::SPECIAL_DEFENSE, _evSDEF, _ivSDEF);
        _icon_path = _base_pkmn->get_icon_path((_gender != Genders::FEMALE));
        _sprite_path = _base_pkmn->get_sprite_path((_gender != Genders::FEMALE), is_shiny());
        _ability = _determine_ability();
    }

    unsigned int team_pokemon_modernimpl::_get_hp() const
    {
        dict<unsigned int, unsigned int> stats = _base_pkmn->get_base_stats();

        unsigned int hp_val = int(floor(((double(_ivHP) + (2.0*double(stats[Stats::HP])) + (0.25*double(_evHP)) + 100.0)
                            * double(_level))/100.0 + 10.0));
        return hp_val;
    }

    unsigned int team_pokemon_modernimpl::_get_stat(unsigned int stat, unsigned int EV, unsigned int IV) const
    {
        dict<unsigned int, unsigned int> stats = _base_pkmn->get_base_stats();
        double _nature_mod = database::get_nature_stat_effect(_nature, stat);

        unsigned int stat_val = int(ceil(((((double(IV) + 2.0*double(stats[stat]) + 0.25*double(EV))
                              * double(_level))/100.0) + 5.0) * _nature_mod));
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
