/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <math.h>
#include <time.h>

#include <boost/format.hpp>

#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/database/queries.hpp>

#include <sqlitecpp/SQLiteCPP.h>

#include "team_pokemon_gen345impl.hpp"

using namespace std;

namespace pkmnsim
{
    team_pokemon_gen345impl::team_pokemon_gen345impl(base_pokemon::sptr base, unsigned int game, unsigned int lvl,
                                                     unsigned int move1, unsigned int move2,
                                                     unsigned int move3, unsigned int move4): team_pokemon_impl(base,game,lvl,
                                                                                              move1,move2,move3,move4)
    {
        srand ( time(NULL) );

        //Random individual values
        ivHP = rand() % 32;
        ivATK = rand() % 32;
        ivDEF = rand() % 32;
        ivSPD = rand() % 32;
        ivSATK = rand() % 32;
        ivSDEF = rand() % 32;

        /*
         * Random effort values within following rules:
         *  - Sum <= 510
         *  - EV <= 255
         */
        evHP = 256;
        evATK = 256;
        evDEF = 256;
        evSPD = 256;
        evSATK = 256;
        evSDEF = 256;
        while((evHP+evATK+evDEF+evSPD+evSATK+evSDEF)>510 || evHP>255 || evATK>255 || evDEF>255 || evSPD>255 || evSATK>255 || evSDEF>255)
        {
            evHP = rand() % 256;
            evATK = rand() % 256;
            evDEF = rand() % 256;
            evSPD = rand() % 256;
            evSATK = rand() % 256;
            evSDEF = rand() % 256;
        }

        gender = determine_gender();
        nature = determine_nature();
        ability = determine_ability();
        held_item = 0; //Placeholder

        if(base_pkmn->get_species_id() == Species::SHEDINJA) HP = 1;
        else HP = get_hp_from_ev_iv();
        ATK = get_stat_from_ev_iv(Stats::ATTACK,evATK,ivATK);
        DEF = get_stat_from_ev_iv(Stats::DEFENSE,evDEF,ivDEF);
        SPD = get_stat_from_ev_iv(Stats::SPEED,evSPD,ivSPD);
        SATK = get_stat_from_ev_iv(Stats::SPECIAL_ATTACK,evSATK,ivSATK);
        SDEF = get_stat_from_ev_iv(Stats::SPECIAL_ATTACK,evSDEF,ivSDEF);

        icon_path = base_pkmn->get_icon_path((gender != Genders::FEMALE));
        sprite_path = base_pkmn->get_sprite_path((gender != Genders::FEMALE), is_shiny());

        nonvolatile_status = Statuses::OK;
    }

    string team_pokemon_gen345impl::get_info() const
    {
        string types_str;
        dict<unsigned int, unsigned int> types = base_pkmn->get_types();
        if(types[1] == Types::NONE) types_str = database::get_type_name_from_id(types[0]);
        else types_str = database::get_type_name_from_id(types[0]) + "/"
                       + database::get_type_name_from_id(types[1]);
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
        output_string = nickname + " (" + base_pkmn->get_species_name() + " " + gender_char + ")\n"
                      + "Level " + to_string(level) + "\n"
                      + "Type: " + types_str + "\n"
                      + "Ability: " + database::get_ability_name_from_id(ability) + "\n"
                      + "Held Item: " + to_string(held_item) + "\n"
                      + "Stats: " + stats_str;

        return output_string;
    }

    string team_pokemon_gen345impl::get_info_verbose() const
    {
        string types_str;
        dict<unsigned int, unsigned int> types = base_pkmn->get_types();
        if(types[1] == Types::NONE) types_str = database::get_type_name_from_id(types[0]);
        else types_str = database::get_type_name_from_id(types[0]) + "/"
                       + database::get_type_name_from_id(types[1]);

        string output_string;
        output_string = nickname + " (" + base_pkmn->get_species_name() + ")\n"
                      + "Level " + to_string(level) + "\n"
                      + "Type: " + types_str + "\n"
                      + "Ability: " + database::get_ability_name_from_id(ability) + "\n"
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
    
    unsigned int team_pokemon_gen345impl::get_ability() const {return ability;}
    
    unsigned int team_pokemon_gen345impl::get_gender() const {return gender;}

    unsigned int team_pokemon_gen345impl::get_nature() const {return nature;}
    
    bool team_pokemon_gen345impl::is_shiny() const
    {
        int p1, p2, E, F;
        p1 = (personality & 0xFFFF0000) >> 16;
        p2 = personality & 0xFFFF;
        E = tid.public_id ^ tid.secret_id;
        F = p1 ^ p2;
        return (E ^ F) < 8;
    }
    
    void team_pokemon_gen345impl::set_hidden_ability(bool val)
    {
        has_hidden_ability = val;
        ability = determine_ability();
    }
    
    dict<unsigned int, unsigned int> team_pokemon_gen345impl::get_stats() const
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

    dict<unsigned int, unsigned int> team_pokemon_gen345impl::get_EVs() const
    {
        dict<unsigned int, unsigned int> EVs;
        EVs[Stats::HP] = evHP;
        EVs[Stats::ATTACK] = evATK;
        EVs[Stats::DEFENSE] = evDEF;
        EVs[Stats::SPECIAL_ATTACK] = evSATK;
        EVs[Stats::SPECIAL_DEFENSE] = evSDEF;
        EVs[Stats::SPEED] = evSPD;
        
        return EVs;
    }
    
    dict<unsigned int, unsigned int> team_pokemon_gen345impl::get_IVs() const
    {
        dict<unsigned int, unsigned int> IVs;
        IVs[Stats::HP] = ivHP;
        IVs[Stats::ATTACK] = ivATK;
        IVs[Stats::DEFENSE] = ivDEF;
        IVs[Stats::SPECIAL_ATTACK] = ivSATK;
        IVs[Stats::SPECIAL_DEFENSE] = ivSDEF;
        IVs[Stats::SPEED] = ivSPD;

        return IVs;
    }

    void team_pokemon_gen345impl::set_EV(unsigned int EV, unsigned int val)
    {
        if(val > 255)
        {
            cerr << "Gen 3-5 EV's must be 0-255." << endl;
            exit(EXIT_FAILURE);
        }

        switch(EV)
        {
            case Stats::HP:
                evHP = val;
                HP = get_hp_from_ev_iv();
                break;
                
            case Stats::ATTACK:
                evATK = val;
                ATK = get_stat_from_ev_iv(Stats::ATTACK, evATK, ivATK);
                break;
                
            case Stats::DEFENSE:
                evDEF = val;
                DEF = get_stat_from_ev_iv(Stats::DEFENSE, evDEF, ivDEF);
                break;
            
            case Stats::SPECIAL_ATTACK:
                evSATK = val;
                SATK = get_stat_from_ev_iv(Stats::SPECIAL_ATTACK, evSPCL, ivSPCL);
                break;
                
            case Stats::SPECIAL_DEFENSE:
                evSDEF = val;
                SDEF = get_stat_from_ev_iv(Stats::SPECIAL_DEFENSE, evSPCL, ivSPCL);
                break;
                
            case Stats::SPEED:
                evSPD = val;
                SPD = get_stat_from_ev_iv(Stats::SPEED, evSPD, ivSPD);
                break;
            
            default:
                break;
        }
    }
    
    void team_pokemon_gen345impl::set_IV(unsigned int IV, unsigned int val)
    {
        if(val > 31)
        {
            cerr << "Gen 3-5 IV's must be 0-31." << endl;
            exit(EXIT_FAILURE);
        }

        switch(IV)
        {
            case Stats::HP:
                ivHP = val;
                HP = get_hp_from_ev_iv();
                break;
                
            case Stats::ATTACK:
                ivATK = val;
                ATK = get_stat_from_ev_iv(Stats::ATTACK, ivATK, ivATK);
                break;
                
            case Stats::DEFENSE:
                ivDEF = val;
                DEF = get_stat_from_ev_iv(Stats::DEFENSE, ivDEF, ivDEF);
                break;
            
            case Stats::SPECIAL_ATTACK:
                ivSATK = val;
                SATK = get_stat_from_ev_iv(Stats::SPECIAL_ATTACK, ivSPCL, ivSPCL);
                break;
                
            case Stats::SPECIAL_DEFENSE:
                ivSDEF = val;
                SDEF = get_stat_from_ev_iv(Stats::SPECIAL_DEFENSE, ivSPCL, ivSPCL);
                break;
                
            case Stats::SPEED:
                ivSPD = val;
                SPD = get_stat_from_ev_iv(Stats::SPEED, ivSPD, ivSPD);
                break;
            
            default:
                break;
        }
    }

    //TODO: better way of internally dealing with alternate forms
    
    std::string team_pokemon_gen345impl::get_form_name() const
    {
        //Still doesn't account for things like West Sea vs. East Sea Shellos
        //Will need to be done with ugly manual work
        if(base_pkmn->get_species_id() == base_pkmn->get_pokemon_id()) return base_pkmn->get_species_name();
        else
        {
            SQLite::Database db(get_database_path().c_str());
            string query_string = "SELECT id FROM pokemon_forms WHERE pokemon_id=%d" + base_pkmn->get_pokemon_id();
            unsigned int id = db.execAndGet(query_string.c_str());
            
            query_string = str(boost::format("SELECT pokemon_name FROM pokemon_form_names WHERE pokemon_form_id=%d AND local_language_id=9")
                                             % id);
            return db.execAndGetStr(query_string.c_str(), "pokemon_name");
        }
    }
    
    unsigned int team_pokemon_gen345impl::get_form_id() const {return base_pkmn->get_pokemon_id();}
    
    void team_pokemon_gen345impl::set_form(unsigned int form)
    {
        base_pkmn->set_form(form);
        HP = get_hp_from_ev_iv();
        ATK = get_stat_from_ev_iv(Stats::ATTACK, evATK, ivATK);
        DEF = get_stat_from_ev_iv(Stats::DEFENSE, evDEF, ivDEF);
        SPD = get_stat_from_ev_iv(Stats::SPEED, evSPD, ivSPD);
        SATK = get_stat_from_ev_iv(Stats::SPECIAL_ATTACK, evSATK, ivSATK);
        SDEF = get_stat_from_ev_iv(Stats::SPECIAL_DEFENSE, evSDEF, ivSDEF);
        icon_path = base_pkmn->get_icon_path((gender != Genders::FEMALE));
        sprite_path = base_pkmn->get_sprite_path((gender != Genders::FEMALE), is_shiny());
        ability = determine_ability();
    }

    void team_pokemon_gen345impl::set_form(std::string form)
    {
        base_pkmn->set_form(form);
        HP = get_hp_from_ev_iv();
        ATK = get_stat_from_ev_iv(Stats::ATTACK, evATK, ivATK);
        DEF = get_stat_from_ev_iv(Stats::DEFENSE, evDEF, ivDEF);
        SPD = get_stat_from_ev_iv(Stats::SPEED, evSPD, ivSPD);
        SATK = get_stat_from_ev_iv(Stats::SPECIAL_ATTACK, evSATK, ivSATK);
        SDEF = get_stat_from_ev_iv(Stats::SPECIAL_DEFENSE, evSDEF, ivSDEF);
        icon_path = base_pkmn->get_icon_path((gender != Genders::FEMALE));
        sprite_path = base_pkmn->get_sprite_path((gender != Genders::FEMALE), is_shiny());
        ability = determine_ability();
    }

    unsigned int team_pokemon_gen345impl::get_hp_from_ev_iv() const
    {
        dict<unsigned int, unsigned int> stats = base_pkmn->get_base_stats();

        unsigned int hp_val = int(floor(((double(ivHP) + (2.0*double(stats[Stats::HP])) + (0.25*double(evHP)) + 100.0)
                            * double(level))/100.0 + 10.0));
        return hp_val;
    }

    unsigned int team_pokemon_gen345impl::get_stat_from_ev_iv(unsigned int stat, unsigned int EV, unsigned int IV) const
    {
        dict<unsigned int, unsigned int> stats = base_pkmn->get_base_stats();
        double nature_mod = database::get_nature_stat_effect_from_id(nature, stat);

        unsigned int stat_val = int(ceil(((((double(IV) + 2.0*double(stats[stat]) + 0.25*double(EV))
                              * double(level))/100.0) + 5.0) * nature_mod));
        return stat_val;
    }

    unsigned int team_pokemon_gen345impl::determine_ability() const
    {
        SQLite::Database db(get_database_path().c_str());
        string query_string;
    
        if(has_hidden_ability and from_gen >= 5)
        {
            query_string = "SELECT ability_id FROM pokemon_abilities WHERE is_hidden=1 AND pokemon_id=" + base_pkmn->get_pokemon_id();
            return db.execAndGet(query_string.c_str());
        }
        else
        {
            unsigned int ability_num = personality % 2;
            query_string = "SELECT ability_id FROM pokemon_abilities WHERE slot=1 AND pokemon_id=" + base_pkmn->get_pokemon_id();
            SQLite::Statement query(db, query_string.c_str());
            
            if(query.executeStep() and ability_num == 1) return query.getColumn(0);
            else
            {
                query_string = "SELECT ability_id FROM pokemon_abilities WHERE slot=0 AND pokemon_id" + base_pkmn->get_pokemon_id();
                return db.execAndGet(query_string.c_str());
            }
        }
    }
    
    unsigned int team_pokemon_gen345impl::determine_gender() const
    {
        if(base_pkmn->get_chance_male() + base_pkmn->get_chance_female() == 0) return Genders::GENDERLESS;
        else if(base_pkmn->get_chance_male() == 1.0) return Genders::MALE;
        else if(base_pkmn->get_chance_female() == 1.0) return Genders::FEMALE;
        else
        {
            if((personality % 256) > int(floor(255*(1-base_pkmn->get_chance_male())))) return Genders::MALE;
            else return Genders::FEMALE;
        }

        //Should never get here, this stops Clang from complaining
        return Genders::MALE;
    }
    
    unsigned int team_pokemon_gen345impl::determine_nature() const {return (personality % 25);}
}
