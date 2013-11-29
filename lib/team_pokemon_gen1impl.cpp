/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <math.h>
#include <time.h>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/database/queries.hpp>

#include "team_pokemon_gen1impl.hpp"

using namespace std;

namespace pkmnsim
{
    team_pokemon_gen1impl::team_pokemon_gen1impl(base_pokemon::sptr base, unsigned int game, unsigned int lvl,
                                                 unsigned int move1, unsigned int move2,
                                                 unsigned int move3, unsigned int move4): team_pokemon_impl(base,game,lvl,
                                                                                          move1,move2,move3,move4)
    {
        srand ( time(NULL) );

        //Random individual values
        ivHP = rand() % 16;
        ivATK = rand() % 16;
        ivDEF = rand() % 16;
        ivSPD = rand() % 16;
        ivSPCL = rand() % 16;

        //Random effort values
        evHP = rand() % 65536;
        evATK = rand() % 65536;
        evDEF = rand() % 65536;
        evSPD = rand() % 65536;
        evSPCL = rand() % 65536;

        HP = get_hp();
        ATK = get_stat(Stats::ATTACK,evATK,ivATK);
        DEF = get_stat(Stats::DEFENSE,evDEF,ivDEF);
        SPD = get_stat(Stats::SPEED,evSPD,ivSPD);
        SPCL = get_stat(Stats::SPECIAL,evSPCL,ivSPCL);
        
        held_item = 0; //Placeholder
        
        icon_path = base_pkmn->get_icon_path(true);
        sprite_path = base_pkmn->get_sprite_path(true, false);

        nonvolatile_status = Statuses::OK;
    }

    string team_pokemon_gen1impl::get_info() const
    {
        switch(get_species_id())
        {
            case Species::NONE:
            case Species::INVALID:
                return "No info";

            default:
                string types_str;
                dict<unsigned int, unsigned int> types = base_pkmn->get_types();
                if(types[1] == Types::NONE) types_str = database::get_type_name(types[0]);
                else types_str = database::get_type_name(types[0]) + "/"
                               + database::get_type_name(types[1]);
                string stats_str = to_string(HP) + ", " + to_string(ATK) + ", "
                                 + to_string(DEF) + ", " + to_string(SPD) + ", "
                                 + to_string(SPCL);

                string output_string;
                output_string = nickname.std_string() + " (" + base_pkmn->get_species_name() + ")\n"
                              + "Level " + to_string(level) + "\n"
                              + "Type: " + types_str + "\n"
                              + "Stats: " + stats_str;

                return output_string;
        }
    }

    string team_pokemon_gen1impl::get_info_verbose() const
    {
        switch(get_species_id())
        {
            case Species::NONE:
            case Species::INVALID:
                return "No info";

            default:
                string types_str;
                dict<unsigned int, unsigned int> types = base_pkmn->get_types();
                if(types[1] == Types::NONE) types_str = database::get_type_name(types[0]);
                else types_str = database::get_type_name(types[0]) + "/"
                               + database::get_type_name(types[1]);

                string output_string;
                output_string = nickname.std_string() + " (" + base_pkmn->get_species_name() + ")\n"
                              + "Level " + to_string(level) + "\n"
                              + "Type: " + types_str + "\n"
                              + "Stats:\n"
                              + " - HP: " + to_string(HP) + "\n"
                              + " - Attack: " + to_string(ATK) + "\n"
                              + " - Defense: " + to_string(DEF) + "\n"
                              + " - Speed: " + to_string(SPD) + "\n"
                              + " - Special: " + to_string(SPCL) + "\n"
                              + "Individual Values:\n"
                              + " - HP: " + to_string(ivHP) + "\n"
                              + " - Attack: " + to_string(ivATK) + "\n"
                              + " - Defense: " + to_string(ivDEF) + "\n"
                              + " - Speed: " + to_string(ivSPD) + "\n"
                              + " - Special: " + to_string(ivSPCL) + "\n"
                              + "Effort Values:\n"
                              + " - HP: " + to_string(evHP) + "\n"
                              + " - Attack: " + to_string(evATK) + "\n"
                              + " - Defense: " + to_string(evDEF) + "\n"
                              + " - Speed: " + to_string(evSPD) + "\n"
                              + " - Special: " + to_string(evSPCL) + "\n";

                return output_string;
        }
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
        stats[Stats::HP] = HP;
        stats[Stats::ATTACK] = ATK;
        stats[Stats::DEFENSE] = DEF;
        stats[Stats::SPEED] = SPD;
        stats[Stats::SPECIAL] = SPCL;

        return stats;
    }

    dict<unsigned int, unsigned int> team_pokemon_gen1impl::get_EVs() const
    {
        dict<unsigned int, unsigned int> EVs;
        EVs[Stats::HP] = evHP;
        EVs[Stats::ATTACK] = evATK;
        EVs[Stats::DEFENSE] = evDEF;
        EVs[Stats::SPEED] = evSPD;
        EVs[Stats::SPECIAL] = evSPCL;

        return EVs;
    }
    
    dict<unsigned int, unsigned int> team_pokemon_gen1impl::get_IVs() const
    {
        dict<unsigned int, unsigned int> IVs;
        IVs[Stats::HP] = ivHP;
        IVs[Stats::ATTACK] = ivATK;
        IVs[Stats::DEFENSE] = ivDEF;
        IVs[Stats::SPEED] = ivSPD;
        IVs[Stats::SPECIAL] = ivSPCL;

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
                evHP = val;
                HP = get_hp();
                break;
                
            case Stats::ATTACK:
                evATK = val;
                ATK = get_stat(Stats::ATTACK, evATK, ivATK);
                break;
                
            case Stats::DEFENSE:
                evDEF = val;
                DEF = get_stat(Stats::DEFENSE, evDEF, ivDEF);
                break;
                
            case Stats::SPEED:
                evSPD = val;
                SPD = get_stat(Stats::SPEED, evSPD, ivSPD);
                break;
            
            case Stats::SPECIAL:
                evSPCL = val;
                SPCL = get_stat(Stats::SPECIAL, evSPCL, ivSPCL);
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
                ivHP = val;
                HP = get_hp();
                break;
                
            case Stats::ATTACK:
                ivATK = val;
                ATK = get_stat(Stats::ATTACK, evATK, ivATK);
                break;
                
            case Stats::DEFENSE:
                ivDEF = val;
                DEF = get_stat(Stats::DEFENSE, evDEF, ivDEF);
                break;
                
            case Stats::SPEED:
                ivSPD = val;
                SPD = get_stat(Stats::SPEED, evSPD, ivSPD);
                break;
            
            case Stats::SPECIAL:
                ivSPCL = val;
                SPCL = get_stat(Stats::SPECIAL, evSPCL, ivSPCL);
                break;
            
            default:
                break;
        }
    }

    std::string team_pokemon_gen1impl::get_form_name() const {return base_pkmn->get_species_name();}
    
    unsigned int team_pokemon_gen1impl::get_form_id() const {return base_pkmn->get_pokemon_id();}
    
    void team_pokemon_gen1impl::set_form(unsigned int form)
    {
        base_pkmn->set_form(form);
        HP = get_hp();
        ATK = get_stat(Stats::ATTACK, evATK, ivATK);
        DEF = get_stat(Stats::DEFENSE, evDEF, ivDEF);
        SPD = get_stat(Stats::SPEED, evSPD, ivSPD);
        SPCL = get_stat(Stats::SPECIAL, evSPCL, ivSPCL);
    }

    void team_pokemon_gen1impl::set_form(std::string form)
    {
        base_pkmn->set_form(form);
        HP = get_hp();
        ATK = get_stat(Stats::ATTACK, evATK, ivATK);
        DEF = get_stat(Stats::DEFENSE, evDEF, ivDEF);
        SPD = get_stat(Stats::SPEED, evSPD, ivSPD);
        SPCL = get_stat(Stats::SPECIAL, evATK, ivATK);
    }

    unsigned int team_pokemon_gen1impl::get_hp() const
    {
        dict<unsigned int, unsigned int> stats = base_pkmn->get_base_stats();

        unsigned int hp_val = int(floor((((double(ivHP) + double(stats[Stats::HP]) + (pow(double(evHP),0.5)/8.0)
                            + 50.0) * double(level))/50.0) + 10.0));
        return hp_val;
    }

    unsigned int team_pokemon_gen1impl::get_stat(unsigned int stat, unsigned int EV, unsigned int IV) const
    {
        dict<unsigned int, unsigned int> stats = base_pkmn->get_base_stats();

        unsigned int stat_val = int(ceil((((double(IV) + double(stats[stat]) + (pow(double(EV),0.5)/8.0))
                              * double(level))/50.0) + 5.0));
        return stat_val;
    }
} /* namespace pkmnsim */