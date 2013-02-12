#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <math.h>
#include "spec_pkmn_gen2impl.hpp"
#include <time.h>

using namespace std;

namespace pkmnsim
{
    spec_pkmn_gen2impl::spec_pkmn_gen2impl(base_pkmn::sptr b, string n, int lvl)
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

        base = b;
        nickname = n;
        level = lvl;
        gender = determine_gender();
        held_item = "None";

        HP = get_hp_from_iv_ev();
        ATK = get_stat_from_iv_ev("ATK",ivATK,evATK);
        DEF = get_stat_from_iv_ev("DEF",ivDEF,evDEF);
        SPD = get_stat_from_iv_ev("SPD",ivSPD,evSPD);
        SATK = get_stat_from_iv_ev("SATK",ivSATK,evSATK);
        SDEF = get_stat_from_iv_ev("SDEF",ivSDEF,evSATK);

        move1 = "None";
        move2 = "None";
        move3 = "None";
        move4 = "None";

        move1PP = 0;
        move2PP = 0;
        move3PP = 0;
        move4PP = 0;

        nonvolatile_status = "OK";
        reset_volatile_status_map();
    }

    map<string, int> spec_pkmn_gen2impl::get_stats()
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

    map<string, int> spec_pkmn_gen2impl::get_IVs()
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

    map<string, int> spec_pkmn_gen2impl::get_EVs()
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

    char spec_pkmn_gen2impl::get_gender() {return gender;}

    string spec_pkmn_gen2impl::get_info()
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
            "Stats: %d, %d, %d, %d, %d, %d"
            ) % nickname.c_str() % get_gender_map()[gender].c_str() % base->get_display_name().c_str()
              % level
              % types_str.c_str()
              % held_item.c_str()
              % HP % ATK % DEF % SATK % SDEF % SPD
        );
    }

    string spec_pkmn_gen2impl::get_info_verbose()
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

    map<char, string> spec_pkmn_gen2impl::get_gender_map()
    {
        map<char, string> gender_map;
        gender_map['M'] = "Male";
        gender_map['F'] = "Female";
        gender_map['U'] = "Ungendered";
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
            ("recharging",0)
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
        map<string, int> stats = base->get_base_stats();

        int hp_val = floor((((ivHP + stats["HP"] + (pow(evHP,0.5)/8) + 50) * level)/50) + 10);
        return hp_val;
    }

    int spec_pkmn_gen2impl::get_stat_from_iv_ev(string stat, int ivSTAT, int evSTAT)
    {
        map<string, int> stats = base->get_base_stats();

        int stat_val = ceil((((ivSTAT + stats[stat] + (pow(evSTAT,0.5)/8)) * level)/50) + 5);
        return stat_val;
    }

    char spec_pkmn_gen2impl::determine_gender()
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
}
