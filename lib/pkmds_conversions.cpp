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
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/pkmn_nature.hpp>
#include <pkmnsim/spec_pkmn.hpp>
#include <pkmnsim/database/queries.hpp>

#include <pkmds/pkmds_g5_sqlite.h>

#include "conversions.hpp"
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
        s_pkmn->attributes["tameness"] = p_pkm->pkm_data.tameness;
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
        s_pkmn->attributes["ability_ribbon"] = p_pkm->pkm_data.sribbon1.ability_ribbon;
        s_pkmn->attributes["great_ability_ribbon"] = p_pkm->pkm_data.sribbon1.great_ability_ribbon;
        s_pkmn->attributes["double_ability_ribbon"] = p_pkm->pkm_data.sribbon1.double_ability_ribbon;
        s_pkmn->attributes["multi_ability_ribbon"] = p_pkm->pkm_data.sribbon1.multi_ability_ribbon;
        s_pkmn->attributes["pair_ability_ribbon"] = p_pkm->pkm_data.sribbon1.pair_ability_ribbon;
        s_pkmn->attributes["world_ability_ribbon"] = p_pkm->pkm_data.sribbon1.world_ability_ribbon;
        s_pkmn->attributes["alert_ribbon"] = p_pkm->pkm_data.sribbon1.alert_ribbon;
        s_pkmn->attributes["shock_ribbon"] = p_pkm->pkm_data.sribbon1.shock_ribbon;
        s_pkmn->attributes["downcast_ribbon"] = p_pkm->pkm_data.sribbon1.downcast_ribbon;
        s_pkmn->attributes["careless_ribbon"] = p_pkm->pkm_data.sribbon1.careless_ribbon;
        s_pkmn->attributes["relax_ribbon"] = p_pkm->pkm_data.sribbon1.relax_ribbon;
        s_pkmn->attributes["snooze_ribbon"] = p_pkm->pkm_data.sribbon1.snooze_ribbon;
        s_pkmn->attributes["smile_ribbon"] = p_pkm->pkm_data.sribbon1.smile_ribbon;
        s_pkmn->attributes["gorgeous_ribbon"] = p_pkm->pkm_data.sribbon1.gorgeous_ribbon;
        s_pkmn->attributes["royal_ribbon"] = p_pkm->pkm_data.sribbon1.royal_ribbon;
        s_pkmn->attributes["gorgeous_royal_ribbon"] = p_pkm->pkm_data.sribbon2.gorgeous_royal_ribbon;
        s_pkmn->attributes["footprint_ribbon"] = p_pkm->pkm_data.sribbon2.footprint_ribbon;
        s_pkmn->attributes["record_ribbon"] = p_pkm->pkm_data.sribbon2.record_ribbon;
        s_pkmn->attributes["history_ribbon"] = p_pkm->pkm_data.sribbon2.history_ribbon;
        s_pkmn->attributes["legend_ribbon"] = p_pkm->pkm_data.sribbon2.legend_ribbon;
        s_pkmn->attributes["red_ribbon"] = p_pkm->pkm_data.sribbon2.red_ribbon;
        s_pkmn->attributes["green_ribbon"] = p_pkm->pkm_data.sribbon2.green_ribbon;
        s_pkmn->attributes["blue_ribbon"] = p_pkm->pkm_data.sribbon2.blue_ribbon;
        s_pkmn->attributes["festival_ribbon"] = p_pkm->pkm_data.sribbon2.festival_ribbon;
        s_pkmn->attributes["carnival_ribbon"] = p_pkm->pkm_data.sribbon2.carnival_ribbon;
        s_pkmn->attributes["classic_ribbon"] = p_pkm->pkm_data.sribbon2.classic_ribbon;
        s_pkmn->attributes["premier_ribbon"] = p_pkm->pkm_data.sribbon2.premier_ribbon;
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
        if(s_pkmn->attributes.has_key("tameness"))
            p_pkm->pkm_data.tameness = byte(s_pkmn->attributes["tameness"]);
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
            p_pkm->pkm_data.sribbon1.ability_ribbon = bool(s_pkmn->attributes["ability_ribbon"]);
        if(s_pkmn->attributes.has_key("great_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.great_ability_ribbon = bool(s_pkmn->attributes["great_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("double_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.double_ability_ribbon = bool(s_pkmn->attributes["double_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("multi_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.multi_ability_ribbon = bool(s_pkmn->attributes["multi_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("pair_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.pair_ability_ribbon = bool(s_pkmn->attributes["pair_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("world_ability_ribbon"))
            p_pkm->pkm_data.sribbon1.world_ability_ribbon = bool(s_pkmn->attributes["world_ability_ribbon"]);
        if(s_pkmn->attributes.has_key("alert_ribbon"))
            p_pkm->pkm_data.sribbon1.alert_ribbon = bool(s_pkmn->attributes["alert_ribbon"]);
        if(s_pkmn->attributes.has_key("shock_ribbon"))
            p_pkm->pkm_data.sribbon1.shock_ribbon = bool(s_pkmn->attributes["shock_ribbon"]);
        if(s_pkmn->attributes.has_key("downcast_ribbon"))
            p_pkm->pkm_data.sribbon1.downcast_ribbon = bool(s_pkmn->attributes["downcast_ribbon"]);
        if(s_pkmn->attributes.has_key("careless_ribbon"))
            p_pkm->pkm_data.sribbon1.careless_ribbon = bool(s_pkmn->attributes["careless_ribbon"]);
        if(s_pkmn->attributes.has_key("relax_ribbon"))
            p_pkm->pkm_data.sribbon1.relax_ribbon = bool(s_pkmn->attributes["relax_ribbon"]);
        if(s_pkmn->attributes.has_key("snooze_ribbon"))
            p_pkm->pkm_data.sribbon1.snooze_ribbon = bool(s_pkmn->attributes["snooze_ribbon"]);
        if(s_pkmn->attributes.has_key("smile_ribbon"))
            p_pkm->pkm_data.sribbon1.smile_ribbon = bool(s_pkmn->attributes["smile_ribbon"]);
        if(s_pkmn->attributes.has_key("gorgeous_ribbon"))
            p_pkm->pkm_data.sribbon1.gorgeous_ribbon = bool(s_pkmn->attributes["gorgeous_ribbon"]);
        if(s_pkmn->attributes.has_key("royal_ribbon"))
            p_pkm->pkm_data.sribbon1.royal_ribbon = bool(s_pkmn->attributes["royal_ribbon"]);
        if(s_pkmn->attributes.has_key("gorgeous_royal_ribbon"))
            p_pkm->pkm_data.sribbon2.gorgeous_royal_ribbon = bool(s_pkmn->attributes["gorgeous_royal_ribbon"]);
        if(s_pkmn->attributes.has_key("footprint_ribbon"))
            p_pkm->pkm_data.sribbon2.footprint_ribbon = bool(s_pkmn->attributes["footprint_ribbon"]);
        if(s_pkmn->attributes.has_key("record_ribbon"))
            p_pkm->pkm_data.sribbon2.record_ribbon = bool(s_pkmn->attributes["record_ribbon"]);
        if(s_pkmn->attributes.has_key("history_ribbon"))
            p_pkm->pkm_data.sribbon2.history_ribbon = bool(s_pkmn->attributes["history_ribbon"]);
        if(s_pkmn->attributes.has_key("legend_ribbon"))
            p_pkm->pkm_data.sribbon2.legend_ribbon = bool(s_pkmn->attributes["legend_ribbon"]);
        if(s_pkmn->attributes.has_key("red_ribbon"))
            p_pkm->pkm_data.sribbon2.red_ribbon = bool(s_pkmn->attributes["red_ribbon"]);
        if(s_pkmn->attributes.has_key("green_ribbon"))
            p_pkm->pkm_data.sribbon2.green_ribbon = bool(s_pkmn->attributes["green_ribbon"]);
        if(s_pkmn->attributes.has_key("blue_ribbon"))
            p_pkm->pkm_data.sribbon2.blue_ribbon = bool(s_pkmn->attributes["blue_ribbon"]);
        if(s_pkmn->attributes.has_key("festival_ribbon"))
            p_pkm->pkm_data.sribbon2.festival_ribbon = bool(s_pkmn->attributes["festival_ribbon"]);
        if(s_pkmn->attributes.has_key("carnival_ribbon"))
            p_pkm->pkm_data.sribbon2.carnival_ribbon = bool(s_pkmn->attributes["carnival_ribbon"]);
        if(s_pkmn->attributes.has_key("classic_ribbon"))
            p_pkm->pkm_data.sribbon2.classic_ribbon = bool(s_pkmn->attributes["classic_ribbon"]);
        if(s_pkmn->attributes.has_key("premier_ribbon"))
            p_pkm->pkm_data.sribbon2.premier_ribbon = bool(s_pkmn->attributes["premier_ribbon"]);
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
    }
    
    void converter::pkmds_pkmn_to_pokehack_pkmn(party_pkm* p_pkm,
                                                belt_pokemon_t* b_pkmn_t,
                                                pokemon_attacks_t* pkmn_a_t,
                                                pokemon_effort_t* pkmn_e_t,
                                                pokemon_misc_t* pkmn_m_t,
                                                pokemon_growth_t* pkmn_g_t)
    {
        pkmn_g_t->species = p_pkm->pkm_data.species;
        b_pkmn_t->level = p_pkm->party_data.level;
        wstring nickname_wide = getpkmnickname(p_pkm->pkm_data);
        char nickname_buffer[11];
        memset(nickname_buffer,0,11);
        wcstombs(nickname_buffer, nickname_wide.c_str(), 11);
        for(int i = 0; i < 10; i++)
        {
            if(nickname_buffer != 0) b_pkmn_t->name[i] = pokehack_reverse_char_map[nickname_buffer[i]];
            else b_pkmn_t->name[i] = 0xFF;
        }
        b_pkmn_t->personality = p_pkm->pkm_data.pid;
        unsigned short otid[2] = {p_pkm->pkm_data.tid, p_pkm->pkm_data.sid};
        b_pkmn_t->otid = *((unsigned int*)(&otid[0]));

        //Moves
        pkmn_a_t->atk1 = p_pkm->pkm_data.moves[0];
        pkmn_a_t->atk2 = p_pkm->pkm_data.moves[1];
        pkmn_a_t->atk3 = p_pkm->pkm_data.moves[2];
        pkmn_a_t->atk4 = p_pkm->pkm_data.moves[3];
        pkmn_a_t->pp1 = p_pkm->pkm_data.pp[0];
        pkmn_a_t->pp2 = p_pkm->pkm_data.pp[1];
        pkmn_a_t->pp3 = p_pkm->pkm_data.pp[2];
        pkmn_a_t->pp4 = p_pkm->pkm_data.pp[3];

        //Stats
        b_pkmn_t->currentHP = p_pkm->party_data.hp;
        b_pkmn_t->maxHP = p_pkm->party_data.maxhp;
        b_pkmn_t->attack = p_pkm->party_data.attack;
        b_pkmn_t->defense = p_pkm->party_data.defense;
        b_pkmn_t->spatk = p_pkm->party_data.spatk;
        b_pkmn_t->spdef = p_pkm->party_data.spdef;
        b_pkmn_t->speed = p_pkm->party_data.speed;
        
        //IV's
        pkmn_m_t->IVs.hp = p_pkm->pkm_data.ivs.hp;
        pkmn_m_t->IVs.atk = p_pkm->pkm_data.ivs.attack;
        pkmn_m_t->IVs.def = p_pkm->pkm_data.ivs.defense;
        pkmn_m_t->IVs.spatk = p_pkm->pkm_data.ivs.spatk;
        pkmn_m_t->IVs.spdef = p_pkm->pkm_data.ivs.spdef;
        pkmn_m_t->IVs.spd = p_pkm->pkm_data.ivs.speed;
        
        //EV's
        pkmn_e_t->hp = p_pkm->pkm_data.evs.hp;
        pkmn_e_t->attack = p_pkm->pkm_data.evs.attack;
        pkmn_e_t->defense = p_pkm->pkm_data.evs.defense;
        pkmn_e_t->spatk = p_pkm->pkm_data.evs.spatk;
        pkmn_e_t->spdef = p_pkm->pkm_data.evs.spdef;
        pkmn_e_t->speed = p_pkm->pkm_data.evs.speed;
    }
    
    PokeLib::Pokemon converter::pkmds_pkmn_to_pokelib_pkmn(party_pkm* p_pkm)
    {
        PokeLib::Pokemon pokelib_pkmn;
    
        pokelib_pkmn.pkm->pkm.species = p_pkm->pkm_data.species;
        pokelib_pkmn.setLevel(p_pkm->party_data.level);
        pokelib_pkmn.setNickname(getpkmnickname(p_pkm->pkm_data));
        pokelib_pkmn.pkm->pkm.pid = p_pkm->pkm_data.pid;
        pokelib_pkmn.pkm->pkm.pid;
        pokelib_pkmn.pkm->pkm.ot_id;
        pokelib_pkmn.pkm->pkm.ot_sid;
        
        //Moves
        pokelib_pkmn.pkm->pkm.move[0] = p_pkm->pkm_data.moves[0];
        pokelib_pkmn.pkm->pkm.move[1] = p_pkm->pkm_data.moves[1];
        pokelib_pkmn.pkm->pkm.move[2] = p_pkm->pkm_data.moves[2];
        pokelib_pkmn.pkm->pkm.move[3] = p_pkm->pkm_data.moves[3];
        pokelib_pkmn.pkm->pkm.movePP[0] = p_pkm->pkm_data.pp[0];
        pokelib_pkmn.pkm->pkm.movePP[1] = p_pkm->pkm_data.pp[1];
        pokelib_pkmn.pkm->pkm.movePP[2] = p_pkm->pkm_data.pp[2];
        pokelib_pkmn.pkm->pkm.movePP[3] = p_pkm->pkm_data.pp[3];
        
        //Stats
        pokelib_pkmn.pkm->pkm.battle_hp = p_pkm->party_data.hp;
        pokelib_pkmn.pkm->pkm.battle_max_hp = p_pkm->party_data.maxhp;
        pokelib_pkmn.pkm->pkm.battle_atk = p_pkm->party_data.attack;
        pokelib_pkmn.pkm->pkm.battle_def = p_pkm->party_data.defense;
        pokelib_pkmn.pkm->pkm.battle_satk = p_pkm->party_data.spatk;
        pokelib_pkmn.pkm->pkm.battle_sdef = p_pkm->party_data.spdef;
        pokelib_pkmn.pkm->pkm.battle_spd = p_pkm->party_data.speed;
        
        //IV's
        pokelib_pkmn.pkm->pkm.iv_hp = p_pkm->pkm_data.ivs.hp;
        pokelib_pkmn.pkm->pkm.iv_atk = p_pkm->pkm_data.ivs.attack;
        pokelib_pkmn.pkm->pkm.iv_def = p_pkm->pkm_data.ivs.defense;
        pokelib_pkmn.pkm->pkm.iv_satk = p_pkm->pkm_data.ivs.spatk;
        pokelib_pkmn.pkm->pkm.iv_sdef = p_pkm->pkm_data.ivs.spdef;
        pokelib_pkmn.pkm->pkm.iv_spd = p_pkm->pkm_data.ivs.speed;

        //EV's
        pokelib_pkmn.pkm->pkm.ev_hp = p_pkm->pkm_data.evs.hp;
        pokelib_pkmn.pkm->pkm.ev_atk = p_pkm->pkm_data.evs.attack;
        pokelib_pkmn.pkm->pkm.ev_def = p_pkm->pkm_data.evs.defense;
        pokelib_pkmn.pkm->pkm.ev_satk = p_pkm->pkm_data.evs.spatk;
        pokelib_pkmn.pkm->pkm.ev_sdef = p_pkm->pkm_data.evs.spdef;
        pokelib_pkmn.pkm->pkm.ev_spd = p_pkm->pkm_data.evs.speed;

        return pokelib_pkmn;
    }
}
