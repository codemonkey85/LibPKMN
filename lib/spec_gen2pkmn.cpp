#include <iostream>
#include <string>
#include <map>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <pkmnsim/spec_gen2pkmn.hpp>
#include <pkmnsim/pkmn_natures.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>

//Random values
spec_gen2pkmn::spec_gen2pkmn(base_gen2pkmn b, int lvl, int force_vals)
{
    srand ( time(NULL) );
    if(force_vals == -1)
    {
        //Random IV's
        ivHP = rand() % 16;
        ivATK = rand() % 16;
        ivDEF = rand() % 16;
        ivSPD = rand() % 16;
        ivSATK = rand() % 16;
        ivSDEF = rand() % 16;
    }
    else
    {
        ivHP = force_vals;
        ivATK = force_vals;
        ivDEF = force_vals;
        ivSPD = force_vals;
        ivSATK = force_vals;
        ivSDEF = force_vals;
    }

    evHP = rand() % 65536;
    evATK = rand() % 65536;
    evDEF = rand() % 65536;
    evSPD = rand() % 65536;
    evSATK = rand() % 65536;
    evSDEF = rand() % 65536;

    base = b;
    nickname = base.get_display_name();
    level = lvl;
    gender = determine_gender();
    held_item = "None";

    HP = get_hp_from_iv_ev();
    ATK = get_stat_from_iv_ev(1,ivATK,evATK);
    DEF = get_stat_from_iv_ev(2,ivDEF,evDEF);
    SPD = get_stat_from_iv_ev(3,ivSPD,evSPD);
    SATK = get_stat_from_iv_ev(4,ivSATK,evSATK);
    SDEF = get_stat_from_iv_ev(5,ivSDEF,evSATK);

    move1 = "None";
    move2 = "None";
    move3 = "None";
    move4 = "None";

    nonvolatile_status = "OK";
    reset_volatile_status_map();
}

//Importing specific Pokemon, such as when building a team
spec_gen2pkmn::spec_gen2pkmn(base_gen2pkmn b, std::string name, int lvl, char gndr, std::string hld, int hpstat, int atkstat, int defstat, int spdstat,
                     int satkstat, int sdefstat, std::string m1, std::string m2, std::string m3, std::string m4) 
{
    base = b;
    nickname = name;
    level = lvl;
    gender = gndr;
    held_item = hld;
    ATK = atkstat;
    DEF = defstat;
    SPD = spdstat;
    SATK = satkstat;
    SDEF = sdefstat;
    move1 = m1;
    move2 = m2;
    move3 = m3;
    move4 = m4;

    //Random IV's
    ivHP = rand() % 16;
    ivATK = rand() % 16;
    ivDEF = rand() % 16;
    ivSPD = rand() % 16;
    ivSATK = rand() % 16;
    ivSDEF = rand() % 16;

    nonvolatile_status = "OK";
    reset_volatile_status_map();

    //TODO: check for impossible stats
}

void spec_gen2pkmn::print()
{
    std::cout << boost::format("%s (%d, %s)\n") % nickname % level % gender;
    std::cout << base.get_display_name() << std::endl;
    std::cout << boost::format("Stats: %d,%d,%d,%d,%d,%d\n") % HP % ATK % DEF % SPD % SATK % SDEF;
}

void spec_gen2pkmn::print_verbose()
{
    std::cout << boost::format("%s (%s)\n") % base.get_display_name() % gender;
    std::cout << boost::format("Nickname: %s\n") % nickname;
    std::cout << boost::format("Level %d\n") % level;
    if(held_item == "None") std::cout << "Item: None" << std::endl;
    else std::cout << boost::format("Item: %s\n") % held_item;
    std::cout << "Stats:" << std::endl;
    std::cout << boost::format(" - HP: %d (IV: %d, EV: %d)\n") % HP % ivHP % evHP;
    std::cout << boost::format(" - Attack: %d (IV: %d, EV: %d)\n") % ATK % ivATK % evATK;
    std::cout << boost::format(" - Defense: %d (IV: %d, EV: %d)\n") % DEF % ivDEF % evDEF;
    std::cout << boost::format(" - Speed: %d (IV: %d, EV: %d)\n") % SPD % ivSPD % evSPD;
    std::cout << boost::format(" - Special Attack: %d (IV: %d, EV: %d)\n") % SATK % ivSATK % evSATK;
    std::cout << boost::format(" - Special Defense: %d (IV: %d, EV: %d)\n") % SDEF % ivSDEF % evSDEF;
}

base_gen2pkmn spec_gen2pkmn::get_base_gen2pkmn() {return base;}

int spec_gen2pkmn::get_hp_from_iv_ev()
{
    int *stats;
    stats = base.get_base_stats();

    int hp_val = floor((((ivHP + stats[0] + (pow(evHP,0.5)/8) + 50) * level)/50) + 10);
    return hp_val;
}

int spec_gen2pkmn::get_stat_from_iv_ev(int arr_index, int ivSTAT, int evSTAT)
{
    int *stats;
    stats = base.get_base_stats();

    int stat_val = ceil((((ivSTAT + stats[arr_index] + (pow(evSTAT,0.5)/8)) * level)/50) + 5);
    return stat_val;
}

void spec_gen2pkmn::reset_volatile_status_map()
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
