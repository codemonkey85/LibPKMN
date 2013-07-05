/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <math.h>
#include <time.h>

#include <boost/assign.hpp>

#include "spec_pkmn_gen345impl.hpp"

using namespace std;

namespace pkmnsim
{
    spec_pkmn_gen345impl::spec_pkmn_gen345impl(base_pkmn::sptr b, int lvl, int gen,
                                           string m1, string m2, string m3, string m4,
                                           bool i): spec_pkmn(b,i,m1,m2,m3,m4,gen,lvl)
    {
        srand ( time(NULL) );

        //Generation 2 uses IV's to determine shininess, but using the overall chance (1/8192) is simpler
        int shiny_val = rand() % 8192;
        if(shiny_val == 500) shiny = true; //Arbitrary
        else shiny = false;

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

        sprite_path = b->get_sprite_path((gender != Genders::FEMALE), shiny);

        nonvolatile_status = Statuses::OK;
        reset_volatile_status_map();
    }

    dict<string, int> spec_pkmn_gen345impl::get_stats()
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

    dict<string, int> spec_pkmn_gen345impl::get_IVs()
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

    dict<string, int> spec_pkmn_gen345impl::get_EVs()
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

    int spec_pkmn_gen345impl::get_gender() {return gender;}

    pkmn_nature::sptr spec_pkmn_gen345impl::get_nature() {return nature;}

    string spec_pkmn_gen345impl::get_ability() {return ability;}

    string spec_pkmn_gen345impl::get_held_item() {return held_item;}

    string spec_pkmn_gen345impl::get_info()
    {
        string types_str;
        dict<int, string> types = base->get_types();
        if(types[1] == "None") types_str = types[0];

        else types_str = types[0] + "/" + types[1];
        string stats_str = to_string(HP) + ", " + to_string(ATK) + ", "
                         + to_string(DEF) + ", " + to_string(SATK) + ", "
                         + to_string(SDEF) + ", " + to_string(SPD);

        //Get gender character
        char gender_char;
        switch(gender)
        {
            case Genders::MALE:
                gender_char = 'M';
                break;

            case Genders::FEMALE:
                gender_char = 'F';
                break;

            default:
                gender_char = 'N';
                break;
        }


        string output_string;
        output_string = nickname + " (" + base->get_display_name() + " " + gender_char + ")\n"
                      + "Level " + to_string(level) + "\n"
                      + "Type: " + types_str + "\n"
                      + "Ability: " + ability + "\n"
                      + "Held Item: " + held_item + "\n"
                      + "Stats: " + stats_str;

        return output_string;
    }

    string spec_pkmn_gen345impl::get_info_verbose()
    {
        string types_str;
        dict<int, string> types = base->get_types();
        if(types[1] == "None") types_str = types[0];
        else types_str = types[0] + "/" + types[1];

        string output_string;
        output_string = nickname + " (" + base->get_display_name() + ")\n"
                      + "Level " + to_string(level) + "\n"
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
                      + " - Special Attack: " + to_string(evSATK) + "\n"
                      + " - Special Defense: " + to_string(evSDEF) + "\n"
                      + " - Speed: " + to_string(evSPD) + "\n";

        return output_string;
    }

    int spec_pkmn_gen345impl::get_hp_from_iv_ev()
    {
        dict<string, int> stats = base->get_base_stats();

        int hp_val = int(floor(((double(ivHP) + (2.0*double(stats["HP"])) + (0.25*double(evHP)) + 100.0)
                     * double(level))/100.0 + 10.0));
        return hp_val;
    }

    int spec_pkmn_gen345impl::get_stat_from_iv_ev(string stat, int ivSTAT, int evSTAT)
    {
        dict<string, int> stats = base->get_base_stats();
        double nature_mod = nature->get_mods()[stat];

        int stat_val = int(ceil(((((double(ivSTAT) + 2.0*double(stats[stat]) + 0.25*double(evSTAT)) * double(level))/100.0) + 5.0) * nature_mod));
        return stat_val;
    }

    void spec_pkmn_gen345impl::reset_volatile_status_map()
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
            ("reintging",0)
            ("semi-invulnerable",0)
            ("substitute",0)
            ("taking aim",0)
            ("taking in sunlight",0)
            ("withdrawing",0)
            ("whipping up a whirlwind",0)
        ;
    }

    int spec_pkmn_gen345impl::determine_gender()
    {
        if(base->get_chance_male() + base->get_chance_female() == 0) return Genders::GENDERLESS;
        else if(base->get_chance_male() == 1.0) return Genders::MALE;
        else if(base->get_chance_female() == 1.0) return Genders::FEMALE;
        else
        {
            srand( time(NULL) );
            double val = (rand() % 1000 + 1)/1000.0;
            if(val <= base->get_chance_male()) return Genders::MALE;
            else return Genders::FEMALE;
        }
    }

    pkmn_nature::sptr spec_pkmn_gen345impl::determine_nature()
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

    string spec_pkmn_gen345impl::determine_ability()
    {
        srand( time(NULL) );
        dict<int, string> abilities = base->get_abilities();

        if(abilities[1] == "None" and abilities[2] == "None") ability = abilities[0]; //Single ability
        else if(abilities[2] == "None") ability = abilities[rand() % 2]; //Two abilities, no hidden ability
        else if(abilities[1] == "None" and abilities[2] != "None") //One normal ability, one hidden ability
        {
            int num;

            do
            {
                num = rand() % 3;
            } while(num == 1);

            ability = abilities[num];
        }
        else ability = abilities[rand() % 3]; //Three abilities
		
		return ability;
    }
}
