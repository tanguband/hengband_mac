﻿#include "birth/birth-select-class.h"
#include "birth/birth-util.h"
#include "io/input-key-acceptor.h"
#include "player-info/class-info.h"
#include "player-info/race-info.h"
#include "system/player-type-definition.h"
#include "term/screen-processor.h"
#include "term/term-color-types.h"
#include "util/int-char-converter.h"
#include "world/world.h"

static const char p2 = ')';

static TERM_COLOR birth_class_color(int cs)
{
    if (cs < MAX_CLASS) {
        if (is_retired_class(static_cast<player_class_type>(cs)))
            return TERM_L_DARK;
        if (is_winner_class(static_cast<player_class_type>(cs)))
            return TERM_SLATE;
    }
    return TERM_WHITE;
}

static void enumerate_class_list(char *sym)
{
    for (int n = 0; n < MAX_CLASS; n++) {
        cp_ptr = &class_info[n];
        mp_ptr = &m_info[n];
        concptr str = cp_ptr->title;
        if (n < 26)
            sym[n] = I2A(n);
        else
            sym[n] = ('A' + n - 26);

        char buf[80];
        if (!(rp_ptr->choice & (1UL << n)))
            sprintf(buf, "%c%c(%s)", sym[n], p2, str);
        else
            sprintf(buf, "%c%c%s", sym[n], p2, str);

        c_put_str(birth_class_color(n), buf, 13 + (n / 4), 2 + 19 * (n % 4));
    }
}

static void display_class_stat(int cs, int *os, char *cur, char *sym)
{
    if (cs == *os)
        return;

    c_put_str(birth_class_color(*os), cur, 13 + (*os / 4), 2 + 19 * (*os % 4));
    put_str("                                   ", 3, 40);
    if (cs == MAX_CLASS) {
        sprintf(cur, "%c%c%s", '*', p2, _("ランダム", "Random"));
        put_str("                                   ", 4, 40);
        put_str("                                   ", 5, 40);
        put_str("                                   ", 6, 40);
    } else {
        cp_ptr = &class_info[cs];
        mp_ptr = &m_info[cs];
        concptr str = cp_ptr->title;
        if (!(rp_ptr->choice & (1UL << cs)))
            sprintf(cur, "%c%c(%s)", sym[cs], p2, str);
        else
            sprintf(cur, "%c%c%s", sym[cs], p2, str);

        c_put_str(TERM_L_BLUE, cp_ptr->title, 3, 40);
        put_str(_("の職業修正", ": Class modification"), 3, 40 + strlen(cp_ptr->title));
        put_str(_("腕力 知能 賢さ 器用 耐久 魅力 経験 ", "Str  Int  Wis  Dex  Con  Chr   EXP "), 4, 40);
        char buf[80];
        sprintf(buf, "%+3d  %+3d  %+3d  %+3d  %+3d  %+3d %+4d%% ", cp_ptr->c_adj[0], cp_ptr->c_adj[1], cp_ptr->c_adj[2], cp_ptr->c_adj[3], cp_ptr->c_adj[4],
            cp_ptr->c_adj[5], cp_ptr->c_exp);
        c_put_str(TERM_L_BLUE, buf, 5, 40);

        put_str("HD", 6, 40);
        sprintf(buf, "%+3d", cp_ptr->c_mhp);
        c_put_str(TERM_L_BLUE, buf, 6, 42);

        put_str(_("隠密", "Stealth"), 6, 47);
        if (cs == CLASS_BERSERKER)
            strcpy(buf, " xx");
        else
            sprintf(buf, " %+2d", cp_ptr->c_stl);
        c_put_str(TERM_L_BLUE, buf, 6, _(51, 54));
    }

    c_put_str(TERM_YELLOW, cur, 13 + (cs / 4), 2 + 19 * (cs % 4));
    *os = cs;
}

static void interpret_class_select_key_move(char c, int *cs)
{
    if (c == '8') {
        if (*cs >= 4)
            *cs -= 4;
    }

    if (c == '4') {
        if (*cs > 0)
            (*cs)--;
    }

    if (c == '6') {
        if (*cs < MAX_CLASS)
            (*cs)++;
    }

    if (c == '2') {
        if ((*cs + 4) <= MAX_CLASS)
            *cs += 4;
    }
}

static bool select_class(player_type *creature_ptr, char *cur, char *sym, int *k)
{
    int cs = creature_ptr->pclass;
    int os = MAX_CLASS;
    while (true) {
        display_class_stat(cs, &os, cur, sym);
        if (*k >= 0)
            break;

        char buf[80];
        sprintf(buf, _("職業を選んで下さい (%c-%c) ('='初期オプション設定, 灰色:勝利済): ", "Choose a class (%c-%c) ('=' for options, Gray is winner): "),
            sym[0], sym[MAX_CLASS - 1]);

        put_str(buf, 10, 6);
        char c = inkey();
        if (c == 'Q')
            birth_quit();

        if (c == 'S')
            return false;

        if (c == ' ' || c == '\r' || c == '\n') {
            if (cs == MAX_CLASS) {
                *k = randint0(MAX_CLASS);
                cs = *k;
                continue;
            } else {
                *k = cs;
                break;
            }
        }

        interpret_class_select_key_move(c, &cs);
        if (c == '*') {
            *k = randint0(MAX_CLASS);
            cs = *k;
            continue;
        }

        *k = (islower(c) ? A2I(c) : -1);
        if ((*k >= 0) && (*k < MAX_CLASS)) {
            cs = *k;
            continue;
        }

        *k = (isupper(c) ? (26 + c - 'A') : -1);
        if ((*k >= 26) && (*k < MAX_CLASS)) {
            cs = *k;
            continue;
        } else
            *k = -1;

        birth_help_option(creature_ptr, c, BK_CLASS);
    }

    return true;
}

/*!
 * @brief プレイヤーの職業選択を行う / Player class
 */
bool get_player_class(player_type *creature_ptr)
{
    clear_from(10);
    put_str(
        _("注意：《職業》によってキャラクターの先天的な能力やボーナスが変化します。", "Note: Your 'class' determines various intrinsic abilities and bonuses."),
        23, 5);
    put_str(_("()で囲まれた選択肢はこの種族には似合わない職業です。", "Any entries in parentheses should only be used by advanced players."), 11, 5);
    put_str("                                   ", 6, 40);

    char sym[MAX_CLASS];
    enumerate_class_list(sym);

    char cur[80];
    sprintf(cur, "%c%c%s", '*', p2, _("ランダム", "Random"));
    int k = -1;
    if (!select_class(creature_ptr, cur, sym, &k))
        return false;

    creature_ptr->pclass = static_cast<player_class_type>(k);
    cp_ptr = &class_info[creature_ptr->pclass];
    mp_ptr = &m_info[creature_ptr->pclass];
    c_put_str(TERM_L_BLUE, cp_ptr->title, 5, 15);
    return true;
}
