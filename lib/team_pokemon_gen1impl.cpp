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

#include "team_pokemon_gen1impl.hpp"

using namespace std;

namespace pkmn
{
    team_pokemon_gen1impl::team_pokemon_gen1impl(base_pokemon::sptr base, unsigned int game, unsigned int level,
                                                 unsigned int move1, unsigned int move2,
                                                 unsigned int move3, unsigned int move4): team_pokemon_impl(base,game,level,
                                                                                          move1,move2,move3,move4)
    {
        srand ( time(NULL) );

        //Random ind_ividual values
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

        _HP = _get_hp();
        _ATK = _get_stat(Stats::ATTACK, _evATK, _ivATK);
        _DEF = _get_stat(Stats::DEFENSE, _evDEF, _ivDEF);
        _SPD = _get_stat(Stats::SPEED, _evSPD, _ivSPD);
        _SPCL = _get_stat(Stats::SPECIAL, _evSPCL, _ivSPCL);
        
        _icon_path = _base_pkmn->get_icon_path(true);
        _sprite_path = _base_pkmn->get_sprite_path(true, false);
    }

    //No abilities in Gen 1, but don't throw error
    unsigned int team_pokemon_gen1impl::get_ability() const {return Abilities::NONE;}
    
    //Gender doesn't exist in Gen 1, Male is default
    unsigned int team_pokemon_gen1impl::get_gender() const {return Genders::MALE;}
    
    //No natures in Gen 1, but don't throw error
    unsigned int team_pokemon_gen1impl::get_nature() const {return Natures::NONE;}
    
    //No shininess in Gen 1, but don't throw error
    bool team_pokemon_gen1impl::is_shiny() const {return false;}
    
    dict<unsigned int, unsigned int> team_pokemon_gen1impl::get_stats() const
    {
        dict<unsigned int, unsigned int> stats;
        stats[Stats::HP] = _HP;
        stats[Stats::ATTACK] = _ATK;
        stats[Stats::DEFENSE] = _DEF;
        stats[Stats::SPEED] = _SPD;
        stats[Stats::SPECIAL] = _SPCL;

        return stats;
    }

    dict<unsigned int, unsigned int> team_pokemon_gen1impl::get_EVs() const
    {
        dict<unsigned int, unsigned int> EVs;
        EVs[Stats::HP] = _evHP;
        EVs[Stats::ATTACK] = _evATK;
        EVs[Stats::DEFENSE] = _evDEF;
        EVs[Stats::SPEED] = _evSPD;
        EVs[Stats::SPECIAL] = _evSPCL;

        return EVs;
    }
    
    dict<unsigned int, unsigned int> team_pokemon_gen1impl::get_IVs() const
    {
        dict<unsigned int, unsigned int> IVs;
        IVs[Stats::HP] = _ivHP;
        IVs[Stats::ATTACK] = _ivATK;
        IVs[Stats::DEFENSE] = _ivDEF;
        IVs[Stats::SPEED] = _ivSPD;
        IVs[Stats::SPECIAL] = _ivSPCL;

        return IVs;
    }

    void team_pokemon_gen1impl::set_EV(unsigned int EV, unsigned int val)
    {
        if(val > 65535)
        {
            cerr << "Gen 1 EV's must be 0-65535." << endl;
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
                _SPCL = _get_stat(Stats::SPECIAL, _evSPCL, _ivSPCL);
                break;
            
            default:
                break;
        }
    }
    
    void team_pokemon_gen1impl::set_IV(unsigned int IV, unsigned int val)
    {
        if(val > 15)
        {
            cerr << "Gen 1 IV's must be 0-15." << endl;
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
                _SPCL = _get_stat(Stats::SPECIAL, _evSPCL, _ivSPCL);
                break;
            
            default:
                break;
        }
    }

    unsigned int team_pokemon_gen1impl::get_form_id() const {return _base_pkmn->get_pokemon_id();}
    
    void team_pokemon_gen1impl::set_form(unsigned int form)
    {
        _base_pkmn->set_form(form);
        _HP = _get_hp();
        _ATK = _get_stat(Stats::ATTACK, _evATK, _ivATK);
        _DEF = _get_stat(Stats::DEFENSE, _evDEF, _ivDEF);
        _SPD = _get_stat(Stats::SPEED, _evSPD, _ivSPD);
        _SPCL = _get_stat(Stats::SPECIAL, _evSPCL, _ivSPCL);
    }

    void team_pokemon_gen1impl::set_form(std::string form)
    {
        _base_pkmn->set_form(form);
        _HP = _get_hp();
        _ATK = _get_stat(Stats::ATTACK, _evATK, _ivATK);
        _DEF = _get_stat(Stats::DEFENSE, _evDEF, _ivDEF);
        _SPD = _get_stat(Stats::SPEED, _evSPD, _ivSPD);
        _SPCL = _get_stat(Stats::SPECIAL, _evATK, _ivATK);
    }

    unsigned int team_pokemon_gen1impl::_get_hp() const
    {
        dict<unsigned int, unsigned int> stats = _base_pkmn->get_base_stats();

        unsigned int hp_val = int(floor((((double(_ivHP) + double(stats[Stats::HP]) + (pow(double(_evHP),0.5)/8.0)
                            + 50.0) * double(_level))/50.0) + 10.0));
        return hp_val;
    }

    unsigned int team_pokemon_gen1impl::_get_stat(unsigned int stat, unsigned int EV, unsigned int IV) const
    {
        dict<unsigned int, unsigned int> stats = _base_pkmn->get_base_stats();

        unsigned int stat_val = int(ceil((((double(IV) + double(stats[stat]) + (pow(double(EV),0.5)/8.0))
                              * double(_level))/50.0) + 5.0));
        return stat_val;
    }
} /* namespace pkmn */
