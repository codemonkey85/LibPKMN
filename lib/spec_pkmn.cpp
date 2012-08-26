#include <iostream>
#include <string>
#include <time.h>
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

    nonvolative_status = "OK";
    std::map<std::string, int> volative_status_map = init_volative_status_map();
}

//Importing specific Pokemon, such as when building a team
spec_pkmn::spec_pkmn(base b, std::string name, int lvl, char gndr, std::string nat, std::string hld, int hpstat, int atkstat, int defstat, int spdstat,
                     int satkstat, int sdefstat, std::string m1, std::string m2, std::string m3, std::string m4) 
{
    base b;
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
}

char spec_pkmn::determine_gender()
{
    if(chance_male + chance_female == 0) return "U";
    else if(chance_male == 1.0) return "M";
    else if(chance_female == 1.0) return "F";
    else
    {
        srand( time(NULL) );
        double val = (rand() % 1000 + 1)/1000;
        if(val <= chance_male) return "M";
        else return "F";
    }
}

char spec_pkmn::determine_nature()
{
    //May make nature its own class
    std::string nature_pool[] = {"Hardy","Lonely","Brave","Adamant","Naughty","Bold",
                                 "Docile","Relaxed","Impish","Lax","Timid","Hasty",
                                 "Serious","Jolly","Naive","Modest","Mild","Quiet",
                                 "Bashful","Rash","Calm","Gentle","Sassy","Careful",
                                 "Quirky"};
    srand( time(NULL) );
    double index = rand() % 25;
    return nature_pool[index];
}

int get_hp_from_iv()
{
    hp_val = (ivHP + 2(base.get_base_stats()[0]) + 0.25(base.get_ev_yields[0]) + 100)/100 + 10;
    return hp_val;
}

int get_stat_from_iv(int arr_index, int ivSTAT)
{
    int nature_mod = 0; //TODO: generic method
    stat_val = ((((ivSTAT + 2(base.get_base_stats()[arr_index]) + 0.25(base.get_ev_yields[arr_index])) * level)/100) + 5) * nature_mod;
    return stat_val;
}

int get_iv_from_hp()
{
    iv_val = (11(HP - 10))/level - 2(base.get_base_stats()[0]) - 0.25(base.get_ev_yields()[0]) - 100;
    return iv_val;
}

int get_iv_from_stat(int arr_index, int STAT)
{
    nature_mod = 0; //TODO: generic method
    stat = ((base.get_base_stats()[arr_index]/nature_mod) - 5)(100 / level) - 2(base.get_base_stats()[arr_index]) - 0.25(base.get_ev_yields()[arr_index]);
    return stat;
}

std::map<std::string, int> init_nonvolative_status_map()
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
