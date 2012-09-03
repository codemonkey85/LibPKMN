#include <iostream>
#include <string>
#include <map>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <pkmnsim/spec_pkmn.hpp>
#include <pkmnsim/pkmn_nature.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>

//Random values
spec_pkmn::spec_pkmn(base_pkmn b, int lvl, int force_vals)
{
    if(force_vals == -1)
    {
        //Random IV's
        srand ( time(NULL) );
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

    base = b;
    nickname = base.get_display_name();
    level = lvl;
    gender = determine_gender();
    nature = determine_nature();
    held_item = "";

    if(base.get_display_name() == "Shedinja") HP = 1;
    else HP = get_hp_from_iv();
    ATK = get_stat_from_iv(1,ivATK);
    DEF = get_stat_from_iv(2,ivDEF);
    SPD = get_stat_from_iv(3,ivSPD);
    SATK = get_stat_from_iv(4,ivSATK);
    SDEF = get_stat_from_iv(5,ivSDEF);

    move1 = "";
    move2 = "";
    move3 = "";
    move4 = "";

    nonvolatile_status = "OK";
    reset_volatile_status_map();
}

//Importing specific Pokemon, such as when building a team
spec_pkmn::spec_pkmn(base_pkmn b, std::string name, int lvl, char gndr, std::string nat, std::string hld, int hpstat, int atkstat, int defstat, int spdstat,
                     int satkstat, int sdefstat, std::string m1, std::string m2, std::string m3, std::string m4) 
{
    extern std::map<std::string, pkmn_nature> pkmn_nature_map;
    base = b;
    nickname = name;
    level = lvl;
    gender = gndr;
    nature = pkmn_nature_map[nat];
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

    ivHP = get_iv_from_hp();
    ivATK = get_iv_from_stat(1,ATK);
    ivDEF = get_iv_from_stat(1,DEF);
    ivSPD = get_iv_from_stat(1,SPD);
    ivSATK = get_iv_from_stat(1,SATK);
    ivSDEF = get_iv_from_stat(1,SDEF);

    nonvolatile_status = "OK";
    reset_volatile_status_map();

    //TODO: check for impossible stats
}

void spec_pkmn::print()
{
    std::cout << boost::format("%s (%s)") % base.get_display_name() % gender << std::endl;
    std::cout << boost::format("Nickname: %s") % nickname << std::endl;
    std::cout << boost::format("Level %d") % level << std::endl;
    std::cout << boost::format("Nature: %s") % nature.get_name() << std::endl; //TODO: change to reflect new nature class
    if(held_item == "") std::cout << "Item: None" << std::endl;
    else std::cout << boost::format("Item: %s") % held_item << std::endl;
    std::cout << "Stats:" << std::endl;
    std::cout << boost::format(" - HP: %d (IV: %d)") % HP % ivHP << std::endl;
    std::cout << boost::format(" - Attack: %d (IV: %d)") % ATK % ivATK << std::endl;
    std::cout << boost::format(" - Defense: %d (IV: %d)") % DEF % ivDEF << std::endl;
    std::cout << boost::format(" - Speed: %d (IV: %d)") % SPD % ivSPD << std::endl;
    std::cout << boost::format(" - Special Attack: %d (IV: %d)") % SATK % ivSATK << std::endl;
    std::cout << boost::format(" - Special Defense: %d (IV: %d)") % SDEF % ivSDEF << std::endl;
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
        double val = (rand() % 1000 + 1)/1000;
        if(val <= base.get_chance_male()) return 'M';
        else return 'F';
    }
}

//TODO: there has to be a better way to do this...
pkmn_nature spec_pkmn::determine_nature()
{
    extern std::map<std::string, pkmn_nature> pkmn_nature_map;
    std::string nature_names[] = {"hardy","lonely","brave","adamant","naughty","bold",
                                 "docile","relaxed","impish","lax","timid","hasty",
                                 "serious","jolly","naive","modest","mild","quiet",
                                 "bashful","rash","calm","gentle","sassy","careful",
                                 "quirky"};
    srand( time(NULL) );
    int index = rand() % 25;
    std::string nature_name = nature_names[index];
    return pkmn_nature_map[nature_name];
}

int spec_pkmn::get_hp_from_iv()
{
    int *stats;
    stats = base.get_base_stats();
    int *yields;
    yields = base.get_ev_yields();

    int hp_val = floor((ivHP + 2*(stats[0]) + 0.25*(yields[0]) + 100)/100 + 10);
    return hp_val;
}

int spec_pkmn::get_stat_from_iv(int arr_index, int ivSTAT)
{
    int *stats;
    stats = base.get_base_stats();
    int *yields;
    yields = base.get_ev_yields();

    double nature_mod = 1.0; //TODO: generic method
    int stat_val = ceil(((((ivSTAT + 2*(stats[arr_index]) + 0.25*(yields[arr_index])) * level)/100) + 5) * nature_mod);
    return stat_val;
}

int spec_pkmn::get_iv_from_hp()
{
    int *stats;
    stats = base.get_base_stats();
    int *yields;
    yields = base.get_ev_yields();

    int iv_val = floor((11*(HP - 10))/level - 2*(stats[0]) - 0.25*(yields[0]) - 100);
    return iv_val;
}

int spec_pkmn::get_iv_from_stat(int arr_index, int STAT)
{
    int *stats;
    stats = base.get_base_stats();
    int *yields;
    yields = base.get_ev_yields();

    double nature_mod = 1.0; //TODO: generic method
    int stat = ceil(((STAT / nature_mod) - 5)*(100 / level) - 2*(stats[arr_index]) - 0.25*(yields[arr_index]));
    return stat;
}

void spec_pkmn::reset_volatile_status_map()
{
    volatile_status_map = boost::assign::map_list_of
        ("confusion",0)
        ("curse",0)
        ("embargo",0)
        ("encore",0)
        ("flinch",0)
        ("heal_block",0)
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
