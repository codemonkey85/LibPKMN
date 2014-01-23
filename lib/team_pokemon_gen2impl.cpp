/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
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
        _ATK = _get_stat("Attack", _evATK, _ivATK);
        _DEF = _get_stat("Defense", _evDEF, _ivDEF);
        _SPD = _get_stat("Speed", _evSPD, _ivSPD);
        _SATK = _get_stat("Special Attack", _evSPCL, _ivSPCL);
        _SDEF = _get_stat("Special Defense", _evSPCL, _ivSPCL);

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

    dict<std::string, unsigned int> team_pokemon_gen2impl::get_stats() const
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

    dict<std::string, unsigned int> team_pokemon_gen2impl::get_EVs() const
    {
        dict<std::string, unsigned int> EVs;
        EVs["HP"] = _evHP;
        EVs["Attack"] = _evATK;
        EVs["Defense"] = _evDEF;
        EVs["Speed"] = _evSPD;
        EVs["Special"] = _evSPCL;
        
        return EVs;
    }
    
    dict<std::string, unsigned int> team_pokemon_gen2impl::get_IVs() const
    {
        dict<std::string, unsigned int> IVs;
        IVs["HP"] = _ivHP;
        IVs["Attack"] = _ivATK;
        IVs["Defense"] = _ivDEF;
        IVs["Speed"] = _ivSPD;
        IVs["Special"] = _ivSPCL;

        return IVs;
    }

    void team_pokemon_gen2impl::set_EV(std::string EV, unsigned int val)
    {
        if(val > 65535)
        {
            cerr << "Gen 2 EV's must be 0-65535." << endl;
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
        else if(EV == "Special")
        {
            _evSPCL = val;
            _SATK = _get_stat("Special Attack", _evSPCL, _ivSPCL);
            _SDEF = _get_stat("Special Defense", _evSPCL, _ivSPCL);
        }
    }
    
    void team_pokemon_gen2impl::set_IV(std::string IV, unsigned int val)
    {
        if(val > 15)
        {
            cerr << "Gen 2 IV's must be 0-15." << endl;
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
        else if(IV == "Special")
        {
            _ivSPCL = val;
            _SATK = _get_stat("Special Attack", _evSPCL, _ivSPCL);
            _SDEF = _get_stat("Special Defense", _evSPCL, _ivSPCL);
        }
    }

    unsigned int team_pokemon_gen2impl::get_form_id() const {return _base_pkmn->get_pokemon_id();}
    
    void team_pokemon_gen2impl::set_form(unsigned int form)
    {
    }

    void team_pokemon_gen2impl::set_form(std::string form)
    {
    }

    unsigned int team_pokemon_gen2impl::_get_hp() const
    {
        dict<std::string, unsigned int> stats = _base_pkmn->get_base_stats();

        unsigned int hp_val = int(floor((((double(_ivHP) + double(stats["HP"]) + (pow(_evHP,0.5)/8.0)
                            + 50.0) * double(_level))/50.0) + 10.0));
        return hp_val;
    }

    unsigned int team_pokemon_gen2impl::_get_stat(std::string stat, unsigned int EV, unsigned int IV) const
    {
        dict<std::string, unsigned int> stats = _base_pkmn->get_base_stats();

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
