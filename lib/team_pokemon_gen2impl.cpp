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

#include "team_pokemon_gen2impl.hpp"

using namespace std;

namespace pkmnsim
{
    team_pokemon_gen2impl::team_pokemon_gen2impl(base_pokemon::sptr base, unsigned int game, unsigned int lvl,
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

        gender = determine_gender();
        held_item = 0; //Placeholder

        HP = get_hp();
        ATK = get_stat(Stats::ATTACK,evATK,ivATK);
        DEF = get_stat(Stats::DEFENSE,evDEF,ivDEF);
        SPD = get_stat(Stats::SPEED,evSPD,ivSPD);
        SATK = get_stat(Stats::SPECIAL_ATTACK,evSPCL,ivSPCL);
        SDEF = get_stat(Stats::SPECIAL_DEFENSE,evSPCL,ivSPCL);

        icon_path = base_pkmn->get_icon_path(true);
        sprite_path = base_pkmn->get_sprite_path((gender != Genders::FEMALE), is_shiny());

        nonvolatile_status = Statuses::OK;
    }

    string team_pokemon_gen2impl::get_info() const
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
                                 + to_string(DEF) + ", " + to_string(SATK) + ", "
                                 + to_string(SDEF) + ", " + to_string(SPD);

                //Get gender character
                char gender_char;
                switch(gender)
                {
                    case Genders::MALE:
                        gender_char = 'M';
                        break;

                    case Genders::FEMALE:
                        gender_char = 'F';
                        break;

                    default:
                        gender_char = 'N';
                        break;
                }

                string output_string;
                output_string = nickname.std_string() + " (" + base_pkmn->get_species_name() + " " + gender_char + ")\n"
                              + "Level " + to_string(level) + "\n"
                              + "Type: " + types_str + "\n"
                              + "Held Item: " + to_string(held_item) + "\n"
                              + "Stats: " + stats_str;

                return output_string;
        }
    }

    string team_pokemon_gen2impl::get_info_verbose() const
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
                              + "Held Item: " + to_string(held_item) + "\n"
                              + "Stats:\n"
                              + " - HP: " + to_string(HP) + "\n"
                              + " - Attack: " + to_string(ATK) + "\n"
                              + " - Defense: " + to_string(DEF) + "\n"
                              + " - Special Attack: " + to_string(SATK) + "\n"
                              + " - Special Defense: " + to_string(SDEF) + "\n"
                              + " - Speed: " + to_string(SPD) + "\n"
                              + "Individual Values:\n"
                              + " - HP: " + to_string(ivHP) + "\n"
                              + " - Attack: " + to_string(ivATK) + "\n"
                              + " - Defense: " + to_string(ivDEF) + "\n"
                              + " - Special Attack: " + to_string(ivSATK) + "\n"
                              + " - Special Defense: " + to_string(ivSDEF) + "\n"
                              + " - Speed: " + to_string(ivSPD) + "\n"
                              + "Effort Values:\n"
                              + " - HP: " + to_string(evHP) + "\n"
                              + " - Attack: " + to_string(evATK) + "\n"
                              + " - Defense: " + to_string(evDEF) + "\n"
                              + " - Special Attack: " + to_string(evSATK) + "\n"
                              + " - Special Defense: " + to_string(evSDEF) + "\n"
                              + " - Speed: " + to_string(evSPD) + "\n";

                return output_string;
        }
    }

    //No abilities in Gen 2, but don't throw error
    unsigned int team_pokemon_gen2impl::get_ability() const {return Abilities::NONE;}
    
    unsigned int team_pokemon_gen2impl::get_gender() const{return gender;}
    
    //No natures in Gen 2, but don't throw error
    unsigned int team_pokemon_gen2impl::get_nature() const {return Natures::NONE;}

    bool team_pokemon_gen2impl::is_shiny() const
    {
        return (ivSPD == 10 and ivDEF == 10 and ivSPCL == 10 and
                   (ivATK == 2 or ivATK == 3 or ivATK == 6 or
                    ivATK == 7 or ivATK == 10 or ivATK == 11 or
                    ivATK == 14 or ivATK == 15)
               );
    }

    dict<unsigned int, unsigned int> team_pokemon_gen2impl::get_stats() const
    {
        dict<unsigned int, unsigned int> stats;
        stats[Stats::HP] = HP;
        stats[Stats::ATTACK] = ATK;
        stats[Stats::DEFENSE] = DEF;
        stats[Stats::SPECIAL_ATTACK] = SATK;
        stats[Stats::SPECIAL_DEFENSE] = SDEF;
        stats[Stats::SPEED] = SPD;

        return stats;
    }

    dict<unsigned int, unsigned int> team_pokemon_gen2impl::get_EVs() const
    {
        dict<unsigned int, unsigned int> EVs;
        EVs[Stats::HP] = evHP;
        EVs[Stats::ATTACK] = evATK;
        EVs[Stats::DEFENSE] = evDEF;
        EVs[Stats::SPEED] = evSPD;
        EVs[Stats::SPECIAL] = evSATK;
        
        return EVs;
    }
    
    dict<unsigned int, unsigned int> team_pokemon_gen2impl::get_IVs() const
    {
        dict<unsigned int, unsigned int> IVs;
        IVs[Stats::HP] = ivHP;
        IVs[Stats::ATTACK] = ivATK;
        IVs[Stats::DEFENSE] = ivDEF;
        IVs[Stats::SPEED] = ivSPD;
        IVs[Stats::SPECIAL] = ivSATK;

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
                SATK = get_stat(Stats::SPECIAL_ATTACK, evSPCL, ivSPCL);
                SDEF = get_stat(Stats::SPECIAL_DEFENSE, evSPCL, ivSPCL);
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
                SATK = get_stat(Stats::SPECIAL_ATTACK, evSPCL, ivSPCL);
                SDEF = get_stat(Stats::SPECIAL_DEFENSE, evSPCL, ivSPCL);
                break;
            
            default:
                break;
        }
    }

    std::string team_pokemon_gen2impl::get_form_name() const {return base_pkmn->get_species_name();}
    
    unsigned int team_pokemon_gen2impl::get_form_id() const {return base_pkmn->get_pokemon_id();}
    
    void team_pokemon_gen2impl::set_form(unsigned int form)
    {
        base_pkmn->set_form(form);
        HP = get_hp();
        ATK = get_stat(Stats::ATTACK, evATK, ivATK);
        DEF = get_stat(Stats::DEFENSE, evDEF, ivDEF);
        SPD = get_stat(Stats::SPEED, evSPD, ivSPD);
        SATK = get_stat(Stats::SPECIAL_ATTACK, evSPCL, ivSPCL);
        SDEF = get_stat(Stats::SPECIAL_DEFENSE, evSPCL, ivSPCL);
        icon_path = base_pkmn->get_icon_path(true);
        sprite_path = base_pkmn->get_sprite_path((gender != Genders::FEMALE), is_shiny());
    }

    void team_pokemon_gen2impl::set_form(std::string form)
    {
        base_pkmn->set_form(form);
        HP = get_hp();
        ATK = get_stat(Stats::ATTACK, evATK, ivATK);
        DEF = get_stat(Stats::DEFENSE, evDEF, ivDEF);
        SPD = get_stat(Stats::SPEED, evSPD, ivSPD);
        SATK = get_stat(Stats::SPECIAL_ATTACK, evSPCL, ivSPCL);
        SDEF = get_stat(Stats::SPECIAL_DEFENSE, evSPCL, ivSPCL);
        icon_path = base_pkmn->get_icon_path(true);
        sprite_path = base_pkmn->get_sprite_path((gender != Genders::FEMALE), is_shiny());
    }

    unsigned int team_pokemon_gen2impl::get_hp() const
    {
        dict<unsigned int, unsigned int> stats = base_pkmn->get_base_stats();

        unsigned int hp_val = int(floor((((double(ivHP) + double(stats[Stats::HP]) + (pow(evHP,0.5)/8.0)
                            + 50.0) * double(level))/50.0) + 10.0));
        return hp_val;
    }

    unsigned int team_pokemon_gen2impl::get_stat(unsigned int stat, unsigned int EV, unsigned int IV) const
    {
        dict<unsigned int, unsigned int> stats = base_pkmn->get_base_stats();

        unsigned int stat_val = int(ceil((((double(IV) + double(stats[stat]) + (pow(EV,0.5)/8.0))
                                * double(level))/50.0) + 5.0));
        return stat_val;
    }

    unsigned int team_pokemon_gen2impl::determine_gender() const
    {
        if(base_pkmn->get_chance_male() + base_pkmn->get_chance_female() == 0.0) return Genders::GENDERLESS;
        else if(base_pkmn->get_chance_male() == 1.0) return Genders::MALE;
        else if(base_pkmn->get_chance_female() == 1.0) return Genders::FEMALE;
        else
        {
            //Gender is determined by gender ratio and Attack IV
            //If the Attack IV is below a certain number, the Pokemon will be female
            if(base_pkmn->get_chance_male() == 0.875)
            {
                if(ivATK > 1) return Genders::MALE;
                else return Genders::FEMALE;
            }
            else if(base_pkmn->get_chance_male() == 0.75)
            {
                if(ivATK > 3) return Genders::MALE;
                else return Genders::FEMALE;
            }
            else if(base_pkmn->get_chance_male() == 0.5)
            {
                if(ivATK > 7) return Genders::MALE;
                else return Genders::FEMALE;
            }
            else //25% male
            {
                if(ivATK > 11) return Genders::MALE;
                else return Genders::FEMALE;
            }
        }
    }
}
