/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/conversions.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/pkmn_nature.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include <pkmnsim/database/queries.hpp>

#include <pkmds/pkmds_g5_sqlite.h>

#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    spec_pkmn::sptr converter::pkmds_pkmn_to_spec_pkmn(party_pkm* p_pkm)
    {
        opendb(get_database_path().c_str());

        string identifier, move1, move2, move3, move4;
        int level;

        identifier = database::to_database_format(lookuppkmname(p_pkm->pkm_data));
        level = getpkmlevel(p_pkm->pkm_data);

        move1 = lookupmovename(p_pkm->pkm_data.moves[0]);
        if(p_pkm->pkm_data.moves[1] == Moves::NOTHING) move2 = "None";
        else move2 = lookupmovename(p_pkm->pkm_data.moves[1]);
        if(p_pkm->pkm_data.moves[2] == Moves::NOTHING) move3 = "None";
        else move3 = lookupmovename(p_pkm->pkm_data.moves[2]);
        if(p_pkm->pkm_data.moves[3] == Moves::NOTHING) move4 = "None";
        else move4 = lookupmovename(p_pkm->pkm_data.moves[3]);

        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, 5, level, move1, move2, move3, move4);

        wstring nickname_wide = getpkmnickname(p_pkm->pkm_data);
        char nickname_buffer[11];
        memset(nickname_buffer,0,11);
        wcstombs(nickname_buffer, nickname_wide.c_str(), 11);
        s_pkmn->nickname = nickname_buffer;

        if(p_pkm->pkm_data.item == Items::NOTHING) s_pkmn->held_item = "None";
        else s_pkmn->held_item = lookupitemname(p_pkm->pkm_data);
        s_pkmn->nature = pkmn_nature::make(getnaturename(p_pkm->pkm_data));
        s_pkmn->pid = p_pkm->pkm_data.pid;
        s_pkmn->sid = p_pkm->pkm_data.sid;
        s_pkmn->tid = p_pkm->pkm_data.tid;
        switch(int(getpkmgender(p_pkm->pkm_data)))
        {
            case ::Genders::male:
                s_pkmn->gender = Genders::MALE;
                break;

            case ::Genders::female:
                s_pkmn->gender = Genders::FEMALE;
                break;

            default:
                s_pkmn->gender = Genders::GENDERLESS;
                break;
        }

        //Stats
        s_pkmn->HP = getpkmstat(p_pkm->pkm_data, Stat_IDs::hp);
        s_pkmn->ATK = getpkmstat(p_pkm->pkm_data, Stat_IDs::attack);
        s_pkmn->DEF = getpkmstat(p_pkm->pkm_data, Stat_IDs::defense);
        s_pkmn->SATK = getpkmstat(p_pkm->pkm_data, Stat_IDs::spatk);
        s_pkmn->SDEF = getpkmstat(p_pkm->pkm_data, Stat_IDs::spdef);
        s_pkmn->SPD = getpkmstat(p_pkm->pkm_data, Stat_IDs::speed);

        //IV's
        s_pkmn->ivHP = p_pkm->pkm_data.ivs.hp;
        s_pkmn->ivATK = p_pkm->pkm_data.ivs.attack;
        s_pkmn->ivDEF = p_pkm->pkm_data.ivs.defense;
        s_pkmn->ivSATK = p_pkm->pkm_data.ivs.spatk;
        s_pkmn->ivSDEF = p_pkm->pkm_data.ivs.spdef;
        s_pkmn->ivSPD = p_pkm->pkm_data.ivs.speed;

        //EV's
        s_pkmn->evHP = p_pkm->pkm_data.evs.hp;
        s_pkmn->evATK = p_pkm->pkm_data.evs.attack;
        s_pkmn->evDEF = p_pkm->pkm_data.evs.defense;
        s_pkmn->evSATK = p_pkm->pkm_data.evs.spatk;
        s_pkmn->evSDEF = p_pkm->pkm_data.evs.spdef;
        s_pkmn->evSPD = p_pkm->pkm_data.evs.speed;

        //Attributes
        s_pkmn->attributes["friendship"] = p_pkm->pkm_data.tameness;
        s_pkmn->attributes["circle"] = p_pkm->pkm_data.markings.circle;
        s_pkmn->attributes["triangle"] = p_pkm->pkm_data.markings.triangle;
        s_pkmn->attributes["square"] = p_pkm->pkm_data.markings.square;
        s_pkmn->attributes["heart"] = p_pkm->pkm_data.markings.heart;
        s_pkmn->attributes["star"] = p_pkm->pkm_data.markings.star;
        s_pkmn->attributes["diamond"] = p_pkm->pkm_data.markings.diamond;
        s_pkmn->attributes["country"] = int(p_pkm->pkm_data.country);
        s_pkmn->attributes["cool"] = p_pkm->pkm_data.contest.cool;
        s_pkmn->attributes["beauty"] = p_pkm->pkm_data.contest.beauty;
        s_pkmn->attributes["cute"] = p_pkm->pkm_data.contest.cute;
        s_pkmn->attributes["smart"] = p_pkm->pkm_data.contest.smart;
        s_pkmn->attributes["tough"] = p_pkm->pkm_data.contest.tough;
        s_pkmn->attributes["sheen"] = p_pkm->pkm_data.contest.sheen;
        s_pkmn->attributes["sinnoh_champ_ribbon"] = p_pkm->pkm_data.sribbon1.sinnoh_champ_ribbon;
        s_pkmn->attributes["sinnoh_ability_ribbon"] = p_pkm->pkm_data.sribbon1.ability_ribbon;
        s_pkmn->attributes["sinnoh_great_ability_ribbon"] = p_pkm->pkm_data.sribbon1.great_ability_ribbon;
        s_pkmn->attributes["sinnoh_double_ability_ribbon"] = p_pkm->pkm_data.sribbon1.double_ability_ribbon;
        s_pkmn->attributes["sinnoh_multi_ability_ribbon"] = p_pkm->pkm_data.sribbon1.multi_ability_ribbon;
        s_pkmn->attributes["sinnoh_pair_ability_ribbon"] = p_pkm->pkm_data.sribbon1.pair_ability_ribbon;
        s_pkmn->attributes["sinnoh_world_ability_ribbon"] = p_pkm->pkm_data.sribbon1.world_ability_ribbon;
        s_pkmn->attributes["sinnoh_alert_ribbon"] = p_pkm->pkm_data.sribbon1.alert_ribbon;
        s_pkmn->attributes["sinnoh_shock_ribbon"] = p_pkm->pkm_data.sribbon1.shock_ribbon;
        s_pkmn->attributes["sinnoh_downcast_ribbon"] = p_pkm->pkm_data.sribbon1.downcast_ribbon;
        s_pkmn->attributes["sinnoh_careless_ribbon"] = p_pkm->pkm_data.sribbon1.careless_ribbon;
        s_pkmn->attributes["sinnoh_relax_ribbon"] = p_pkm->pkm_data.sribbon1.relax_ribbon;
        s_pkmn->attributes["sinnoh_snooze_ribbon"] = p_pkm->pkm_data.sribbon1.snooze_ribbon;
        s_pkmn->attributes["sinnoh_smile_ribbon"] = p_pkm->pkm_data.sribbon1.smile_ribbon;
        s_pkmn->attributes["sinnoh_gorgeous_ribbon"] = p_pkm->pkm_data.sribbon1.gorgeous_ribbon;
        s_pkmn->attributes["sinnoh_royal_ribbon"] = p_pkm->pkm_data.sribbon1.royal_ribbon;
        s_pkmn->attributes["sinnoh_gorgeous_royal_ribbon"] = p_pkm->pkm_data.sribbon2.gorgeous_royal_ribbon;
        s_pkmn->attributes["sinnoh_footprint_ribbon"] = p_pkm->pkm_data.sribbon2.footprint_ribbon;
        s_pkmn->attributes["sinnoh_record_ribbon"] = p_pkm->pkm_data.sribbon2.record_ribbon;
        s_pkmn->attributes["sinnoh_history_ribbon"] = p_pkm->pkm_data.sribbon2.history_ribbon;
        s_pkmn->attributes["sinnoh_legend_ribbon"] = p_pkm->pkm_data.sribbon2.legend_ribbon;
        s_pkmn->attributes["sinnoh_red_ribbon"] = p_pkm->pkm_data.sribbon2.red_ribbon;
        s_pkmn->attributes["sinnoh_green_ribbon"] = p_pkm->pkm_data.sribbon2.green_ribbon;
        s_pkmn->attributes["sinnoh_blue_ribbon"] = p_pkm->pkm_data.sribbon2.blue_ribbon;
        s_pkmn->attributes["sinnoh_festival_ribbon"] = p_pkm->pkm_data.sribbon2.festival_ribbon;
        s_pkmn->attributes["sinnoh_carnival_ribbon"] = p_pkm->pkm_data.sribbon2.carnival_ribbon;
        s_pkmn->attributes["sinnoh_classic_ribbon"] = p_pkm->pkm_data.sribbon2.classic_ribbon;
        s_pkmn->attributes["sinnoh_premier_ribbon"] = p_pkm->pkm_data.sribbon2.premier_ribbon;
        s_pkmn->attributes["hoenn_cool_ribbon"] = p_pkm->pkm_data.hribbon1.cool_ribbon;
        s_pkmn->attributes["hoenn_cool_ribbon_super"] = p_pkm->pkm_data.hribbon1.cool_ribbon_super;
        s_pkmn->attributes["hoenn_cool_ribbon_hyper"] = p_pkm->pkm_data.hribbon1.cool_ribbon_hyper;
        s_pkmn->attributes["hoenn_cool_ribbon_master"] = p_pkm->pkm_data.hribbon1.cool_ribbon_master;
        s_pkmn->attributes["hoenn_beauty_ribbon"] = p_pkm->pkm_data.hribbon1.beauty_ribbon;
        s_pkmn->attributes["hoenn_beauty_ribbon_super"] = p_pkm->pkm_data.hribbon1.beauty_ribbon_super;
        s_pkmn->attributes["hoenn_beauty_ribbon_hyper"] = p_pkm->pkm_data.hribbon1.beauty_ribbon_hyper;
        s_pkmn->attributes["hoenn_beauty_ribbon_master"] = p_pkm->pkm_data.hribbon1.beauty_ribbon_master;
        s_pkmn->attributes["hoenn_cute_ribbon"] = p_pkm->pkm_data.hribbon1.cute_ribbon;
        s_pkmn->attributes["hoenn_cute_ribbon_super"] = p_pkm->pkm_data.hribbon1.cute_ribbon_super;
        s_pkmn->attributes["hoenn_cute_ribbon_hyper"] = p_pkm->pkm_data.hribbon1.cute_ribbon_hyper;
        s_pkmn->attributes["hoenn_cute_ribbon_master"] = p_pkm->pkm_data.hribbon1.cute_ribbon_master;
        s_pkmn->attributes["hoenn_smart_ribbon"] = p_pkm->pkm_data.hribbon1.smart_ribbon;
        s_pkmn->attributes["hoenn_smart_ribbon_super"] = p_pkm->pkm_data.hribbon1.smart_ribbon_super;
        s_pkmn->attributes["hoenn_smart_ribbon_hyper"] = p_pkm->pkm_data.hribbon1.smart_ribbon_hyper;
        s_pkmn->attributes["hoenn_smart_ribbon_master"] = p_pkm->pkm_data.hribbon1.smart_ribbon_master;
        s_pkmn->attributes["hoenn_tough_ribbon"] = p_pkm->pkm_data.hribbon2.tough_ribbon;
        s_pkmn->attributes["hoenn_tough_ribbon_super"] = p_pkm->pkm_data.hribbon2.tough_ribbon_super;
        s_pkmn->attributes["hoenn_tough_ribbon_hyper"] = p_pkm->pkm_data.hribbon2.tough_ribbon_hyper;
        s_pkmn->attributes["hoenn_tough_ribbon_master"] = p_pkm->pkm_data.hribbon2.tough_ribbon_master;
        s_pkmn->attributes["hoenn_champion_ribbon"] = p_pkm->pkm_data.hribbon2.champion_ribbon;
        s_pkmn->attributes["hoenn_winning_ribbon"] = p_pkm->pkm_data.hribbon2.winning_ribbon;
        s_pkmn->attributes["hoenn_victory_ribbon"] = p_pkm->pkm_data.hribbon2.victory_ribbon;
        s_pkmn->attributes["hoenn_artist_ribbon"] = p_pkm->pkm_data.hribbon2.artist_ribbon;
        s_pkmn->attributes["hoenn_effort_ribbon"] = p_pkm->pkm_data.hribbon2.effort_ribbon;
        s_pkmn->attributes["hoenn_marine_ribbon"] = p_pkm->pkm_data.hribbon2.marine_ribbon;
        s_pkmn->attributes["hoenn_land_ribbon"] = p_pkm->pkm_data.hribbon2.land_ribbon;
        s_pkmn->attributes["hoenn_sky_ribbon"] = p_pkm->pkm_data.hribbon2.sky_ribbon;
        s_pkmn->attributes["hoenn_country_ribbon"] = p_pkm->pkm_data.hribbon2.country_ribbon;
        s_pkmn->attributes["hoenn_national_ribbon"] = p_pkm->pkm_data.hribbon2.national_ribbon;
        s_pkmn->attributes["hoenn_earth_ribbon"] = p_pkm->pkm_data.hribbon2.earth_ribbon;
        s_pkmn->attributes["hoenn_world_ribbon"] = p_pkm->pkm_data.hribbon2.world_ribbon;
        s_pkmn->attributes["fateful_encounter"] = p_pkm->pkm_data.forms.fencounter;
        s_pkmn->attributes["has_dream_world_ability"] = p_pkm->pkm_data.dwability.hasdwability;
        s_pkmn->attributes["n_flag"] = p_pkm->pkm_data.dwability.n_pkm;
        s_pkmn->attributes["original_game"] = int(p_pkm->pkm_data.hometown);
        s_pkmn->attributes["sinnoh_cool_ribbon"] = p_pkm->pkm_data.sribbon3.cool_ribbon;
        s_pkmn->attributes["sinnoh_cool_ribbon_great"] = p_pkm->pkm_data.sribbon3.cool_ribbon_great;
        s_pkmn->attributes["sinnoh_cool_ribbon_ultra"] = p_pkm->pkm_data.sribbon3.cool_ribbon_ultra;
        s_pkmn->attributes["sinnoh_cool_ribbon_master"] = p_pkm->pkm_data.sribbon3.cool_ribbon_master;
        s_pkmn->attributes["sinnoh_beauty_ribbon"] = p_pkm->pkm_data.sribbon3.beauty_ribbon;
        s_pkmn->attributes["sinnoh_beauty_ribbon_great"] = p_pkm->pkm_data.sribbon3.beauty_ribbon_great;
        s_pkmn->attributes["sinnoh_beauty_ribbon_ultra"] = p_pkm->pkm_data.sribbon3.beauty_ribbon_ultra;
        s_pkmn->attributes["sinnoh_beauty_ribbon_master"] = p_pkm->pkm_data.sribbon3.beauty_ribbon_master;
        s_pkmn->attributes["sinnoh_cute_ribbon"] = p_pkm->pkm_data.sribbon3.cute_ribbon;
        s_pkmn->attributes["sinnoh_cute_ribbon_great"] = p_pkm->pkm_data.sribbon3.cute_ribbon_great;
        s_pkmn->attributes["sinnoh_cute_ribbon_ultra"] = p_pkm->pkm_data.sribbon3.cute_ribbon_ultra;
        s_pkmn->attributes["sinnoh_cute_ribbon_master"] = p_pkm->pkm_data.sribbon3.cute_ribbon_master;
        s_pkmn->attributes["sinnoh_smart_ribbon"] = p_pkm->pkm_data.sribbon3.smart_ribbon;
        s_pkmn->attributes["sinnoh_smart_ribbon_great"] = p_pkm->pkm_data.sribbon3.smart_ribbon_great;
        s_pkmn->attributes["sinnoh_smart_ribbon_ultra"] = p_pkm->pkm_data.sribbon3.smart_ribbon_ultra;
        s_pkmn->attributes["sinnoh_smart_ribbon_master"] = p_pkm->pkm_data.sribbon3.smart_ribbon_master;
        s_pkmn->attributes["sinnoh_tough_ribbon"] = p_pkm->pkm_data.sribbon4.tough_ribbon;
        s_pkmn->attributes["sinnoh_tough_ribbon_great"] = p_pkm->pkm_data.sribbon4.tough_ribbon_great;
        s_pkmn->attributes["sinnoh_tough_ribbon_ultra"] = p_pkm->pkm_data.sribbon4.tough_ribbon_ultra;
        s_pkmn->attributes["sinnoh_tough_ribbon_master"] = p_pkm->pkm_data.sribbon4.tough_ribbon_master;
        s_pkmn->attributes["eggmet_year"] = p_pkm->pkm_data.eggdate.year;
        s_pkmn->attributes["eggmet_month"] = p_pkm->pkm_data.eggdate.month;
        s_pkmn->attributes["eggmet_day"] = p_pkm->pkm_data.eggdate.day;
        s_pkmn->attributes["met_year"] = p_pkm->pkm_data.metdate.year;
        s_pkmn->attributes["met_month"] = p_pkm->pkm_data.metdate.month;
        s_pkmn->attributes["met_day"] = p_pkm->pkm_data.metdate.day;
        s_pkmn->attributes["eggmet_location"] = int(p_pkm->pkm_data.eggmet);
        s_pkmn->attributes["met_location"] = int(p_pkm->pkm_data.met);
        s_pkmn->attributes["unova_ball"] = int(p_pkm->pkm_data.ball);
        s_pkmn->attributes["met_level"] = p_pkm->pkm_data.metlevel_otgender.metlevel;
        s_pkmn->attributes["ot_is_female"] = int(p_pkm->pkm_data.metlevel_otgender.metlevel);
        s_pkmn->attributes["unova_encounter"] = int(p_pkm->pkm_data.encounter);

        return s_pkmn;
    }

    void converter::spec_pkmn_to_pkmds_pkmn(spec_pkmn::sptr s_pkmn, party_pkm* p_pkm)
    {
        p_pkm->pkm_data.species = Species::pkmspecies(s_pkmn->get_base_pkmn()->get_nat_pokedex_num());
        p_pkm->pkm_data.moves[0] = Moves::moves(s_pkmn->get_moves()[0]->get_move_id());
        if(s_pkmn->get_moves()[1]->get_name() == "Struggle") p_pkm->pkm_data.moves[1] = Moves::NOTHING;
        else p_pkm->pkm_data.moves[1] = Moves::moves(s_pkmn->get_moves()[1]->get_move_id());
        if(s_pkmn->get_moves()[2]->get_name() == "Struggle") p_pkm->pkm_data.moves[2] = Moves::NOTHING;
        else p_pkm->pkm_data.moves[2] = Moves::moves(s_pkmn->get_moves()[2]->get_move_id());
        if(s_pkmn->get_moves()[3]->get_name() == "Struggle") p_pkm->pkm_data.moves[3] = Moves::NOTHING;
        else p_pkm->pkm_data.moves[3] = Moves::moves(s_pkmn->get_moves()[3]->get_move_id());

        //Unlike species and moves, nature ID's don't correspond to anything PKMNsim uses
        unsigned char nature_map[] = {0,1,6,16,21,11,2,7,17,22,12,4,9,19,24,20,14,5,10,25,15,3,8,18,23,13};
        p_pkm->pkm_data.nature = Natures::natures(nature_map[s_pkmn->nature->get_nature_id()]);
        setlevel(p_pkm->pkm_data, s_pkmn->level);
        #ifdef __linux__
        setpkmnickname(p_pkm->pkm_data, (wchar_t*)getwstring((char*)s_pkmn->nickname.c_str(), s_pkmn->nickname.size()).c_str(), s_pkmn->nickname.size());
        #else
        setpkmnickname(p_pkm->pkm_data, (wchar_t*)getwstring(s_pkmn->nickname).c_str(), s_pkmn->nickname.size());
        #endif
        p_pkm->pkm_data.pid = s_pkmn->pid;
        p_pkm->pkm_data.sid = s_pkmn->sid;
        p_pkm->pkm_data.tid = s_pkmn->tid;

        p_pkm->party_data.maxhp = s_pkmn->HP;
        p_pkm->party_data.attack = s_pkmn->ATK;
        p_pkm->party_data.defense = s_pkmn->DEF;
        p_pkm->party_data.spatk = s_pkmn->SATK;
        p_pkm->party_data.spdef = s_pkmn->SDEF;
        p_pkm->party_data.speed = s_pkmn->SPD;

        p_pkm->pkm_data.ivs.hp = s_pkmn->ivHP;
        p_pkm->pkm_data.ivs.attack = s_pkmn->ivATK;
        p_pkm->pkm_data.ivs.defense = s_pkmn->ivDEF;
        p_pkm->pkm_data.ivs.spatk = s_pkmn->ivSATK;
        p_pkm->pkm_data.ivs.spdef = s_pkmn->ivSDEF;
        p_pkm->pkm_data.ivs.speed = s_pkmn->ivSPD;

        p_pkm->pkm_data.evs.hp = s_pkmn->evHP;
        p_pkm->pkm_data.evs.attack = s_pkmn->evATK;
        p_pkm->pkm_data.evs.defense = s_pkmn->evDEF;
        p_pkm->pkm_data.evs.spatk = s_pkmn->evSATK;
        p_pkm->pkm_data.evs.spdef = s_pkmn->evSDEF;
        p_pkm->pkm_data.evs.speed = s_pkmn->evSPD;

        //Attributes
        if(s_pkmn->attributes.has_key("friendship"))
            p_pkm->pkm_data.tameness = byte(s_pkmn->attributes["friendship"]);
        if(s_pkmn->attributes.has_key("circle"))
            p_pkm->pkm_data.markings.circle = bool(s_pkmn->attributes["circle"]);
        if(s_pkmn->attributes.has_key("triangle"))
            p_pkm->pkm_data.markings.circle = bool(s_pkmn->attributes["triangle"]);
        if(s_pkmn->attributes.has_key("square"))
            p_pkm->pkm_data.markings.circle = bool(s_pkmn->attributes["square"]);
        if(s_pkmn->attributes.has_key("heart")) 
            p_pkm->pkm_data.markings.circle = bool(s_pkmn->attributes["heart"]);
        if(s_pkmn->attributes.has_key("star"))
            p_pkm->pkm_data.markings.circle = bool(s_pkmn->attributes["star"]);
        if(s_pkmn->attributes.has_key("diamond"))
            p_pkm->pkm_data.markings.circle = bool(s_pkmn->attributes["diamond"]);
        if(s_pkmn->attributes.has_key("country"))
            p_pkm->pkm_data.country = Countries::countries(s_pkmn->attributes["country"]);
        if(s_pkmn->attributes.has_key("cool"))
            p_pkm->pkm_data.contest.cool = byte(s_pkmn->attributes["cool"]);
        if(s_pkmn->attributes.has_key("beauty"))
            p_pkm->pkm_data.contest.cool = byte(s_pkmn->attributes["beauty"]);
        if(s_pkmn->attributes.has_key("cute"))
            p_pkm->pkm_data.contest.cool = byte(s_pkmn->attributes["cute"]);
        if(s_pkmn->attributes.has_key("smart"))
            p_pkm->pkm_data.contest.cool = byte(s_pkmn->attributes["smart"]);
        if(s_pkmn->attributes.has_key("tough"))
            p_pkm->pkm_data.contest.cool = byte(s_pkmn->attributes["tough"]);
        if(s_pkmn->attributes.has_key("sheen"))
            p_pkm->pkm_data.contest.cool = byte(s_pkmn->attributes["sheen"]);
        if(s_pkmn->attributes.has_key("sinnoh_champ_ribbon"))
            p_pkm->pkm_data.sribbon1.sinnoh_champ_ribbon = bool(s_pkmn->attributes["sinnoh_champ_ribbon"]);
        if(s_pkmn->attributes.has_key("ability_ribbon"))
            p_pkm->pkm_data.sribbon1.ability_ribbon = bool(s_pkmn->attributes["sinnoh_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_great_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.great_ability_ribbon = bool(s_pkmn->attributes["sinnoh_great_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_double_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.double_ability_ribbon = bool(s_pkmn->attributes["sinnoh_double_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_multi_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.multi_ability_ribbon = bool(s_pkmn->attributes["sinnoh_multi_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_pair_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.pair_ability_ribbon = bool(s_pkmn->attributes["sinnoh_pair_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_world_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.world_ability_ribbon = bool(s_pkmn->attributes["sinnoh_world_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_alert_ribbon"))
            p_pkm->pkm_data.sribbon1.alert_ribbon = bool(s_pkmn->attributes["sinnoh_alert_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_shock_ribbon"))
            p_pkm->pkm_data.sribbon1.shock_ribbon = bool(s_pkmn->attributes["sinnoh_shock_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_downcast_ribbon"))
            p_pkm->pkm_data.sribbon1.downcast_ribbon = bool(s_pkmn->attributes["sinnoh_downcast_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_careless_ribbon"))
            p_pkm->pkm_data.sribbon1.careless_ribbon = bool(s_pkmn->attributes["sinnoh_careless_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_relax_ribbon"))
            p_pkm->pkm_data.sribbon1.relax_ribbon = bool(s_pkmn->attributes["sinnoh_relax_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_snooze_ribbon"))
            p_pkm->pkm_data.sribbon1.snooze_ribbon = bool(s_pkmn->attributes["sinnoh_snooze_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_smile_ribbon"))
            p_pkm->pkm_data.sribbon1.smile_ribbon = bool(s_pkmn->attributes["sinnoh_smile_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_gorgeous_ribbon"))
            p_pkm->pkm_data.sribbon1.gorgeous_ribbon = bool(s_pkmn->attributes["sinnoh_gorgeous_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_royal_ribbon"))
            p_pkm->pkm_data.sribbon1.royal_ribbon = bool(s_pkmn->attributes["sinnoh_royal_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_gorgeous_royal_ribbon"))
            p_pkm->pkm_data.sribbon2.gorgeous_royal_ribbon = bool(s_pkmn->attributes["sinnoh_gorgeous_royal_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_footprint_ribbon"))
            p_pkm->pkm_data.sribbon2.footprint_ribbon = bool(s_pkmn->attributes["sinnoh_footprint_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_record_ribbon"))
            p_pkm->pkm_data.sribbon2.record_ribbon = bool(s_pkmn->attributes["sinnoh_record_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_history_ribbon"))
            p_pkm->pkm_data.sribbon2.history_ribbon = bool(s_pkmn->attributes["sinnoh_history_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_legend_ribbon"))
            p_pkm->pkm_data.sribbon2.legend_ribbon = bool(s_pkmn->attributes["sinnoh_legend_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_red_ribbon"))
            p_pkm->pkm_data.sribbon2.red_ribbon = bool(s_pkmn->attributes["sinnoh_red_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_green_ribbon"))
            p_pkm->pkm_data.sribbon2.green_ribbon = bool(s_pkmn->attributes["sinnoh_green_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_blue_ribbon"))
            p_pkm->pkm_data.sribbon2.blue_ribbon = bool(s_pkmn->attributes["sinnoh_blue_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_festival_ribbon"))
            p_pkm->pkm_data.sribbon2.festival_ribbon = bool(s_pkmn->attributes["sinnoh_festival_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_carnival_ribbon"))
            p_pkm->pkm_data.sribbon2.carnival_ribbon = bool(s_pkmn->attributes["sinnoh_carnival_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_classic_ribbon"))
            p_pkm->pkm_data.sribbon2.classic_ribbon = bool(s_pkmn->attributes["sinnoh_classic_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_premier_ribbon"))
            p_pkm->pkm_data.sribbon2.premier_ribbon = bool(s_pkmn->attributes["sinnoh_premier_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon"))
            p_pkm->pkm_data.hribbon1.cute_ribbon = bool(s_pkmn->attributes["hoenn_cute_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_super"))
            p_pkm->pkm_data.hribbon1.cool_ribbon_super = bool(s_pkmn->attributes["hoenn_cool_ribbon_super"]);
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_hyper"))
            p_pkm->pkm_data.hribbon1.cool_ribbon_hyper = bool(s_pkmn->attributes["hoenn_cool_ribbon_hyper"]);
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_master"))
            p_pkm->pkm_data.hribbon1.cool_ribbon_master = bool(s_pkmn->attributes["hoenn_cool_ribbon_master"]);
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon"))
            p_pkm->pkm_data.hribbon1.beauty_ribbon = bool(s_pkmn->attributes["hoenn_beauty_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_super"))
            p_pkm->pkm_data.hribbon1.beauty_ribbon_super = bool(s_pkmn->attributes["hoenn_beauty_ribbon_super"]);
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_hyper"))
            p_pkm->pkm_data.hribbon1.beauty_ribbon_hyper = bool(s_pkmn->attributes["hoenn_beauty_ribbon_hyper"]);
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_master"))
            p_pkm->pkm_data.hribbon1.beauty_ribbon_master = bool(s_pkmn->attributes["hoenn_beauty_ribbon_master"]);
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon"))
            p_pkm->pkm_data.hribbon1.cute_ribbon = bool(s_pkmn->attributes["hoenn_cute_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_super"))
            p_pkm->pkm_data.hribbon1.cute_ribbon_super = bool(s_pkmn->attributes["hoenn_cute_ribbon_super"]);
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_hyper"))
            p_pkm->pkm_data.hribbon1.cute_ribbon_hyper = bool(s_pkmn->attributes["hoenn_cute_ribbon_hyper"]);
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_master"))
            p_pkm->pkm_data.hribbon1.cute_ribbon_master = bool(s_pkmn->attributes["hoenn_cute_ribbon_master"]);
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon"))
            p_pkm->pkm_data.hribbon1.smart_ribbon = bool(s_pkmn->attributes["hoenn_smart_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_super"))
            p_pkm->pkm_data.hribbon1.smart_ribbon_super = bool(s_pkmn->attributes["hoenn_smart_ribbon_super"]);
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_hyper"))
            p_pkm->pkm_data.hribbon1.smart_ribbon_hyper = bool(s_pkmn->attributes["hoenn_smart_ribbon_hyper"]);
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_master"))
            p_pkm->pkm_data.hribbon1.smart_ribbon_master = bool(s_pkmn->attributes["hoenn_smart_ribbon_master"]);
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon"))
            p_pkm->pkm_data.hribbon2.tough_ribbon = bool(s_pkmn->attributes["hoenn_tough_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_super"))
            p_pkm->pkm_data.hribbon2.tough_ribbon_super = bool(s_pkmn->attributes["hoenn_tough_ribbon_super"]);
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_hyper"))
            p_pkm->pkm_data.hribbon2.tough_ribbon_hyper = bool(s_pkmn->attributes["hoenn_tough_ribbon_hyper"]);
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_master"))
            p_pkm->pkm_data.hribbon2.tough_ribbon_master = bool(s_pkmn->attributes["hoenn_tough_ribbon_master"]);
        if(s_pkmn->attributes.has_key("hoenn_champion_ribbon"))
            p_pkm->pkm_data.hribbon2.champion_ribbon = bool(s_pkmn->attributes["hoenn_champion_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_winning_ribbon"))
            p_pkm->pkm_data.hribbon2.winning_ribbon = bool(s_pkmn->attributes["hoenn_winning_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_victory_ribbon"))
            p_pkm->pkm_data.hribbon2.victory_ribbon = bool(s_pkmn->attributes["hoenn_victory_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_artist_ribbon"))
            p_pkm->pkm_data.hribbon2.artist_ribbon = bool(s_pkmn->attributes["hoenn_artist_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_effort_ribbon"))
            p_pkm->pkm_data.hribbon2.effort_ribbon = bool(s_pkmn->attributes["hoenn_effort_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_marine_ribbon"))
            p_pkm->pkm_data.hribbon2.marine_ribbon = bool(s_pkmn->attributes["hoenn_marine_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_land_ribbon"))
            p_pkm->pkm_data.hribbon2.land_ribbon = bool(s_pkmn->attributes["hoenn_land_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_sky_ribbon"))
            p_pkm->pkm_data.hribbon2.sky_ribbon = bool(s_pkmn->attributes["hoenn_sky_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_country_ribbon"))
            p_pkm->pkm_data.hribbon2.country_ribbon = bool(s_pkmn->attributes["hoenn_country_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_national_ribbon"))
            p_pkm->pkm_data.hribbon2.national_ribbon = bool(s_pkmn->attributes["hoenn_national_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_earth_ribbon"))
            p_pkm->pkm_data.hribbon2.earth_ribbon = bool(s_pkmn->attributes["hoenn_earth_ribbon"]);
        if(s_pkmn->attributes.has_key("hoenn_world_ribbon"))
            p_pkm->pkm_data.hribbon2.world_ribbon = bool(s_pkmn->attributes["hoenn_world_ribbon"]);
        if(s_pkmn->attributes.has_key("fateful_encounter"))
            p_pkm->pkm_data.forms.fencounter = bool(s_pkmn->attributes["fateful_encounter"]);
        if(s_pkmn->attributes.has_key("has_dream_world_ability"))
            p_pkm->pkm_data.dwability.hasdwability = bool(s_pkmn->attributes["has_dream_world_ability"]);
        if(s_pkmn->attributes.has_key("n_flag"))
            p_pkm->pkm_data.dwability.n_pkm = bool(s_pkmn->attributes["n_flag"]);
        if(s_pkmn->attributes.has_key("original_game"))
            p_pkm->pkm_data.hometown = Hometowns::hometowns(s_pkmn->attributes["original_game"]);
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon"))
            p_pkm->pkm_data.sribbon3.cute_ribbon = bool(s_pkmn->attributes["sinnoh_cute_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon_great"))
            p_pkm->pkm_data.sribbon3.cool_ribbon_great = bool(s_pkmn->attributes["sinnoh_cool_ribbon_great"]);
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon_ultra"))
            p_pkm->pkm_data.sribbon3.cool_ribbon_ultra = bool(s_pkmn->attributes["sinnoh_cool_ribbon_ultra"]);
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon_master"))
            p_pkm->pkm_data.sribbon3.cool_ribbon_master = bool(s_pkmn->attributes["sinnoh_cool_ribbon_master"]);
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon"))
            p_pkm->pkm_data.sribbon3.beauty_ribbon = bool(s_pkmn->attributes["sinnoh_beauty_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon_great"))
            p_pkm->pkm_data.sribbon3.beauty_ribbon_great = bool(s_pkmn->attributes["sinnoh_beauty_ribbon_great"]);
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon_ultra"))
            p_pkm->pkm_data.sribbon3.beauty_ribbon_ultra = bool(s_pkmn->attributes["sinnoh_beauty_ribbon_ultra"]);
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon_master"))
            p_pkm->pkm_data.sribbon3.beauty_ribbon_master = bool(s_pkmn->attributes["sinnoh_beauty_ribbon_master"]);
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon"))
            p_pkm->pkm_data.sribbon3.cute_ribbon = bool(s_pkmn->attributes["sinnoh_cute_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon_great"))
            p_pkm->pkm_data.sribbon3.cute_ribbon_great = bool(s_pkmn->attributes["sinnoh_cute_ribbon_great"]);
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon_ultra"))
            p_pkm->pkm_data.sribbon3.cute_ribbon_ultra = bool(s_pkmn->attributes["sinnoh_cute_ribbon_ultra"]);
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon_master"))
            p_pkm->pkm_data.sribbon3.cute_ribbon_master = bool(s_pkmn->attributes["sinnoh_cute_ribbon_master"]);
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon"))
            p_pkm->pkm_data.sribbon3.smart_ribbon = bool(s_pkmn->attributes["sinnoh_smart_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon_great"))
            p_pkm->pkm_data.sribbon3.smart_ribbon_great = bool(s_pkmn->attributes["sinnoh_smart_ribbon_great"]);
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon_ultra"))
            p_pkm->pkm_data.sribbon3.smart_ribbon_ultra = bool(s_pkmn->attributes["sinnoh_smart_ribbon_ultra"]);
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon_master"))
            p_pkm->pkm_data.sribbon3.smart_ribbon_master = bool(s_pkmn->attributes["sinnoh_smart_ribbon_master"]);
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon"))
            p_pkm->pkm_data.sribbon4.tough_ribbon = bool(s_pkmn->attributes["sinnoh_tough_ribbon"]);
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon_great"))
            p_pkm->pkm_data.sribbon4.tough_ribbon_great = bool(s_pkmn->attributes["sinnoh_tough_ribbon_great"]);
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon_ultra"))
            p_pkm->pkm_data.sribbon4.tough_ribbon_ultra = bool(s_pkmn->attributes["sinnoh_tough_ribbon_ultra"]);
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon_master"))
            p_pkm->pkm_data.sribbon4.tough_ribbon_master = bool(s_pkmn->attributes["sinnoh_tough_ribbon_master"]);
        if(s_pkmn->attributes.has_key("unova_ball"))
            p_pkm->pkm_data.ball = Balls::balls(s_pkmn->attributes["unova_ball"]);

        //Don't fill in datefields unless all three attributes are present
        if(s_pkmn->attributes.has_key("eggmet_year") and s_pkmn->attributes.has_key("eggmet_month") and
           s_pkmn->attributes.has_key("eggmet_day"))
        {
            p_pkm->pkm_data.eggdate.year = byte(s_pkmn->attributes["eggmet_year"]);
            p_pkm->pkm_data.eggdate.month = byte(s_pkmn->attributes["eggmet_month"]);
            p_pkm->pkm_data.eggdate.day = byte(s_pkmn->attributes["eggmet_day"]);
        }
        if(s_pkmn->attributes.has_key("met_year") and s_pkmn->attributes.has_key("met_month") and
           s_pkmn->attributes.has_key("met_day"))
        {
            p_pkm->pkm_data.metdate.year = byte(s_pkmn->attributes["met_year"]);
            p_pkm->pkm_data.metdate.month = byte(s_pkmn->attributes["met_month"]);
            p_pkm->pkm_data.metdate.day = byte(s_pkmn->attributes["met_day"]);
        }

        if(s_pkmn->attributes.has_key("eggmet_location"))
            p_pkm->pkm_data.eggmet = Locations::locations(s_pkmn->attributes["eggmet_location"]);
        if(s_pkmn->attributes.has_key("met_location"))
            p_pkm->pkm_data.met = Locations::locations(s_pkmn->attributes["met_location"]);
        if(s_pkmn->attributes.has_key("met_level"))
            p_pkm->pkm_data.metlevel_otgender.metlevel = byte(s_pkmn->attributes["met_level"]);
        if(s_pkmn->attributes.has_key("met_level"))
            #if defined(__linux__)
            p_pkm->pkm_data.metlevel_otgender.metlevel = byte(s_pkmn->attributes["ot_is_female"]);
            #else
            p_pkm->pkm_data.metlevel_otgender.metlevel = ::Genders::genders(s_pkmn->attributes["ot_is_female"]);
            #endif
        if(s_pkmn->attributes.has_key("unova_encounter"))
            p_pkm->pkm_data.encounter = Encounters::encounters(s_pkmn->attributes["unova_encounter"]);
    }
}
