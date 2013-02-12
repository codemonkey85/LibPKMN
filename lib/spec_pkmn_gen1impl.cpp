#include <boost/assign.hpp>
#include "spec_pkmn_gen1impl.hpp"
#include <time.h>

using namespace std;

namespace pkmnsim
{
    spec_pkmn_gen1impl::spec_pkmn_gen1impl(base_pkmn::sptr b, string n, int lvl)
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

        base = b;
        nickname = n;
        level = lvl;

        HP = get_hp_from_iv_ev();
        ATK = get_stat_from_iv_ev("ATK",ivATK,evATK);
        DEF = get_stat_from_iv_ev("DEF",ivDEF,evDEF);
        SPD = get_stat_from_iv_ev("SPD",ivSPD,evSPD);
        SPCL = get_stat_from_iv_ev("SPCL",ivSPCL,evSPCL);

        nonvolatile_status = "OK";

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

    map<string, int> get_stats()
    {
        map<string, int> stats;
        stats["HP"] = HP;
        stats["ATK"] = ATK;
        stats["DEF"] = DEF;
        stats["SPD"] = SPD;
        stats["SPCL"] = SPCL;

        return stats;
    }

    map<string, int> spec_pkmn_gen1impl::get_IVs()
    {
        map<string, int> stats;
        stats["HP"] = ivHP;
        stats["ATK"] = ivATK;
        stats["DEF"] = ivDEF;
        stats["SPD"] = ivSPD;
        stats["SPCL"] = ivSPCL;

        return stats;
    }

    map<string, int> get_EVs()
    {
        map<string, int> stats;
        stats["HP"] = evHP;
        stats["ATK"] = evATK;
        stats["DEF"] = evDEF;
        stats["SPD"] = evSPD;
        stats["SPCL"] = evSPCL;

        return stats;
    }

    string spec_pkmn_gen1impl::get_info()
    {
        string types_str;
        string * types = base->get_types();
        if(types[1] == "None") types_str = types[0];
        else types_str = str(boost::format("%s/%s") % types[0] % types[1]);

        return str(boost::format(
            "%s (%s)\n"
            "Level %d\n"
            "Type: %s\n"
            "Stats: %d, %d, %d, %d, %d"
            ) % nickname.c_str() % base->get_display_name().c_str()
              % level
              % types_str.c_str()
              % HP % ATK % DEF % SPD % SPCL
        );
    }

    string spec_pkmn_gen1impl::get_info_verbose()
    {
        string types_str;
        string * types = base->get_types();
        if(types[1] == "None") types_str = types[0];
        else types_str = str(boost::format("%s/%s") % types[0] % types[1]);

        return str(boost::format(
                "%s (%s)\n"
                "Level %d\n"
                "Type: %s\n"
                "Stats:\n"
                " - HP: %d\n"
                " - Attack: %d\n"
                " - Defense: %d\n"
                " - Speed: %d\n"
                " - Special: %d\n"
                "Individual Values:\n"
                " - HP: %d\n"
                " - Attack: %d\n"
                " - Defense: %d\n"
                " - Speed: %d\n"
                " - Special: %d\n"
                "Effort Values:\n"
                " - HP: %d\n"
                " - Attack: %d\n"
                " - Defense: %d\n"
                " - Speed: %d\n"
                " - Special: %d\n"
            ) % nickname.c_str() % base->get_display_name().c_str()
              % level
              % types_str.c_str()
              % HP
              % ATK
              % DEF
              % SPD
              % SPCL
              % ivHP
              % ivATK
              % ivDEF
              % ivSPD
              % ivSPCL
              % evHP
              % evATK
              % evDEF
              % evSPD
              % evSPCL
        );
    }

    void spec_pkmn_gen1impl::reset_volatile_status_map()
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

    int spec_pkmn_gen1impl::get_hp_from_iv_ev()
    {
        map<string, int> stats= base->get_base_stats();

        int hp_val = floor((((ivHP + stats["HP"] + (pow(evHP,0.5)/8) + 50) * level)/50) + 10);
        return hp_val;
    }

    int spec_pkmn_gen1impl::get_stat_from_iv_ev(string stat, int ivSTAT, int evSTAT)
    {
        map<string, int> stats = base->get_base_stats();

        int stat_val = ceil((((ivSTAT + stats[stat] + (pow(evSTAT,0.5)/8)) * level)/50) + 5);
        return stat_val;
    }
}
