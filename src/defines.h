﻿#pragma once

/*!
 *  @file defines.h
 *  @brief 主要なマクロ定義ヘッダ / Purpose: global constants and macro definitions
 *  @date 2014/01/02
 *  @author
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke\n
 *\n
 * This software may be copied and distributed for educational, research,\n
 * and not for profit purposes provided that this copyright and statement\n
 * are included in all such copies.  Other copyrights may also apply.\n
 *  @details
 * Do not edit this file unless you know *exactly* what you are doing.\n
 *\n
 * Some of the values in this file were chosen to preserve game balance,\n
 * while others are hard-coded based on the format of old save-files, the\n
 * definition of arrays in various places, mathematical properties, fast\n
 * computation, storage limits, or the format of external text files.\n
 *\n
 * Changing some of these values will induce crashes or memory errors or\n
 * savefile mis-reads.  Most of the comments in this file are meant as\n
 * reminders, not complete descriptions, and even a complete knowledge\n
 * of the source may not be sufficient to fully understand the effects\n
 * of changing certain definitions.\n
 *\n
 * Lastly, note that the code does not always use the symbolic constants\n
 * below, and sometimes uses various hard-coded values that may not even\n
 * be defined in this file, but which may be related to definitions here.\n
 * This is of course bad programming practice, but nobody is perfect...\n
 *\n
 * For example, there are MANY things that depend on the screen being\n
 * 80x24, with the top line used for messages, the bottom line being\n
 * used for status, and exactly 22 lines used to show the dungeon.\n
 * Just because your screen can hold 46 lines does not mean that the\n
 * game will work if you try to use 44 lines to show the dungeon.\n
 *\n
 * You have been warned.\n
 */

/*
 * Arena constants
 */
#define ARENA_DEFEATED_OLD_VER (-(MAX_SHORT)) /*<! 旧バージョンの闘技場敗北定義 */

/*!
 * @brief 視界及び光源の過渡処理配列サイズ / Maximum size of the "temp" array (see "current_floor_ptr->grid_array.c")
 * @details We must be as large as "VIEW_MAX" and "LITE_MAX" for proper functioning
 * of "update_view()" and "update_lite()".  We must also be as large as the
 * largest illuminatable room, but no room is larger than 800 grids.  We
 * must also be large enough to allow "good enough" use as a circular queue,
 * to calculate monster flow, but note that the flow code is "paranoid".
 */
#define TEMP_MAX 2298

/*!
 * @brief 再描画処理用配列サイズ / Maximum size of the "redraw" array (see "current_floor_ptr->grid_array.c")
 * @details We must be large for proper functioning of delayed redrawing.
 * We must also be as large as two times of the largest view area.
 * Note that maximum view grids are 1149 entries.
 */
#define REDRAW_MAX 2298


/*!
 * @brief マクロ登録の最大数 / Maximum number of macros (see "io.c")
 * @note Default: assume at most 256 macros are used
 */
#define MACRO_MAX       256

/*!
 * @brief 銘情報の最大数 / Maximum number of "quarks" (see "io.c")
 * @note 
 * Default: assume at most 512 different inscriptions are used<br>
 * Was 512... 256 quarks added for random artifacts<br>
 */
#define QUARK_MAX       768

/*
 * OPTION: Maximum number of messages to remember (see "io.c")
 * Default: assume maximal memorization of 2048 total messages
 */
#define MESSAGE_MAX  81920

/*
 * OPTION: Maximum space for the message text buffer (see "io.c")
 * Default: assume that each of the 2048 messages is repeated an
 * average of three times, and has an average length of 48
 */
#define MESSAGE_BUF 655360


/*** Screen Locations ***/




#define OBJ_GOLD_LIST   480     /* First "gold" entry */
#define MAX_GOLD        18      /* Number of "gold" entries */

/*
 * Object flags
 *
 * Old variables for object flags such as flags1, flags2, and flags3
 * are obsolated.  Now single array flgs[TR_FLAG_SIZE] contains all
 * object flags.  And each flag is refered by single index number
 * instead of a bit mask.
 *
 * Therefore it's very easy to add a lot of new flags; no one need to
 * worry about in which variable a new flag should be put, nor to
 * modify a huge number of files all over the source directory at once
 * to add new flag variables such as flags4, a_ability_flags1, etc...
 *
 * All management of flags is now treated using a set of macros
 * instead of bit operations.
 * Note: These macros are using division, modulo, and bit shift
 * operations, and it seems that these operations are rather slower
 * than original bit operation.  But since index numbers are almost
 * always given as constant, such slow operations are performed in the
 * compile time.  So there is no problem on the speed.
 *
 * Exceptions of new flag management is a set of flags to control
 * object generation and the curse flags.  These are not yet rewritten
 * in new index form; maybe these have no merit of rewriting.
 */

#define have_flag(ARRAY, INDEX) !!((ARRAY)[(INDEX)/32] & (1L << ((INDEX)%32)))
#define add_flag(ARRAY, INDEX) ((ARRAY)[(INDEX)/32] |= (1L << ((INDEX)%32)))
#define remove_flag(ARRAY, INDEX) ((ARRAY)[(INDEX)/32] &= ~(1L << ((INDEX)%32)))
#define is_pval_flag(INDEX) ((TR_STR <= (INDEX) && (INDEX) <= TR_MAGIC_MASTERY) || (TR_STEALTH <= (INDEX) && (INDEX) <= TR_BLOWS))
#define have_pval_flags(ARRAY) !!((ARRAY)[0] & (0x00003f7f))

/*
 * Is the monster seen by the player?
 */
#define is_seen(A) \
	((bool)((A)->ml && (!ignore_unview || p_ptr->inside_battle || \
	 (player_can_see_bold((A)->fy, (A)->fx) && projectable(p_ptr->y, p_ptr->x, (A)->fy, (A)->fx)))))


#define EATER_EXT 36
#define EATER_CHARGE 0x10000L
#define EATER_ROD_CHARGE 0x10L



/* Maximum "Nazguls" number */
#define MAX_NAZGUL_NUM 5

#define VIRTUE_LARGE 1
#define VIRTUE_SMALL 2


#define DUNGEON_FEAT_PROB_NUM 3

#define MTIMED_CSLEEP   0 /* Monster is sleeping */
#define MTIMED_FAST     1 /* Monster is temporarily fast */
#define MTIMED_SLOW     2 /* Monster is temporarily slow */
#define MTIMED_STUNNED  3 /* Monster is stunned */
#define MTIMED_CONFUSED 4 /* Monster is confused */
#define MTIMED_MONFEAR  5 /* Monster is afraid */
#define MTIMED_INVULNER 6 /* Monster is temporarily invulnerable */

#define MAX_MTIMED      7

#define MON_CSLEEP(M_PTR)   ((M_PTR)->mtimed[MTIMED_CSLEEP])
#define MON_FAST(M_PTR)     ((M_PTR)->mtimed[MTIMED_FAST])
#define MON_SLOW(M_PTR)     ((M_PTR)->mtimed[MTIMED_SLOW])
#define MON_STUNNED(M_PTR)  ((M_PTR)->mtimed[MTIMED_STUNNED])
#define MON_CONFUSED(M_PTR) ((M_PTR)->mtimed[MTIMED_CONFUSED])
#define MON_MONFEAR(M_PTR)  ((M_PTR)->mtimed[MTIMED_MONFEAR])
#define MON_INVULNER(M_PTR) ((M_PTR)->mtimed[MTIMED_INVULNER])

/*
 * For travel command (auto run)
 */
#define TRAVEL

#define CONCENT_RADAR_THRESHOLD 2
#define CONCENT_TELE_THRESHOLD  5

/*
  Language selection macro
*/
#ifdef JP
#define _(JAPANESE,ENGLISH) (JAPANESE)
#else
#define _(JAPANESE,ENGLISH) (ENGLISH)
#endif

/* Lite flag macro */
#define have_lite_flag(ARRAY) \
	(have_flag(ARRAY, TR_LITE_1) || have_flag(ARRAY, TR_LITE_2) || have_flag(ARRAY, TR_LITE_3))

#define have_dark_flag(ARRAY) \
	(have_flag(ARRAY, TR_LITE_M1) || have_flag(ARRAY, TR_LITE_M2) || have_flag(ARRAY, TR_LITE_M3))


#define COMMAND_ARG_REST_UNTIL_DONE -2   /*!<休憩コマンド引数 … 必要な分だけ回復 */
#define COMMAND_ARG_REST_FULL_HEALING -1 /*!<休憩コマンド引数 … HPとMPが全回復するまで */

