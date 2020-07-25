﻿#pragma once

#include "system/angband.h"

extern MONSTER_IDX target_who;
extern POSITION target_col;
extern POSITION target_row;

void verify_panel(player_type *creature_ptr);
bool target_okay(player_type *creature_ptr);
bool get_aim_dir(player_type *creature_ptr, DIRECTION *dp);
bool get_direction(player_type *creature_ptr, DIRECTION *dp, bool allow_under, bool with_steed);
bool get_rep_dir(player_type *creature_ptr, DIRECTION *dp, bool under);
