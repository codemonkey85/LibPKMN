/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <boost/assign.hpp>
#include <math.h>
#include "spec_pkmn_gen3impl.hpp"
#include <time.h>

using namespace std;

namespace pkmnsim
{
    spec_pkmn_gen3impl::spec_pkmn_gen3impl(base_pkmn::sptr b, int lvl, int gen,
                                           string m1, string m2, string m3, string m4,
                                           bool i): spec_pkmn(b,i,m1,m2,m3,m4,gen,lvl)
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
        held_item = "None";

        if(base->get_display_name() == "Shedinja") HP = 1;
        else HP = get_hp_from_iv_ev();
        ATK = get_stat_from_iv_ev("ATK",ivATK,evATK);
        DEF = get_stat_from_iv_ev("DEF",ivDEF,evDEF);
        SPD = get_stat_from_iv_ev("SPD",ivSPD,evSPD);
        SATK = get_stat_from_iv_ev("SATK",ivSATK,evSATK);
        SDEF = get_stat_from_iv_ev("SDEF",ivSDEF,evSATK);

        nonvolatile_status = "OK";
        reset_volatile_status_map();
    }

    dict<string, int> spec_pkmn_gen3impl::get_stats()
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

    dict<string, int> spec_pkmn_gen3impl::get_IVs()
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

    dict<string, int> spec_pkmn_gen3impl::get_EVs()
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

    char spec_pkmn_gen3impl::get_gender() {return gender;}

    pkmn_nature::sptr spec_pkmn_gen3impl::get_nature() {return nature;}

    std::string spec_pkmn_gen3impl::get_ability() {return ability;}

    string spec_pkmn_gen3impl::get_info()
    {
        string types_str;
        dict<int, string> types = base->get_types();
        if(types[2] == "None") types_str = types[1];

        else types_str = types[1] + "/" + types[2];
        string stats_str = to_string(HP) + ", " + to_string(ATK) + ", "
                         + to_string(DEF) + ", " + to_string(SATK) + ", "
                         + to_string(SDEF) + ", " + to_string(SPD);

        string output_string;
        output_string = nickname + " (" + base->get_display_name() + " " + gender + ")\n"
                      + "Level " + to_string(level) + "\n"
                      + "Type: " + types_str + "\n"
                      + "Ability: " + ability + "\n"
                      + "Held Item: " + held_item + "\n"
                      + "Stats: " + stats_str;

        return output_string;
    }

    string spec_pkmn_gen3impl::get_info_verbose()
    {
        string types_str;
        dict<int, string> types = base->get_types();
        if(types[2] == "None") types_str = types[1];
        else types_str = types[1] + "/" + types[2];

        string output_string;
        output_string = nickname + "(" + base->get_display_name() + ")\n"
                      + "Level" + to_string(level) + "\n"
                      + "Type: " + types_str + "\n"
                      + "Ability: " + ability + "\n"
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

    dict<char, string> spec_pkmn_gen3impl::get_gender_map()
    {
        dict<char, string> gender_map;
        gender_map['M'] = "Male";
        gender_map['F'] = "Female";
        gender_map['U'] = "Ungendered";
		
		return gender_map;
    }

    int spec_pkmn_gen3impl::get_hp_from_iv_ev()
    {
        dict<string, int> stats = base->get_base_stats();

        int hp_val = floor(((ivHP + (2*stats["HP"]) + (0.25*evHP) + 100) * level)/100 + 10);
        return hp_val;
    }

    int spec_pkmn_gen3impl::get_stat_from_iv_ev(string stat, int ivSTAT, int evSTAT)
    {
        dict<string, int> stats = base->get_base_stats();

        double nature_mod = nature->get_mods()[stat];
        int stat_val = ceil(((((ivSTAT + 2*(stats[stat]) + 0.25*(evSTAT)) * level)/100) + 5) * nature_mod);
        return stat_val;
    }

    void spec_pkmn_gen3impl::reset_volatile_status_map()
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
            ("recharging",0)
            ("semi-invulnerable",0)
            ("substitute",0)
            ("taking aim",0)
            ("taking in sunlight",0)
            ("withdrawing",0)
            ("whipping up a whirlwind",0)
        ;
    }

    char spec_pkmn_gen3impl::determine_gender()
    {
        if(base->get_chance_male() + base->get_chance_female() == 0) return 'N';
        else if(base->get_chance_male() == 1.0) return 'M';
        else if(base->get_chance_female() == 1.0) return 'F';
        else
        {
            srand( time(NULL) );
            double val = (rand() % 1000 + 1)/1000.0;
            std::cout << "gender val: " << val << std::endl;
            if(val <= base->get_chance_male()) return 'M';
            else return 'F';
        }
    }

    pkmn_nature::sptr spec_pkmn_gen3impl::determine_nature()
    {
        string nature_names[] = {"Hardy","Lonely","Brave","Adamant","Naughty","Bold",
                                      "Docile","Relaxed","Impish","Lax","Timid","Hasty",
                                      "Serious","Jolly","Naive","Modest","Mild","Quiet",
                                      "Bashful","Rash","Calm","Gentle","Sassy","Careful",
                                      "Quirky"};
        srand( time(NULL) );
        int index = rand() % 25;
        return pkmn_nature::make(nature_names[index]);
    }

    string spec_pkmn_gen3impl::determine_ability()
    {
        srand( time(NULL) );
        dict<int, string> abilities = base->get_abilities();

        if(abilities[2] == "None" and abilities[1] == "None") ability = abilities[1];
        else if(abilities[2] != "None" and abilities[1] == "None") ability = abilities[rand() % 2];
        else //1 normal ability, hidden ability, so only use index 0 or 2
        {
            int num = rand() % 3;
            while(num != 1) num = rand() % 2;
            ability = abilities[num+1];
        }
		
		return ability;
    }
}
