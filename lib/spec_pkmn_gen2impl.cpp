/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <math.h>
#include <time.h>

#include <boost/assign.hpp>

#include <pkmnsim/base_move.hpp>

#include "spec_pkmn_gen2impl.hpp"

using namespace std;

namespace pkmnsim
{
    spec_pkmn_gen2impl::spec_pkmn_gen2impl(base_pkmn::sptr b, int lvl, string m1,
                                           string m2, string m3, string m4, bool i): spec_pkmn(
                                           b,i,m1,m2,m3,m4,2,lvl)
    {
        srand ( time(NULL) );

        //Random individual values
        ivHP = rand() % 16;
        ivATK = rand() % 16;
        ivDEF = rand() % 16;
        ivSPD = rand() % 16;
        ivSATK = rand() % 16;
        ivSDEF = rand() % 16;

        //Random effort values
        evHP = rand() % 65536;
        evATK = rand() % 65536;
        evDEF = rand() % 65536;
        evSPD = rand() % 65536;
        evSATK = rand() % 65536;
        evSDEF = rand() % 65536;

        gender = determine_gender();
        held_item = "None";

        HP = get_hp_from_iv_ev();
        ATK = get_stat_from_iv_ev("ATK",ivATK,evATK);
        DEF = get_stat_from_iv_ev("DEF",ivDEF,evDEF);
        SPD = get_stat_from_iv_ev("SPD",ivSPD,evSPD);
        SATK = get_stat_from_iv_ev("SATK",ivSATK,evSATK);
        SDEF = get_stat_from_iv_ev("SDEF",ivSDEF,evSATK);

        nonvolatile_status = Statuses::OK;
        reset_volatile_status_map();
    }

    dict<string, int> spec_pkmn_gen2impl::get_stats()
    {
        dict<string, int> stats;
        stats["HP"] = HP;
        stats["ATK"] = ATK;
        stats["DEF"] = DEF;
        stats["SATK"] = SATK;
        stats["SDEF"] = SDEF;
        stats["SPD"] = SPD;

        return stats;
    }

    dict<string, int> spec_pkmn_gen2impl::get_IVs()
    {
        dict<string, int> stats;
        stats["HP"] = ivHP;
        stats["ATK"] = ivATK;
        stats["DEF"] = ivDEF;
        stats["SATK"] = ivSATK;
        stats["SDEF"] = ivSDEF;
        stats["SPD"] = ivSPD;

        return stats;
    }

    dict<string, int> spec_pkmn_gen2impl::get_EVs()
    {
        dict<string, int> stats;
        stats["HP"] = evHP;
        stats["ATK"] = evATK;
        stats["DEF"] = evDEF;
        stats["SATK"] = evSATK;
        stats["SDEF"] = evSDEF;
        stats["SPD"] = evSPD;

        return stats;
    }

    int spec_pkmn_gen2impl::get_gender() {return gender;}

    string spec_pkmn_gen2impl::get_info()
    {
        string types_str;
        dict<int,string> types = base->get_types();
        if(types[2] == "None") types_str = types[1];
        else types_str = types[1] + "/" + types[2];
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
        output_string = nickname + " (" + base->get_display_name() + " " + gender_char + ")\n"
                      + "Level " + to_string(level) + "\n"
                      + "Type: " + types_str + "\n"
                      + "Held Item: " + held_item + "\n"
                      + "Stats: " + stats_str;

        return output_string;
    }

    string spec_pkmn_gen2impl::get_info_verbose()
    {
        string types_str;
        dict<int,string> types = base->get_types();
        if(types[2] == "None") types_str = types[1];
        else types_str = types[1] + "/" + types[2];

        string output_string;
        output_string = nickname + "(" + base->get_display_name() + ")\n"
                      + "Level" + to_string(level) + "\n"
                      + "Type: " + types_str + "\n"
                      + "Held Item: " + held_item + "\n"
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
                      + " - Special Attack: " + to_string(evSATK) + "\n";
                      + " - Special Defense: " + to_string(evSDEF) + "\n";
                      + " - Speed: " + to_string(evSPD) + "\n";

        return output_string;
    }

    void spec_pkmn_gen2impl::reset_volatile_status_map()
    {
        volatile_status_map = boost::assign::map_list_of
            ("confusion",0)
            ("curse",0)
            ("embargo",0)
            ("encore",0)
            ("flinch",0)
            ("heal block",0)
            ("identification",0)
            ("infatuation",0)
            ("nightmare",0)
            ("partially trapped",0)
            ("perish song",0)
            ("seeded",0)
            ("taunt",0)
            ("telekinetic levitation",0)
            ("torment",0)
            ("trapped",0)
            ("aqua ring",0)
            ("bracing",0)
            ("center of attention",0)
            ("defense curl",0)
            ("focus energy",0)
            ("glowing",0)
            ("rooting",0)
            ("magic coat",0)
            ("magnetic levitation",0)
            ("minimize",0)
            ("protection",0)
            ("reintging",0)
            ("semi-invulnerable",0)
            ("substitute",0)
            ("taking aim",0)
            ("taking in sunlight",0)
            ("withdrawing",0)
            ("whipping up a whirlwind",0)
        ;
    }

    int spec_pkmn_gen2impl::get_hp_from_iv_ev()
    {
        dict<string, int> stats = base->get_base_stats();

        int hp_val = int(floor((((ivHP + stats["HP"] + (pow(evHP,0.5)/8) + 50) * level)/50) + 10));
        return hp_val;
    }

    int spec_pkmn_gen2impl::get_stat_from_iv_ev(string stat, int ivSTAT, int evSTAT)
    {
        dict<string, int> stats = base->get_base_stats();

        int stat_val = int(ceil((((ivSTAT + stats[stat] + (pow(evSTAT,0.5)/8)) * level)/50) + 5));
        return stat_val;
    }

    int spec_pkmn_gen2impl::determine_gender()
    {
        if(base->get_chance_male() + base->get_chance_female() == 0) return Genders::GENDERLESS;
        else if(base->get_chance_male() == 1.0) return Genders::MALE;
        else if(base->get_chance_female() == 1.0) return Genders::FEMALE;
        else
        {
            srand( time(NULL) );
            double val = (rand() % 1000 + 1)/1000.0;
            if(val <= base->get_chance_male()) return Genders::MALE;
            else return Genders::FEMALE;
        }
    }
}
