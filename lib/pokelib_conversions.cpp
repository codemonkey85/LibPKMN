/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <bitset>
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
    spec_pkmn::sptr converter::pokelib_pkmn_to_spec_pkmn(PokeLib::Pokemon pokelib_pkmn)
    {
        string identifier, move1, move2, move3, move4;
        int level, gender;

        identifier = PokeLib::species[int(pokelib_pkmn.pkm->pkm.species)];
        level = int(pokelib_pkmn.getLevel());

        move1 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[0])]);
        move2 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[1])]);
        move3 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[2])]);
        move4 = database::to_database_format(PokeLib::movelist[int(pokelib_pkmn.pkm->pkm.move[3])]);

        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, 4, level, move1, move2, move3, move4);

        PokeLib::widetext nickname_wide = pokelib_pkmn.getNickname();
        char nickname_buffer[10];
        memset(nickname_buffer,0,10);
        wcstombs(nickname_buffer, nickname_wide.c_str(), 10);
        s_pkmn->nickname = nickname_buffer;
        s_pkmn->held_item = PokeLib::items[pokelib_pkmn.pkm->pkm.held_item];
        s_pkmn->nature = pkmn_nature::make(PokeLib::nature[int(pokelib_pkmn.getNatureValue())]);
        s_pkmn->pid = pokelib_pkmn.pkm->pkm.pid;
        s_pkmn->tid = pokelib_pkmn.pkm->pkm.ot_id;
        s_pkmn->sid = pokelib_pkmn.pkm->pkm.ot_sid;
        switch(int(pokelib_pkmn.getGenderValue()))
        {
            case PokeLib::MALE:
                gender = Genders::MALE;
                break;

            case PokeLib::FEMALE:
                gender = Genders::FEMALE;
                break;

            default:
                gender = Genders::GENDERLESS;
                break;
        }

        s_pkmn->HP = pokelib_pkmn.pkm->pkm.battle_max_hp;
        s_pkmn->ATK = pokelib_pkmn.pkm->pkm.battle_atk;
        s_pkmn->DEF = pokelib_pkmn.pkm->pkm.battle_def;
        s_pkmn->SATK = pokelib_pkmn.pkm->pkm.battle_satk;
        s_pkmn->SDEF = pokelib_pkmn.pkm->pkm.battle_sdef;
        s_pkmn->SPD = pokelib_pkmn.pkm->pkm.battle_spd;

        s_pkmn->ivHP = pokelib_pkmn.pkm->pkm.iv_hp;
        s_pkmn->ivATK = pokelib_pkmn.pkm->pkm.iv_atk;
        s_pkmn->ivDEF = pokelib_pkmn.pkm->pkm.iv_def;
        s_pkmn->ivSATK = pokelib_pkmn.pkm->pkm.iv_satk;
        s_pkmn->ivSDEF = pokelib_pkmn.pkm->pkm.iv_sdef;
        s_pkmn->ivSPD = pokelib_pkmn.pkm->pkm.iv_spd;

        s_pkmn->evHP = pokelib_pkmn.pkm->pkm.ev_hp;
        s_pkmn->evATK = pokelib_pkmn.pkm->pkm.ev_atk;
        s_pkmn->evDEF = pokelib_pkmn.pkm->pkm.ev_def;
        s_pkmn->evSATK = pokelib_pkmn.pkm->pkm.ev_satk;
        s_pkmn->evSDEF = pokelib_pkmn.pkm->pkm.ev_sdef;
        s_pkmn->evSPD = pokelib_pkmn.pkm->pkm.ev_spd;

        //Attributes
        s_pkmn->attributes["friendship"] = pokelib_pkmn.pkm->pkm.friendship;

        //Need to get markings manually, no bitfield in PokeLib
        bitset<8> markings_bitset(pokelib_pkmn.pkm->pkm.markings);
        s_pkmn->attributes["circle"] = int(markings_bitset[0]);
        s_pkmn->attributes["triangle"] = int(markings_bitset[1]);
        s_pkmn->attributes["square"] = int(markings_bitset[2]);
        s_pkmn->attributes["heart"] = int(markings_bitset[3]);
        s_pkmn->attributes["star"] = int(markings_bitset[4]);
        s_pkmn->attributes["diamond"] = int(markings_bitset[5]);

        s_pkmn->attributes["country"] = pokelib_pkmn.pkm->pkm.country;
        s_pkmn->attributes["cool"] = pokelib_pkmn.pkm->pkm.contest_cool;
        s_pkmn->attributes["beauty"] = pokelib_pkmn.pkm->pkm.contest_beauty;
        s_pkmn->attributes["cute"] = pokelib_pkmn.pkm->pkm.contest_cute;
        s_pkmn->attributes["smart"] = pokelib_pkmn.pkm->pkm.contest_smart;
        s_pkmn->attributes["tough"] = pokelib_pkmn.pkm->pkm.contest_tough;
        s_pkmn->attributes["sheen"] = pokelib_pkmn.pkm->pkm.contest_sheen;
        s_pkmn->attributes["sinnoh_champ_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.sinnoh_champ;
        s_pkmn->attributes["sinnoh_ability_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.ability;
        s_pkmn->attributes["sinnoh_great_ability_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.great_ability;
        s_pkmn->attributes["sinnoh_double_ability_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.double_ability;
        s_pkmn->attributes["sinnoh_multi_ability_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.multi_ability;
        s_pkmn->attributes["sinnoh_pair_ability_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.pair_ability;
        s_pkmn->attributes["sinnoh_world_ability_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.world_ability;
        s_pkmn->attributes["sinnoh_alert_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.alert;
        s_pkmn->attributes["sinnoh_shock_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.shock;
        s_pkmn->attributes["sinnoh_downcast_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.downcast;
        s_pkmn->attributes["sinnoh_careless_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.careless;
        s_pkmn->attributes["sinnoh_relax_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.relax;
        s_pkmn->attributes["sinnoh_snooze_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.ability;
        s_pkmn->attributes["sinnoh_smile_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.smile;
        s_pkmn->attributes["sinnoh_gorgeous_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.gorgeous;
        s_pkmn->attributes["sinnoh_royal_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.royal;
        s_pkmn->attributes["sinnoh_gorgeous_royal_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.gorgeous_royal;
        s_pkmn->attributes["sinnoh_footprint_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.footprint;
        s_pkmn->attributes["sinnoh_record_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.record;
        s_pkmn->attributes["sinnoh_history_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.history;
        s_pkmn->attributes["sinnoh_legend_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.legend;
        s_pkmn->attributes["sinnoh_red_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.red;
        s_pkmn->attributes["sinnoh_green_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.green;
        s_pkmn->attributes["sinnoh_blue_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.blue;
        s_pkmn->attributes["sinnoh_festival_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.festival;
        s_pkmn->attributes["sinnoh_carnival_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.carnival;
        s_pkmn->attributes["sinnoh_classic_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.classic;
        s_pkmn->attributes["sinnoh_premier_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohA.premier;
        s_pkmn->attributes["hoenn_cool_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_normal;
        s_pkmn->attributes["hoenn_cool_ribbon_super"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_super;
        s_pkmn->attributes["hoenn_cool_ribbon_hyper"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_hyper;
        s_pkmn->attributes["hoenn_cool_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_master;
        s_pkmn->attributes["hoenn_beauty_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_normal;
        s_pkmn->attributes["hoenn_beauty_ribbon_super"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_super;
        s_pkmn->attributes["hoenn_beauty_ribbon_hyper"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_hyper;
        s_pkmn->attributes["hoenn_cute_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_master;
        s_pkmn->attributes["hoenn_cute_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_normal;
        s_pkmn->attributes["hoenn_cute_ribbon_super"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_super;
        s_pkmn->attributes["hoenn_cute_ribbon_hyper"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_hyper;
        s_pkmn->attributes["hoenn_cute_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_master;
        s_pkmn->attributes["hoenn_smart_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_normal;
        s_pkmn->attributes["hoenn_smart_ribbon_super"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_super;
        s_pkmn->attributes["hoenn_smart_ribbon_hyper"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_hyper;
        s_pkmn->attributes["hoenn_smart_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_master;
        s_pkmn->attributes["hoenn_tough_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_normal;
        s_pkmn->attributes["hoenn_tough_ribbon_super"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_super;
        s_pkmn->attributes["hoenn_tough_ribbon_hyper"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_hyper;
        s_pkmn->attributes["hoenn_tough_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_master;
        s_pkmn->attributes["hoenn_champion_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.champion;
        s_pkmn->attributes["hoenn_winning_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.winning;
        s_pkmn->attributes["hoenn_victory_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.victory;
        s_pkmn->attributes["hoenn_artist_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.artist;
        s_pkmn->attributes["hoenn_effort_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.effort;
        s_pkmn->attributes["hoenn_marine_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.marine;
        s_pkmn->attributes["hoenn_land_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.land;
        s_pkmn->attributes["hoenn_sky_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.sky;
        s_pkmn->attributes["hoenn_country_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.country;
        s_pkmn->attributes["hoenn_national_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.national;
        s_pkmn->attributes["hoenn_earth_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.earth;
        s_pkmn->attributes["hoenn_world_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonHoenn.world;
        s_pkmn->attributes["sinnoh_encounter"] = pokelib_pkmn.pkm->pkm.encounterType;
        s_pkmn->attributes["original_game"] = pokelib_pkmn.pkm->pkm.hometown;
        s_pkmn->attributes["sinnoh_cool_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_normal;
        s_pkmn->attributes["sinnoh_cool_ribbon_great"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_super;
        s_pkmn->attributes["sinnoh_cool_ribbon_ultra"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_hyper;
        s_pkmn->attributes["sinnoh_cool_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_master;
        s_pkmn->attributes["sinnoh_beauty_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_normal;
        s_pkmn->attributes["sinnoh_beauty_ribbon_great"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_super;
        s_pkmn->attributes["sinnoh_beauty_ribbon_ultra"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_hyper;
        s_pkmn->attributes["sinnoh_cute_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_master;
        s_pkmn->attributes["sinnoh_cute_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_normal;
        s_pkmn->attributes["sinnoh_cute_ribbon_great"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_super;
        s_pkmn->attributes["sinnoh_cute_ribbon_ultra"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_hyper;
        s_pkmn->attributes["sinnoh_cute_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_master;
        s_pkmn->attributes["sinnoh_smart_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_normal;
        s_pkmn->attributes["sinnoh_smart_ribbon_great"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_super;
        s_pkmn->attributes["sinnoh_smart_ribbon_ultra"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_hyper;
        s_pkmn->attributes["sinnoh_smart_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_master;
        s_pkmn->attributes["sinnoh_tough_ribbon"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_normal;
        s_pkmn->attributes["sinnoh_tough_ribbon_great"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_super;
        s_pkmn->attributes["sinnoh_tough_ribbon_ultra"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_hyper;
        s_pkmn->attributes["sinnoh_tough_ribbon_master"] = pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_master;
        s_pkmn->attributes["eggmet_year"] = pokelib_pkmn.pkm->pkm.eggDate[0];
        s_pkmn->attributes["eggmet_month"] = pokelib_pkmn.pkm->pkm.eggDate[1];
        s_pkmn->attributes["eggmet_day"] = pokelib_pkmn.pkm->pkm.eggDate[2];
        s_pkmn->attributes["met_year"] = pokelib_pkmn.pkm->pkm.metDate[0];
        s_pkmn->attributes["met_month"] = pokelib_pkmn.pkm->pkm.metDate[1];
        s_pkmn->attributes["met_day"] = pokelib_pkmn.pkm->pkm.metDate[2];
        s_pkmn->attributes["met_level"] = pokelib_pkmn.pkm->pkm.metLevel;
        s_pkmn->attributes["ot_is_female"] = pokelib_pkmn.pkm->pkm.ot_female;

        return s_pkmn;
    }

    PokeLib::Pokemon converter::spec_pkmn_to_pokelib_pkmn(spec_pkmn::sptr s_pkmn)
    {
        PokeLib::Pokemon pokelib_pkmn;

        pokelib_pkmn.pkm->pkm.species = s_pkmn->get_species_id();
        pokelib_pkmn.setLevel(uint8_t(s_pkmn->level));
        pokelib_pkmn.setNickname(s_pkmn->nickname.c_str(), s_pkmn->nickname.size());
        pokelib_pkmn.pkm->pkm.pid = s_pkmn->pid;
        pokelib_pkmn.pkm->pkm.ot_id = s_pkmn->tid;
        pokelib_pkmn.pkm->pkm.ot_sid = s_pkmn->sid;

        pokelib_pkmn.pkm->pkm.move[0] = s_pkmn->moves[0]->get_move_id();
        pokelib_pkmn.pkm->pkm.move[1] = s_pkmn->moves[1]->get_move_id();
        pokelib_pkmn.pkm->pkm.move[2] = s_pkmn->moves[2]->get_move_id();
        pokelib_pkmn.pkm->pkm.move[3] = s_pkmn->moves[3]->get_move_id();

        pokelib_pkmn.pkm->pkm.battle_max_hp = s_pkmn->HP;
        pokelib_pkmn.pkm->pkm.battle_atk = s_pkmn->ATK;
        pokelib_pkmn.pkm->pkm.battle_def = s_pkmn->DEF;
        pokelib_pkmn.pkm->pkm.battle_satk = s_pkmn->SATK;
        pokelib_pkmn.pkm->pkm.battle_sdef = s_pkmn->SDEF;
        pokelib_pkmn.pkm->pkm.battle_spd = s_pkmn->SPD;

        pokelib_pkmn.pkm->pkm.iv_hp = s_pkmn->ivHP;
        pokelib_pkmn.pkm->pkm.iv_atk = s_pkmn->ivATK;
        pokelib_pkmn.pkm->pkm.iv_def = s_pkmn->ivDEF;
        pokelib_pkmn.pkm->pkm.iv_satk = s_pkmn->ivSATK;
        pokelib_pkmn.pkm->pkm.iv_sdef = s_pkmn->ivSDEF;
        pokelib_pkmn.pkm->pkm.iv_spd = s_pkmn->ivSPD;

        pokelib_pkmn.pkm->pkm.ev_hp = s_pkmn->evHP;
        pokelib_pkmn.pkm->pkm.ev_atk = s_pkmn->evATK;
        pokelib_pkmn.pkm->pkm.ev_def = s_pkmn->evDEF;
        pokelib_pkmn.pkm->pkm.ev_satk = s_pkmn->evSATK;
        pokelib_pkmn.pkm->pkm.ev_sdef = s_pkmn->evSDEF;
        pokelib_pkmn.pkm->pkm.ev_spd = s_pkmn->evSPD;

        //Attributes
        if(s_pkmn->attributes.has_key("friendship"))
            pokelib_pkmn.pkm->pkm.friendship = s_pkmn->attributes["friendship"];

        //Need to set markings manually, no bitfield in PokeLib
        bitset<8> markings_bitset(0);
        if(s_pkmn->attributes.has_key("circle"))
            markings_bitset[0] = s_pkmn->attributes["circle"];
        if(s_pkmn->attributes.has_key("triangle"))
            markings_bitset[1] = s_pkmn->attributes["triangle"];
        if(s_pkmn->attributes.has_key("square"))
            markings_bitset[2] = s_pkmn->attributes["square"];
        if(s_pkmn->attributes.has_key("heart"))
            markings_bitset[3] = s_pkmn->attributes["heart"];
        if(s_pkmn->attributes.has_key("star"))
            markings_bitset[4] = s_pkmn->attributes["star"];
        if(s_pkmn->attributes.has_key("diamond"))
            markings_bitset[5] = s_pkmn->attributes["diamond"];
        pokelib_pkmn.pkm->pkm.markings = markings_bitset.to_ulong();

        if(s_pkmn->attributes.has_key("country"))
            pokelib_pkmn.pkm->pkm.country = char(s_pkmn->attributes["country"]);
        if(s_pkmn->attributes.has_key("cool"))
            pokelib_pkmn.pkm->pkm.contest_cool = char(s_pkmn->attributes["cool"]);
        if(s_pkmn->attributes.has_key("beauty"))
            pokelib_pkmn.pkm->pkm.contest_beauty = char(s_pkmn->attributes["beauty"]);
        if(s_pkmn->attributes.has_key("cute"))
            pokelib_pkmn.pkm->pkm.contest_cute = char(s_pkmn->attributes["cute"]);
        if(s_pkmn->attributes.has_key("smart"))
            pokelib_pkmn.pkm->pkm.contest_smart = char(s_pkmn->attributes["smart"]);
        if(s_pkmn->attributes.has_key("tough"))
            pokelib_pkmn.pkm->pkm.contest_tough = char(s_pkmn->attributes["tough"]);
        if(s_pkmn->attributes.has_key("sheen"))
            pokelib_pkmn.pkm->pkm.contest_sheen = char(s_pkmn->attributes["sheen"]);
        if(s_pkmn->attributes.has_key("sinnoh_champ_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.sinnoh_champ = s_pkmn->attributes["sinnoh_champ_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_ability_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.ability = s_pkmn->attributes["sinnoh_ability_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_great_ability_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.great_ability = s_pkmn->attributes["sinnoh_great_ability_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_double_ability_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.double_ability = s_pkmn->attributes["sinnoh_double_ability_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_multi_ability_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.multi_ability = s_pkmn->attributes["sinnoh_multi_ability_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_pair_ability_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.pair_ability = s_pkmn->attributes["sinnoh_pair_ability_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_world_ability_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.world_ability = s_pkmn->attributes["sinnoh_world_ability_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_alert_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.alert = s_pkmn->attributes["sinnoh_alert_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_shock_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.shock = s_pkmn->attributes["sinnoh_shock_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_downcast_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.downcast = s_pkmn->attributes["sinnoh_downcast_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_careless_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.careless = s_pkmn->attributes["sinnoh_careless_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_relax_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.relax = s_pkmn->attributes["sinnoh_relax_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_snooze_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.snooze = s_pkmn->attributes["sinnoh_snooze_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_smile_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.smile = s_pkmn->attributes["sinnoh_smile_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_gorgeous_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.gorgeous = s_pkmn->attributes["sinnoh_gorgeous_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_royal_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.royal = s_pkmn->attributes["sinnoh_royal_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_gorgeous_royal_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.gorgeous_royal = s_pkmn->attributes["sinnoh_gorgeous_royal_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_footprint_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.footprint = s_pkmn->attributes["sinnoh_footprint_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_record_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.record = s_pkmn->attributes["sinnoh_record_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_history_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.history = s_pkmn->attributes["sinnoh_history_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_legend_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.legend = s_pkmn->attributes["sinnoh_legend_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_red_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.red = s_pkmn->attributes["sinnoh_red_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_green_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.green = s_pkmn->attributes["sinnoh_green_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_blue_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.blue = s_pkmn->attributes["sinnoh_blue_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_festival_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.festival = s_pkmn->attributes["sinnoh_festival_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_carnival_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.carnival = s_pkmn->attributes["sinnoh_carnival_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_classic_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.classic = s_pkmn->attributes["sinnoh_classic_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_premier_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohA.premier = s_pkmn->attributes["sinnoh_premier_ribbon"];
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_normal = s_pkmn->attributes["hoenn_cool_ribbon"];
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_super = s_pkmn->attributes["hoenn_cool_ribbon_super"];
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_hyper = s_pkmn->attributes["hoenn_cool_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("hoenn_cool_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cool_master = s_pkmn->attributes["hoenn_cool_ribbon_master"];
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_normal = s_pkmn->attributes["hoenn_beauty_ribbon"];
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_super = s_pkmn->attributes["hoenn_beauty_ribbon_super"];
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_hyper = s_pkmn->attributes["hoenn_beauty_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("hoenn_beauty_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.beauty_master = s_pkmn->attributes["hoenn_beauty_ribbon_master"];
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_normal = s_pkmn->attributes["hoenn_cute_ribbon"];
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_super = s_pkmn->attributes["hoenn_cute_ribbon_super"];
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_hyper = s_pkmn->attributes["hoenn_cute_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("hoenn_cute_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.cute_master = s_pkmn->attributes["hoenn_cute_ribbon_master"];
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_normal = s_pkmn->attributes["hoenn_smart_ribbon"];
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_super = s_pkmn->attributes["hoenn_smart_ribbon_super"];
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_hyper = s_pkmn->attributes["hoenn_smart_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("hoenn_smart_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.smart_master = s_pkmn->attributes["hoenn_smart_ribbon_master"];
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_normal = s_pkmn->attributes["hoenn_tough_ribbon"];
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_super = s_pkmn->attributes["hoenn_tough_ribbon_super"];
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_hyper = s_pkmn->attributes["hoenn_tough_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("hoenn_tough_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.tough_master = s_pkmn->attributes["hoenn_tough_ribbon_master"];
        if(s_pkmn->attributes.has_key("hoenn_champion_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.champion = s_pkmn->attributes["hoenn_champion_master"];
        if(s_pkmn->attributes.has_key("hoenn_winning_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.winning = s_pkmn->attributes["hoenn_winning_master"];
        if(s_pkmn->attributes.has_key("hoenn_victory_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.victory = s_pkmn->attributes["hoenn_victory_master"];
        if(s_pkmn->attributes.has_key("hoenn_artist_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.artist = s_pkmn->attributes["hoenn_artist_master"];
        if(s_pkmn->attributes.has_key("hoenn_effort_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.effort = s_pkmn->attributes["hoenn_effort_master"];
        if(s_pkmn->attributes.has_key("hoenn_marine_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.marine = s_pkmn->attributes["hoenn_marine_master"];
        if(s_pkmn->attributes.has_key("hoenn_land_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.land = s_pkmn->attributes["hoenn_land_master"];
        if(s_pkmn->attributes.has_key("hoenn_sky_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.sky = s_pkmn->attributes["hoenn_sky_master"];
        if(s_pkmn->attributes.has_key("hoenn_country_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.country = s_pkmn->attributes["hoenn_country_master"];
        if(s_pkmn->attributes.has_key("hoenn_national_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.national = s_pkmn->attributes["hoenn_national_master"];
        if(s_pkmn->attributes.has_key("hoenn_earth_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.earth = s_pkmn->attributes["hoenn_earth_master"];
        if(s_pkmn->attributes.has_key("hoenn_world_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonHoenn.world = s_pkmn->attributes["hoenn_world_master"];
        if(s_pkmn->attributes.has_key("sinnoh_encounter"))
            pokelib_pkmn.pkm->pkm.encounterType = s_pkmn->attributes["sinnoh_encounter"];
        if(s_pkmn->attributes.has_key("original_game"))
            pokelib_pkmn.pkm->pkm.hometown = s_pkmn->attributes["original_game"];
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_normal = s_pkmn->attributes["sinnoh_cool_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_super = s_pkmn->attributes["sinnoh_cool_ribbon_super"];
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_hyper = s_pkmn->attributes["sinnoh_cool_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("sinnoh_cool_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cool_master = s_pkmn->attributes["sinnoh_cool_ribbon_master"];
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_normal = s_pkmn->attributes["sinnoh_beauty_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_super = s_pkmn->attributes["sinnoh_beauty_ribbon_super"];
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_hyper = s_pkmn->attributes["sinnoh_beauty_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("sinnoh_beauty_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.beauty_master = s_pkmn->attributes["sinnoh_beauty_ribbon_master"];
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_normal = s_pkmn->attributes["sinnoh_cute_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_super = s_pkmn->attributes["sinnoh_cute_ribbon_super"];
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_hyper = s_pkmn->attributes["sinnoh_cute_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("sinnoh_cute_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.cute_master = s_pkmn->attributes["sinnoh_cute_ribbon_master"];
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_normal = s_pkmn->attributes["sinnoh_smart_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_super = s_pkmn->attributes["sinnoh_smart_ribbon_super"];
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_hyper = s_pkmn->attributes["sinnoh_smart_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("sinnoh_smart_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.smart_master = s_pkmn->attributes["sinnoh_smart_ribbon_master"];
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_normal = s_pkmn->attributes["sinnoh_tough_ribbon"];
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon_super"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_super = s_pkmn->attributes["sinnoh_tough_ribbon_super"];
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon_hyper"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_hyper = s_pkmn->attributes["sinnoh_tough_ribbon_hyper"];
        if(s_pkmn->attributes.has_key("sinnoh_tough_ribbon_master"))
            pokelib_pkmn.pkm->pkm.ribbonSinnohB.tough_master = s_pkmn->attributes["sinnoh_tough_ribbon_master"];
        if(s_pkmn->attributes.has_key("eggmet_year"))
            pokelib_pkmn.pkm->pkm.eggDate[0] = char(s_pkmn->attributes["eggmet_year"]);
        if(s_pkmn->attributes.has_key("eggmet_month"))
            pokelib_pkmn.pkm->pkm.eggDate[1] = char(s_pkmn->attributes["eggmet_month"]);
        if(s_pkmn->attributes.has_key("eggmet_day"))
            pokelib_pkmn.pkm->pkm.eggDate[2] = char(s_pkmn->attributes["eggmet_day"]);
        if(s_pkmn->attributes.has_key("met_year"))
            pokelib_pkmn.pkm->pkm.metDate[0] = char(s_pkmn->attributes["met_year"]);
        if(s_pkmn->attributes.has_key("met_month"))
            pokelib_pkmn.pkm->pkm.metDate[1] = char(s_pkmn->attributes["met_month"]);
        if(s_pkmn->attributes.has_key("met_day"))
            pokelib_pkmn.pkm->pkm.metDate[2] = char(s_pkmn->attributes["met_day"]);
        if(s_pkmn->attributes.has_key("met_level"))
            pokelib_pkmn.pkm->pkm.metLevel = char(s_pkmn->attributes["met_level"]);
        if(s_pkmn->attributes.has_key("ot_is_female"))
            pokelib_pkmn.pkm->pkm.ot_female = char(s_pkmn->attributes["ot_is_female"]);

        return pokelib_pkmn;
    }
}
