/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cmath>
#include <ctime>

#include <pkmn/enums.hpp>
#include <pkmn/database/queries.hpp>

#include "team_pokemon_gen2impl.hpp"

using namespace std;

namespace pkmn
{
    team_pokemon_gen2impl::team_pokemon_gen2impl(base_pokemon::sptr base, unsigned int game, unsigned int level,
                                                 unsigned int move1, unsigned int move2,
                                                 unsigned int move3, unsigned int move4): team_pokemon_impl(base,game,level,
                                                                                          move1,move2,move3,move4)
    {
        srand ( time(NULL) );

        //Random individual values
        _ivHP = rand() % 16;
        _ivATK = rand() % 16;
        _ivDEF = rand() % 16;
        _ivSPD = rand() % 16;
        _ivSPCL = rand() % 16;

        //Random effort values
        _evHP = rand() % 65536;
        _evATK = rand() % 65536;
        _evDEF = rand() % 65536;
        _evSPD = rand() % 65536;
        _evSPCL = rand() % 65536;

        _gender = _determine_gender();

        _HP = _get_hp();
        _ATK = _get_stat(Stats::ATTACK, _evATK, _ivATK);
        _DEF = _get_stat(Stats::DEFENSE, _evDEF, _ivDEF);
        _SPD = _get_stat(Stats::SPEED, _evSPD, _ivSPD);
        _SATK = _get_stat(Stats::SPECIAL_ATTACK, _evSPCL, _ivSPCL);
        _SDEF = _get_stat(Stats::SPECIAL_DEFENSE, _evSPCL, _ivSPCL);

        _icon_path = _base_pkmn->get_icon_path(true);
        _sprite_path = _base_pkmn->get_sprite_path((_gender != Genders::FEMALE), is_shiny());
    }

    //No abilities in Gen 2, but don't throw error
    unsigned int team_pokemon_gen2impl::get_ability() const {return Abilities::NONE;}
    
    unsigned int team_pokemon_gen2impl::get_gender() const{return _gender;}
    
    //No natures in Gen 2, but don't throw error
    unsigned int team_pokemon_gen2impl::get_nature() const {return Natures::NONE;}

    bool team_pokemon_gen2impl::is_shiny() const
    {
        return (_ivSPD == 10 and _ivDEF == 10 and _ivSPCL == 10 and
                   (_ivATK == 2 or _ivATK == 3 or _ivATK == 6 or
                    _ivATK == 7 or _ivATK == 10 or _ivATK == 11 or
                    _ivATK == 14 or _ivATK == 15)
               );
    }

    dict<unsigned int, unsigned int> team_pokemon_gen2impl::get_stats() const
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

    dict<unsigned int, unsigned int> team_pokemon_gen2impl::get_EVs() const
    {
        dict<unsigned int, unsigned int> EVs;
        EVs[Stats::HP] = _evHP;
        EVs[Stats::ATTACK] = _evATK;
        EVs[Stats::DEFENSE] = _evDEF;
        EVs[Stats::SPEED] = _evSPD;
        EVs[Stats::SPECIAL] = _evSATK;
        
        return EVs;
    }
    
    dict<unsigned int, unsigned int> team_pokemon_gen2impl::get_IVs() const
    {
        dict<unsigned int, unsigned int> IVs;
        IVs[Stats::HP] = _ivHP;
        IVs[Stats::ATTACK] = _ivATK;
        IVs[Stats::DEFENSE] = _ivDEF;
        IVs[Stats::SPEED] = _ivSPD;
        IVs[Stats::SPECIAL] = _ivSATK;

        return IVs;
    }

    void team_pokemon_gen2impl::set_EV(unsigned int EV, unsigned int val)
    {
        if(val > 65535)
        {
            cerr << "Gen 2 EV's must be 0-65535." << endl;
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
            
            case Stats::SPEED:
                _evSPD = val;
                _SPD = _get_stat(Stats::SPEED, _evSPD, _ivSPD);
                break;
            
            case Stats::SPECIAL:
                _evSPCL = val;
                _SATK = _get_stat(Stats::SPECIAL_ATTACK, _evSPCL, _ivSPCL);
                _SDEF = _get_stat(Stats::SPECIAL_DEFENSE, _evSPCL, _ivSPCL);
                break;
            
            default:
                break;
        }
    }
    
    void team_pokemon_gen2impl::set_IV(unsigned int IV, unsigned int val)
    {
        if(val > 15)
        {
            cerr << "Gen 2 IV's must be 0-15." << endl;
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
                _ATK = _get_stat(Stats::ATTACK, _evATK, _ivATK);
                break;
                
            case Stats::DEFENSE:
                _ivDEF = val;
                _DEF = _get_stat(Stats::DEFENSE, _evDEF, _ivDEF);
                break;
                
            case Stats::SPEED:
                _ivSPD = val;
                _SPD = _get_stat(Stats::SPEED, _evSPD, _ivSPD);
                break;
            
            case Stats::SPECIAL:
                _ivSPCL = val;
                _SATK = _get_stat(Stats::SPECIAL_ATTACK, _evSPCL, _ivSPCL);
                _SDEF = _get_stat(Stats::SPECIAL_DEFENSE, _evSPCL, _ivSPCL);
                break;
            
            default:
                break;
        }
    }

    std::string team_pokemon_gen2impl::get_form_name() const {return _base_pkmn->get_species_name();}
    
    unsigned int team_pokemon_gen2impl::get_form_id() const {return _base_pkmn->get_pokemon_id();}
    
    void team_pokemon_gen2impl::set_form(unsigned int form)
    {
        _base_pkmn->set_form(form);
        _HP = _get_hp();
        _ATK = _get_stat(Stats::ATTACK, _evATK, _ivATK);
        _DEF = _get_stat(Stats::DEFENSE, _evDEF, _ivDEF);
        _SPD = _get_stat(Stats::SPEED, _evSPD, _ivSPD);
        _SATK = _get_stat(Stats::SPECIAL_ATTACK, _evSPCL, _ivSPCL);
        _SDEF = _get_stat(Stats::SPECIAL_DEFENSE, _evSPCL, _ivSPCL);
        _icon_path = _base_pkmn->get_icon_path(true);
        _sprite_path = _base_pkmn->get_sprite_path((_gender != Genders::FEMALE), is_shiny());
    }

    void team_pokemon_gen2impl::set_form(std::string form)
    {
        _base_pkmn->set_form(form);
        _HP = _get_hp();
        _ATK = _get_stat(Stats::ATTACK, _evATK, _ivATK);
        _DEF = _get_stat(Stats::DEFENSE, _evDEF, _ivDEF);
        _SPD = _get_stat(Stats::SPEED, _evSPD, _ivSPD);
        _SATK = _get_stat(Stats::SPECIAL_ATTACK, _evSPCL, _ivSPCL);
        _SDEF = _get_stat(Stats::SPECIAL_DEFENSE, _evSPCL, _ivSPCL);
        _icon_path = _base_pkmn->get_icon_path(true);
        _sprite_path = _base_pkmn->get_sprite_path((_gender != Genders::FEMALE), is_shiny());
    }

    unsigned int team_pokemon_gen2impl::_get_hp() const
    {
        dict<unsigned int, unsigned int> stats = _base_pkmn->get_base_stats();

        unsigned int hp_val = int(floor((((double(_ivHP) + double(stats[Stats::HP]) + (pow(_evHP,0.5)/8.0)
                            + 50.0) * double(_level))/50.0) + 10.0));
        return hp_val;
    }

    unsigned int team_pokemon_gen2impl::_get_stat(unsigned int stat, unsigned int EV, unsigned int IV) const
    {
        dict<unsigned int, unsigned int> stats = _base_pkmn->get_base_stats();

        unsigned int stat_val = int(ceil((((double(IV) + double(stats[stat]) + (pow(EV,0.5)/8.0))
                                * double(_level))/50.0) + 5.0));
        return stat_val;
    }

    unsigned int team_pokemon_gen2impl::_determine_gender() const
    {
        if(_base_pkmn->get_chance_male() + _base_pkmn->get_chance_female() == 0.0) return Genders::GENDERLESS;
        else if(_base_pkmn->get_chance_male() == 1.0) return Genders::MALE;
        else if(_base_pkmn->get_chance_female() == 1.0) return Genders::FEMALE;
        else
        {
            //Gender is determined by gender ratio and Attack IV
            //If the Attack IV is below a certain number, the Pokemon will be female
            if(_base_pkmn->get_chance_male() == 0.875)
            {
                if(_ivATK > 1) return Genders::MALE;
                else return Genders::FEMALE;
            }
            else if(_base_pkmn->get_chance_male() == 0.75)
            {
                if(_ivATK > 3) return Genders::MALE;
                else return Genders::FEMALE;
            }
            else if(_base_pkmn->get_chance_male() == 0.5)
            {
                if(_ivATK > 7) return Genders::MALE;
                else return Genders::FEMALE;
            }
            else //25% male
            {
                if(_ivATK > 11) return Genders::MALE;
                else return Genders::FEMALE;
            }
        }
    }
} /* namespace pkmn */
