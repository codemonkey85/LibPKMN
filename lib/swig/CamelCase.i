/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * This file is used to rename functions and classes for C# and Java, whose
 * main naming convention is CamelCase/lowerCamelCase.
 */

/*
 * pkmn/io.hpp
 */
%rename(exportToPKM)   pkmn::export_to_pkm;
%rename(importFromPKM) pkmn::import_from_pkm;
%rename(exportToPKX)   pkmn::export_to_pkx;
%rename(importFromPKX) pkmn::import_from_pkx;

/*
 * pkmn/lists.hpp
 */
%rename(getGameList)      pkmn::get_game_list;
%rename(getGameGroupList) pkmn::get_game_group_list;
%rename(getItemList)      pkmn::get_item_list;
%rename(getPokemonList)   pkmn::get_pokemon_list;
%rename(getTypeList)      pkmn::get_type_list;
%rename(getAbilityList)   pkmn::get_ability_list;
%rename(getNatureList)    pkmn::get_nature_list;
%rename(getPokemonOfType) pkmn::get_pokemon_of_type;

/*
 * pkmn/paths.hpp
 */
%rename(getAppdataPath)  pkmn::get_appdata_path;
%rename(getDatabasePath) pkmn::get_database_path;
%rename(getImagesDir)    pkmn::get_images_dir;
%rename(getTmpDir)       pkmn::get_tmp_dir;

/*
 * pkmn/analysis/damage.hpp
 */
%rename(getTypeDamageMod)   pkmn::analysis::get_type_damage_mod;
%rename(getBaseDamage)      pkmn::analysis::get_base_damage;
%rename(getDamageRange)     pkmn::analysis::get_damage_range;
%rename(getMinPossibleStat) pkmn::analysis::get_min_possible_stat;
%rename(getMaxPossibleStat) pkmn::analysis::get_max_possible_stat;
%rename(isStatPossible)     pkmn::analysis::is_stat_possible;
%rename(getStatRange)       pkmn::analysis::get_stat_range;

/*
 * pkmn/database/lists.hpp
 */
%rename(getTableList)  pkmn::get_table_list;
%rename(getColumnList) pkmn::get_column_list;

/*
 * pkmn/database/queries.hpp
 */
%rename(toDatabaseFormat)    pkmn::database::to_database_format;
%rename(getAbilityID)        pkmn::database::get_ability_id;
%rename(getAbilityName)      pkmn::database::get_ability_name;
%rename(getDamageClass)      pkmn::database::get_damage_class;
%rename(getEggGroupID)       pkmn::database::get_egg_group_id;
%rename(getEggGroupName)     pkmn::database::get_egg_group_name;
%rename(getGameID)           pkmn::database::get_game_id;
%rename(getGameName)         pkmn::database::get_game_name;
%rename(getGeneration)       pkmn::database::get_generation;
%rename(getItemIndex)        pkmn::database::get_item_index;
%rename(getItemDescription)  pkmn::database::get_item_description;
%rename(getItemID)           pkmn::database::get_item_id;
%rename(getItemName)         pkmn::database::get_item_name;
%rename(getItemCategory)     pkmn::database::get_item_category;
%rename(getMoveDescription)  pkmn::database::get_move_description;
%rename(getMoveID)           pkmn::database::get_move_id;
%rename(getMoveName)         pkmn::database::get_move_name;
%rename(getMovePP)           pkmn::database::get_move_pp;
%rename(getNatureStatEffect) pkmn::database::get_nature_stat_effect;
%rename(getNatureID)         pkmn::database::get_nature_id;
%rename(getNatureName)       pkmn::database::get_nature_name;
%rename(getPokedexEntry)     pkmn::database::get_pokedex_entry;
%rename(getSpeciesID)        pkmn::database::get_species_id;
%rename(getSpeciesName)      pkmn::database::get_species_name;
%rename(getTypeID)           pkmn::database::get_type_id;
%rename(getTypeName)         pkmn::database::get_type_name;
%rename(getVersionGroup)     pkmn::database::get_version_group;

/*
 * pkmn/types/*.hpp
 */
%rename(PRNG)           pkmn::prng;
%rename(StringIntDict)  pkmn::dict<std::string, int>;
%rename(StringUintDict) pkmn::dict<std::string, unsigned int>;
%rename(UIntUIntDict)   pkmn::dict<unsigned int, unsigned int>;
%rename(PokemonText)    pkmn::pokemon_text;
%rename(UIntVLA)        pkmn::vla<unsigned int>;
%rename(BasePokemonVLA) pkmn::vla<pkmn::base_pokemon::sptr>;
%rename(Moveset)        pkmn::vla<pkmn::move::sptr>;
%rename(PokemonTeam)    pkmn::vla<pkmn::team_pokemon::sptr>;

/*
 * Vectors
 */
%rename(BasePokemonVector) std::vector<pkmn::base_pokemon::sptr>;
%rename(MoveVector)        std::vector<pkmn::move::sptr>;
%rename(TeamPokemonVector) std::vector<pkmn::team_pokemon::sptr>;
