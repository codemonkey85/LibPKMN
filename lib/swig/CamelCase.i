/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * This file is used to rename functions and classes for C# and Java, whose
 * main naming convention is CamelCase/lowerCamelCase.
 */

/*
 * pkmnsim/io.hpp
 */
%rename(exportToPKM)   pkmnsim::export_to_pkm;
%rename(importFromPKM) pkmnsim::import_from_pkm;
%rename(exportToPKX)   pkmnsim::export_to_pkx;
%rename(importFromPKX) pkmnsim::import_from_pkx;

/*
 * pkmnsim/lists.hpp
 */
%rename(getGameList)      pkmnsim::get_game_list;
%rename(getGameGroupList) pkmnsim::get_game_group_list;
%rename(getItemList)      pkmnsim::get_item_list;
%rename(getPokemonList)   pkmnsim::get_pokemon_list;
%rename(getTypeList)      pkmnsim::get_type_list;
%rename(getAbilityList)   pkmnsim::get_ability_list;
%rename(getNatureList)    pkmnsim::get_nature_list;
%rename(getPokemonOfType) pkmnsim::get_pokemon_of_type;

/*
 * pkmnsim/paths.hpp
 */
%rename(getAppdataPath)  pkmnsim::get_appdata_path;
%rename(getDatabasePath) pkmnsim::get_database_path;
%rename(getImagesDir)    pkmnsim::get_images_dir;
%rename(getTmpDir)       pkmnsim::get_tmp_dir;

/*
 * pkmnsim/analysis/damage.hpp
 */
%rename(getTypeDamageMod)   pkmnsim::analysis::get_type_damage_mod;
%rename(getBaseDamage)      pkmnsim::analysis::get_base_damage;
%rename(getDamageRange)     pkmnsim::analysis::get_damage_range;
%rename(getMinPossibleStat) pkmnsim::analysis::get_min_possible_stat;
%rename(getMaxPossibleStat) pkmnsim::analysis::get_max_possible_stat;
%rename(isStatPossible)     pkmnsim::analysis::is_stat_possible;
%rename(getStatRange)       pkmnsim::analysis::get_stat_range;

/*
 * pkmnsim/database/lists.hpp
 */
%rename(getTableList)  pkmnsim::get_table_list;
%rename(getColumnList) pkmnsim::get_column_list;

/*
 * pkmnsim/database/queries.hpp
 */
%rename(toDatabaseFormat)    pkmnsim::database::to_database_format;
%rename(getAbilityID)        pkmnsim::database::get_ability_id;
%rename(getAbilityName)      pkmnsim::database::get_ability_name;
%rename(getDamageClass)      pkmnsim::database::get_damage_class;
%rename(getEggGroupID)       pkmnsim::database::get_egg_group_id;
%rename(getEggGroupName)     pkmnsim::database::get_egg_group_name;
%rename(getGameID)           pkmnsim::database::get_game_id;
%rename(getGameName)         pkmnsim::database::get_game_name;
%rename(getGeneration)       pkmnsim::database::get_generation;
%rename(getItemIndex)        pkmnsim::database::get_item_index;
%rename(getItemDescription)  pkmnsim::database::get_item_description;
%rename(getItemID)           pkmnsim::database::get_item_id;
%rename(getItemName)         pkmnsim::database::get_item_name;
%rename(getItemCategory)     pkmnsim::database::get_item_category;
%rename(getMoveDescription)  pkmnsim::database::get_move_description;
%rename(getMoveID)           pkmnsim::database::get_move_id;
%rename(getMoveName)         pkmnsim::database::get_move_name;
%rename(getNatureStatEffect) pkmnsim::database::get_nature_stat_effect;
%rename(getNatureID)         pkmnsim::database::get_nature_id;
%rename(getNatureName)       pkmnsim::database::get_nature_name;
%rename(getPokedexEntry)     pkmnsim::database::get_pokedex_entry;
%rename(getSpeciesID)        pkmnsim::database::get_species_id;
%rename(getSpeciesName)      pkmnsim::database::get_species_name;
%rename(getTypeID)           pkmnsim::database::get_type_id;
%rename(getTypeName)         pkmnsim::database::get_type_name;
%rename(getVersionGroup)     pkmnsim::database::get_version_group;

/*
 * pkmnsim/types/*.hpp
 */
%rename(PRNG)           pkmnsim::prng;
%rename(StringIntDict)  pkmnsim::dict<std::string, int>;
%rename(StringUintDict) pkmnsim::dict<std::string, unsigned int>;
%rename(UIntUIntDict)   pkmnsim::dict<unsigned int, unsigned int>;
%rename(PokemonText)    pkmnsim::pokemon_text;
%rename(UIntVLA)        pkmnsim::vla<unsigned int>;
%rename(BasePokemonVLA) pkmnsim::vla<pkmnsim::base_pokemon::sptr>;
%rename(Moveset)        pkmnsim::vla<pkmnsim::move::sptr>;
%rename(PokemonTeam)    pkmnsim::vla<pkmnsim::team_pokemon::sptr>;

/*
 * Vectors
 */
%rename(BasePokemonVector) std::vector<pkmnsim::base_pokemon::sptr>;
%rename(MoveVector)        std::vector<pkmnsim::move::sptr>;
%rename(TeamPokemonVector) std::vector<pkmnsim::team_pokemon::sptr>;