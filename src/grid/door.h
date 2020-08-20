#pragma once

#include "system/angband.h"

void add_door(player_type *player_ptr, POSITION x, POSITION y);
void place_secret_door(player_type *player_ptr, POSITION y, POSITION x, int type);
void place_locked_door(player_type *player_ptr, POSITION y, POSITION x);
