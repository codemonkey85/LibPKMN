/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <boost/assign.hpp>
#include <boost/format.hpp>
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

    map<string, int> spec_pkmn_gen3impl::get_stats()
    {
        map<string, int> stats;
        stats["HP"] = HP;
        stats["ATK"] = ATK;
        stats["DEF"] = DEF;
        stats["SATK"] = SATK;
        stats["SDEF"] = SDEF;
        stats["SPD"] = SPD;

        return stats;
    }

    map<string, int> spec_pkmn_gen3impl::get_IVs()
    {
        map<string, int> stats;
        stats["HP"] = ivHP;
        stats["ATK"] = ivATK;
        stats["DEF"] = ivDEF;
        stats["SATK"] = ivSATK;
        stats["SDEF"] = ivSDEF;
        stats["SPD"] = ivSPD;

        return stats;
    }

    map<string, int> spec_pkmn_gen3impl::get_EVs()
    {
        map<string, int> stats;
        stats["HP"] = evHP;
        stats["ATK"] = evATK;
        stats["DEF"] = evDEF;
        stats["SATK"] = evSATK;
        stats["SDEF"] = evSDEF;
        stats["SPD"] = evSPD;

        return stats;
    }

    char spec_pkmn_gen3impl::get_gender() {return gender;}

    string spec_pkmn_gen3impl::get_info()
    {
        string types_str;
        string * types = base->get_types();
        if(types[1] == "None") types_str = types[0];
        else types_str = str(boost::format("%s/%s") % types[0] % types[1]);

        return str(boost::format(
            "%s (%s %s)\n"
            "Level %d\n"
            "Type: %s\n"
            "Held: %s\n"
            "Ability: %s\n"
            "Stats: %d, %d, %d, %d, %d, %d"
            ) % nickname.c_str() % get_gender_map()[gender].c_str() % base->get_display_name().c_str()
              % level
              % types_str.c_str()
              % held_item.c_str()
              % ability.c_str()
              % HP % ATK % DEF % SATK % SDEF % SPD
        );
    }

    string spec_pkmn_gen3impl::get_info_verbose()
    {
        string types_str;
        string * types = base->get_types();
        if(types[1] == "None") types_str = types[0];
        else types_str = str(boost::format("%s/%s") % types[0] % types[1]);

        return str(boost::format(
                "%s (%s %s)\n"
                "Level %d\n"
                "Type: %s\n"
                "Held: %s\n"
                "Ability: %s\n"
                "Stats:\n"
                " - HP: %d\n"
                " - Attack: %d\n"
                " - Defense: %d\n"
                " - Special Attack: %d\n"
                " - Special Defense: %d\n"
                " - Speed: %d\n"
                "Individual Values:\n"
                " - HP: %d\n"
                " - Attack: %d\n"
                " - Defense: %d\n"
                " - Special Attack: %d\n"
                " - Special Defense: %d\n"
                " - Speed: %d\n"
                "Effort Values:\n"
                " - HP: %d\n"
                " - Attack: %d\n"
                " - Defense: %d\n"
                " - Special Attack: %d\n"
                " - Special Defense: %d\n"
                " - Speed: %d\n"
            ) % nickname.c_str()  % get_gender_map()[gender].c_str() % base->get_display_name().c_str()
              % level
              % types_str.c_str()
              % held_item.c_str()
              % ability.c_str()
              % HP
              % ATK
              % DEF
              % SATK
              % SDEF
              % SPD
              % ivHP
              % ivATK
              % ivDEF
              % ivSATK
              % ivSDEF
              % ivSPD
              % evHP
              % evATK
              % evDEF
              % evSATK
              % evSDEF
              % evSPD
        );
    }

    map<char, string> spec_pkmn_gen3impl::get_gender_map()
    {
        map<char, string> gender_map;
        gender_map['M'] = "Male";
        gender_map['F'] = "Female";
        gender_map['U'] = "Ungendered";
		
		return gender_map;
    }

    int spec_pkmn_gen3impl::get_hp_from_iv_ev()
    {
        map<string, int> stats = base->get_base_stats();

        int hp_val = floor(((ivHP + (2*stats["HP"]) + (0.25*evHP) + 100) * level)/100 + 10);
        return hp_val;
    }

    int spec_pkmn_gen3impl::get_stat_from_iv_ev(string stat, int ivSTAT, int evSTAT)
    {
        map<string, int> stats = base->get_base_stats();

        double nature_mod = nature.get_mods()[stat];
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

    pkmnsim::pkmn_nature spec_pkmn_gen3impl::determine_nature()
    {
        std::string nature_names[] = {"Hardy","Lonely","Brave","Adamant","Naughty","Bold",
                                      "Docile","Relaxed","Impish","Lax","Timid","Hasty",
                                      "Serious","Jolly","Naive","Modest","Mild","Quiet",
                                      "Bashful","Rash","Calm","Gentle","Sassy","Careful",
                                      "Quirky"};
        srand( time(NULL) );
        int index = rand() % 25;
        return pkmnsim::get_nature(nature_names[index]);
    }

    string spec_pkmn_gen3impl::determine_ability()
    {
        srand( time(NULL) );
        string * abilities = base->get_abilities();

        if(abilities[1] == "None" and abilities[2] == "None") ability = abilities[0];
        else if(abilities[1] != "None" and abilities[2] == "None") ability = abilities[rand() % 2];
        else //1 normal ability, hidden ability, so only use index 0 or 2
        {
            int num = rand() % 3;
            while(num != 1) num = rand() % 2;
            ability = abilities[num];
        }
		
		return ability;
    }
}
