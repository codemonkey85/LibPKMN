#include <stdlib.h>
#include "stat.h"

const uint32_t STAT_TOTAL_EXP_ERRATIC[100] = { 0, 15, 52, 122, 237, 406, 637,
		942, 1326, 1800, 2369, 3041, 3822, 4719, 5737, 6881, 8155, 9564, 11111,
		12800, 14632, 16610, 18737, 21012, 23437, 26012, 28737, 31610, 34632,
		37800, 41111, 44564, 48155, 51881, 55737, 59719, 63822, 68041, 72369,
		76800, 81326, 85942, 90637, 95406, 100237, 105122, 110052, 115015,
		120001, 125000, 131324, 137795, 144410, 151165, 158056, 165079, 172229,
		179503, 186894, 194400, 202013, 209728, 217540, 225443, 233431, 241496,
		249633, 257834, 267406, 276458, 286328, 296358, 305767, 316074, 326531,
		336255, 346965, 357812, 367807, 378880, 390077, 400293, 411686, 423190,
		433572, 445239, 457001, 467489, 479378, 491346, 501878, 513934, 526049,
		536557, 548720, 560922, 571333, 583539, 591882, 600000 };

const uint32_t STAT_TOTAL_EXP_FAST[100] = { 0, 6, 21, 51, 100, 172, 274, 409,
		583, 800, 1064, 1382, 1757, 2195, 2700, 3276, 3930, 4665, 5487, 6400,
		7408, 8518, 9733, 11059, 12500, 14060, 15746, 17561, 19511, 21600,
		23832, 26214, 28749, 31443, 34300, 37324, 40522, 43897, 47455, 51200,
		55136, 59270, 63605, 68147, 72900, 77868, 83058, 88473, 94119, 100000,
		106120, 112486, 119101, 125971, 133100, 140492, 148154, 156089, 164303,
		172800, 181584, 190662, 200037, 209715, 219700, 229996, 240610, 251545,
		262807, 274400, 286328, 298598, 311213, 324179, 337500, 351180, 365226,
		379641, 394431, 409600, 425152, 441094, 457429, 474163, 491300, 508844,
		526802, 545177, 563975, 583200, 602856, 622950, 643485, 664467, 685900,
		707788, 730138, 752953, 776239, 800000 };

const uint32_t STAT_TOTAL_EXP_MEDIUM_FAST[100] = { 0, 8, 27, 64, 125, 216, 343,
		512, 729, 1000, 1331, 1728, 2197, 2744, 3375, 4096, 4913, 5832, 6859,
		8000, 9261, 10648, 12167, 13824, 15625, 17576, 19683, 21952, 24389,
		27000, 29791, 32768, 35937, 39304, 42875, 46656, 50653, 54872, 59319,
		64000, 68921, 74088, 79507, 85184, 91125, 97336, 103823, 110592, 117649,
		125000, 132651, 140608, 148877, 157464, 166375, 175616, 185193, 195112,
		205379, 216000, 226981, 238328, 250047, 262144, 274625, 287496, 300763,
		314432, 328509, 343000, 357911, 373248, 389017, 405224, 421875, 438976,
		456533, 474552, 493039, 512000, 531441, 551368, 571787, 592704, 614125,
		636056, 658503, 681472, 704969, 729000, 753571, 778688, 804357, 830584,
		857375, 884736, 912673, 941192, 970299, 1000000 };

const uint32_t STAT_TOTAL_EXP_MEDIUM_SLOW[100] = { 0, 9, 57, 96, 135, 179, 236,
		314, 419, 560, 742, 973, 1261, 1612, 2035, 2535, 3120, 3798, 4575, 5460,
		6458, 7577, 8825, 10208, 11735, 13411, 15244, 17242, 19411, 21760,
		24294, 27021, 29949, 33084, 36435, 40007, 43808, 47846, 52127, 56660,
		61450, 66505, 71833, 77440, 83335, 89523, 96012, 102810, 109923, 117360,
		125126, 133229, 141677, 150476, 159635, 169159, 179056, 189334, 199999,
		211060, 222522, 234393, 246681, 259392, 272535, 286115, 300140, 314618,
		329555, 344960, 360838, 377197, 394045, 411388, 429235, 447591, 466464,
		485862, 505791, 526260, 547274, 568841, 590969, 613664, 636935, 660787,
		685228, 710266, 735907, 762160, 789030, 816525, 844653, 873420, 902835,
		932903, 963632, 995030, 1027103, 1059860 };

const uint32_t STAT_TOTAL_EXP_SLOW[100] = { 0, 10, 33, 80, 156, 270, 428, 640,
		911, 1250, 1663, 2160, 2746, 3430, 4218, 5120, 6141, 7290, 8573, 10000,
		11576, 13310, 15208, 17280, 19531, 21970, 24603, 27440, 30486, 33750,
		37238, 40960, 44921, 49130, 53593, 58320, 63316, 68590, 74148, 80000,
		86151, 92610, 99383, 106480, 113906, 121670, 129778, 138240, 147061,
		156250, 165813, 175760, 186096, 196830, 207968, 219520, 231491, 243890,
		256723, 270000, 283726, 297910, 312558, 327680, 343281, 359370, 375953,
		393040, 410636, 428750, 447388, 466560, 486271, 506530, 527343, 548720,
		570666, 593190, 616298, 640000, 664301, 689210, 714733, 740880, 767656,
		795070, 823128, 851840, 881211, 911250, 941963, 973360, 1005446,
		1038230, 1071718, 1105920, 1140841, 1176490, 1212873, 1250000 };

const uint32_t STAT_TOTAL_EXP_FLUCTUATING[100] = { 0, 4, 13, 32, 65, 112, 178,
		276, 393, 540, 745, 967, 1230, 1591, 1957, 2457, 3046, 3732, 4526, 5440,
		6482, 7666, 9003, 10506, 12187, 14060, 16140, 18439, 20974, 23760,
		26811, 30146, 33780, 37731, 42017, 46656, 50653, 55969, 60505, 66560,
		71677, 78533, 84277, 91998, 98415, 107069, 114205, 123863, 131766,
		142500, 151222, 163105, 172697, 185807, 196322, 210739, 222231, 238036,
		250562, 267840, 281456, 300293, 315059, 335544, 351520, 373744, 390991,
		415050, 433631, 459620, 479600, 507617, 529063, 559209, 582187, 614566,
		639146, 673863, 700115, 737280, 765275, 804997, 834809, 877201, 908905,
		954084, 987754, 1035837, 1071552, 1122660, 1160499, 1214753, 1254796,
		1312322, 1354652, 1415577, 1460276, 1524731, 1571884, 1640000 };

static const stat_bonus_t STAT_NATURE_BONUS[25][6] = {
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE},//0
	{STAT_BONUS_NONE, STAT_BONUS_POSITIVE, STAT_BONUS_NEGATIVE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE},//1
	{STAT_BONUS_NONE, STAT_BONUS_POSITIVE, STAT_BONUS_NONE, STAT_BONUS_NEGATIVE, STAT_BONUS_NONE, STAT_BONUS_NONE},//2
	{STAT_BONUS_NONE, STAT_BONUS_POSITIVE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NEGATIVE, STAT_BONUS_NONE},//3
	{STAT_BONUS_NONE, STAT_BONUS_POSITIVE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NEGATIVE},//4
	{STAT_BONUS_NONE, STAT_BONUS_NEGATIVE, STAT_BONUS_POSITIVE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE},//5
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE},//6
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_POSITIVE, STAT_BONUS_NEGATIVE, STAT_BONUS_NONE, STAT_BONUS_NONE},//7
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_POSITIVE, STAT_BONUS_NONE, STAT_BONUS_NEGATIVE, STAT_BONUS_NONE},//8
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_POSITIVE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NEGATIVE},//9
	{STAT_BONUS_NONE, STAT_BONUS_NEGATIVE, STAT_BONUS_NONE, STAT_BONUS_POSITIVE, STAT_BONUS_NONE, STAT_BONUS_NONE},//10
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NEGATIVE, STAT_BONUS_POSITIVE, STAT_BONUS_NONE, STAT_BONUS_NONE},//11
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE},//12
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_POSITIVE, STAT_BONUS_NEGATIVE, STAT_BONUS_NONE},//13
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_POSITIVE, STAT_BONUS_NONE, STAT_BONUS_NEGATIVE},//14 +Spd -SpDef
	{STAT_BONUS_NONE, STAT_BONUS_NEGATIVE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_POSITIVE, STAT_BONUS_NONE},//15
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NEGATIVE, STAT_BONUS_NONE, STAT_BONUS_POSITIVE, STAT_BONUS_NONE},//16
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NEGATIVE, STAT_BONUS_POSITIVE, STAT_BONUS_NONE},//17
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE},//18
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_POSITIVE, STAT_BONUS_NEGATIVE},//19
	{STAT_BONUS_NONE, STAT_BONUS_NEGATIVE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_POSITIVE},//20
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NEGATIVE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_POSITIVE},//21
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NEGATIVE, STAT_BONUS_NONE, STAT_BONUS_POSITIVE},//22
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NEGATIVE, STAT_BONUS_POSITIVE},//23
	{STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE, STAT_BONUS_NONE},//24
};

stat_bonus_t stat_get_bonus(stat_nature_t nature, stat_stat_t stat) {
	return STAT_NATURE_BONUS[nature][stat];
}

stat_nature_t stat_get_nature(uint32_t pid) {
	return pid % 25;
}

uint8_t stat_get_level(stat_growth_rate_t growth_rate, uint32_t exp) {
	const uint32_t *exp_table;
	switch(growth_rate) {
		case STAT_GROWTH_RATE_ERRATIC:
			exp_table = STAT_TOTAL_EXP_ERRATIC;
			break;
		case STAT_GROWTH_RATE_FAST:
			exp_table = STAT_TOTAL_EXP_FAST;
			break;
		case STAT_GROWTH_RATE_MEDIUM_FAST:
			exp_table = STAT_TOTAL_EXP_MEDIUM_FAST;
			break;
		case STAT_GROWTH_RATE_MEDIUM_SLOW:
			exp_table = STAT_TOTAL_EXP_MEDIUM_SLOW;
			break;
		case STAT_GROWTH_RATE_SLOW:
			exp_table = STAT_TOTAL_EXP_SLOW;
			break;
		case STAT_GROWTH_RATE_FLUCTUATING:
			exp_table = STAT_TOTAL_EXP_FLUCTUATING;
			break;
	}
	for(size_t i = 0; i <= 100; ++i) {
		if(exp_table[i] > exp) {
			return i;
		}
	}
	return 100;
}

//calculate GB STATS
static inline uint8_t gb_calc_ev(uint16_t stat_exp) {
	if(stat_exp >= 0xfe02) {
		return 64;
	}
	uint8_t b = 0;
	while(b * b < stat_exp) {
		++b;
	}
	return b >> 2;
}

static inline uint16_t gb_calc_base_stat(uint8_t level, uint8_t base_stat, uint8_t iv, uint16_t stat_exp) {
	return ((((uint32_t)base_stat + iv) << 1) + gb_calc_ev(stat_exp)) * (uint32_t)level / 100;
}

// non-HP: (((Base + IV) * 2 + ceil(Sqrt(stat exp)) / 4) * Level) / 100 + 5
/**
 * @brief Used to calculate a non-HP stat for a pokemon from generation 1 or 2.
 * @param level The pokemon's level.
 * @param base_stat The base stat we are claculating.
 * @param iv The pokemon's iv for the given stat.
 * @param stat_exp The pokemon's stat experience.
 * @return The value for the pokemons stat.
 */
uint16_t gb_calc_stat(uint8_t level, uint8_t base_stat, uint8_t iv, uint16_t stat_exp) {
	return gb_calc_base_stat(level, base_stat, iv, stat_exp) + 5;
}

// HP: (((Base + IV) * 2 + ceil(Sqrt(stat exp)) / 4) * Level) / 100 + Level + 10
/**
 * @brief Used to calculate the HP stat for a pokemon from generation 1 or 2.
 * @param level The pokemon's level.
 * @param base_stat The base stat we are claculating.
 * @param iv The pokemon's iv for the given stat.
 * @param stat_exp The pokemon's stat experience.
 * @return The value for the pokemons stat.
 */
uint16_t gb_calc_hp_stat(uint8_t level, uint8_t base_stat, uint8_t iv, uint16_t stat_exp) {
	return gb_calc_base_stat(level, base_stat, iv, stat_exp) + level + 10;
}

static inline uint16_t gba_calc_base_stat(uint8_t level, uint8_t base_stat, uint8_t iv, uint8_t ev) {
	return (((uint32_t)base_stat << 1) + iv + (ev >> 2)) * (uint32_t)level / 100;
}

//; non-HP (((base_stat * 2) + iv + (ev / 4)) * level) / 100 + 5
/**
 * @brief Used to calculate a non-HP stat for a pokemon from generation 3.
 * @param level The pokemon's level.
 * @param base_stat The base stat we are claculating.
 * @param iv The pokemon's iv for the given stat.
 * @param ev The pokemon's effort value for the given stat.
 * @param bonus The bonus from nature for the given stat.
 * @return The value for the pokemons stat.
 */
uint16_t gba_calc_stat(uint8_t level, uint8_t base_stat, uint8_t iv, uint8_t ev, stat_bonus_t bonus) {
	uint32_t stat = gba_calc_base_stat(level, base_stat, iv, ev) + 5;
	if(bonus == STAT_BONUS_POSITIVE) {
		return stat * 110 / 100;
	}
	if(bonus == STAT_BONUS_NEGATIVE) {
		return stat * 90 / 100;
	}
	return stat;
}

//; HP (((base_stat * 2) + iv + (ev / 4)) * level) / 100 + level + 10
/**
 * @brief Used to calculate the HP stat for a pokemon from generation 3.
 * @param level The pokemon's level.
 * @param base_stat The base stat we are claculating.
 * @param iv The pokemon's iv for the given stat.
 * @param ev The pokemon's effort value for the given stat.
 * @return The value for the pokemons stat.
 */
uint16_t gba_calc_hp_stat(uint8_t level, uint8_t base_stat, uint8_t iv, uint8_t ev) {
	return gba_calc_base_stat(level, base_stat, iv, ev) + level + 10;
}

/**
 * @brief Used to calculate a non-HP stat for a pokemon from generation 4.
 * @param level The pokemon's level.
 * @param base_stat The base stat we are claculating.
 * @param iv The pokemon's iv for the given stat.
 * @param ev The pokemon's effort value for the given stat.
 * @param bonus The bonus from nature for the given stat.
 * @return The value for the pokemons stat.
 */
uint16_t nds_calc_stat(uint8_t level, uint8_t base_stat, uint8_t iv, uint8_t ev, stat_bonus_t bonus) {
	return gba_calc_stat(level, base_stat, iv, ev, bonus);
}

/**
 * @brief Used to calculate the HP stat for a pokemon from generation 4.
 * @param level The pokemon's level.
 * @param base_stat The base stat we are claculating.
 * @param iv The pokemon's iv for the given stat.
 * @param ev The pokemon's effort value for the given stat.
 * @return The value for the pokemons stat.
 */
uint16_t nds_calc_hp_stat(uint8_t level, uint8_t base_stat, uint8_t iv, uint8_t ev) {
	return gba_calc_hp_stat(level, base_stat, iv, ev);
}

/**
 * @brief Used to calculate a non-HP stat for a pokemon from generation 5.
 * @param level The pokemon's level.
 * @param base_stat The base stat we are claculating.
 * @param iv The pokemon's iv for the given stat.
 * @param ev The pokemon's effort value for the given stat.
 * @param bonus The bonus from nature for the given stat.
 * @return The value for the pokemons stat.
 */
uint16_t dsi_calc_stat(uint8_t level, uint8_t base_stat, uint8_t iv, uint8_t ev, stat_bonus_t bonus) {
	return gba_calc_stat(level, base_stat, iv, ev, bonus);
}

/**
 * @brief Used to calculate the HP stat for a pokemon from generation 5.
 * @param level The pokemon's level.
 * @param base_stat The base stat we are claculating.
 * @param iv The pokemon's iv for the given stat.
 * @param ev The pokemon's effort value for the given stat.
 * @return The value for the pokemons stat.
 */
uint16_t dsi_calc_hp_stat(uint8_t level, uint8_t base_stat, uint8_t iv, uint8_t ev) {
	return gba_calc_hp_stat(level, base_stat, iv, ev);
}
