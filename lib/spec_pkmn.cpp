#include <iostream>
#include <string>
#include <map>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <pkmnsim/spec_pkmn.hpp>
#include <pkmnsim/pkmn_natures.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>

//Random values
spec_pkmn::spec_pkmn(base_pkmn b, int lvl, int force_vals)
{
    srand ( time(NULL) );
    if(force_vals == -1)
    {
        //Random IV's
        ivHP = rand() % 32;
        ivATK = rand() % 32;
        ivDEF = rand() % 32;
        ivSPD = rand() % 32;
        ivSATK = rand() % 32;
        ivSDEF = rand() % 32;
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

    /*
     * Random EV's within following rules:
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

    base = b;
    nickname = base.get_display_name();
    level = lvl;
    gender = determine_gender();
    nature = determine_nature();
    held_item = "None";

    if(base.get_display_name() == "Shedinja") HP = 1;
    else HP = get_hp_from_iv_ev();
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
spec_pkmn::spec_pkmn(base_pkmn b, std::string name, int lvl, char gndr, std::string nat, std::string hld, int hpstat, int atkstat, int defstat, int spdstat,
                     int satkstat, int sdefstat, std::string m1, std::string m2, std::string m3, std::string m4) 
{
    base = b;
    nickname = name;
    level = lvl;
    gender = gndr;
    nature = get_nature(nat);
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
    ivHP = rand() % 32;
    ivATK = rand() % 32;
    ivDEF = rand() % 32;
    ivSPD = rand() % 32;
    ivSATK = rand() % 32;
    ivSDEF = rand() % 32;

    nonvolatile_status = "OK";
    reset_volatile_status_map();

    //TODO: check for impossible stats
}

void spec_pkmn::print()
{
    std::cout << boost::format("%s (%d, %s)\n") % nickname % level % gender;
    std::cout << base.get_display_name() << std::endl;
    std::cout << boost::format("Stats: %d,%d,%d,%d,%d,%d\n") % HP % ATK % DEF % SPD % SATK % SDEF;
}

void spec_pkmn::print_verbose()
{
    std::cout << boost::format("%s (%s)\n") % base.get_display_name() % gender;
    std::cout << boost::format("Nickname: %s\n") % nickname;
    std::cout << boost::format("Level %d\n") % level;
    std::cout << boost::format("Nature: %s\n") % nature.get_name();
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

base_pkmn spec_pkmn::get_base_pkmn() {return base;}

char spec_pkmn::determine_gender()
{
    if(base.get_chance_male() + base.get_chance_female() == 0) return 'N';
    else if(base.get_chance_male() == 1.0) return 'M';
    else if(base.get_chance_female() == 1.0) return 'F';
    else
    {
        srand( time(NULL) );
        double val = (rand() % 1000 + 1)/1000.0;
        std::cout << "gender val: " << val << std::endl;
        if(val <= base.get_chance_male()) return 'M';
        else return 'F';
    }
}

//TODO: there has to be a better way to do this...
pkmn_nature spec_pkmn::determine_nature()
{
    std::string nature_names[] = {"Hardy","Lonely","Brave","Adamant","Naughty","Bold",
                                  "Docile","Relaxed","Impish","Lax","Timid","Hasty",
                                  "Serious","Jolly","Naive","Modest","Mild","Quiet",
                                  "Bashful","Rash","Calm","Gentle","Sassy","Careful",
                                  "Quirky"};
    srand( time(NULL) );
    int index = rand() % 25;
    return get_nature(nature_names[index]);
}

int spec_pkmn::get_hp_from_iv_ev()
{
    int *stats;
    stats = base.get_base_stats();

    int hp_val = floor(((ivHP + (2*stats[0]) + (0.25*evHP) + 100) * level)/100 + 10);
    return hp_val;
}

int spec_pkmn::get_stat_from_iv_ev(int arr_index, int ivSTAT, int evSTAT)
{
    int *stats;
    stats = base.get_base_stats();

    double nature_mod = nature.get_mods()[arr_index-1];
    int stat_val = ceil(((((ivSTAT + 2*(stats[arr_index]) + 0.25*(evSTAT)) * level)/100) + 5) * nature_mod);
    return stat_val;
}

void spec_pkmn::reset_volatile_status_map()
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
