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

#include "spec_pokemon_gen1impl.hpp"

using namespace std;

namespace pkmnsim
{
    spec_pokemon_gen1impl::spec_pokemon_gen1impl(base_pokemon::sptr b, int lvl, int g, int m1,
                                           int m2, int m3, int m4): spec_pokemon(
                                           b,m1,m2,m3,m4,g,lvl)
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

        HP = get_hp_from_iv_ev();
        ATK = get_stat_from_iv_ev("ATK",ivATK,evATK);
        DEF = get_stat_from_iv_ev("DEF",ivDEF,evDEF);
        SPD = get_stat_from_iv_ev("SPD",ivSPD,evSPD);
        SPCL = get_stat_from_iv_ev("SPCL",ivSPCL,evSPCL);

        sprite_path = b->get_sprite_path(true, false);

        nonvolatile_status = Statuses::OK;
        reset_volatile_status_map();
        
    }

    dict<string, int> spec_pokemon_gen1impl::get_stats()
    {
        dict<string, int> stats;
        stats["HP"] = HP;
        stats["ATK"] = ATK;
        stats["DEF"] = DEF;
        stats["SPD"] = SPD;
        stats["SPCL"] = SPCL;

        return stats;
    }

    dict<string, int> spec_pokemon_gen1impl::get_IVs()
    {
        dict<string, int> stats;
        stats["HP"] = ivHP;
        stats["ATK"] = ivATK;
        stats["DEF"] = ivDEF;
        stats["SPD"] = ivSPD;
        stats["SPCL"] = ivSPCL;

        return stats;
    }

    void spec_pokemon_gen1impl::set_IV(string IVname, int val)
    {
        if(val < 0 or val > 16)
        {
            cerr << "Gen 1 IV's must be 0-15." << endl;
            exit(EXIT_FAILURE);
        }

        if(IVname == "HP")
        {
            ivHP = val;
            HP = get_hp_from_iv_ev();
        }
        else if(IVname == "ATK")
        {
            ivATK = val;
            ATK = get_stat_from_iv_ev("ATK",ivATK,evATK);
        }
        else if(IVname == "DEF")
        {
            ivDEF = val;
            DEF = get_stat_from_iv_ev("DEF",ivDEF,evDEF);
        }
        else if(IVname == "SPD")
        {
            ivSPD = val;
            SPD = get_stat_from_iv_ev("SPD",ivSPD,evSPD);
        }
        else if(IVname == "SPCL")
        {
            ivSPCL = val;
            SPCL = get_stat_from_iv_ev("SPCL",ivSPCL,evSPCL);
        }
    }

    dict<string, int> spec_pokemon_gen1impl::get_EVs()
    {
        dict<string, int> stats;
        stats["HP"] = evHP;
        stats["ATK"] = evATK;
        stats["DEF"] = evDEF;
        stats["SPD"] = evSPD;
        stats["SPCL"] = evSPCL;

        return stats;
    }

    void spec_pokemon_gen1impl::set_EV(string EVname, int val)
    {
        if(val < 0 or val > 65535)
        {
            cerr << "Gen 1 EV's must be 0-65535." << endl;
            exit(EXIT_FAILURE);
        }

        if(EVname == "HP")
        {
            ivHP = val;
            HP = get_hp_from_iv_ev();
        }
        else if(EVname == "ATK")
        {
            ivATK = val;
            ATK = get_stat_from_iv_ev("ATK",ivATK,evATK);
        }
        else if(EVname == "DEF")
        {
            ivDEF = val;
            DEF = get_stat_from_iv_ev("DEF",ivDEF,evDEF);
        }
        else if(EVname == "SPD")
        {
            ivSPD = val;
            SPD = get_stat_from_iv_ev("SPD",ivSPD,evSPD);
        }
        else if(EVname == "SPCL")
        {
            ivSPCL = val;
            SPCL = get_stat_from_iv_ev("SPCL",ivSPCL,evSPCL);
        }
    }

    string spec_pokemon_gen1impl::get_info()
    {
        string types_str;
        dict<int, string> types = base->get_types();
        if(types[1] == "None") types_str = types[0];
        else types_str = types[0] + "/" + types[1];
        string stats_str = to_string(HP) + ", " + to_string(ATK) + ", "
                         + to_string(DEF) + ", " + to_string(SPD) + ", "
                         + to_string(SPCL);

        string output_string;
        output_string = nickname + " (" + base->get_species_name() + ")\n"
                      + "Level " + to_string(level) + "\n"
                      + "Type: " + types_str + "\n"
                      + "Stats: " + stats_str;

        return output_string;
    }

    string spec_pokemon_gen1impl::get_info_verbose()
    {
        string types_str;
        dict<int, string> types = base->get_types();
        if(types[1] == "None") types_str = types[0];
        else types_str = types[0] + "/" + types[1];

        string output_string;
        output_string = nickname + " (" + base->get_species_name() + ")\n"
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

    bool spec_pokemon_gen1impl::is_shiny() {return false;}

    void spec_pokemon_gen1impl::set_form(int form)
    {
        base->set_form(form);
        HP = get_hp_from_iv_ev();
        ATK = get_stat_from_iv_ev("ATK", ivATK, evATK);
        DEF = get_stat_from_iv_ev("DEF", ivDEF, evDEF);
        SPD = get_stat_from_iv_ev("SPD", ivSPD, evSPD);
        SPCL = get_stat_from_iv_ev("SPCL", ivATK, evATK);
    }

    void spec_pokemon_gen1impl::set_form(std::string form)
    {
        base->set_form(form);
        HP = get_hp_from_iv_ev();
        ATK = get_stat_from_iv_ev("ATK", ivATK, evATK);
        DEF = get_stat_from_iv_ev("DEF", ivDEF, evDEF);
        SPD = get_stat_from_iv_ev("SPD", ivSPD, evSPD);
        SPCL = get_stat_from_iv_ev("SPCL", ivATK, evATK);
    }

    void spec_pokemon_gen1impl::reset_volatile_status_map()
    {
        volatile_status_map = boost::assign::map_list_of
            ("confusion",0)
            ("flinch",0)
            ("heal block",0)
            ("partially trapped",0)
            ("seeded",0)
            ("focus energy",0)
            ("glowing",0)
            ("magnetic levitation",0)
            ("minimize",0)
            ("recharging",0)
            ("semi-invulnerable",0)
            ("substitute",0)
            ("taking in sunlight",0)
            ("withdrawing",0)
            ("whipping up a whirlwind",0)
        ;
    }

    int spec_pokemon_gen1impl::get_hp_from_iv_ev()
    {
        dict<string, int> stats = base->get_base_stats();

        int hp_val = int(floor((((double(ivHP) + double(stats["HP"]) + (pow(double(evHP),0.5)/8.0)
                     + 50.0) * double(level))/50.0) + 10.0));
        return hp_val;
    }

    int spec_pokemon_gen1impl::get_stat_from_iv_ev(string stat, int ivSTAT, int evSTAT)
    {
        dict<string, int> stats = base->get_base_stats();

        int stat_val = int(ceil((((double(ivSTAT) + double(stats[stat]) + (pow(double(evSTAT),0.5)/8.0))
                       * double(level))/50.0) + 5.0));
        return stat_val;
    }
}
