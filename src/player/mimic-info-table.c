﻿#include "angband.h"
#include "mimic-info-table.h"

/*!
 * @brief 変身種族情報
 */
const player_race mimic_info[MAX_MIMIC_FORMS] =
{
	{
#ifdef JP
		"[標準形態]",
#endif
		"Default",

		{  0,  0,  0,  0,  0,  0 },
		0,  0,  0,  0,  0,  10,  0,  0,
		10,  100,
		0,  0,
		0,  0, 0, 0,
		0,  0, 0, 0,
		0,
		0x000000,
	},
	{
#ifdef JP
		"[悪魔]",
#endif
		"[Demon]",

		{  5,  3,  2,  3,  4,  -6 },
		-5,  18, 20, -2,  3,  10, 40, 20,
		12,  0,
		0,  0,
		0,  0, 0, 0,
		0,  0, 0, 0,
		5,
		0x000003,
	},
	{
#ifdef JP
		"[魔王]",
#endif
		"[Demon lord]",

		{  20,  20,  20,  20,  20,  20 },
		20,  20, 25, -2,  3,  10, 70, 40,
		14,  0,
		0,  0,
		0,  0, 0, 0,
		0,  0, 0, 0,
		20,
		0x000003,
	},
	{
#ifdef JP
		"[吸血鬼]",
#endif
		"[Vampire]",

		{ 4, 4, 1, 1, 2, 3 },
		6, 12, 8, 6, 2, 12, 30, 20,
		11,  0,
		0,  0,
		0,  0, 0, 0,
		0,  0, 0, 0,
		5,
		0x000005,
	},
};

