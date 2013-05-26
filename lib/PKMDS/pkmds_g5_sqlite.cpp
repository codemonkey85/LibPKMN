#pragma once
#include "pkmds_g5_sqlite.h"
using namespace std;
sqlite3 *database;
sqlite3_stmt *statement;
void opendb(const char db_file[])
{
	sqlite3_open(db_file, &database);
}
void closedb()
{
	sqlite3_close(database);
}
string getastring(const std::ostringstream &o)
{
	string s = "";
    char cmd[BUFF_SIZE];
    strcpy(cmd,o.str().c_str());
	if(sqlite3_prepare_v2(database,cmd,-1,&statement,0) == SQLITE_OK)
	{
		int cols = sqlite3_column_count(statement);
		int result = 0;
		result = sqlite3_step(statement);
		if(result == SQLITE_ROW)
		{
			for(int col = 0; col < cols; col++)
			{
				const unsigned char* test;
				test = sqlite3_column_text(statement, col);
				if(sqlite3_column_text(statement, col) != 0)
				{
					s = (char*)test;
				}
			}
		}
		sqlite3_finalize(statement);
	}
	return s;
}
int getanint(const std::ostringstream &o)
{
	int i = 0;
	char cmd[BUFF_SIZE];
    strcpy(cmd,o.str().c_str());
	if(sqlite3_prepare_v2(database,cmd,-1,&statement,0) == SQLITE_OK)
	{
		int cols = sqlite3_column_count(statement);
		int result = 0;
		result = sqlite3_step(statement);
		if(result == SQLITE_ROW)
		{
			for(int col = 0; col < cols; col++)
			{
				i = sqlite3_column_int(statement, col);
			}
		}
		sqlite3_finalize(statement);
	}
	return i;
}
const void * getablob(const std::ostringstream &o)
{
	const void * blob;
	string test;
	test = o.str();
	char cmd[BUFF_SIZE];
    strcpy(cmd,o.str().c_str());
	if(sqlite3_prepare_v2(database,cmd,-1,&statement,0) == SQLITE_OK)
	{
		int cols = sqlite3_column_count(statement);
		int result = 0;
		result = sqlite3_step(statement);
		if(result == SQLITE_ROW)
		{
			for(int col = 0; col < cols; col++)
			{
				blob = sqlite3_column_blob(statement, col);
			}
		}
		sqlite3_finalize(statement);
	}
	return blob;
}
void dostatement(const std::string &cmd)
{
	sqlite3_prepare_v2(database,cmd.c_str(),-1,&statement,0);
	sqlite3_step(statement);
}
string lookuppkmname(const int speciesid, const int langid)
{
	std::ostringstream o;
	o << "SELECT name FROM pokemon_species_names WHERE (pokemon_species_id = "
		<< speciesid << " ) AND (local_language_id = " << langid << ")";
	return getastring(o);
}
string lookuppkmname(const pokemon_obj &pkm, const int langid)
{
	return lookuppkmname(pkm.blocka.species,langid);
}
string lookuppkmname(const pokemon_obj *pkm, const int langid)
{
	return lookuppkmname(pkm->blocka.species,langid);
}
string lookupmovename(const int moveid, const int langid)
{
	std::ostringstream o;
	o << "SELECT name FROM move_names WHERE (move_id = "
		<< moveid << ") AND (local_language_id = "
		<< langid << ")";
	return getastring(o);
}
string lookupmovename(const pokemon_obj &pkm, const int movenum, const int langid)
{
	return lookupmovename(pkm.blockb.moves[movenum],langid);
}
string lookupmovename(const pokemon_obj *pkm, const int movenum, const int langid)
{
	return lookupmovename(pkm->blockb.moves[movenum],langid);
}
string lookupmoveflavortext(const uint16 moveid, const int langid, const int versiongroup)
{
	std::ostringstream o;

	o << "SELECT move_flavor_text.flavor_text FROM moves " <<
		"INNER JOIN move_flavor_text ON moves.id = move_flavor_text.move_id WHERE" <<
		"(move_flavor_text.language_id = " << langid << ")" <<
		"AND (move_flavor_text.move_id = " << moveid << ")" <<
		"AND (move_flavor_text.version_group_id = " << versiongroup << ")";

	return getastring(o);
}
string lookupmoveflavortext(const pokemon_obj &pkm, const int movenum, const int langid, const int versiongroup)
{
	return lookupmoveflavortext(pkm.blockb.moves[movenum],langid,versiongroup);
}
string lookupmoveflavortext(const pokemon_obj *pkm, const int movenum, const int langid, const int versiongroup)
{
	return lookupmoveflavortext(pkm->blockb.moves[movenum],langid,versiongroup);
}
string lookupmovetypename(const uint16 moveid, const int langid)
{
	std::ostringstream o;
	o << "SELECT type_names.name "
		<< "FROM   moves "
		<< "       INNER JOIN type_names "
		<< "               ON moves.type_id = type_names.type_id "
		<< "       INNER JOIN types "
		<< "               ON moves.type_id = types.id "
		<< "                  AND type_names.type_id = types.id "
		<< "WHERE  ( moves.id = " << moveid << " ) "
		<< "       AND ( type_names.local_language_id = " << langid << " ) ";
	return getastring(o);
}
string lookupmovetypename(const pokemon_obj &pkm, const int movenum, const int langid)
{
	return lookupmovetypename(pkm.blockb.moves[movenum],langid);
}
string lookupmovetypename(const pokemon_obj *pkm, const int movenum, const int langid)
{
	return lookupmovetypename(pkm->blockb.moves[movenum],langid);
}
string lookupmovedamagetypename(const uint16 moveid, const int langid)
{
	std::ostringstream o;
	o << ""
		<< "SELECT move_damage_class_prose.name "
		<< "FROM   moves "
		<< "       INNER JOIN move_damage_classes "
		<< "               ON moves.damage_class_id = move_damage_classes.id "
		<< "       INNER JOIN move_damage_class_prose "
		<< "               ON move_damage_classes.id = "
		<< "                  move_damage_class_prose.move_damage_class_id "
		<< "WHERE  ( moves.id = " << moveid << " ) "
		<< "       AND ( move_damage_class_prose.local_language_id = " << langid << " ) ";
	return getastring(o);
}
string lookupmovedamagetypename(const pokemon_obj &pkm, const int movenum, const int langid)
{
	return lookupmovedamagetypename(pkm.blockb.moves[movenum],langid);
}
int getpkmlevel(const int id, const int exp)
{
	std::ostringstream o;
	o << ""
		<< "SELECT experience.level "
		<< "FROM   experience "
		<< "       INNER JOIN pokemon_species "
		<< "               ON experience.growth_rate_id = pokemon_species.growth_rate_id "
		<< "                  AND experience.growth_rate_id = pokemon_species.growth_rate_id "
		<< "WHERE  ( experience.experience <= " << exp << " ) "
		<< "       AND ( pokemon_species.id = " << id << " ) "
		<< "ORDER  BY experience.experience DESC ";
	return getanint(o);
}
int getpkmlevel(const pokemon_obj &pkm)
{
	return getpkmlevel(pkm.blocka.species,pkm.blocka.exp);
}
int getpkmlevel(const pokemon_obj *pkm)
{
	return getpkmlevel(pkm->blocka.species,pkm->blocka.exp);
}
int getpkmexptonext(const int id, const int exp)
{
	if(getpkmlevel(id,exp) == 100)
	{
		return 0;
	};
	std::ostringstream o;
	o << ""
		<< "SELECT experience.experience "
		<< "FROM   pokemon_species "
		<< "       INNER JOIN experience "
		<< "               ON pokemon_species.growth_rate_id = experience.growth_rate_id "
		<< "WHERE  ( pokemon_species.id = " << id << " ) "
		<< "       AND ( experience.experience > " << exp << " ) "
		<< "ORDER  BY experience.experience ";
	int expatnext = getanint(o);
	return expatnext - exp;
}
int getpkmexptonext(const pokemon_obj &pkm)
{
	return getpkmexptonext(pkm.blocka.species, pkm.blocka.exp);
}
int getpkmexpatcur(const int id, const int exp)
{
	std::ostringstream o;
	o << ""
		<< "SELECT experience.experience "
		<< "FROM   pokemon_species "
		<< "       INNER JOIN experience "
		<< "               ON pokemon_species.growth_rate_id = experience.growth_rate_id "
		<< "WHERE  ( pokemon_species.id = " << id << " ) "
		<< "       AND ( experience.level = " << getpkmlevel(id,exp) << " ) "
		<< "ORDER  BY experience.experience ";
	return getanint(o);
}
int getpkmexpatcur(const pokemon_obj &pkm)
{
	return getpkmexpatcur(pkm.blocka.species,pkm.blocka.exp);
}
string getnaturename(const int natureid, const int langid)
{
	std::ostringstream o;
	o << ""
		<< "SELECT name "
		<< "FROM   nature_names "
		<< "WHERE  ( nature_id = " << (int)(natureconvert[natureid][1]) << " ) "
		<< "       AND ( local_language_id = " << langid << " ) ";
	return getastring(o);
}
string getnaturename(const pokemon_obj &pkm, const int langid)
{
	if(pkm.blockb.nature == 0 && pkm.blockc.hometown != Hometowns::black && pkm.blockc.hometown != Hometowns::white)
	{
		return getnaturename(pkm.block0.pid % 25,langid);
	}
	else
	{
		return getnaturename(pkm.blockb.nature,langid);
	}
}
int getnatureincrease(const int natureid)
{
	std::ostringstream o;
	o << ""
		<< "SELECT increased_stat_id "
		<< "FROM   natures "
		<< "WHERE  ( id = " << (int)(natureconvert[natureid][1]) << " ) ";
	return getanint(o);
}
int getnatureincrease(const pokemon_obj &pkm)
{
	if(pkm.blockb.nature == 0 && pkm.blockc.hometown != Hometowns::black && pkm.blockc.hometown != Hometowns::white)
	{
		return getnatureincrease(pkm.block0.pid % 25);
	}
	else
	{
		return getnatureincrease(pkm.blockb.nature);
	}
}
int getnatureincrease(const pokemon_obj *pkm)
{
	if(pkm->blockb.nature == 0 && pkm->blockc.hometown != Hometowns::black && pkm->blockc.hometown != Hometowns::white)
	{
		return getnatureincrease(pkm->block0.pid % 25);
	}
	else
	{
		return getnatureincrease(pkm->blockb.nature);
	}
}
int getnaturedecrease(const int natureid)
{
	std::ostringstream o;
	o << ""
		<< "SELECT decreased_stat_id "
		<< "FROM   natures "
		<< "WHERE  ( id = " << (int)(natureconvert[natureid][1]) << " ) ";
	return getanint(o);
}
int getnaturedecrease(const pokemon_obj &pkm)
{
	if(pkm.blockb.nature == 0 && pkm.blockc.hometown != Hometowns::black && pkm.blockc.hometown != Hometowns::white)
	{
		return getnaturedecrease(pkm.block0.pid % 25);
	}
	else
	{
		return getnaturedecrease(pkm.blockb.nature);
	}
}
int getnaturedecrease(const pokemon_obj *pkm)
{
	if(pkm->blockb.nature == 0 && pkm->blockc.hometown != Hometowns::black && pkm->blockc.hometown != Hometowns::white)
	{
		return getnaturedecrease(pkm->block0.pid % 25);
	}
	else
	{
		return getnaturedecrease(pkm->blockb.nature);
	}
}
string lookupitemname(const int itemid, const int generation, const int langid)
{
	std::ostringstream o;
	o << ""
		<< "SELECT item_names.name "
		<< "FROM   item_game_indices "
		<< "       INNER JOIN item_names "
		<< "               ON item_game_indices.item_id = item_names.item_id "
		<< "WHERE  ( item_game_indices.generation_id = " << generation << " ) "
		<< "       AND ( item_game_indices.game_index = " << itemid << " ) "
		<< "       AND ( item_names.local_language_id = " << langid << " ) ";
	return getastring(o);
}
string lookupitemname(const pokemon_obj &pkm, const int generation, const int langid)
{
	return lookupitemname(pkm.blocka.item,generation,langid);
}
int getpkmstat(const pokemon_obj &pkm, const Stat_IDs::stat_ids stat_id)
{
	int basestat = 0;
	std::ostringstream o;
	o << ""
		<< "SELECT pokemon_stats.base_stat "
		<< "FROM   pokemon_stats "
		<< "       INNER JOIN pokemon_forms "
		<< "               ON pokemon_stats.pokemon_id = pokemon_forms.pokemon_id "
		<< "       INNER JOIN stats "
		<< "               ON pokemon_stats.stat_id = stats.id "
		<< "       INNER JOIN stat_names "
		<< "               ON stats.id = stat_names.stat_id "
		<< "       INNER JOIN pokemon_species_names "
		<< "               ON stat_names.local_language_id = "
		<< "                  pokemon_species_names.local_language_id "
		<< "       INNER JOIN pokemon "
		<< "               ON pokemon_stats.pokemon_id = pokemon.id "
		<< "                  AND pokemon_forms.pokemon_id = pokemon.id "
		<< "                  AND pokemon_species_names.pokemon_species_id = "
		<< "                      pokemon.species_id "
		<< "WHERE  ( pokemon_species_names.local_language_id = 9 ) "
		<< "       AND ( stat_names.local_language_id = 9 ) "
		<< "       AND ( pokemon_species_names.pokemon_species_id = " << (uint16)pkm.blocka.species << " ) "
		<< "       AND ( pokemon_forms.form_order = " << (int)(pkm.blockb.forms.form) << " + 1 ) "
		<< "       AND ( stat_names.stat_id = " << (int)stat_id << " ) ";
	basestat = getanint(o);
	int level = getpkmlevel(pkm);
	int iv = 0;
	int ev = 0;
	switch(stat_id)
	{
    case Stat_IDs::hp:
		iv = pkm.blockb.ivs.hp;
		ev = pkm.blocka.evs.hp;
		return (int)((floor((double)(floor((double)(((iv + (2 * basestat) + floor((double)(ev/4))+100) * level) / 100)) + 10))));
		break;
    case Stat_IDs::attack:
		iv = pkm.blockb.ivs.attack;
		ev = pkm.blocka.evs.atk;
		break;
    case Stat_IDs::defense:
		iv = pkm.blockb.ivs.defense;
		ev = pkm.blocka.evs.def;
		break;
    case Stat_IDs::spatk:
		iv = pkm.blockb.ivs.spatk;
		ev = pkm.blocka.evs.spatk;
		break;
    case Stat_IDs::spdef:
		iv = pkm.blockb.ivs.spdef;
		ev = pkm.blocka.evs.spdef;
		break;
    case Stat_IDs::speed:
		iv = pkm.blockb.ivs.speed;
		ev = pkm.blocka.evs.spd;
		break;
	};
	double naturemod = 1.0;
	if (getnatureincrease(pkm) != getnaturedecrease(pkm)) {
		if (stat_id == getnatureincrease(pkm)){
			naturemod = 1.1;
		};
		if (stat_id == getnaturedecrease(pkm)){
			naturemod = 0.9;
		};
	};
	return (int)((floor((double)(floor((double)(((iv + (2 * basestat) + floor((double)(ev/4))) * level) / 100)) + 5)) * naturemod));
	return 0;
}
int getpkmstat(const pokemon_obj *pkm, const Stat_IDs::stat_ids stat_id)
{
	int basestat = 0;
	std::ostringstream o;
	o << ""
		<< "SELECT pokemon_stats.base_stat "
		<< "FROM   pokemon_stats "
		<< "       INNER JOIN pokemon_forms "
		<< "               ON pokemon_stats.pokemon_id = pokemon_forms.pokemon_id "
		<< "       INNER JOIN stats "
		<< "               ON pokemon_stats.stat_id = stats.id "
		<< "       INNER JOIN stat_names "
		<< "               ON stats.id = stat_names.stat_id "
		<< "       INNER JOIN pokemon_species_names "
		<< "               ON stat_names.local_language_id = "
		<< "                  pokemon_species_names.local_language_id "
		<< "       INNER JOIN pokemon "
		<< "               ON pokemon_stats.pokemon_id = pokemon.id "
		<< "                  AND pokemon_forms.pokemon_id = pokemon.id "
		<< "                  AND pokemon_species_names.pokemon_species_id = "
		<< "                      pokemon.species_id "
		<< "WHERE  ( pokemon_species_names.local_language_id = 9 ) "
		<< "       AND ( stat_names.local_language_id = 9 ) "
		<< "       AND ( pokemon_species_names.pokemon_species_id = " << (uint16)pkm->blocka.species << " ) "
		<< "       AND ( pokemon_forms.form_order = " << (int)(pkm->blockb.forms.form) << " + 1 ) "
		<< "       AND ( stat_names.stat_id = " << (int)stat_id << " ) ";
	basestat = getanint(o);
	int level = getpkmlevel(pkm);
	int iv = 0;
	int ev = 0;
	switch(stat_id)
	{
    case Stat_IDs::hp:
		iv = pkm->blockb.ivs.hp;
		ev = pkm->blocka.evs.hp;
		return (int)((floor((double)(floor((double)(((iv + (2 * basestat) + floor((double)(ev/4))+100) * level) / 100)) + 10))));
		break;
    case Stat_IDs::attack:
		iv = pkm->blockb.ivs.attack;
		ev = pkm->blocka.evs.atk;
		break;
    case Stat_IDs::defense:
		iv = pkm->blockb.ivs.defense;
		ev = pkm->blocka.evs.def;
		break;
    case Stat_IDs::spatk:
		iv = pkm->blockb.ivs.spatk;
		ev = pkm->blocka.evs.spatk;
		break;
    case Stat_IDs::spdef:
		iv = pkm->blockb.ivs.spdef;
		ev = pkm->blocka.evs.spdef;
		break;
    case Stat_IDs::speed:
		iv = pkm->blockb.ivs.speed;
		ev = pkm->blocka.evs.spd;
		break;
	};
	double naturemod = 1.0;
	if (getnatureincrease(pkm) != getnaturedecrease(pkm)) {
		if (stat_id == getnatureincrease(pkm)){
			naturemod = 1.1;
		};
		if (stat_id == getnaturedecrease(pkm)){
			naturemod = 0.9;
		};
	};
	return (int)((floor((double)(floor((double)(((iv + (2 * basestat) + floor((double)(ev/4))) * level) / 100)) + 5)) * naturemod));
	return 0;
}
string getpkmgendername(const pokemon_obj &pkm)
{
	return getgendername(getpkmgender(pkm));
};
bool pkmhasgenddiff(const int species)
{
	std::ostringstream o;
	o << ""
		<< "SELECT has_gender_differences "
		<< "FROM   pokemon_species "
		<< "WHERE  ( id = " << (int)species << " ) ";
	int h = getanint(o);
	return (h == 1);
}
bool pkmhasgenddiff(const pokemon_obj &pkm)
{
	return pkmhasgenddiff(pkm.blocka.species);
}
string lookupabilityname(const int abilityid, const int langid)
{
	std::ostringstream o;
	o << ""
		<< "SELECT name "
		<< "FROM   ability_names "
		<< "WHERE  ( local_language_id = " << langid << " ) "
		<< "       AND ( ability_id = " << abilityid << " ) ";
	return getastring(o);
}
string lookupabilityname(const pokemon_obj &pkm, const int langid)
{
	return lookupabilityname(pkm.blocka.ability, langid);
}
string getpkmformname(const pokemon_obj &pkm, const int langid)
{
	std::ostringstream o;
	o << ""
		<< "SELECT pokemon_form_names.form_name "
		<< "FROM   pokemon_forms "
		<< "       INNER JOIN pokemon_form_names "
		<< "               ON pokemon_forms.id = pokemon_form_names.pokemon_form_id "
		<< "       INNER JOIN pokemon "
		<< "               ON pokemon_forms.pokemon_id = pokemon.id "
		<< "       INNER JOIN pokemon_species "
		<< "               ON pokemon.species_id = pokemon_species.id "
		<< "       INNER JOIN pokemon_species_names "
		<< "               ON pokemon_species.id = pokemon_species_names.pokemon_species_id "
		<< "WHERE  ( pokemon_form_names.local_language_id = " << langid << " ) "
		<< "       AND ( pokemon_species_names.local_language_id = " << langid << " ) "
		<< "       AND ( pokemon.species_id = " << (uint16)pkm.blocka.species << " ) "
		<< "       AND ( pokemon_forms.form_order = " << (int)(pkm.blockb.forms.form) << " + 1 ) ";
	return getastring(o);
}
string lookuplocname(const int locid, const int gen, const int langid)
{
	std::ostringstream o;
	o << ""
		<< "SELECT location_names.name AS [Location Name] "
		<< "FROM   locations "
		<< "       INNER JOIN location_game_indices "
		<< "               ON locations.id = location_game_indices.location_id "
		<< "       INNER JOIN location_names "
		<< "               ON locations.id = location_names.location_id "
		<< "       INNER JOIN generations "
		<< "               ON location_game_indices.generation_id = generations.id "
		<< "WHERE  ( location_names.local_language_id = " << langid << " ) "
		<< "       AND ( generations.id = " << gen << " ) "
		<< "       AND ( location_game_indices.game_index = " << locid << " ) "
		<< "ORDER  BY location_game_indices.game_index ";
	return getastring(o);
}
string getpkmmetlocname(const pokemon_obj &pkm, const int gen, const int langid)
{
	switch(pkm.blockd.met)
	{
	case Locations::poketransfer:
		return "Poké Transfer Lab";
		break;
	case Locations::daycarecouple:
		return "Day-Care Couple";
		break;
	default:
		return lookuplocname(pkm.blockd.met, gen, langid);
	}
}
string getpkmegglocname(const pokemon_obj &pkm, const int gen, const int langid)
{
	switch(pkm.blockd.eggmet)
	{
	case Locations::poketransfer:
		return "Poké Transfer Lab";
		break;
	case Locations::daycarecouple:
		return "Day-Care Couple";
		break;
	default:
		return lookuplocname(pkm.blockd.eggmet, gen, langid);
	}
}
string lookupitemflavortext(const int itemid, const int generation, const int langid, const int versiongroup)
{
	std::ostringstream o;
	o << ""
		<< "SELECT item_flavor_text.flavor_text "
		<< "FROM   item_flavor_text "
		<< "       INNER JOIN item_game_indices "
		<< "               ON item_flavor_text.item_id = item_game_indices.item_id "
		<< "       INNER JOIN items "
		<< "               ON item_flavor_text.item_id = items.id "
		<< "                  AND item_game_indices.item_id = items.id "
		<< "WHERE  ( item_flavor_text.language_id = " << langid << " ) "
		<< "       AND ( item_game_indices.generation_id = " << generation << " ) "
		<< "       AND ( item_game_indices.game_index = " << itemid << " ) "
		<< "       AND ( item_flavor_text.version_group_id = " << versiongroup << " ) ";
	return getastring(o);
}
string lookupitemflavortext(const pokemon_obj &pkm, const int generation, const int langid, const int versiongroup)
{
	return lookupitemflavortext(pkm.blocka.item,generation,langid,versiongroup);
}
string lookuppkmcolorname(const int species, const int langid)
{
	std::ostringstream o;
	o << ""
		<< "SELECT pokemon_color_names.name "
		<< "FROM   pokemon_colors "
		<< "       INNER JOIN pokemon_color_names "
		<< "               ON pokemon_colors.id = pokemon_color_names.pokemon_color_id "
		<< "       INNER JOIN pokemon_species "
		<< "               ON pokemon_colors.id = pokemon_species.color_id "
		<< "WHERE  ( pokemon_color_names.local_language_id = " << langid << " ) "
		<< "       AND ( pokemon_species.id = " << species << " ) ";
	return getastring(o);
}
string lookuppkmcolorname(const pokemon_obj &pkm, const int langid)
{
	return lookuppkmcolorname(pkm.blocka.species, langid);
}
int lookuppkmcolorid(const int species)
{
	std::ostringstream o;
	o << ""
		<< "SELECT pokemon_color_names.pokemon_color_id "
		<< "FROM   pokemon_colors "
		<< "       INNER JOIN pokemon_color_names "
		<< "               ON pokemon_colors.id = pokemon_color_names.pokemon_color_id "
		<< "       INNER JOIN pokemon_species "
		<< "               ON pokemon_colors.id = pokemon_species.color_id "
		<< "WHERE  ( pokemon_color_names.local_language_id = 9 ) "
		<< "       AND ( pokemon_species.id = " << species << " ) ";
	return getanint(o);
}
int lookuppkmcolorid(const pokemon_obj &pkm)
{
	return lookuppkmcolorid(pkm.blocka.species);
}
string lookupabilityflavortext(const int abilityid, const int version_group, const int langid)
{
	std::ostringstream o;
	o << ""
		<< "SELECT ability_flavor_text.flavor_text "
		<< "FROM   abilities "
		<< "       INNER JOIN ability_flavor_text "
		<< "               ON abilities.id = ability_flavor_text.ability_id "
		<< "       INNER JOIN languages "
		<< "               ON ability_flavor_text.language_id = languages.id "
		<< "WHERE  ( ability_flavor_text.version_group_id = " << version_group << " ) "
		<< "       AND ( ability_flavor_text.language_id = " << langid << " ) "
		<< "       AND ( abilities.id = " << abilityid << " ) ";
	return getastring(o);
}
string lookupabilityflavortext(const pokemon_obj &pkm, const int version_group, const int langid)
{
	return lookupabilityflavortext(pkm.blocka.ability,version_group,langid);
}
string lookupcharacteristic(const int statid, const int iv, const int langid)
{
	std::ostringstream o;
	o << ""
		<< "SELECT stat_hint_names.message "
		<< "FROM   stat_hints "
		<< "       INNER JOIN stat_hint_names "
		<< "               ON stat_hints.id = stat_hint_names.stat_hint_id "
		<< "WHERE  ( stat_hint_names.local_language_id = " << langid << " ) "
		<< "       AND ( stat_hints.stat_id = " << statid << " ) "
		<< "       AND ( stat_hints.gene_mod_5 = " << (int)(iv % 5) << " ) ";
	return getastring(o);
}
bool compareivbyval(const ivtest &a, const ivtest &b)
{
	return a.val > b.val;
}
bool compareivbyid(const ivtest &a, const ivtest &b)
{
	return a.id < b.id;
}
bool compareivbyorder(const ivtest &a, const ivtest &b)
{
	return a.order < b.order;
}
string lookupcharacteristic(const pokemon_obj &pkm, const int langid)
{
	int statid = 0;
	int highval = 0;
	ivtest ivbuf = {};
	vector<ivtest> ivs;
	ivbuf.id = Stat_IDs::hp;
	ivbuf.val = pkm.blockb.ivs.hp;
	ivbuf.order = 0;
	ivs.push_back(ivbuf);
	ivbuf.id = Stat_IDs::attack;
	ivbuf.val = pkm.blockb.ivs.attack;
	ivbuf.order = 1;
	ivs.push_back(ivbuf);
	ivbuf.id = Stat_IDs::defense;
	ivbuf.val = pkm.blockb.ivs.defense;
	ivbuf.order = 2;
	ivs.push_back(ivbuf);
	ivbuf.id = Stat_IDs::speed;
	ivbuf.val = pkm.blockb.ivs.speed;
	ivbuf.order = 3;
	ivs.push_back(ivbuf);
	ivbuf.id = Stat_IDs::spatk;
	ivbuf.val = pkm.blockb.ivs.spatk;
	ivbuf.order = 4;
	ivs.push_back(ivbuf);
	ivbuf.id = Stat_IDs::spdef;
	ivbuf.val = pkm.blockb.ivs.spdef;
	ivbuf.order = 5;
	ivs.push_back(ivbuf);
	std::sort(ivs.begin(),ivs.end(),compareivbyval);
	highval = ivs[0].val;
	std::sort(ivs.begin(),ivs.end(),compareivbyorder);
	int highcount = 0;
	for(int i=0;i<6;i++)
	{
		if(ivs[i].val == highval)
		{
			statid = i+1;
			highcount++;
		}
	}
	if(highcount == 1)
	{
		return lookupcharacteristic(statid,highval,langid);
	}
	int startindex = pkm.block0.pid % 6;

	for(int i=0; i<6; i++)
	{
		if(ivs[startindex].val == highval)
		{
			return lookupcharacteristic(ivs[startindex].id,highval,langid);
		}
		startindex++;
		if(startindex ==6)
		{
			startindex = 0;
		}
	}
	return lookupcharacteristic(statid,highval,langid);
}
void setlevel(pokemon_obj &pkm, int level)
{
	if(level > 100)
	{
		level = 100;
	}
	if(level < 1)
	{
		level = 1;
	}
	std::ostringstream o;
	o << ""
		<< "SELECT experience.experience "
		<< "FROM   pokemon_species "
		<< "       INNER JOIN experience "
		<< "               ON pokemon_species.growth_rate_id = experience.growth_rate_id "
		<< "WHERE  ( pokemon_species.id = " << (int)(pkm.blocka.species) << " ) "
		<< "       AND ( experience.level = " << level << " ) "
		<< "ORDER  BY experience.experience ";
	pkm.blocka.exp = getanint(o);
}
string lookuptypename(const int type, const int langid)
{
	std::ostringstream o;
	o << ""
	<< "SELECT type_names.name "
		<< "FROM   types "
		<< "       INNER JOIN type_names "
		<< "               ON types.id = type_names.type_id "
		<< "WHERE  ( type_names.local_language_id = " << langid << " ) "
		<< "       AND ( types.id = " << (type+1) << " ) ";
	return getastring(o);
}
int lookuppkmtype(const int species, const int form, const int slot, const int langid)
{
	std::ostringstream o;
	o << ""
	<< "SELECT pokemon_types.type_id "
<< "FROM   pokemon_types "
<< "       INNER JOIN pokemon_forms "
<< "               ON pokemon_types.pokemon_id = pokemon_forms.pokemon_id "
<< "       INNER JOIN pokemon "
<< "               ON pokemon_types.pokemon_id = pokemon.id "
<< "                  AND pokemon_forms.pokemon_id = pokemon.id "
<< "       INNER JOIN pokemon_species "
<< "               ON pokemon.species_id = pokemon_species.id "
<< "       INNER JOIN pokemon_species_names "
<< "               ON pokemon_species.id = pokemon_species_names.pokemon_species_id "
<< "WHERE  ( pokemon_species_names.local_language_id = " << langid << " ) "
<< "       AND ( pokemon_forms.form_order = " << form << " + 1 ) "
<< "       AND ( pokemon_species.id = " << species << " ) "
<< "       AND ( pokemon_types.slot = " << slot << " ) ";
	return getanint(o) - 1;
}
int lookuppkmtype(const pokemon_obj &pkm, const int slot, const int langid)
{
	return lookuppkmtype(pkm.blocka.species,pkm.blockb.forms.form,slot,langid);
}
string lookupmovedamagetypename(const pokemon_obj *pkm, const int movenum, const int langid)
{
	return lookupmovedamagetypename(pkm->blockb.moves[movenum],langid);
}
int getpkmexptonext(const pokemon_obj *pkm)
{
	return getpkmexptonext(pkm->blocka.species, pkm->blocka.exp);
}
int getpkmexpatcur(const pokemon_obj *pkm)
{
	return getpkmexpatcur(pkm->blocka.species,pkm->blocka.exp);
}
string getnaturename(const pokemon_obj *pkm, const int langid)
{
	if(pkm->blockb.nature == 0 && pkm->blockc.hometown != Hometowns::black && pkm->blockc.hometown != Hometowns::white)
	{
		return getnaturename(pkm->block0.pid % 25,langid);
	}
	else
	{
		return getnaturename(pkm->blockb.nature,langid);
	}
}
string lookupitemname(const pokemon_obj *pkm, const int generation, const int langid)
{
	return lookupitemname(pkm->blocka.item,generation,langid);
}
string getpkmgendername(const pokemon_obj *pkm)
{
	return getgendername(getpkmgender(pkm));
};
bool pkmhasgenddiff(const pokemon_obj *pkm)
{
	return pkmhasgenddiff(pkm->blocka.species);
}
string lookupabilityname(const pokemon_obj *pkm, const int langid)
{
	return lookupabilityname(pkm->blocka.ability, langid);
}
string getpkmformname(const pokemon_obj *pkm, const int langid)
{
	std::ostringstream o;
	o << ""
		<< "SELECT pokemon_form_names.form_name "
		<< "FROM   pokemon_forms "
		<< "       INNER JOIN pokemon_form_names "
		<< "               ON pokemon_forms.id = pokemon_form_names.pokemon_form_id "
		<< "       INNER JOIN pokemon "
		<< "               ON pokemon_forms.pokemon_id = pokemon.id "
		<< "       INNER JOIN pokemon_species "
		<< "               ON pokemon.species_id = pokemon_species.id "
		<< "       INNER JOIN pokemon_species_names "
		<< "               ON pokemon_species.id = pokemon_species_names.pokemon_species_id "
		<< "WHERE  ( pokemon_form_names.local_language_id = " << langid << " ) "
		<< "       AND ( pokemon_species_names.local_language_id = " << langid << " ) "
		<< "       AND ( pokemon.species_id = " << (uint16)pkm->blocka.species << " ) "
		<< "       AND ( pokemon_forms.form_order = " << (int)(pkm->blockb.forms.form) << " + 1 ) ";
	return getastring(o);
}
string getpkmmetlocname(const pokemon_obj *pkm, const int gen, const int langid)
{
	switch(pkm->blockd.met)
	{
	case Locations::poketransfer:
		return "Poké Transfer Lab";
		break;
	case Locations::daycarecouple:
		return "Day-Care Couple";
		break;
	default:
		return lookuplocname(pkm->blockd.met, gen, langid);
	}
}
string getpkmegglocname(const pokemon_obj *pkm, const int gen, const int langid)
{
	switch(pkm->blockd.eggmet)
	{
	case Locations::poketransfer:
		return "Poké Transfer Lab";
		break;
	case Locations::daycarecouple:
		return "Day-Care Couple";
		break;
	default:
		return lookuplocname(pkm->blockd.eggmet, gen, langid);
	}
}
string lookupitemflavortext(const pokemon_obj *pkm, const int generation, const int langid, const int versiongroup)
{
	return lookupitemflavortext(pkm->blocka.item,generation,langid,versiongroup);
}
string lookuppkmcolorname(const pokemon_obj *pkm, const int langid)
{
	return lookuppkmcolorname(pkm->blocka.species, langid);
}
int lookuppkmcolorid(const pokemon_obj *pkm)
{
	return lookuppkmcolorid(pkm->blocka.species);
}
string lookupabilityflavortext(const pokemon_obj *pkm, const int version_group, const int langid)
{
	return lookupabilityflavortext(pkm->blocka.ability,version_group,langid);
}
string lookupcharacteristic(const pokemon_obj *pkm, const int langid)
{
	int statid = 0;
	int highval = 0;
	ivtest ivbuf = {};
	vector<ivtest> ivs;
	ivbuf.id = Stat_IDs::hp;
	ivbuf.val = pkm->blockb.ivs.hp;
	ivbuf.order = 0;
	ivs.push_back(ivbuf);
	ivbuf.id = Stat_IDs::attack;
	ivbuf.val = pkm->blockb.ivs.attack;
	ivbuf.order = 1;
	ivs.push_back(ivbuf);
	ivbuf.id = Stat_IDs::defense;
	ivbuf.val = pkm->blockb.ivs.defense;
	ivbuf.order = 2;
	ivs.push_back(ivbuf);
	ivbuf.id = Stat_IDs::speed;
	ivbuf.val = pkm->blockb.ivs.speed;
	ivbuf.order = 3;
	ivs.push_back(ivbuf);
	ivbuf.id = Stat_IDs::spatk;
	ivbuf.val = pkm->blockb.ivs.spatk;
	ivbuf.order = 4;
	ivs.push_back(ivbuf);
	ivbuf.id = Stat_IDs::spdef;
	ivbuf.val = pkm->blockb.ivs.spdef;
	ivbuf.order = 5;
	ivs.push_back(ivbuf);
	std::sort(ivs.begin(),ivs.end(),compareivbyval);
	highval = ivs[0].val;
	std::sort(ivs.begin(),ivs.end(),compareivbyorder);
	int highcount = 0;
	for(int i=0;i<6;i++)
	{
		if(ivs[i].val == highval)
		{
			statid = i+1;
			highcount++;
		}
	}
	if(highcount == 1)
	{
		return lookupcharacteristic(statid,highval,langid);
	}
	int startindex = pkm->block0.pid % 6;

	for(int i=0; i<6; i++)
	{
		if(ivs[startindex].val == highval)
		{
			return lookupcharacteristic(ivs[startindex].id,highval,langid);
		}
		startindex++;
		if(startindex ==6)
		{
			startindex = 0;
		}
	}
	return lookupcharacteristic(statid,highval,langid);
}
void setlevel(pokemon_obj *pkm, int level)
{
	if(level > 100)
	{
		level = 100;
	}
	if(level < 1)
	{
		level = 1;
	}
	std::ostringstream o;
	o << ""
		<< "SELECT experience.experience "
		<< "FROM   pokemon_species "
		<< "       INNER JOIN experience "
		<< "               ON pokemon_species.growth_rate_id = experience.growth_rate_id "
		<< "WHERE  ( pokemon_species.id = " << (int)(pkm->blocka.species) << " ) "
		<< "       AND ( experience.level = " << level << " ) "
		<< "ORDER  BY experience.experience ";
	pkm->blocka.exp = getanint(o);
}
int lookuppkmtype(const pokemon_obj *pkm, const int slot, const int langid)
{
	return lookuppkmtype(pkm->blocka.species,pkm->blockb.forms.form,slot,langid);
}
int lookuppkmevolvedspecies(int speciesid)
{
	std::ostringstream o;
	o << ""
	<< "SELECT pokemon.species_id "
<< "FROM   pokemon_species "
<< "       INNER JOIN pokemon "
<< "               ON pokemon_species.id = pokemon.species_id "
<< "       INNER JOIN evolution_chains "
<< "               ON pokemon_species.evolution_chain_id = evolution_chains.id "
<< "       INNER JOIN pokemon_evolution "
<< "               ON pokemon_species.id = pokemon_evolution.evolved_species_id "
<< "WHERE  ( pokemon_species.evolves_from_species_id = " << (int)speciesid << " ) ";
	return getanint(o);
}
void pctoparty(party_pkm *ppkm, const pokemon_obj *pkm)
{
	ppkm->pkm_data = *pkm;
	ppkm->party_data.maxhp = getpkmstat(pkm,Stat_IDs::hp);
	ppkm->party_data.hp = ppkm->party_data.maxhp;
	ppkm->party_data.attack = getpkmstat(pkm,Stat_IDs::attack);
	ppkm->party_data.defense = getpkmstat(pkm,Stat_IDs::defense);
	ppkm->party_data.speed = getpkmstat(pkm,Stat_IDs::speed);
	ppkm->party_data.spatk = getpkmstat(pkm,Stat_IDs::spatk);
	ppkm->party_data.spdef = getpkmstat(pkm,Stat_IDs::spdef);
	ppkm->party_data.level = getpkmlevel(pkm);
}
void pctoparty(party_pkm &ppkm, const pokemon_obj *pkm)
{
	ppkm.pkm_data = *pkm;
	ppkm.party_data.maxhp = getpkmstat(pkm,Stat_IDs::hp);
	ppkm.party_data.hp = ppkm.party_data.maxhp;
	ppkm.party_data.attack = getpkmstat(pkm,Stat_IDs::attack);
	ppkm.party_data.defense = getpkmstat(pkm,Stat_IDs::defense);
	ppkm.party_data.speed = getpkmstat(pkm,Stat_IDs::speed);
	ppkm.party_data.spatk = getpkmstat(pkm,Stat_IDs::spatk);
	ppkm.party_data.spdef = getpkmstat(pkm,Stat_IDs::spdef);
	ppkm.party_data.level = getpkmlevel(pkm);
}
void pctoparty(party_pkm *ppkm, const pokemon_obj &pkm)
{
	ppkm->pkm_data = pkm;
	ppkm->party_data.maxhp = getpkmstat(pkm,Stat_IDs::hp);
	ppkm->party_data.hp = ppkm->party_data.maxhp;
	ppkm->party_data.attack = getpkmstat(pkm,Stat_IDs::attack);
	ppkm->party_data.defense = getpkmstat(pkm,Stat_IDs::defense);
	ppkm->party_data.speed = getpkmstat(pkm,Stat_IDs::speed);
	ppkm->party_data.spatk = getpkmstat(pkm,Stat_IDs::spatk);
	ppkm->party_data.spdef = getpkmstat(pkm,Stat_IDs::spdef);
	ppkm->party_data.level = getpkmlevel(pkm);
}
void pctoparty(party_pkm &ppkm, const pokemon_obj &pkm)
{
	ppkm.pkm_data = pkm;
	ppkm.party_data.maxhp = getpkmstat(pkm,Stat_IDs::hp);
	ppkm.party_data.hp = ppkm.party_data.maxhp;
	ppkm.party_data.attack = getpkmstat(pkm,Stat_IDs::attack);
	ppkm.party_data.defense = getpkmstat(pkm,Stat_IDs::defense);
	ppkm.party_data.speed = getpkmstat(pkm,Stat_IDs::speed);
	ppkm.party_data.spatk = getpkmstat(pkm,Stat_IDs::spatk);
	ppkm.party_data.spdef = getpkmstat(pkm,Stat_IDs::spdef);
	ppkm.party_data.level = getpkmlevel(pkm);
}



//System::String^ getitemidentifier(int generation, int itemid)
//{
//	System::String^ item;
//				 SQLiteConnection ^db = getsqlc();
//				 db->Open();
//				 
//				 SQLiteCommand ^cmdSelect = db->CreateCommand();
//				 
//				 cmdSelect->CommandText = ""
//+ "SELECT items.identifier, "
//+ "       item_game_indices.game_index "
//+ "FROM   items "
//+ "       INNER JOIN item_game_indices "
//+ "               ON items.id = item_game_indices.item_id "
//+ "WHERE  ( item_game_indices.generation_id = " + generation + " ) "
//+ "       AND ( item_game_indices.game_index = " + itemid + " ) ";
//
//				 SQLiteDataReader ^reader = cmdSelect->ExecuteReader();
//
//				 item = reader->GetValue(0)->ToString();
//				 
//				 db->Close();
//				 delete (IDisposable^)db;
//
//				 return item;
//};

//System::String^ getpkmformident(int species, int form){
//	System::String^ formname;
//
//				 SQLiteConnection ^db = getsqlc();
//				 db->Open();
//				 
//				 SQLiteCommand ^cmdSelect = db->CreateCommand();
//				 
//				 cmdSelect->CommandText = ""
//+ "SELECT pokemon_forms.form_identifier "
//+ "FROM   pokemon_forms "
//+ "       INNER JOIN pokemon "
//+ "               ON pokemon_forms.pokemon_id = pokemon.id "
//+ "WHERE  ( pokemon.species_id = " + species + " ) "
//+ "       AND ( pokemon_forms.form_order = " + form + " + 1 ) ";
//
//				 SQLiteDataReader ^reader = cmdSelect->ExecuteReader();
//
//				 formname = reader->GetValue(0)->ToString();
//				 
//
//				 db->Close();
//				 delete (IDisposable^)db;
//				 if(formname=="-"){return "";}
//				 return formname;
//};

//System::String^ getpkmformident(pokemon_obj pkm){
//return getpkmformident(pkm.blocka.species, pkm.blockb.forms.form);
//};

// TODO Country

// TODO Pokerus



// TODO Ball

//System::DateTime getpkmmetdate(pokemon_obj pkm){
//	System::DateTime metdate;
//		metdate = DateTime(pkm.blockd.metdate[0]+2000,pkm.blockd.metdate[1],pkm.blockd.metdate[2]);
//	return metdate;
//};
//System::DateTime getpkmeggdate(pokemon_obj pkm){
//System::DateTime metdate;
//	if((pkm.blockd.eggdate[0] != 0) & (pkm.blockd.eggdate[1] != 0) & (pkm.blockd.eggdate[2] != 0)){
//		metdate = DateTime(pkm.blockd.eggdate[0]+2000,pkm.blockd.eggdate[1],pkm.blockd.eggdate[2]);
//	};
//	return metdate;
//};

//// TODO Hidden Power

//Image^ estoimg(System::Object ^ obj){
//	
//		Image ^ img;
//	try {
//		// http://www.digitalcoding.com/Code-Snippets/CPP-CLI/C-CLI-Code-Snippet-Get-Image-from-sql-server.html
//        array<System::Byte> ^_ImageData = gcnew array<System::Byte>(0);
//        _ImageData = safe_cast<array<System::Byte>^>(obj);
//        System::IO::MemoryStream ^_MemoryStream = gcnew System::IO::MemoryStream(_ImageData);
//        img = System::Drawing::Image::FromStream(_MemoryStream);
//		return img;
//	}
//   catch(...){ // char * str ) {
//      //std::cout << "Exception raised: " << str << '\n';
//   }
//   return img;
//};

//	Image^ getpkmsprite(int species, int form, bool female, bool shiny)
//{
//	System::String ^ strJoin = "";
//	if (female && pkmhasgenddiff(species)) {
//					strJoin += "female_";
//	} else {
//		strJoin += "male_";
//	}
//	if (shiny) {
//		strJoin += "shiny_";
//	} else {
//		strJoin += "normal_";
//	}
//		SQLiteConnection ^ con = getsqlcimages();
//		con->Open();
//		SQLiteCommand ^ cmd = con->CreateCommand();
//		cmd->CommandText = "SELECT image FROM front_" + strJoin + "sprites WHERE pokemon_species=\"" + 
//		species.ToString() + "\"";
//		if (getpkmformident(species,form) != ""){
//					cmd->CommandText = "SELECT image FROM front_" + strJoin + "sprites WHERE pokemon_species=\"" + 
//			species.ToString() + "-" + getpkmformident(species,form) + "\"";
//		}
//		Image ^ img = estoimg(cmd->ExecuteScalar());
//		con->Close();
//		return img;
//};

//	Image^ getpkmsprite(pokemon_obj pkm){
//		return getpkmsprite(pkm.blocka.species,pkm.blockb.forms.form,getpkmgendername(pkm)=="Female",getpkmshiny(pkm));
//	}

//	Image^ getpkmicon(int species, int form, bool female)
//{
//	System::String ^ strJoin = "";
//	if (species == 521 | species == 592 | species == 593) {
//					strJoin += "female";
//	} else {
//		strJoin += "male";
//	}
//		SQLiteConnection ^ con = getsqlcimages();
//		con->Open();
//		SQLiteCommand ^ cmd = con->CreateCommand();
//		cmd->CommandText = "SELECT image FROM icons_" + strJoin + " WHERE identifier=\"" + 
//		species.ToString() + "\"";
//		if ((getpkmformident(species,form) != "") && (species != arceus)){
//					cmd->CommandText = "SELECT image FROM icons_" + strJoin + " WHERE identifier=\"" + 
//			species.ToString() + "-" + getpkmformident(species,form) + "\"";
//		}
//		Image ^ img = estoimg(cmd->ExecuteScalar());
//		con->Close();
//		return img;
//};

//	Image^ getpkmicon(pokemon_obj pkm){
//		return getpkmicon(pkm.blocka.species,pkm.blockb.forms.form,getpkmgendername(pkm)=="Female");
//	}

//	Image^ getitemimg(System::String^ identifier)
//{
//		SQLiteConnection ^ con = getsqlcimages();
//		con->Open();
//		SQLiteCommand ^ cmd = con->CreateCommand();
//		cmd->CommandText = "SELECT image FROM items WHERE identifier=\"" + 
//		identifier + "\"";
//		Image ^ img = estoimg(cmd->ExecuteScalar());
//		con->Close();
//		return img;
//};

//	Image^ getitemimg(int generation, int itemid)
//{
//	return getitemimg(getitemidentifier(generation,itemid));
//};

//		Color getpkmcolor(int colorid){
//			switch(colorid){
//			case 1:
//				return Color::Black;
//				break;
//			case 2:
//				return Color::Blue;
//				break;
//			case 3:
//				return Color::Brown;
//				break;
//			case 4:
//				return Color::Gray;
//				break;
//			case 5:
//				return Color::Green;
//				break;
//			case 6:
//				return Color::Pink;
//				break;
//			case 7:
//				return Color::Purple;
//				break;
//			case 8:
//				return Color::Red;
//				break;
//			case 9:
//				return Color::White;
//				break;
//			case 10:
//				return Color::Yellow;
//				break;
//			default:
//				return Color::Black;
//				break;
//			};
//		};
//
//		Color getpkmcolor(pokemon_obj pkm)
//		{
//			return getpkmcolor(getpkmcolorid(pkm));
//		};

//void MarshalString ( String ^ s, string& os ) {
//   using namespace Runtime::InteropServices;
//   const char* chars = 
//      (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
//   os = chars;
//   Marshal::FreeHGlobal(IntPtr((void*)chars));
//}

//void MarshalString ( String ^ s, wstring& os ) {
//   using namespace Runtime::InteropServices;
//   const wchar_t* chars = 
//      (const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
//   os = chars;
//   Marshal::FreeHGlobal(IntPtr((void*)chars));
//}

//void read(System::String ^ file_name, pokemon_obj& data) // Writes the given file and assigns the data to the given Pokemon object.
//{
//	std::ifstream in;
//	std::string fname;
//	MarshalString(file_name,fname);
//	in.open(fname,std::ios::binary);
//  in.read(reinterpret_cast<char*>(&data), sizeof(pokemon_obj));
//  in.close();
//};

//void read(System::String ^ file_name, bw2sav_obj& data) // Writes the given file and assigns the data to the given Pokemon object.
//{
//	std::ifstream in;
//	std::string fname;
//	MarshalString(file_name,fname);
//	in.open(fname,std::ios::binary);
//  in.read(reinterpret_cast<char*>(&data), sizeof(bw2sav_obj));
//  in.close();
//};

// TODO: Figure out better way to deal with database files - http://stackoverflow.com/questions/1528298/get-path-of-executable

//String^ getcurdir(){
//return Directory::GetCurrentDirectory();
//};
//
//SQLiteConnection ^getsqlc(){
//				 SQLiteConnection ^db = gcnew SQLiteConnection();
//				 db->ConnectionString = "Data Source=" + getcurdir() + "\\veekun-pokedex.sqlite";
//
//				 return db;
//};
//
//SQLiteConnection ^getsqlcimages(){
//				 SQLiteConnection ^db = gcnew SQLiteConnection();
//				 db->ConnectionString = "Data Source=" + getcurdir() + "\\images.sqlite";
//
//				 return db;
//};

//SQLiteConnection ^db = getsqlc();
//db->Open();
//
//SQLiteCommand ^cmdSelect = db->CreateCommand();
//
//cmdSelect->CommandText = ""

//SQLiteDataReader ^reader = cmdSelect->ExecuteReader();

//blahblah = Convert::ToInt16(reader->GetValue(0));
//reader->GetValue(0)->ToString();


//db->Close();
//delete (IDisposable^)db;

//return blahblah;

//int testlodepng()
//{
//	const void * blob;
//	std::ostringstream o;
//
//	//	Image^ getpkmsprite(int species, int form, bool female, bool shiny)
//	//{
//	//		SQLiteConnection ^ con = getsqlcimages();
//	//		con->Open();
//	//		SQLiteCommand ^ cmd = con->CreateCommand();
//	//		cmd->CommandText = "SELECT image FROM front_" + strJoin + "sprites WHERE pokemon_species=\"" + 
//	//		species.ToString() + "\"";
//	//		if (getpkmformident(species,form) != ""){
//	//					cmd->CommandText = "SELECT image FROM front_" + strJoin + "sprites WHERE pokemon_species=\"" + 
//	//			species.ToString() + "-" + getpkmformident(species,form) + "\"";
//	//		}
//	//		Image ^ img = estoimg(cmd->ExecuteScalar());
//	//		con->Close();
//	//		return img;
//	//};
//
//	pkmspecies species = charmander;
//	int form = 0;
//	bool female = false, shiny = false;
//
//		string strJoin = "";
///*		if (female && pkmhasgenddiff(species)) {
//						strJoin += "female_";
//		} else*/ if(female) {
//			strJoin += "male_";
//		}
//		if (shiny) {
//			strJoin += "shiny_";
//		} else {
//			strJoin += "normal_";
//		}
//
//
//	o << "SELECT image FROM front_" << strJoin << "sprites WHERE pokemon_species=\"" << lookuppkmname(species) << "\""; 
//
//	blob = getablob(o);
//
//	//std::vector<unsigned char>& out, unsigned& w, unsigned& h,
// //               State& state,
// //               const unsigned char* in, size_t insize
//
//	vector<unsigned char>& out;
//		unsigned& w = 100;
//		unsigned& h = 100;
//		State& state;
//		const unsigned char* in;
//		size_t insize;
//
//	lodepng::decode(out,w,h,in,insize);
//
//	return 0;
//}

//Image^ estoimg(System::Object ^ obj){
//	
//		Image ^ img;
//	try {
//		// http://www.digitalcoding.com/Code-Snippets/CPP-CLI/C-CLI-Code-Snippet-Get-Image-from-sql-server.html
//        array<System::Byte> ^_ImageData = gcnew array<System::Byte>(0);
//        _ImageData = safe_cast<array<System::Byte>^>(obj);
//        System::IO::MemoryStream ^_MemoryStream = gcnew System::IO::MemoryStream(_ImageData);
//        img = System::Drawing::Image::FromStream(_MemoryStream);
//		return img;
//	}
//   catch(...){ // char * str ) {
//      //std::cout << "Exception raised: " << str << '\n';
//   }
//   return img;
//};
//
//	Image^ getpkmsprite(int species, int form, bool female, bool shiny)
//{
//	System::String ^ strJoin = "";
//	if (female && pkmhasgenddiff(species)) {
//					strJoin += "female_";
//	} else {
//		strJoin += "male_";
//	}
//	if (shiny) {
//		strJoin += "shiny_";
//	} else {
//		strJoin += "normal_";
//	}
//		SQLiteConnection ^ con = getsqlcimages();
//		con->Open();
//		SQLiteCommand ^ cmd = con->CreateCommand();
//		cmd->CommandText = "SELECT image FROM front_" + strJoin + "sprites WHERE pokemon_species=\"" + 
//		species.ToString() + "\"";
//		if (getpkmformident(species,form) != ""){
//					cmd->CommandText = "SELECT image FROM front_" + strJoin + "sprites WHERE pokemon_species=\"" + 
//			species.ToString() + "-" + getpkmformident(species,form) + "\"";
//		}
//		Image ^ img = estoimg(cmd->ExecuteScalar());
//		con->Close();
//		return img;
//};
//
//	Image^ getpkmsprite(pokemon_obj pkm){
//		return getpkmsprite(pkm.blocka.species,pkm.blockb.forms.form,getpkmgendername(pkm)=="Female",getpkmshiny(pkm));
//	}
//
