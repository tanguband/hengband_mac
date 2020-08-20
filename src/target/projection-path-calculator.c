#include "target/projection-path-calculator.h"
#include "effect/effect-characteristics.h"
#include "floor/cave.h"
#include "grid/feature-flag-types.h"
#include "grid/grid.h"
#include "system/floor-type-definition.h"

typedef struct projection_path_type {
    POSITION y1;
    POSITION x1;
    POSITION y2;
    POSITION x2;
    POSITION y;
    POSITION x;
    POSITION ay;
    POSITION ax;
    POSITION sy;
    POSITION sx;
    int frac;
    int m;
    int half;
    int full;
    int n;
    int k;
} projection_path_type;

/*
 * @brief Convert a "location" (Y, X) into a "grid" (G)
 * @param y Y座標
 * @param x X座標
 * return 経路座標
 */
static u16b location_to_grid(POSITION y, POSITION x) { return 256 * y + x; }

static projection_path_type *initialize_projection_path_type(projection_path_type *pp_ptr, POSITION y1, POSITION x1, POSITION y2, POSITION x2)
{
    pp_ptr->y1 = y1;
    pp_ptr->x1 = x1;
    pp_ptr->y2 = y2;
    pp_ptr->x2 = x2;
    return pp_ptr;
}

static void set_asxy(projection_path_type *pp_ptr)
{
    if (pp_ptr->y2 < pp_ptr->y1) {
        pp_ptr->ay = pp_ptr->y1 - pp_ptr->y2;
        pp_ptr->sy = -1;
    } else {
        pp_ptr->ay = pp_ptr->y2 - pp_ptr->y1;
        pp_ptr->sy = 1;
    }

    if (pp_ptr->x2 < pp_ptr->x1) {
        pp_ptr->ax = pp_ptr->x1 - pp_ptr->x2;
        pp_ptr->sx = -1;
    } else {
        pp_ptr->ax = pp_ptr->x2 - pp_ptr->x1;
        pp_ptr->sx = 1;
    }
}

/*!
 * @brief 始点から終点への直線経路を返す /
 * Determine the path taken by a projection.
 * @param player_ptr プレーヤーへの参照ポインタ
 * @param gp 経路座標リストを返す参照ポインタ
 * @param range 距離
 * @param y1 始点Y座標
 * @param x1 始点X座標
 * @param y2 終点Y座標
 * @param x2 終点X座標
 * @param flg フラグID
 * @return リストの長さ
 */
int projection_path(player_type *player_ptr, u16b *gp, POSITION range, POSITION y1, POSITION x1, POSITION y2, POSITION x2, BIT_FLAGS flg)
{
    if ((x1 == x2) && (y1 == y2))
        return 0;

    projection_path_type tmp_projection_path;
    projection_path_type *pp_ptr = initialize_projection_path_type(&tmp_projection_path, y1, x1, y2, x2);
    set_asxy(pp_ptr);
    pp_ptr->half = pp_ptr->ay * pp_ptr->ax;
    pp_ptr->full = pp_ptr->half << 1;
    pp_ptr->n = 0;
    pp_ptr->k = 0;

    /* Vertical */
    if (pp_ptr->ay > pp_ptr->ax) {
        pp_ptr->m = pp_ptr->ax * pp_ptr->ax * 2;
        pp_ptr->y = y1 + pp_ptr->sy;
        pp_ptr->x = x1;
        pp_ptr->frac = pp_ptr->m;
        if (pp_ptr->frac > pp_ptr->half) {
            pp_ptr->x += pp_ptr->sx;
            pp_ptr->frac -= pp_ptr->full;
            pp_ptr->k++;
        }

        floor_type *floor_ptr = player_ptr->current_floor_ptr;
        while (TRUE) {
            gp[pp_ptr->n++] = location_to_grid(pp_ptr->y, pp_ptr->x);
            if ((pp_ptr->n + (pp_ptr->k >> 1)) >= range)
                break;

            if (!(flg & PROJECT_THRU)) {
                if ((pp_ptr->x == x2) && (pp_ptr->y == y2))
                    break;
            }

            if (flg & PROJECT_DISI) {
                if ((pp_ptr->n > 0) && cave_stop_disintegration(floor_ptr, pp_ptr->y, pp_ptr->x))
                    break;
            } else if (flg & PROJECT_LOS) {
                if ((pp_ptr->n > 0) && !cave_los_bold(floor_ptr, pp_ptr->y, pp_ptr->x))
                    break;
            } else if (!(flg & PROJECT_PATH)) {
                if ((pp_ptr->n > 0) && !cave_have_flag_bold(floor_ptr, pp_ptr->y, pp_ptr->x, FF_PROJECT))
                    break;
            }

            if (flg & PROJECT_STOP) {
                if ((pp_ptr->n > 0) && (player_bold(player_ptr, pp_ptr->y, pp_ptr->x) || floor_ptr->grid_array[pp_ptr->y][pp_ptr->x].m_idx != 0))
                    break;
            }

            if (!in_bounds(floor_ptr, pp_ptr->y, pp_ptr->x))
                break;

            if (pp_ptr->m) {
                pp_ptr->frac += pp_ptr->m;
                if (pp_ptr->frac > pp_ptr->half) {
                    pp_ptr->x += pp_ptr->sx;
                    pp_ptr->frac -= pp_ptr->full;
                    pp_ptr->k++;
                }
            }

            pp_ptr->y += pp_ptr->sy;
        }

        return pp_ptr->n;
    }

    /* Horizontal */
    if (pp_ptr->ax > pp_ptr->ay) {
        pp_ptr->m = pp_ptr->ay * pp_ptr->ay * 2;
        pp_ptr->y = y1;
        pp_ptr->x = x1 + pp_ptr->sx;
        pp_ptr->frac = pp_ptr->m;
        if (pp_ptr->frac > pp_ptr->half) {
            pp_ptr->y += pp_ptr->sy;
            pp_ptr->frac -= pp_ptr->full;
            pp_ptr->k++;
        }

        floor_type *floor_ptr = player_ptr->current_floor_ptr;
        while (TRUE) {
            gp[pp_ptr->n++] = location_to_grid(pp_ptr->y, pp_ptr->x);
            if ((pp_ptr->n + (pp_ptr->k >> 1)) >= range)
                break;

            if (!(flg & (PROJECT_THRU))) {
                if ((pp_ptr->x == x2) && (pp_ptr->y == y2))
                    break;
            }

            if (flg & (PROJECT_DISI)) {
                if ((pp_ptr->n > 0) && cave_stop_disintegration(floor_ptr, pp_ptr->y, pp_ptr->x))
                    break;
            } else if (flg & (PROJECT_LOS)) {
                if ((pp_ptr->n > 0) && !cave_los_bold(floor_ptr, pp_ptr->y, pp_ptr->x))
                    break;
            } else if (!(flg & (PROJECT_PATH))) {
                if ((pp_ptr->n > 0) && !cave_have_flag_bold(floor_ptr, pp_ptr->y, pp_ptr->x, FF_PROJECT))
                    break;
            }

            if (flg & (PROJECT_STOP)) {
                if ((pp_ptr->n > 0) && (player_bold(player_ptr, pp_ptr->y, pp_ptr->x) || floor_ptr->grid_array[pp_ptr->y][pp_ptr->x].m_idx != 0))
                    break;
            }

            if (!in_bounds(floor_ptr, pp_ptr->y, pp_ptr->x))
                break;

            if (pp_ptr->m) {
                pp_ptr->frac += pp_ptr->m;
                if (pp_ptr->frac > pp_ptr->half) {
                    pp_ptr->y += pp_ptr->sy;
                    pp_ptr->frac -= pp_ptr->full;
                    pp_ptr->k++;
                }
            }

            pp_ptr->x += pp_ptr->sx;
        }

        return pp_ptr->n;
    }

    pp_ptr->y = y1 + pp_ptr->sy;
    pp_ptr->x = x1 + pp_ptr->sx;

    floor_type *floor_ptr = player_ptr->current_floor_ptr;
    while (TRUE) {
        gp[pp_ptr->n++] = location_to_grid(pp_ptr->y, pp_ptr->x);
        if ((pp_ptr->n + (pp_ptr->n >> 1)) >= range)
            break;

        if (!(flg & PROJECT_THRU)) {
            if ((pp_ptr->x == x2) && (pp_ptr->y == y2))
                break;
        }

        if (flg & PROJECT_DISI) {
            if ((pp_ptr->n > 0) && cave_stop_disintegration(floor_ptr, pp_ptr->y, pp_ptr->x))
                break;
        } else if (flg & PROJECT_LOS) {
            if ((pp_ptr->n > 0) && !cave_los_bold(floor_ptr, pp_ptr->y, pp_ptr->x))
                break;
        } else if (!(flg & PROJECT_PATH)) {
            if ((pp_ptr->n > 0) && !cave_have_flag_bold(floor_ptr, pp_ptr->y, pp_ptr->x, FF_PROJECT))
                break;
        }

        if (flg & PROJECT_STOP) {
            if ((pp_ptr->n > 0) && (player_bold(player_ptr, pp_ptr->y, pp_ptr->x) || floor_ptr->grid_array[pp_ptr->y][pp_ptr->x].m_idx != 0))
                break;
        }

        if (!in_bounds(floor_ptr, pp_ptr->y, pp_ptr->x))
            break;

        pp_ptr->y += pp_ptr->sy;
        pp_ptr->x += pp_ptr->sx;
    }

    return pp_ptr->n;
}
