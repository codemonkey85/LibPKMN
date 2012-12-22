#include <iostream>
#include <string>
#include <map>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <pkmnsim/spec_gen1pkmn.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>

//Random values
spec_gen1pkmn::spec_gen1pkmn(base_gen1pkmn b, int lvl, int force_vals)
{
    srand ( time(NULL) );
    if(force_vals == -1)
    {
        //Random IV's
        ivHP = rand() % 16;
        ivATK = rand() % 16;
        ivDEF = rand() % 16;
        ivSPD = rand() % 16;
        ivSPCL = rand() % 16;
    }
    else
    {
        ivHP = force_vals;
        ivATK = force_vals;
        ivDEF = force_vals;
        ivSPD = force_vals;
        ivSPCL = force_vals;
    }

        evHP = rand() % 65536;
        evATK = rand() % 65536;
        evDEF = rand() % 65536;
        evSPD = rand() % 65536;
        evSPCL = rand() % 65536;

    base = b;
    nickname = base.get_display_name();
    level = lvl;

    HP = get_hp_from_iv_ev();
    ATK = get_stat_from_iv_ev(1,ivATK,evATK);
    DEF = get_stat_from_iv_ev(2,ivDEF,evDEF);
    SPD = get_stat_from_iv_ev(3,ivSPD,evSPD);
    SPCL = get_stat_from_iv_ev(4,ivSPCL,evSPCL);

    move1 = "None";
    move2 = "None";
    move3 = "None";
    move4 = "None";

    nonvolatile_status = "OK";
    reset_volatile_status_map();
}

//Importing specific Pokemon, such as when building a team
spec_gen1pkmn::spec_gen1pkmn(base_gen1pkmn b, std::string name, int lvl, int hpstat, int atkstat, int defstat, int spdstat,
                     int spclstat, std::string m1, std::string m2, std::string m3, std::string m4) 
{
    base = b;
    nickname = name;
    level = lvl;
    ATK = atkstat;
    DEF = defstat;
    SPD = spdstat;
    SPCL = spclstat;
    move1 = m1;
    move2 = m2;
    move3 = m3;
    move4 = m4;

    //Random IV's
    ivHP = rand() % 16;
    ivATK = rand() % 16;
    ivDEF = rand() % 16;
    ivSPD = rand() % 16;
    ivSPCL = rand() % 16;

    nonvolatile_status = "OK";
    reset_volatile_status_map();

    //TODO: check for impossible stats
}

void spec_gen1pkmn::print()
{
    std::cout << boost::format("%s (%d)\n") % nickname % level;
    std::cout << base.get_display_name() << std::endl;
    std::cout << boost::format("Stats: %d,%d,%d,%d,%d\n") % HP % ATK % DEF % SPD % SPCL;
}

void spec_gen1pkmn::print_verbose()
{
    std::cout << base.get_display_name() << std::endl;
    std::cout << boost::format("Nickname: %s\n") % nickname;
    std::cout << boost::format("Level %d\n") % level;
    std::cout << "Stats:" << std::endl;
    std::cout << boost::format(" - HP: %d (IV: %d, EV: %d)\n") % HP % ivHP % evHP;
    std::cout << boost::format(" - Attack: %d (IV: %d, EV: %d)\n") % ATK % ivATK % evATK;
    std::cout << boost::format(" - Defense: %d (IV: %d, EV: %d)\n") % DEF % ivDEF % evDEF;
    std::cout << boost::format(" - Speed: %d (IV: %d, EV: %d)\n") % SPD % ivSPD % evSPD;
    std::cout << boost::format(" - Special: %d (IV: %d, EV: %d)\n") % SPCL % ivSPCL % evSPCL;
}

base_gen1pkmn spec_gen1pkmn::get_base_gen1pkmn() {return base;}

int spec_gen1pkmn::get_hp_from_iv_ev()
{
    int *stats;
    stats = base.get_base_stats();

    int hp_val = floor((((ivHP + stats[0] + (pow(evHP,0.5)/8) + 50) * level)/50) + 10);
    return hp_val;
}

int spec_gen1pkmn::get_stat_from_iv_ev(int arr_index, int ivSTAT, int evSTAT)
{
    int *stats;
    stats = base.get_base_stats();

    int stat_val = ceil((((ivSTAT + stats[arr_index] + (pow(evSTAT,0.5)/8)) * level)/50) + 5);
    return stat_val;
}

void spec_gen1pkmn::reset_volatile_status_map()
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
