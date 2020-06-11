﻿/*!
 * @file monster2.c
 * @brief モンスター処理 / misc code for monsters
 * @date 2014/07/08
 * @author
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.
 * 2014 Deskull rearranged comment for Doxygen.
 */

#include "monster/monster2.h"
#include "core/player-processor.h"
#include "core/speed-table.h"
#include "dungeon/dungeon.h"
#include "dungeon/quest.h"
#include "effect/effect-characteristics.h"
#include "floor/floor-object.h"
#include "floor/wild.h"
#include "io/files-util.h"
#include "main/sound-definitions-table.h"
#include "mind/drs-types.h"
#include "monster-race/monster-race-hook.h"
#include "monster-race/race-flags1.h"
#include "monster-race/race-flags2.h"
#include "monster-race/race-flags3.h"
#include "monster-race/race-flags7.h"
#include "monster-race/race-indice-types.h"
#include "monster/monster-describer.h" // todo 相互参照している.
#include "monster/monster-flag-types.h"
#include "monster/monster-generator.h"
#include "monster/monster-info.h"
#include "monster/monster-update.h"
#include "monster/monster-util.h"
#include "monster/place-monster-types.h"
#include "monster/smart-learn-types.h"
#include "mspell/summon-checker.h"
#include "object/object-flavor.h"
#include "object/object-generator.h"
#include "object/warning.h"
#include "pet/pet-fall-off.h"
#include "player/eldritch-horror.h"
#include "player/player-move.h"
#include "spell/process-effect.h"
#include "spell/spells-summon.h"
#include "spell/spells-type.h"
#include "world/world.h"

#define HORDE_NOGOOD 0x01 /*!< (未実装フラグ)HORDE生成でGOODなモンスターの生成を禁止する？ */
#define HORDE_NOEVIL 0x02 /*!< (未実装フラグ)HORDE生成でEVILなモンスターの生成を禁止する？ */

/*!
 * @brief モンスターの目標地点をセットする / Set the target of counter attack
 * @param m_ptr モンスターの参照ポインタ
 * @param y 目標y座標
 * @param x 目標x座標
 * @return なし
 */
void set_target(monster_type *m_ptr, POSITION y, POSITION x)
{
    m_ptr->target_y = y;
    m_ptr->target_x = x;
}

/*!
 * @brief モンスターの目標地点をリセットする / Reset the target of counter attack
 * @param m_ptr モンスターの参照ポインタ
 * @return なし
 */
void reset_target(monster_type *m_ptr) { set_target(m_ptr, 0, 0); }

/*!
 * todo ここには本来floor_type*を追加したいが、monster.hにfloor.hの参照を追加するとコンパイルエラーが出るので保留
 * @brief モンスター配列の空きを探す / Acquires and returns the index of a "free" monster.
 * @return 利用可能なモンスター配列の添字
 * @details
 * This routine should almost never fail, but it *can* happen.
 */
MONSTER_IDX m_pop(player_type *player_ptr)
{
    /* Normal allocation */
    floor_type *floor_ptr = player_ptr->current_floor_ptr;
    if (floor_ptr->m_max < current_world_ptr->max_m_idx) {
        MONSTER_IDX i = floor_ptr->m_max;
        floor_ptr->m_max++;
        floor_ptr->m_cnt++;
        return i;
    }

    /* Recycle dead monsters */
    for (MONSTER_IDX i = 1; i < floor_ptr->m_max; i++) {
        monster_type *m_ptr;
        m_ptr = &floor_ptr->m_list[i];
        if (m_ptr->r_idx)
            continue;
        floor_ptr->m_cnt++;
        return i;
    }

    if (current_world_ptr->character_dungeon)
        msg_print(_("モンスターが多すぎる！", "Too many monsters!"));
    return 0;
}

/*!
 * @brief 生成モンスター種族を1種生成テーブルから選択する
 * @param player_ptr プレーヤーへの参照ポインタ
 * @param level 生成階
 * @return 選択されたモンスター生成種族
 */
MONRACE_IDX get_mon_num(player_type *player_ptr, DEPTH level, BIT_FLAGS option)
{
    int i, j, p;
    int r_idx;
    long value, total;
    monster_race *r_ptr;
    alloc_entry *table = alloc_race_table;

    int pls_kakuritu, pls_level, over_days;
    int delay = mysqrt(level * 10000L) + (level * 5);

    /* town level : same delay as 10F, no nasty mons till day18 */
    if (!level)
        delay = 360;

    if (level > MAX_DEPTH - 1)
        level = MAX_DEPTH - 1;

    /* +1 per day after the base date */
    /* base dates : day5(1F), day18(10F,0F), day34(30F), day53(60F), day69(90F) */
    over_days = MAX(0, current_world_ptr->dungeon_turn / (TURNS_PER_TICK * 10000L) - delay / 20);

    /* starts from 1/25, reaches 1/3 after 44days from a level dependent base date */
    pls_kakuritu = MAX(NASTY_MON_MAX, NASTY_MON_BASE - over_days / 2);
    /* starts from 0, reaches +25lv after 75days from a level dependent base date */
    pls_level = MIN(NASTY_MON_PLUS_MAX, over_days / 3);

    if (d_info[player_ptr->dungeon_idx].flags1 & DF1_MAZE) {
        pls_kakuritu = MIN(pls_kakuritu / 2, pls_kakuritu - 10);
        if (pls_kakuritu < 2)
            pls_kakuritu = 2;
        pls_level += 2;
        level += 3;
    }

    /* Boost the level */
    if (!player_ptr->phase_out && !(d_info[player_ptr->dungeon_idx].flags1 & DF1_BEGINNER)) {
        /* Nightmare mode allows more out-of depth monsters */
        if (ironman_nightmare && !randint0(pls_kakuritu)) {
            /* What a bizarre calculation */
            level = 1 + (level * MAX_DEPTH / randint1(MAX_DEPTH));
        } else {
            /* Occasional "nasty" monster */
            if (!randint0(pls_kakuritu)) {
                /* Pick a level bonus */
                level += pls_level;
            }
        }
    }

    total = 0L;

    /* Process probabilities */
    for (i = 0; i < alloc_race_size; i++) {
        if (table[i].level > level)
            break;
        table[i].prob3 = 0;
        r_idx = table[i].index;
        r_ptr = &r_info[r_idx];
        if (!(option & GMN_ARENA) && !chameleon_change_m_idx) {
            if (((r_ptr->flags1 & (RF1_UNIQUE)) || (r_ptr->flags7 & (RF7_NAZGUL))) && (r_ptr->cur_num >= r_ptr->max_num)) {
                continue;
            }

            if ((r_ptr->flags7 & (RF7_UNIQUE2)) && (r_ptr->cur_num >= 1)) {
                continue;
            }

            if (r_idx == MON_BANORLUPART) {
                if (r_info[MON_BANOR].cur_num > 0)
                    continue;
                if (r_info[MON_LUPART].cur_num > 0)
                    continue;
            }
        }

        table[i].prob3 = table[i].prob2;
        total += table[i].prob3;
    }

    if (total <= 0)
        return 0;

    value = randint0(total);
    int found_count = 0;
    for (i = 0; i < alloc_race_size; i++) {
        if (value < table[i].prob3)
            break;
        value = value - table[i].prob3;
        found_count++;
    }

    p = randint0(100);

    /* Try for a "harder" monster once (50%) or twice (10%) */
    if (p < 60) {
        j = found_count;
        value = randint0(total);
        for (found_count = 0; found_count < alloc_race_size; found_count++) {
            if (value < table[found_count].prob3)
                break;

            value = value - table[found_count].prob3;
        }

        if (table[found_count].level < table[j].level)
            found_count = j;
    }

    /* Try for a "harder" monster twice (10%) */
    if (p < 10) {
        j = found_count;
        value = randint0(total);
        for (found_count = 0; found_count < alloc_race_size; found_count++) {
            if (value < table[found_count].prob3)
                break;

            value = value - table[found_count].prob3;
        }

        if (table[found_count].level < table[j].level)
            found_count = j;
    }

    return (table[found_count].index);
}

/*!
 * @brief モンスターIDを取り、モンスター名をm_nameに代入する /
 * @param player_ptr プレーヤーへの参照ポインタ
 * @param m_idx モンスターID
 * @param m_name モンスター名を入力する配列
 */
void monster_name(player_type *player_ptr, MONSTER_IDX m_idx, char *m_name)
{
    monster_type *m_ptr = &player_ptr->current_floor_ptr->m_list[m_idx];
    monster_desc(player_ptr, m_name, m_ptr, 0x00);
}

/*!
 * todo ここにplayer_typeを追加すると関数ポインタ周りの収拾がつかなくなるので保留
 * @param player_ptr プレーヤーへの参照ポインタ
 * @brief カメレオンの王の変身対象となるモンスターかどうか判定する / Hack -- the index of the summoning monster
 * @param r_idx モンスター種族ID
 * @return 対象にできるならtrueを返す
 */
static bool monster_hook_chameleon_lord(MONRACE_IDX r_idx)
{
    floor_type *floor_ptr = p_ptr->current_floor_ptr;
    monster_race *r_ptr = &r_info[r_idx];
    monster_type *m_ptr = &floor_ptr->m_list[chameleon_change_m_idx];
    monster_race *old_r_ptr = &r_info[m_ptr->r_idx];

    if (!(r_ptr->flags1 & (RF1_UNIQUE)))
        return FALSE;
    if (r_ptr->flags7 & (RF7_FRIENDLY | RF7_CHAMELEON))
        return FALSE;

    if (ABS(r_ptr->level - r_info[MON_CHAMELEON_K].level) > 5)
        return FALSE;

    if ((r_ptr->blow[0].method == RBM_EXPLODE) || (r_ptr->blow[1].method == RBM_EXPLODE) || (r_ptr->blow[2].method == RBM_EXPLODE)
        || (r_ptr->blow[3].method == RBM_EXPLODE))
        return FALSE;

    if (!monster_can_cross_terrain(p_ptr, floor_ptr->grid_array[m_ptr->fy][m_ptr->fx].feat, r_ptr, 0))
        return FALSE;

    if (!(old_r_ptr->flags7 & RF7_CHAMELEON)) {
        if (monster_has_hostile_align(p_ptr, m_ptr, 0, 0, r_ptr))
            return FALSE;
    } else if (summon_specific_who > 0) {
        if (monster_has_hostile_align(p_ptr, &floor_ptr->m_list[summon_specific_who], 0, 0, r_ptr))
            return FALSE;
    }

    return TRUE;
}

/*!
 * todo ここにplayer_typeを追加すると関数ポインタ周りの収拾がつかなくなるので保留
 * @brief カメレオンの変身対象となるモンスターかどうか判定する / Hack -- the index of the summoning monster
 * @param r_idx モンスター種族ID
 * @return 対象にできるならtrueを返す
 * @todo グローバル変数対策の上 monster_hook.cへ移す。
 */
static bool monster_hook_chameleon(MONRACE_IDX r_idx)
{
    floor_type *floor_ptr = p_ptr->current_floor_ptr;
    monster_race *r_ptr = &r_info[r_idx];
    monster_type *m_ptr = &floor_ptr->m_list[chameleon_change_m_idx];
    monster_race *old_r_ptr = &r_info[m_ptr->r_idx];

    if (r_ptr->flags1 & (RF1_UNIQUE))
        return FALSE;
    if (r_ptr->flags2 & RF2_MULTIPLY)
        return FALSE;
    if (r_ptr->flags7 & (RF7_FRIENDLY | RF7_CHAMELEON))
        return FALSE;

    if ((r_ptr->blow[0].method == RBM_EXPLODE) || (r_ptr->blow[1].method == RBM_EXPLODE) || (r_ptr->blow[2].method == RBM_EXPLODE)
        || (r_ptr->blow[3].method == RBM_EXPLODE))
        return FALSE;

    if (!monster_can_cross_terrain(p_ptr, floor_ptr->grid_array[m_ptr->fy][m_ptr->fx].feat, r_ptr, 0))
        return FALSE;

    if (!(old_r_ptr->flags7 & RF7_CHAMELEON)) {
        if ((old_r_ptr->flags3 & RF3_GOOD) && !(r_ptr->flags3 & RF3_GOOD))
            return FALSE;
        if ((old_r_ptr->flags3 & RF3_EVIL) && !(r_ptr->flags3 & RF3_EVIL))
            return FALSE;
        if (!(old_r_ptr->flags3 & (RF3_GOOD | RF3_EVIL)) && (r_ptr->flags3 & (RF3_GOOD | RF3_EVIL)))
            return FALSE;
    } else if (summon_specific_who > 0) {
        if (monster_has_hostile_align(p_ptr, &floor_ptr->m_list[summon_specific_who], 0, 0, r_ptr))
            return FALSE;
    }

    return (*(get_monster_hook(p_ptr)))(r_idx);
}

/*!
 * @brief モンスターの変身処理
 * @param player_ptr プレーヤーへの参照ポインタ
 * @param m_idx 変身処理を受けるモンスター情報のID
 * @param born 生成時の初変身先指定ならばtrue
 * @param r_idx 旧モンスター種族のID
 * @return なし
 */
void choose_new_monster(player_type *player_ptr, MONSTER_IDX m_idx, bool born, MONRACE_IDX r_idx)
{
    floor_type *floor_ptr = player_ptr->current_floor_ptr;
    monster_type *m_ptr = &floor_ptr->m_list[m_idx];
    monster_race *r_ptr;

    bool old_unique = FALSE;
    if (r_info[m_ptr->r_idx].flags1 & RF1_UNIQUE)
        old_unique = TRUE;
    if (old_unique && (r_idx == MON_CHAMELEON))
        r_idx = MON_CHAMELEON_K;
    r_ptr = &r_info[r_idx];

    char old_m_name[MAX_NLEN];
    monster_desc(player_ptr, old_m_name, m_ptr, 0);

    if (!r_idx) {
        DEPTH level;

        chameleon_change_m_idx = m_idx;
        if (old_unique)
            get_mon_num_prep(player_ptr, monster_hook_chameleon_lord, NULL);
        else
            get_mon_num_prep(player_ptr, monster_hook_chameleon, NULL);

        if (old_unique)
            level = r_info[MON_CHAMELEON_K].level;
        else if (!floor_ptr->dun_level)
            level = wilderness[player_ptr->wilderness_y][player_ptr->wilderness_x].level;
        else
            level = floor_ptr->dun_level;

        if (d_info[player_ptr->dungeon_idx].flags1 & DF1_CHAMELEON)
            level += 2 + randint1(3);

        r_idx = get_mon_num(player_ptr, level, 0);
        r_ptr = &r_info[r_idx];

        chameleon_change_m_idx = 0;
        if (!r_idx)
            return;
    }

    m_ptr->r_idx = r_idx;
    m_ptr->ap_r_idx = r_idx;
    update_monster(player_ptr, m_idx, FALSE);
    lite_spot(player_ptr, m_ptr->fy, m_ptr->fx);

    int old_r_idx = m_ptr->r_idx;
    if ((r_info[old_r_idx].flags7 & (RF7_LITE_MASK | RF7_DARK_MASK)) || (r_ptr->flags7 & (RF7_LITE_MASK | RF7_DARK_MASK)))
        player_ptr->update |= (PU_MON_LITE);

    if (born) {
        if (r_ptr->flags3 & (RF3_EVIL | RF3_GOOD)) {
            m_ptr->sub_align = SUB_ALIGN_NEUTRAL;
            if (r_ptr->flags3 & RF3_EVIL)
                m_ptr->sub_align |= SUB_ALIGN_EVIL;
            if (r_ptr->flags3 & RF3_GOOD)
                m_ptr->sub_align |= SUB_ALIGN_GOOD;
        }

        return;
    }

    if (m_idx == player_ptr->riding) {
        GAME_TEXT m_name[MAX_NLEN];
        monster_desc(player_ptr, m_name, m_ptr, 0);
        msg_format(_("突然%sが変身した。", "Suddenly, %s transforms!"), old_m_name);
        if (!(r_ptr->flags7 & RF7_RIDING))
            if (process_fall_off_horse(player_ptr, 0, TRUE))
                msg_format(_("地面に落とされた。", "You have fallen from %s."), m_name);
    }

    m_ptr->mspeed = get_mspeed(player_ptr, r_ptr);

    int oldmaxhp = m_ptr->max_maxhp;
    if (r_ptr->flags1 & RF1_FORCE_MAXHP) {
        m_ptr->max_maxhp = maxroll(r_ptr->hdice, r_ptr->hside);
    } else {
        m_ptr->max_maxhp = damroll(r_ptr->hdice, r_ptr->hside);
    }

    if (ironman_nightmare) {
        u32b hp = m_ptr->max_maxhp * 2L;
        m_ptr->max_maxhp = (HIT_POINT)MIN(30000, hp);
    }

    m_ptr->maxhp = (long)(m_ptr->maxhp * m_ptr->max_maxhp) / oldmaxhp;
    if (m_ptr->maxhp < 1)
        m_ptr->maxhp = 1;
    m_ptr->hp = (long)(m_ptr->hp * m_ptr->max_maxhp) / oldmaxhp;
    m_ptr->dealt_damage = 0;
}

/*!
 * todo ここには本来floor_type*を追加したいが、monster.hにfloor.hの参照を追加するとコンパイルエラーが出るので保留
 * @brief モンスターの個体加速を設定する / Get initial monster speed
 * @param r_ptr モンスター種族の参照ポインタ
 * @return 加速値
 */
SPEED get_mspeed(player_type *player_ptr, monster_race *r_ptr)
{
    SPEED mspeed = r_ptr->speed;
    if (!(r_ptr->flags1 & RF1_UNIQUE) && !player_ptr->current_floor_ptr->inside_arena) {
        /* Allow some small variation per monster */
        int i = SPEED_TO_ENERGY(r_ptr->speed) / (one_in_(4) ? 3 : 10);
        if (i)
            mspeed += rand_spread(0, i);
    }

    if (mspeed > 199)
        mspeed = 199;

    return mspeed;
}

/*!
 * @brief ダメージを受けたモンスターの様子を記述する / Dump a message describing a monster's reaction to damage
 * @param player_ptr プレーヤーへの参照ポインタ
 * @param m_idx モンスター情報ID
 * @param dam 与えたダメージ
 * @return なし
 * @details
 * Technically should attempt to treat "Beholder"'s as jelly's
 */
void message_pain(player_type *player_ptr, MONSTER_IDX m_idx, HIT_POINT dam)
{
    monster_type *m_ptr = &player_ptr->current_floor_ptr->m_list[m_idx];
    monster_race *r_ptr = &r_info[m_ptr->r_idx];

    GAME_TEXT m_name[MAX_NLEN];

    monster_desc(player_ptr, m_name, m_ptr, 0);

    if (dam == 0) {
        msg_format(_("%^sはダメージを受けていない。", "%^s is unharmed."), m_name);
        return;
    }

    HIT_POINT newhp = m_ptr->hp;
    HIT_POINT oldhp = newhp + dam;
    HIT_POINT tmp = (newhp * 100L) / oldhp;
    PERCENTAGE percentage = tmp;

    if (my_strchr(",ejmvwQ", r_ptr->d_char)) {
#ifdef JP
        if (percentage > 95)
            msg_format("%^sはほとんど気にとめていない。", m_name);
        else if (percentage > 75)
            msg_format("%^sはしり込みした。", m_name);
        else if (percentage > 50)
            msg_format("%^sは縮こまった。", m_name);
        else if (percentage > 35)
            msg_format("%^sは痛みに震えた。", m_name);
        else if (percentage > 20)
            msg_format("%^sは身もだえした。", m_name);
        else if (percentage > 10)
            msg_format("%^sは苦痛で身もだえした。", m_name);
        else
            msg_format("%^sはぐにゃぐにゃと痙攣した。", m_name);
        return;
#else
        if (percentage > 95)
            msg_format("%^s barely notices.", m_name);
        else if (percentage > 75)
            msg_format("%^s flinches.", m_name);
        else if (percentage > 50)
            msg_format("%^s squelches.", m_name);
        else if (percentage > 35)
            msg_format("%^s quivers in pain.", m_name);
        else if (percentage > 20)
            msg_format("%^s writhes about.", m_name);
        else if (percentage > 10)
            msg_format("%^s writhes in agony.", m_name);
        else
            msg_format("%^s jerks limply.", m_name);
        return;
#endif
    }

    if (my_strchr("l", r_ptr->d_char)) {
#ifdef JP
        if (percentage > 95)
            msg_format("%^sはほとんど気にとめていない。", m_name);
        else if (percentage > 75)
            msg_format("%^sはしり込みした。", m_name);
        else if (percentage > 50)
            msg_format("%^sは躊躇した。", m_name);
        else if (percentage > 35)
            msg_format("%^sは痛みに震えた。", m_name);
        else if (percentage > 20)
            msg_format("%^sは身もだえした。", m_name);
        else if (percentage > 10)
            msg_format("%^sは苦痛で身もだえした。", m_name);
        else
            msg_format("%^sはぐにゃぐにゃと痙攣した。", m_name);
        return;
#else
        if (percentage > 95)
            msg_format("%^s barely notices.", m_name);
        else if (percentage > 75)
            msg_format("%^s flinches.", m_name);
        else if (percentage > 50)
            msg_format("%^s hesitates.", m_name);
        else if (percentage > 35)
            msg_format("%^s quivers in pain.", m_name);
        else if (percentage > 20)
            msg_format("%^s writhes about.", m_name);
        else if (percentage > 10)
            msg_format("%^s writhes in agony.", m_name);
        else
            msg_format("%^s jerks limply.", m_name);
        return;
#endif
    }

    if (my_strchr("g#+<>", r_ptr->d_char)) {
#ifdef JP
        if (percentage > 95)
            msg_format("%sは攻撃を気にとめていない。", m_name);
        else if (percentage > 75)
            msg_format("%sは攻撃に肩をすくめた。", m_name);
        else if (percentage > 50)
            msg_format("%^sは雷鳴のように吠えた。", m_name);
        else if (percentage > 35)
            msg_format("%^sは苦しげに吠えた。", m_name);
        else if (percentage > 20)
            msg_format("%^sはうめいた。", m_name);
        else if (percentage > 10)
            msg_format("%^sは躊躇した。", m_name);
        else
            msg_format("%^sはくしゃくしゃになった。", m_name);
        return;
#else
        if (percentage > 95)
            msg_format("%^s ignores the attack.", m_name);
        else if (percentage > 75)
            msg_format("%^s shrugs off the attack.", m_name);
        else if (percentage > 50)
            msg_format("%^s roars thunderously.", m_name);
        else if (percentage > 35)
            msg_format("%^s rumbles.", m_name);
        else if (percentage > 20)
            msg_format("%^s grunts.", m_name);
        else if (percentage > 10)
            msg_format("%^s hesitates.", m_name);
        else
            msg_format("%^s crumples.", m_name);
        return;
#endif
    }

    if (my_strchr("JMR", r_ptr->d_char) || !isalpha(r_ptr->d_char)) {
#ifdef JP
        if (percentage > 95)
            msg_format("%^sはほとんど気にとめていない。", m_name);
        else if (percentage > 75)
            msg_format("%^sはシーッと鳴いた。", m_name);
        else if (percentage > 50)
            msg_format("%^sは怒って頭を上げた。", m_name);
        else if (percentage > 35)
            msg_format("%^sは猛然と威嚇した。", m_name);
        else if (percentage > 20)
            msg_format("%^sは身もだえした。", m_name);
        else if (percentage > 10)
            msg_format("%^sは苦痛で身もだえした。", m_name);
        else
            msg_format("%^sはぐにゃぐにゃと痙攣した。", m_name);
        return;
#else
        if (percentage > 95)
            msg_format("%^s barely notices.", m_name);
        else if (percentage > 75)
            msg_format("%^s hisses.", m_name);
        else if (percentage > 50)
            msg_format("%^s rears up in anger.", m_name);
        else if (percentage > 35)
            msg_format("%^s hisses furiously.", m_name);
        else if (percentage > 20)
            msg_format("%^s writhes about.", m_name);
        else if (percentage > 10)
            msg_format("%^s writhes in agony.", m_name);
        else
            msg_format("%^s jerks limply.", m_name);
        return;
#endif
    }

    if (my_strchr("f", r_ptr->d_char)) {
#ifdef JP
        if (percentage > 95)
            msg_format("%sは攻撃に肩をすくめた。", m_name);
        else if (percentage > 75)
            msg_format("%^sは吠えた。", m_name);
        else if (percentage > 50)
            msg_format("%^sは怒って吠えた。", m_name);
        else if (percentage > 35)
            msg_format("%^sは痛みでシーッと鳴いた。", m_name);
        else if (percentage > 20)
            msg_format("%^sは痛みで弱々しく鳴いた。", m_name);
        else if (percentage > 10)
            msg_format("%^sは苦痛にうめいた。", m_name);
        else
            msg_format("%sは哀れな鳴き声を出した。", m_name);
        return;
#else
        if (percentage > 95)
            msg_format("%^s shrugs off the attack.", m_name);
        else if (percentage > 75)
            msg_format("%^s roars.", m_name);
        else if (percentage > 50)
            msg_format("%^s growls angrily.", m_name);
        else if (percentage > 35)
            msg_format("%^s hisses with pain.", m_name);
        else if (percentage > 20)
            msg_format("%^s mewls in pain.", m_name);
        else if (percentage > 10)
            msg_format("%^s hisses in agony.", m_name);
        else
            msg_format("%^s mewls pitifully.", m_name);
        return;
#endif
    }

    if (my_strchr("acFIKS", r_ptr->d_char)) {
#ifdef JP
        if (percentage > 95)
            msg_format("%sは攻撃を気にとめていない。", m_name);
        else if (percentage > 75)
            msg_format("%^sはキーキー鳴いた。", m_name);
        else if (percentage > 50)
            msg_format("%^sはヨロヨロ逃げ回った。", m_name);
        else if (percentage > 35)
            msg_format("%^sはうるさく鳴いた。", m_name);
        else if (percentage > 20)
            msg_format("%^sは痛みに痙攣した。", m_name);
        else if (percentage > 10)
            msg_format("%^sは苦痛で痙攣した。", m_name);
        else
            msg_format("%^sはピクピクひきつった。", m_name);
        return;
#else
        if (percentage > 95)
            msg_format("%^s ignores the attack.", m_name);
        else if (percentage > 75)
            msg_format("%^s chitters.", m_name);
        else if (percentage > 50)
            msg_format("%^s scuttles about.", m_name);
        else if (percentage > 35)
            msg_format("%^s twitters.", m_name);
        else if (percentage > 20)
            msg_format("%^s jerks in pain.", m_name);
        else if (percentage > 10)
            msg_format("%^s jerks in agony.", m_name);
        else
            msg_format("%^s twitches.", m_name);
        return;
#endif
    }

    if (my_strchr("B", r_ptr->d_char)) {
#ifdef JP
        if (percentage > 95)
            msg_format("%^sはさえずった。", m_name);
        else if (percentage > 75)
            msg_format("%^sはピーピー鳴いた。", m_name);
        else if (percentage > 50)
            msg_format("%^sはギャーギャー鳴いた。", m_name);
        else if (percentage > 35)
            msg_format("%^sはギャーギャー鳴きわめいた。", m_name);
        else if (percentage > 20)
            msg_format("%^sは苦しんだ。", m_name);
        else if (percentage > 10)
            msg_format("%^sはのたうち回った。", m_name);
        else
            msg_format("%^sはキーキーと鳴き叫んだ。", m_name);
        return;
#else
        if (percentage > 95)
            msg_format("%^s chirps.", m_name);
        else if (percentage > 75)
            msg_format("%^s twitters.", m_name);
        else if (percentage > 50)
            msg_format("%^s squawks.", m_name);
        else if (percentage > 35)
            msg_format("%^s chatters.", m_name);
        else if (percentage > 20)
            msg_format("%^s jeers.", m_name);
        else if (percentage > 10)
            msg_format("%^s flutters about.", m_name);
        else
            msg_format("%^s squeaks.", m_name);
        return;
#endif
    }

    if (my_strchr("duDLUW", r_ptr->d_char)) {
#ifdef JP
        if (percentage > 95)
            msg_format("%sは攻撃を気にとめていない。", m_name);
        else if (percentage > 75)
            msg_format("%^sはしり込みした。", m_name);
        else if (percentage > 50)
            msg_format("%^sは痛みでシーッと鳴いた。", m_name);
        else if (percentage > 35)
            msg_format("%^sは痛みでうなった。", m_name);
        else if (percentage > 20)
            msg_format("%^sは痛みに吠えた。", m_name);
        else if (percentage > 10)
            msg_format("%^sは苦しげに叫んだ。", m_name);
        else
            msg_format("%^sは弱々しくうなった。", m_name);
        return;
#else
        if (percentage > 95)
            msg_format("%^s ignores the attack.", m_name);
        else if (percentage > 75)
            msg_format("%^s flinches.", m_name);
        else if (percentage > 50)
            msg_format("%^s hisses in pain.", m_name);
        else if (percentage > 35)
            msg_format("%^s snarls with pain.", m_name);
        else if (percentage > 20)
            msg_format("%^s roars with pain.", m_name);
        else if (percentage > 10)
            msg_format("%^s gasps.", m_name);
        else
            msg_format("%^s snarls feebly.", m_name);
        return;
#endif
    }

    if (my_strchr("s", r_ptr->d_char)) {
#ifdef JP
        if (percentage > 95)
            msg_format("%sは攻撃を気にとめていない。", m_name);
        else if (percentage > 75)
            msg_format("%sは攻撃に肩をすくめた。", m_name);
        else if (percentage > 50)
            msg_format("%^sはカタカタと笑った。", m_name);
        else if (percentage > 35)
            msg_format("%^sはよろめいた。", m_name);
        else if (percentage > 20)
            msg_format("%^sはカタカタ言った。", m_name);
        else if (percentage > 10)
            msg_format("%^sはよろめいた。", m_name);
        else
            msg_format("%^sはガタガタ言った。", m_name);
        return;
#else
        if (percentage > 95)
            msg_format("%^s ignores the attack.", m_name);
        else if (percentage > 75)
            msg_format("%^s shrugs off the attack.", m_name);
        else if (percentage > 50)
            msg_format("%^s rattles.", m_name);
        else if (percentage > 35)
            msg_format("%^s stumbles.", m_name);
        else if (percentage > 20)
            msg_format("%^s rattles.", m_name);
        else if (percentage > 10)
            msg_format("%^s staggers.", m_name);
        else
            msg_format("%^s clatters.", m_name);
        return;
#endif
    }

    if (my_strchr("z", r_ptr->d_char)) {
#ifdef JP
        if (percentage > 95)
            msg_format("%sは攻撃を気にとめていない。", m_name);
        else if (percentage > 75)
            msg_format("%sは攻撃に肩をすくめた。", m_name);
        else if (percentage > 50)
            msg_format("%^sはうめいた。", m_name);
        else if (percentage > 35)
            msg_format("%sは苦しげにうめいた。", m_name);
        else if (percentage > 20)
            msg_format("%^sは躊躇した。", m_name);
        else if (percentage > 10)
            msg_format("%^sはうなった。", m_name);
        else
            msg_format("%^sはよろめいた。", m_name);
        return;
#else
        if (percentage > 95)
            msg_format("%^s ignores the attack.", m_name);
        else if (percentage > 75)
            msg_format("%^s shrugs off the attack.", m_name);
        else if (percentage > 50)
            msg_format("%^s groans.", m_name);
        else if (percentage > 35)
            msg_format("%^s moans.", m_name);
        else if (percentage > 20)
            msg_format("%^s hesitates.", m_name);
        else if (percentage > 10)
            msg_format("%^s grunts.", m_name);
        else
            msg_format("%^s staggers.", m_name);
        return;
#endif
    }

    if (my_strchr("G", r_ptr->d_char)) {
#ifdef JP
        if (percentage > 95)
            msg_format("%sは攻撃を気にとめていない。", m_name);
        else if (percentage > 75)
            msg_format("%sは攻撃に肩をすくめた。", m_name);
        else if (percentage > 50)
            msg_format("%sはうめいた。", m_name);
        else if (percentage > 35)
            msg_format("%^sは泣きわめいた。", m_name);
        else if (percentage > 20)
            msg_format("%^sは吠えた。", m_name);
        else if (percentage > 10)
            msg_format("%sは弱々しくうめいた。", m_name);
        else
            msg_format("%^sはかすかにうめいた。", m_name);
        return;
#else
        if (percentage > 95)
            msg_format("%^s ignores the attack.", m_name);
        else if (percentage > 75)
            msg_format("%^s shrugs off the attack.", m_name);
        else if (percentage > 50)
            msg_format("%^s moans.", m_name);
        else if (percentage > 35)
            msg_format("%^s wails.", m_name);
        else if (percentage > 20)
            msg_format("%^s howls.", m_name);
        else if (percentage > 10)
            msg_format("%^s moans softly.", m_name);
        else
            msg_format("%^s sighs.", m_name);
        return;
#endif
    }

    if (my_strchr("CZ", r_ptr->d_char)) {
#ifdef JP
        if (percentage > 95)
            msg_format("%^sは攻撃に肩をすくめた。", m_name);
        else if (percentage > 75)
            msg_format("%^sは痛みでうなった。", m_name);
        else if (percentage > 50)
            msg_format("%^sは痛みでキャンキャン吠えた。", m_name);
        else if (percentage > 35)
            msg_format("%^sは痛みで鳴きわめいた。", m_name);
        else if (percentage > 20)
            msg_format("%^sは苦痛のあまり鳴きわめいた。", m_name);
        else if (percentage > 10)
            msg_format("%^sは苦痛でもだえ苦しんだ。", m_name);
        else
            msg_format("%^sは弱々しく吠えた。", m_name);
        return;
#else
        if (percentage > 95)
            msg_format("%^s shrugs off the attack.", m_name);
        else if (percentage > 75)
            msg_format("%^s snarls with pain.", m_name);
        else if (percentage > 50)
            msg_format("%^s yelps in pain.", m_name);
        else if (percentage > 35)
            msg_format("%^s howls in pain.", m_name);
        else if (percentage > 20)
            msg_format("%^s howls in agony.", m_name);
        else if (percentage > 10)
            msg_format("%^s writhes in agony.", m_name);
        else
            msg_format("%^s yelps feebly.", m_name);
        return;
#endif
    }

    if (my_strchr("Xbilqrt", r_ptr->d_char)) {
#ifdef JP
        if (percentage > 95)
            msg_format("%^sは攻撃を気にとめていない。", m_name);
        else if (percentage > 75)
            msg_format("%^sは痛みでうなった。", m_name);
        else if (percentage > 50)
            msg_format("%^sは痛みで叫んだ。", m_name);
        else if (percentage > 35)
            msg_format("%^sは痛みで絶叫した。", m_name);
        else if (percentage > 20)
            msg_format("%^sは苦痛のあまり絶叫した。", m_name);
        else if (percentage > 10)
            msg_format("%^sは苦痛でもだえ苦しんだ。", m_name);
        else
            msg_format("%^sは弱々しく叫んだ。", m_name);
        return;
#else
        if (percentage > 95)
            msg_format("%^s ignores the attack.", m_name);
        else if (percentage > 75)
            msg_format("%^s grunts with pain.", m_name);
        else if (percentage > 50)
            msg_format("%^s squeals in pain.", m_name);
        else if (percentage > 35)
            msg_format("%^s shrieks in pain.", m_name);
        else if (percentage > 20)
            msg_format("%^s shrieks in agony.", m_name);
        else if (percentage > 10)
            msg_format("%^s writhes in agony.", m_name);
        else
            msg_format("%^s cries out feebly.", m_name);
        return;
#endif
    }

#ifdef JP
    if (percentage > 95)
        msg_format("%^sは攻撃に肩をすくめた。", m_name);
    else if (percentage > 75)
        msg_format("%^sは痛みでうなった。", m_name);
    else if (percentage > 50)
        msg_format("%^sは痛みで叫んだ。", m_name);
    else if (percentage > 35)
        msg_format("%^sは痛みで絶叫した。", m_name);
    else if (percentage > 20)
        msg_format("%^sは苦痛のあまり絶叫した。", m_name);
    else if (percentage > 10)
        msg_format("%^sは苦痛でもだえ苦しんだ。", m_name);
    else
        msg_format("%^sは弱々しく叫んだ。", m_name);
#else
    if (percentage > 95)
        msg_format("%^s shrugs off the attack.", m_name);
    else if (percentage > 75)
        msg_format("%^s grunts with pain.", m_name);
    else if (percentage > 50)
        msg_format("%^s cries out in pain.", m_name);
    else if (percentage > 35)
        msg_format("%^s screams in pain.", m_name);
    else if (percentage > 20)
        msg_format("%^s screams in agony.", m_name);
    else if (percentage > 10)
        msg_format("%^s writhes in agony.", m_name);
    else
        msg_format("%^s cries out feebly.", m_name);
#endif
}

/*!
 * @brief SMART(適格に攻撃を行う)モンスターの学習状況を更新する / Learn about an "observed" resistance.
 * @param m_idx 更新を行う「モンスター情報ID
 * @param what 学習対象ID
 * @return なし
 */
void update_smart_learn(player_type *player_ptr, MONSTER_IDX m_idx, int what)
{
    monster_type *m_ptr = &player_ptr->current_floor_ptr->m_list[m_idx];
    monster_race *r_ptr = &r_info[m_ptr->r_idx];

    if (!smart_learn)
        return;
    if (r_ptr->flags2 & (RF2_STUPID))
        return;
    if (!(r_ptr->flags2 & (RF2_SMART)) && (randint0(100) < 50))
        return;

    switch (what) {
    case DRS_ACID:
        if (player_ptr->resist_acid)
            m_ptr->smart |= (SM_RES_ACID);
        if (is_oppose_acid(player_ptr))
            m_ptr->smart |= (SM_OPP_ACID);
        if (player_ptr->immune_acid)
            m_ptr->smart |= (SM_IMM_ACID);
        break;

    case DRS_ELEC:
        if (player_ptr->resist_elec)
            m_ptr->smart |= (SM_RES_ELEC);
        if (is_oppose_elec(player_ptr))
            m_ptr->smart |= (SM_OPP_ELEC);
        if (player_ptr->immune_elec)
            m_ptr->smart |= (SM_IMM_ELEC);
        break;

    case DRS_FIRE:
        if (player_ptr->resist_fire)
            m_ptr->smart |= (SM_RES_FIRE);
        if (is_oppose_fire(player_ptr))
            m_ptr->smart |= (SM_OPP_FIRE);
        if (player_ptr->immune_fire)
            m_ptr->smart |= (SM_IMM_FIRE);
        break;

    case DRS_COLD:
        if (player_ptr->resist_cold)
            m_ptr->smart |= (SM_RES_COLD);
        if (is_oppose_cold(player_ptr))
            m_ptr->smart |= (SM_OPP_COLD);
        if (player_ptr->immune_cold)
            m_ptr->smart |= (SM_IMM_COLD);
        break;

    case DRS_POIS:
        if (player_ptr->resist_pois)
            m_ptr->smart |= (SM_RES_POIS);
        if (is_oppose_pois(player_ptr))
            m_ptr->smart |= (SM_OPP_POIS);
        break;

    case DRS_NETH:
        if (player_ptr->resist_neth)
            m_ptr->smart |= (SM_RES_NETH);
        break;

    case DRS_LITE:
        if (player_ptr->resist_lite)
            m_ptr->smart |= (SM_RES_LITE);
        break;

    case DRS_DARK:
        if (player_ptr->resist_dark)
            m_ptr->smart |= (SM_RES_DARK);
        break;

    case DRS_FEAR:
        if (player_ptr->resist_fear)
            m_ptr->smart |= (SM_RES_FEAR);
        break;

    case DRS_CONF:
        if (player_ptr->resist_conf)
            m_ptr->smart |= (SM_RES_CONF);
        break;

    case DRS_CHAOS:
        if (player_ptr->resist_chaos)
            m_ptr->smart |= (SM_RES_CHAOS);
        break;

    case DRS_DISEN:
        if (player_ptr->resist_disen)
            m_ptr->smart |= (SM_RES_DISEN);
        break;

    case DRS_BLIND:
        if (player_ptr->resist_blind)
            m_ptr->smart |= (SM_RES_BLIND);
        break;

    case DRS_NEXUS:
        if (player_ptr->resist_nexus)
            m_ptr->smart |= (SM_RES_NEXUS);
        break;

    case DRS_SOUND:
        if (player_ptr->resist_sound)
            m_ptr->smart |= (SM_RES_SOUND);
        break;

    case DRS_SHARD:
        if (player_ptr->resist_shard)
            m_ptr->smart |= (SM_RES_SHARD);
        break;

    case DRS_FREE:
        if (player_ptr->free_act)
            m_ptr->smart |= (SM_IMM_FREE);
        break;

    case DRS_MANA:
        if (!player_ptr->msp)
            m_ptr->smart |= (SM_IMM_MANA);
        break;

    case DRS_REFLECT:
        if (player_ptr->reflect)
            m_ptr->smart |= (SM_IMM_REFLECT);
        break;
    }
}

/*!
 * @brief モンスターが盗みや拾いで確保していたアイテムを全てドロップさせる / Drop all items carried by a monster
 * @param player_ptr プレーヤーへの参照ポインタ
 * @param m_ptr モンスター参照ポインタ
 * @return なし
 */
void monster_drop_carried_objects(player_type *player_ptr, monster_type *m_ptr)
{
    floor_type *floor_ptr = player_ptr->current_floor_ptr;
    OBJECT_IDX next_o_idx = 0;
    for (OBJECT_IDX this_o_idx = m_ptr->hold_o_idx; this_o_idx; this_o_idx = next_o_idx) {
        object_type forge;
        object_type *o_ptr;
        object_type *q_ptr;
        o_ptr = &floor_ptr->o_list[this_o_idx];
        next_o_idx = o_ptr->next_o_idx;
        q_ptr = &forge;

        object_copy(q_ptr, o_ptr);
        q_ptr->held_m_idx = 0;
        delete_object_idx(player_ptr, this_o_idx);
        (void)drop_near(player_ptr, q_ptr, -1, m_ptr->fy, m_ptr->fx);
    }

    m_ptr->hold_o_idx = 0;
}

/*!
 * todo ここには本来floor_type*を追加したいが、monster.hにfloor.hの参照を追加するとコンパイルエラーが出るので保留
 * @brief 指定したモンスターに隣接しているモンスターの数を返す。
 * / Count number of adjacent monsters
 * @param player_ptr プレーヤーへの参照ポインタ
 * @param m_idx 隣接数を調べたいモンスターのID
 * @return 隣接しているモンスターの数
 */
int get_monster_crowd_number(player_type *player_ptr, MONSTER_IDX m_idx)
{
    floor_type *floor_ptr = player_ptr->current_floor_ptr;
    monster_type *m_ptr = &floor_ptr->m_list[m_idx];
    POSITION my = m_ptr->fy;
    POSITION mx = m_ptr->fx;
    int count = 0;
    for (int i = 0; i < 7; i++) {
        int ay = my + ddy_ddd[i];
        int ax = mx + ddx_ddd[i];

        if (!in_bounds(floor_ptr, ay, ax))
            continue;
        if (floor_ptr->grid_array[ay][ax].m_idx > 0)
            count++;
    }

    return count;
}
