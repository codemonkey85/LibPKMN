/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string.h>

#include <pkmnsim/pokelib/Pokemon.hpp>

using namespace std;

namespace pkmnsim
{
    namespace PokeLib
    {
        Pokemon::Pokemon() 
        {
            nativePokemon = new ::PokeLib::Pokemon();
            importValues();
        }

        Pokemon::Pokemon(::PokeLib::Pokemon* pokemon)
        {
            nativePokemon = pokemon;
            importValues();
        }

        Pokemon::sptr Pokemon::make() {return sptr(new Pokemon);}
        Pokemon::sptr Pokemon::make(::PokeLib::Pokemon* pokemon) {return sptr(new Pokemon(pokemon));}

        unsigned int Pokemon::getAttribute(string attribute)
        {
            if(pRibbonSinnohA.has_key(attribute)) return pRibbonSinnohA[attribute];
            else if(pRibbonSinnohB.has_key(attribute)) return pRibbonSinnohB[attribute];
            else if(pRibbonHoenn.has_key(attribute)) return pRibbonHoenn[attribute];
            else if(pPKM.has_key(attribute)) return pPKM[attribute];
            else
            {
                cerr << "Invalid attribute." << endl;
                exit(EXIT_FAILURE);
            }
        }

        void Pokemon::setAttribute(string attribute, unsigned int value)
        {
            if(pRibbonSinnohA.has_key(attribute)) pRibbonSinnohA[attribute] = value;
            else if(pRibbonSinnohB.has_key(attribute)) pRibbonSinnohB[attribute] = value;
            else if(pRibbonHoenn.has_key(attribute)) pRibbonHoenn[attribute] = value;
            else if(pPKM.has_key(attribute)) pPKM[attribute] = value;
            else
            {
                cerr << "Invalid attribute." << endl;
                exit(EXIT_FAILURE);
            }
            exportValues();
        }

        string Pokemon::getNickname() const
        {
            ::PokeLib::widetext nameWide = nativePokemon->getNickname();
            char tmp[10];
            memset(tmp,0,10);
            wcstombs(tmp, nameWide.c_str(), 10);
            return string(tmp);
        }

        string Pokemon::getTrainerName() const
        {
            ::PokeLib::widetext nameWide = nativePokemon->getTrainerName();
            char tmp[7];
            memset(tmp,0,7);
            wcstombs(tmp, nameWide.c_str(), 7);
            return string(tmp);
        }

        void Pokemon::updateBattleStats() {nativePokemon->updateBattleStats();}
        void Pokemon::setNickname(string name) {nativePokemon->setNickname(name.c_str(), name.size());}
        void Pokemon::setTrainerName(string name) {nativePokemon->setTrainerName(name.c_str(), name.size());}
        unsigned char Pokemon::getLevel() const {return nativePokemon->getLevel();}
        unsigned int Pokemon::getExpToNextLevel() const {return nativePokemon->getExpToNextLevel();}
        void Pokemon::setLevel(unsigned char level) {nativePokemon->setLevel(level);}
        unsigned char Pokemon::getNatureValue() const {return nativePokemon->getNatureValue();}
        unsigned char Pokemon::getGenderValue() const {return nativePokemon->getGenderValue();}
        unsigned int Pokemon::getPersonalityGender() const {return nativePokemon->getPersonalityGender();}
        unsigned char Pokemon::getHiddenPowerType() const {return nativePokemon->getHiddenPowerType();}
        unsigned char Pokemon::getHiddenPowerAttack() const {return nativePokemon->getHiddenPowerAttack();}
        bool Pokemon::isShiny() const {return nativePokemon->isShiny();}
        void Pokemon::setUnownForm(char f) {nativePokemon->setUnownForm(f);}

        void Pokemon::exportValues()
        {
            nativePokemon->pkm->pkm.ribbonSinnohA.sinnoh_champ = pRibbonSinnohA["sinnoh_champ"];
            nativePokemon->pkm->pkm.ribbonSinnohA.ability = pRibbonSinnohA["ability"];
            nativePokemon->pkm->pkm.ribbonSinnohA.great_ability = pRibbonSinnohA["great_ability"];
            nativePokemon->pkm->pkm.ribbonSinnohA.double_ability = pRibbonSinnohA["double_ability"];
            nativePokemon->pkm->pkm.ribbonSinnohA.multi_ability = pRibbonSinnohA["multi_ability"];
            nativePokemon->pkm->pkm.ribbonSinnohA.pair_ability = pRibbonSinnohA["pair_ability"];
            nativePokemon->pkm->pkm.ribbonSinnohA.world_ability = pRibbonSinnohA["world_ability"];
            nativePokemon->pkm->pkm.ribbonSinnohA.alert = pRibbonSinnohA["alert"];
            nativePokemon->pkm->pkm.ribbonSinnohA.shock = pRibbonSinnohA["shock"];
            nativePokemon->pkm->pkm.ribbonSinnohA.downcast = pRibbonSinnohA["downcast"];
            nativePokemon->pkm->pkm.ribbonSinnohA.careless = pRibbonSinnohA["careless"];
            nativePokemon->pkm->pkm.ribbonSinnohA.relax = pRibbonSinnohA["relax"];
            nativePokemon->pkm->pkm.ribbonSinnohA.snooze = pRibbonSinnohA["snooze"];
            nativePokemon->pkm->pkm.ribbonSinnohA.smile = pRibbonSinnohA["smile"];
            nativePokemon->pkm->pkm.ribbonSinnohA.gorgeous = pRibbonSinnohA["gorgeous"];
            nativePokemon->pkm->pkm.ribbonSinnohA.royal = pRibbonSinnohA["royal"];
            nativePokemon->pkm->pkm.ribbonSinnohA.gorgeous_royal = pRibbonSinnohA["gorgeous_royal"];
            nativePokemon->pkm->pkm.ribbonSinnohA.footprint = pRibbonSinnohA["footprint"];
            nativePokemon->pkm->pkm.ribbonSinnohA.record = pRibbonSinnohA["record"];
            nativePokemon->pkm->pkm.ribbonSinnohA.history = pRibbonSinnohA["history"];
            nativePokemon->pkm->pkm.ribbonSinnohA.legend = pRibbonSinnohA["legend"];
            nativePokemon->pkm->pkm.ribbonSinnohA.red = pRibbonSinnohA["red"];
            nativePokemon->pkm->pkm.ribbonSinnohA.green = pRibbonSinnohA["green"];
            nativePokemon->pkm->pkm.ribbonSinnohA.blue = pRibbonSinnohA["blue"];
            nativePokemon->pkm->pkm.ribbonSinnohA.festival = pRibbonSinnohA["festival"];
            nativePokemon->pkm->pkm.ribbonSinnohA.carnival = pRibbonSinnohA["carnival"];
            nativePokemon->pkm->pkm.ribbonSinnohA.classic = pRibbonSinnohA["classic"];
            nativePokemon->pkm->pkm.ribbonSinnohA.premier = pRibbonSinnohA["premier"];

            nativePokemon->pkm->pkm.ribbonSinnohB.cool_normal = pRibbonSinnohB["cool_normal"];
            nativePokemon->pkm->pkm.ribbonSinnohB.cool_super = pRibbonSinnohB["cool_super"];
            nativePokemon->pkm->pkm.ribbonSinnohB.cool_hyper = pRibbonSinnohB["cool_hyper"];
            nativePokemon->pkm->pkm.ribbonSinnohB.cool_master = pRibbonSinnohB["cool_master"];
            nativePokemon->pkm->pkm.ribbonSinnohB.beauty_normal = pRibbonSinnohB["beauty_normal"];
            nativePokemon->pkm->pkm.ribbonSinnohB.beauty_super = pRibbonSinnohB["beauty_super"];
            nativePokemon->pkm->pkm.ribbonSinnohB.beauty_hyper = pRibbonSinnohB["beauty_hyper"];
            nativePokemon->pkm->pkm.ribbonSinnohB.beauty_master = pRibbonSinnohB["beauty_master"];
            nativePokemon->pkm->pkm.ribbonSinnohB.cute_normal = pRibbonSinnohB["cute_normal"];
            nativePokemon->pkm->pkm.ribbonSinnohB.cute_super = pRibbonSinnohB["cute_super"];
            nativePokemon->pkm->pkm.ribbonSinnohB.cute_hyper = pRibbonSinnohB["cute_hyper"];
            nativePokemon->pkm->pkm.ribbonSinnohB.cute_master = pRibbonSinnohB["cute_master"];
            nativePokemon->pkm->pkm.ribbonSinnohB.smart_normal = pRibbonSinnohB["smart_normal"];
            nativePokemon->pkm->pkm.ribbonSinnohB.smart_super = pRibbonSinnohB["smart_super"];
            nativePokemon->pkm->pkm.ribbonSinnohB.smart_hyper = pRibbonSinnohB["smart_hyper"];
            nativePokemon->pkm->pkm.ribbonSinnohB.smart_master = pRibbonSinnohB["smart_master"];
            nativePokemon->pkm->pkm.ribbonSinnohB.tough_normal = pRibbonSinnohB["tough_normal"];
            nativePokemon->pkm->pkm.ribbonSinnohB.tough_super = pRibbonSinnohB["tough_super"];
            nativePokemon->pkm->pkm.ribbonSinnohB.tough_hyper = pRibbonSinnohB["tough_hyper"];
            nativePokemon->pkm->pkm.ribbonSinnohB.tough_master = pRibbonSinnohB["tough_master"];

            nativePokemon->pkm->pkm.ribbonHoenn.cool_normal = pRibbonHoenn["cool_normal"];
            nativePokemon->pkm->pkm.ribbonHoenn.cool_super = pRibbonHoenn["cool_super"];
            nativePokemon->pkm->pkm.ribbonHoenn.cool_hyper = pRibbonHoenn["cool_hyper"];
            nativePokemon->pkm->pkm.ribbonHoenn.cool_master = pRibbonHoenn["cool_master"];
            nativePokemon->pkm->pkm.ribbonHoenn.beauty_normal = pRibbonHoenn["beauty_normal"];
            nativePokemon->pkm->pkm.ribbonHoenn.beauty_super = pRibbonHoenn["beauty_super"];
            nativePokemon->pkm->pkm.ribbonHoenn.beauty_hyper = pRibbonHoenn["beauty_hyper"];
            nativePokemon->pkm->pkm.ribbonHoenn.beauty_master = pRibbonHoenn["beauty_master"];
            nativePokemon->pkm->pkm.ribbonHoenn.cute_normal = pRibbonHoenn["cute_normal"];
            nativePokemon->pkm->pkm.ribbonHoenn.cute_super = pRibbonHoenn["cute_super"];
            nativePokemon->pkm->pkm.ribbonHoenn.cute_hyper = pRibbonHoenn["cute_hyper"];
            nativePokemon->pkm->pkm.ribbonHoenn.cute_master = pRibbonHoenn["cute_master"];
            nativePokemon->pkm->pkm.ribbonHoenn.smart_normal = pRibbonHoenn["smart_normal"];
            nativePokemon->pkm->pkm.ribbonHoenn.smart_super = pRibbonHoenn["smart_super"];
            nativePokemon->pkm->pkm.ribbonHoenn.smart_hyper = pRibbonHoenn["smart_hyper"];
            nativePokemon->pkm->pkm.ribbonHoenn.smart_master = pRibbonHoenn["smart_master"];
            nativePokemon->pkm->pkm.ribbonHoenn.champion = pRibbonHoenn["champion"];
            nativePokemon->pkm->pkm.ribbonHoenn.winning = pRibbonHoenn["winning"];
            nativePokemon->pkm->pkm.ribbonHoenn.victory = pRibbonHoenn["victory"];
            nativePokemon->pkm->pkm.ribbonHoenn.artist = pRibbonHoenn["artist"];
            nativePokemon->pkm->pkm.ribbonHoenn.effort = pRibbonHoenn["effort"];
            nativePokemon->pkm->pkm.ribbonHoenn.marine = pRibbonHoenn["marine"];
            nativePokemon->pkm->pkm.ribbonHoenn.land = pRibbonHoenn["land"];
            nativePokemon->pkm->pkm.ribbonHoenn.sky = pRibbonHoenn["sky"];
            nativePokemon->pkm->pkm.ribbonHoenn.country = pRibbonHoenn["country"];
            nativePokemon->pkm->pkm.ribbonHoenn.national = pRibbonHoenn["national"];
            nativePokemon->pkm->pkm.ribbonHoenn.earth = pRibbonHoenn["earth"];
            nativePokemon->pkm->pkm.ribbonHoenn.world = pRibbonHoenn["world"];

            nativePokemon->pkm->pkm.pid = pPKM["pid"];
            nativePokemon->pkm->pkm.species = pPKM["species"];
            nativePokemon->pkm->pkm.held_item = pPKM["held_item"];
            nativePokemon->pkm->pkm.ot_id = pPKM["ot_id"];
            nativePokemon->pkm->pkm.ot_sid = pPKM["ot_sid"];
            nativePokemon->pkm->pkm.exp = pPKM["exp"];
            nativePokemon->pkm->pkm.friendship = pPKM["friendship"];
            nativePokemon->pkm->pkm.ability = pPKM["ability"];
            nativePokemon->pkm->pkm.markings = pPKM["markings"]; //TODO: separate markings char into individual markings
            nativePokemon->pkm->pkm.country = pPKM["country"];
            nativePokemon->pkm->pkm.ev_hp = pPKM["ev_hp"];
            nativePokemon->pkm->pkm.ev_atk = pPKM["ev_atk"];
            nativePokemon->pkm->pkm.ev_def = pPKM["ev_def"];
            nativePokemon->pkm->pkm.ev_spd = pPKM["ev_spd"];
            nativePokemon->pkm->pkm.ev_satk = pPKM["ev_satk"];
            nativePokemon->pkm->pkm.ev_sdef = pPKM["ev_sdef"];
            nativePokemon->pkm->pkm.contest_cool = pPKM["contest_cool"];
            nativePokemon->pkm->pkm.contest_beauty = pPKM["contest_beauty"];
            nativePokemon->pkm->pkm.contest_cute = pPKM["contest_cute"];
            nativePokemon->pkm->pkm.contest_smart = pPKM["contest_smart"];
            nativePokemon->pkm->pkm.contest_tough = pPKM["contest_tough"];
            nativePokemon->pkm->pkm.contest_sheen = pPKM["contest_sheen"];
            nativePokemon->pkm->pkm.move[0] = pPKM["move1"];
            nativePokemon->pkm->pkm.move[1] = pPKM["move2"];
            nativePokemon->pkm->pkm.move[2] = pPKM["move3"];
            nativePokemon->pkm->pkm.move[3] = pPKM["move4"];
            nativePokemon->pkm->pkm.movePP[0] = pPKM["movePP1"];
            nativePokemon->pkm->pkm.movePP[1] = pPKM["movePP2"];
            nativePokemon->pkm->pkm.movePP[2] = pPKM["movePP3"];
            nativePokemon->pkm->pkm.movePP[3] = pPKM["movePP4"];
            nativePokemon->pkm->pkm.iv_hp = pPKM["iv_hp"];
            nativePokemon->pkm->pkm.iv_atk = pPKM["iv_atk"];
            nativePokemon->pkm->pkm.iv_def = pPKM["iv_def"];
            nativePokemon->pkm->pkm.iv_spd = pPKM["iv_spd"];
            nativePokemon->pkm->pkm.iv_satk = pPKM["iv_satk"];
            nativePokemon->pkm->pkm.iv_sdef = pPKM["iv_sdef"];
            nativePokemon->pkm->pkm.isEgg = pPKM["isEgg"];
            nativePokemon->pkm->pkm.isNicknamed = pPKM["isNicknamed"];
            nativePokemon->pkm->pkm.isFatefulEncounter = pPKM["isFatefulEncounter"];
            nativePokemon->pkm->pkm.isFemale = pPKM["isFemale"];
            nativePokemon->pkm->pkm.isGenderless = pPKM["isGenderless"];
            nativePokemon->pkm->pkm.form = pPKM["form"];
            nativePokemon->pkm->pkm.form = pPKM["shining_leaf"]; //TODO: separate shining leaf into individual bits
            nativePokemon->pkm->pkm.eggLoc_Plat = pPKM["eggLoc_Plat"];
            nativePokemon->pkm->pkm.metLoc_Plat = pPKM["metLoc_Plat"];
            nativePokemon->pkm->pkm.hometown = pPKM["hometown"];
            nativePokemon->pkm->pkm.eggDate[0] = pPKM["eggYear"];
            nativePokemon->pkm->pkm.eggDate[1] = pPKM["eggMonth"];
            nativePokemon->pkm->pkm.eggDate[2] = pPKM["eggDay"];
            nativePokemon->pkm->pkm.metDate[0] = pPKM["metYear"];
            nativePokemon->pkm->pkm.metDate[1] = pPKM["metMonth"];
            nativePokemon->pkm->pkm.metDate[2] = pPKM["metDay"];
            nativePokemon->pkm->pkm.pokerus = pPKM["pokerus"];
            nativePokemon->pkm->pkm.pokeball = pPKM["pokeball"];
            nativePokemon->pkm->pkm.ot_female = pPKM["ot_female"];
            nativePokemon->pkm->pkm.encounterType = pPKM["encounterType"];
            nativePokemon->pkm->pkm.pokeball_HGSS = pPKM["pokeball_HGSS"];
            nativePokemon->pkm->pkm.eff_sleep = pPKM["eff_sleep"];
            nativePokemon->pkm->pkm.eff_poison = pPKM["eff_poison"];
            nativePokemon->pkm->pkm.eff_burn = pPKM["eff_burn"];
            nativePokemon->pkm->pkm.eff_frozen = pPKM["eff_frozen"];
            nativePokemon->pkm->pkm.eff_paralyze = pPKM["eff_paralyze"];
            nativePokemon->pkm->pkm.eff_toxic = pPKM["eff_toxic"];
            nativePokemon->pkm->pkm.battle_level = pPKM["battle_level"];
            nativePokemon->pkm->pkm.battle_capsule = pPKM["battle_capsule"];
            nativePokemon->pkm->pkm.battle_hp = pPKM["battle_hp"];
            nativePokemon->pkm->pkm.battle_max_hp = pPKM["battle_max_hp"];
            nativePokemon->pkm->pkm.battle_atk = pPKM["battle_atk"];
            nativePokemon->pkm->pkm.battle_def = pPKM["battle_def"];
            nativePokemon->pkm->pkm.battle_spd = pPKM["battle_spd"];
            nativePokemon->pkm->pkm.battle_satk = pPKM["battle_satk"];
            nativePokemon->pkm->pkm.battle_sdef = pPKM["battle_sdef"];
        }

        void Pokemon::importValues()
        {
            pRibbonSinnohA["sinnoh_champ"] = nativePokemon->pkm->pkm.ribbonSinnohA.sinnoh_champ;
            pRibbonSinnohA["ability"] = nativePokemon->pkm->pkm.ribbonSinnohA.ability;
            pRibbonSinnohA["great_ability"] = nativePokemon->pkm->pkm.ribbonSinnohA.great_ability;
            pRibbonSinnohA["double_ability"] = nativePokemon->pkm->pkm.ribbonSinnohA.double_ability;
            pRibbonSinnohA["multi_ability"] = nativePokemon->pkm->pkm.ribbonSinnohA.multi_ability;
            pRibbonSinnohA["pair_ability"] = nativePokemon->pkm->pkm.ribbonSinnohA.pair_ability;
            pRibbonSinnohA["world_ability"] = nativePokemon->pkm->pkm.ribbonSinnohA.world_ability;
            pRibbonSinnohA["alert"] = nativePokemon->pkm->pkm.ribbonSinnohA.alert;
            pRibbonSinnohA["shock"] = nativePokemon->pkm->pkm.ribbonSinnohA.shock;
            pRibbonSinnohA["downcast"] = nativePokemon->pkm->pkm.ribbonSinnohA.downcast;
            pRibbonSinnohA["careless"] = nativePokemon->pkm->pkm.ribbonSinnohA.careless;
            pRibbonSinnohA["relax"] = nativePokemon->pkm->pkm.ribbonSinnohA.relax;
            pRibbonSinnohA["snooze"] = nativePokemon->pkm->pkm.ribbonSinnohA.snooze;
            pRibbonSinnohA["smile"] = nativePokemon->pkm->pkm.ribbonSinnohA.smile;
            pRibbonSinnohA["gorgeous"] = nativePokemon->pkm->pkm.ribbonSinnohA.gorgeous;
            pRibbonSinnohA["royal"] = nativePokemon->pkm->pkm.ribbonSinnohA.royal;
            pRibbonSinnohA["gorgeous_royal"] = nativePokemon->pkm->pkm.ribbonSinnohA.gorgeous_royal;
            pRibbonSinnohA["footprint"] = nativePokemon->pkm->pkm.ribbonSinnohA.footprint;
            pRibbonSinnohA["record"] = nativePokemon->pkm->pkm.ribbonSinnohA.record;
            pRibbonSinnohA["history"] = nativePokemon->pkm->pkm.ribbonSinnohA.history;
            pRibbonSinnohA["legend"] = nativePokemon->pkm->pkm.ribbonSinnohA.legend;
            pRibbonSinnohA["red"] = nativePokemon->pkm->pkm.ribbonSinnohA.red;
            pRibbonSinnohA["green"] = nativePokemon->pkm->pkm.ribbonSinnohA.green;
            pRibbonSinnohA["blue"] = nativePokemon->pkm->pkm.ribbonSinnohA.blue;
            pRibbonSinnohA["festival"] = nativePokemon->pkm->pkm.ribbonSinnohA.festival;
            pRibbonSinnohA["carnival"] = nativePokemon->pkm->pkm.ribbonSinnohA.carnival;
            pRibbonSinnohA["classic"] = nativePokemon->pkm->pkm.ribbonSinnohA.classic;
            pRibbonSinnohA["premier"] = nativePokemon->pkm->pkm.ribbonSinnohA.premier;
            pRibbonSinnohB["cool_normal"] = nativePokemon->pkm->pkm.ribbonSinnohB.cool_normal;
            pRibbonSinnohB["cool_super"] = nativePokemon->pkm->pkm.ribbonSinnohB.cool_super;
            pRibbonSinnohB["cool_hyper"] = nativePokemon->pkm->pkm.ribbonSinnohB.cool_hyper;
            pRibbonSinnohB["cool_master"] = nativePokemon->pkm->pkm.ribbonSinnohB.cool_master;
            pRibbonSinnohB["beauty_normal"] = nativePokemon->pkm->pkm.ribbonSinnohB.beauty_normal;
            pRibbonSinnohB["beauty_super"] = nativePokemon->pkm->pkm.ribbonSinnohB.beauty_super;
            pRibbonSinnohB["beauty_hyper"] = nativePokemon->pkm->pkm.ribbonSinnohB.beauty_hyper;
            pRibbonSinnohB["beauty_master"] = nativePokemon->pkm->pkm.ribbonSinnohB.beauty_master;
            pRibbonSinnohB["cute_normal"] = nativePokemon->pkm->pkm.ribbonSinnohB.cute_normal;
            pRibbonSinnohB["cute_super"] = nativePokemon->pkm->pkm.ribbonSinnohB.cute_super;
            pRibbonSinnohB["cute_hyper"] = nativePokemon->pkm->pkm.ribbonSinnohB.cute_hyper;
            pRibbonSinnohB["cute_master"] = nativePokemon->pkm->pkm.ribbonSinnohB.cute_master;
            pRibbonSinnohB["smart_normal"] = nativePokemon->pkm->pkm.ribbonSinnohB.smart_normal;
            pRibbonSinnohB["smart_super"] = nativePokemon->pkm->pkm.ribbonSinnohB.smart_super;
            pRibbonSinnohB["smart_hyper"] = nativePokemon->pkm->pkm.ribbonSinnohB.smart_hyper;
            pRibbonSinnohB["smart_master"] = nativePokemon->pkm->pkm.ribbonSinnohB.smart_master;
            pRibbonSinnohB["tough_normal"] = nativePokemon->pkm->pkm.ribbonSinnohB.tough_normal;
            pRibbonSinnohB["tough_super"] = nativePokemon->pkm->pkm.ribbonSinnohB.tough_super;
            pRibbonSinnohB["tough_hyper"] = nativePokemon->pkm->pkm.ribbonSinnohB.tough_hyper;
            pRibbonSinnohB["tough_master"] = nativePokemon->pkm->pkm.ribbonSinnohB.tough_master;
            pRibbonHoenn["cool_normal"] = nativePokemon->pkm->pkm.ribbonHoenn.cool_normal;
            pRibbonHoenn["cool_super"] = nativePokemon->pkm->pkm.ribbonHoenn.cool_super;
            pRibbonHoenn["cool_hyper"] = nativePokemon->pkm->pkm.ribbonHoenn.cool_hyper;
            pRibbonHoenn["cool_master"] = nativePokemon->pkm->pkm.ribbonHoenn.cool_master;
            pRibbonHoenn["beauty_normal"] = nativePokemon->pkm->pkm.ribbonHoenn.beauty_normal;
            pRibbonHoenn["beauty_super"] = nativePokemon->pkm->pkm.ribbonHoenn.beauty_super;
            pRibbonHoenn["beauty_hyper"] = nativePokemon->pkm->pkm.ribbonHoenn.beauty_hyper;
            pRibbonHoenn["beauty_master"] = nativePokemon->pkm->pkm.ribbonHoenn.beauty_master;
            pRibbonHoenn["cute_normal"] = nativePokemon->pkm->pkm.ribbonHoenn.cute_normal;
            pRibbonHoenn["cute_super"] = nativePokemon->pkm->pkm.ribbonHoenn.cute_super;
            pRibbonHoenn["cute_hyper"] = nativePokemon->pkm->pkm.ribbonHoenn.cute_hyper;
            pRibbonHoenn["cute_master"] = nativePokemon->pkm->pkm.ribbonHoenn.cute_master;
            pRibbonHoenn["smart_normal"] = nativePokemon->pkm->pkm.ribbonHoenn.smart_normal;
            pRibbonHoenn["smart_super"] = nativePokemon->pkm->pkm.ribbonHoenn.smart_super;
            pRibbonHoenn["smart_hyper"] = nativePokemon->pkm->pkm.ribbonHoenn.smart_hyper;
            pRibbonHoenn["smart_master"] = nativePokemon->pkm->pkm.ribbonHoenn.smart_master;
            pRibbonHoenn["champion"] = nativePokemon->pkm->pkm.ribbonHoenn.champion;
            pRibbonHoenn["winning"] = nativePokemon->pkm->pkm.ribbonHoenn.winning;
            pRibbonHoenn["victory"] = nativePokemon->pkm->pkm.ribbonHoenn.victory;
            pRibbonHoenn["artist"] = nativePokemon->pkm->pkm.ribbonHoenn.artist;
            pRibbonHoenn["effort"] = nativePokemon->pkm->pkm.ribbonHoenn.effort;
            pRibbonHoenn["marine"] = nativePokemon->pkm->pkm.ribbonHoenn.marine;
            pRibbonHoenn["land"] = nativePokemon->pkm->pkm.ribbonHoenn.land;
            pRibbonHoenn["sky"] = nativePokemon->pkm->pkm.ribbonHoenn.sky;
            pRibbonHoenn["country"] = nativePokemon->pkm->pkm.ribbonHoenn.country;
            pRibbonHoenn["national"] = nativePokemon->pkm->pkm.ribbonHoenn.national;
            pRibbonHoenn["earth"] = nativePokemon->pkm->pkm.ribbonHoenn.earth;
            pRibbonHoenn["world"] = nativePokemon->pkm->pkm.ribbonHoenn.world;
            pPKM["pid"] = nativePokemon->pkm->pkm.pid;
            pPKM["species"] = nativePokemon->pkm->pkm.species;
            pPKM["held_item"] = nativePokemon->pkm->pkm.held_item;
            pPKM["ot_id"] = nativePokemon->pkm->pkm.ot_id;
            pPKM["ot_sid"] = nativePokemon->pkm->pkm.ot_sid;
            pPKM["exp"] = nativePokemon->pkm->pkm.exp;
            pPKM["friendship"] = nativePokemon->pkm->pkm.friendship;
            pPKM["ability"] = nativePokemon->pkm->pkm.ability;
            pPKM["markings"] = nativePokemon->pkm->pkm.markings; //TODO: separate markings char into individual markings = nativePokemon->pkm->pkm.markings;
            pPKM["country"] = nativePokemon->pkm->pkm.country;
            pPKM["ev_hp"] = nativePokemon->pkm->pkm.ev_hp;
            pPKM["ev_atk"] = nativePokemon->pkm->pkm.ev_atk;
            pPKM["ev_def"] = nativePokemon->pkm->pkm.ev_def;
            pPKM["ev_spd"] = nativePokemon->pkm->pkm.ev_spd;
            pPKM["ev_satk"] = nativePokemon->pkm->pkm.ev_satk;
            pPKM["ev_sdef"] = nativePokemon->pkm->pkm.ev_sdef;
            pPKM["contest_cool"] = nativePokemon->pkm->pkm.contest_cool;
            pPKM["contest_beauty"] = nativePokemon->pkm->pkm.contest_beauty;
            pPKM["contest_cute"] = nativePokemon->pkm->pkm.contest_cute;
            pPKM["contest_smart"] = nativePokemon->pkm->pkm.contest_smart;
            pPKM["contest_tough"] = nativePokemon->pkm->pkm.contest_tough;
            pPKM["contest_sheen"] = nativePokemon->pkm->pkm.contest_sheen;
            pPKM["move1"] = nativePokemon->pkm->pkm.move[0];
            pPKM["move2"] = nativePokemon->pkm->pkm.move[1];
            pPKM["move3"] = nativePokemon->pkm->pkm.move[2];
            pPKM["move4"] = nativePokemon->pkm->pkm.move[3];
            pPKM["movePP1"] = nativePokemon->pkm->pkm.movePP[0];
            pPKM["movePP2"] = nativePokemon->pkm->pkm.movePP[1];
            pPKM["movePP3"] = nativePokemon->pkm->pkm.movePP[2];
            pPKM["movePP4"] = nativePokemon->pkm->pkm.movePP[3];
            pPKM["iv_hp"] = nativePokemon->pkm->pkm.iv_hp;
            pPKM["iv_atk"] = nativePokemon->pkm->pkm.iv_atk;
            pPKM["iv_def"] = nativePokemon->pkm->pkm.iv_def;
            pPKM["iv_spd"] = nativePokemon->pkm->pkm.iv_spd;
            pPKM["iv_satk"] = nativePokemon->pkm->pkm.iv_satk;
            pPKM["iv_sdef"] = nativePokemon->pkm->pkm.iv_sdef;
            pPKM["isEgg"] = nativePokemon->pkm->pkm.isEgg;
            pPKM["isNicknamed"] = nativePokemon->pkm->pkm.isNicknamed;
            pPKM["isFatefulEncounter"] = nativePokemon->pkm->pkm.isFatefulEncounter;
            pPKM["isFemale"] = nativePokemon->pkm->pkm.isFemale;
            pPKM["isGenderless"] = nativePokemon->pkm->pkm.isGenderless;
            pPKM["form"] = nativePokemon->pkm->pkm.form;
            pPKM["shining_leaf"] = nativePokemon->pkm->pkm.shining_leaf; //TODO: separate shining leaf into individual bits = nativePokemon->pkm->pkm.form;
            pPKM["eggLoc_Plat"] = nativePokemon->pkm->pkm.eggLoc_Plat;
            pPKM["metLoc_Plat"] = nativePokemon->pkm->pkm.metLoc_Plat;
            pPKM["hometown"] = nativePokemon->pkm->pkm.hometown;
            pPKM["eggYear"] = nativePokemon->pkm->pkm.eggDate[0];
            pPKM["eggMonth"] = nativePokemon->pkm->pkm.eggDate[1];
            pPKM["eggDay"] = nativePokemon->pkm->pkm.eggDate[2];
            pPKM["metYear"] = nativePokemon->pkm->pkm.metDate[0];
            pPKM["metMonth"] = nativePokemon->pkm->pkm.metDate[1];
            pPKM["metDay"] = nativePokemon->pkm->pkm.metDate[2];
            pPKM["pokerus"] = nativePokemon->pkm->pkm.pokerus;
            pPKM["pokeball"] = nativePokemon->pkm->pkm.pokeball;
            pPKM["ot_female"] = nativePokemon->pkm->pkm.ot_female;
            pPKM["encounterType"] = nativePokemon->pkm->pkm.encounterType;
            pPKM["pokeball_HGSS"] = nativePokemon->pkm->pkm.pokeball_HGSS;
            pPKM["eff_sleep"] = nativePokemon->pkm->pkm.eff_sleep;
            pPKM["eff_poison"] = nativePokemon->pkm->pkm.eff_poison;
            pPKM["eff_burn"] = nativePokemon->pkm->pkm.eff_burn;
            pPKM["eff_frozen"] = nativePokemon->pkm->pkm.eff_frozen;
            pPKM["eff_paralyze"] = nativePokemon->pkm->pkm.eff_paralyze;
            pPKM["eff_toxic"] = nativePokemon->pkm->pkm.eff_toxic;
            pPKM["battle_level"] = nativePokemon->pkm->pkm.battle_level;
            pPKM["battle_capsule"] = nativePokemon->pkm->pkm.battle_capsule;
            pPKM["battle_hp"] = nativePokemon->pkm->pkm.battle_hp;
            pPKM["battle_max_hp"] = nativePokemon->pkm->pkm.battle_max_hp;
            pPKM["battle_atk"] = nativePokemon->pkm->pkm.battle_atk;
            pPKM["battle_def"] = nativePokemon->pkm->pkm.battle_def;
            pPKM["battle_spd"] = nativePokemon->pkm->pkm.battle_spd;
            pPKM["battle_satk"] = nativePokemon->pkm->pkm.battle_satk;
            pPKM["battle_sdef"] = nativePokemon->pkm->pkm.battle_sdef;
        }
    }
}
