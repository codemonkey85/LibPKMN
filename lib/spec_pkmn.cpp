#include <iostream>
#include <string>
#include <map>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <pkmnsim/spec_pkmn.hpp>

//Random values
spec_pkmn::spec_pkmn(base_pkmn b, int lvl, int force_vals)
{
    if(force_vals == -1)
    {
        //Minimum IV's
        ivHP = 0;
        ivATK = 0;
        ivDEF = 0;
        ivSPD = 0;
        ivSATK = 0;
        ivSDEF = 0;
    }
    else if(force_vals == 1)
    {
        //Maximum IV's
        ivHP = 31;
        ivATK = 31;
        ivDEF = 31;
        ivSPD = 31;
        ivSATK = 31;
        ivSDEF = 31;
    }
    else
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

    base = b;
    nickname = base.get_display_name();
    level = lvl;
    gender = determine_gender();
    nature = determine_nature();
    held_item = "";

    HP = get_hp_from_iv();
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
    std::map<std::string, int> volatile_status_map = init_volatile_status_map();
}

//Importing specific Pokemon, such as when building a team
spec_pkmn::spec_pkmn(base_pkmn b, std::string name, int lvl, char gndr, std::string nat, std::string hld, int hpstat, int atkstat, int defstat, int spdstat,
                     int satkstat, int sdefstat, std::string m1, std::string m2, std::string m3, std::string m4) 
{
    base = b;
    nickname = name;
    level = lvl;
    gender = gndr;
    nature = nat;
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
    std::map<std::string, int> volatile_status_map = init_volatile_status_map();
}

char spec_pkmn::determine_gender()
{
    if(base.get_chance_male() + base.get_chance_female() == 0) return 'U';
    else if(base.get_chance_male() == 1.0) return 'U';
    else if(base.get_chance_female() == 1.0) return 'F';
    else
    {
        srand( time(NULL) );
        double val = (rand() % 1000 + 1)/1000;
        if(val <= base.get_chance_male()) return 'M';
        else return 'F';
    }
}

std::string spec_pkmn::determine_nature()
{
    //May make nature its own class
    std::string nature_pool[] = {"Hardy","Lonely","Brave","Adamant","Naughty","Bold",
                                 "Docile","Relaxed","Impish","Lax","Timid","Hasty",
                                 "Serious","Jolly","Naive","Modest","Mild","Quiet",
                                 "Bashful","Rash","Calm","Gentle","Sassy","Careful",
                                 "Quirky"};
    srand( time(NULL) );
    int index = rand() % 25;
    return nature_pool[index];
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

std::map<std::string, int> spec_pkmn::init_volatile_status_map()
{
    //TODO: status names copied from Bulbapedia, change later
    std::map<std::string, int> status_map;
    status_map["confusion"] = 0;
    status_map["curse"] = 0;
    status_map["embargo"] = 0;
    status_map["encore"] = 0;
    status_map["flinch"] = 0;
    status_map["heal block"] = 0;
    status_map["identification"] = 0;
    status_map["infatuation"] = 0;
    status_map["nightmare"] = 0;
    status_map["partially trapped"] = 0;
    status_map["perish song"] = 0;
    status_map["seeded"] = 0;
    status_map["taunt"] = 0;
    status_map["telekinetic levitation"] = 0;
    status_map["torment"] = 0;
    status_map["trapped"] = 0;
    status_map["aqua ring"] = 0;
    status_map["bracing"] = 0;
    status_map["center of attention"] = 0;
    status_map["defense curl"] = 0;
    status_map["focus energy"] = 0;
    status_map["glowing"] = 0;
    status_map["rooting"] = 0;
    status_map["magic coat"] = 0;
    status_map["magnetic levitation"] = 0;
    status_map["minimize"] = 0;
    status_map["protection"] = 0;
    status_map["recharging"] = 0;
    status_map["semi-invulnerable"] = 0;
    status_map["substitute"] = 0;
    status_map["taking aim"] = 0;
    status_map["taking in sunlight"] = 0;
    status_map["withdrawing"] = 0;
    status_map["whipping up a whirlwind"] = 0;

    return status_map;
}
