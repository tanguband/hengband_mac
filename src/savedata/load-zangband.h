﻿#pragma once

#include "dungeon/quest.h"
#include "system/angband.h"

void load_zangband_options(void);
void set_zangband_realm(player_type *creature_ptr);
void set_zangband_skill(player_type *creature_ptr);
void set_zangband_spells(player_type *creature_ptr);
void set_zangband_race(player_type *creature_ptr);
void set_zangband_bounty_uniques(player_type *creature_ptr);
void set_zangband_timed_effects(player_type *creature_ptr);
void set_zangband_mimic(player_type *creature_ptr);
void set_zangband_holy_aura(player_type *creature_ptr);
void set_zangband_reflection(player_type *creature_ptr);
void rd_zangband_dungeon(void);
void set_zangband_game_turns(player_type *creature_ptr);
void set_zangband_gambling_monsters(int i);
void set_zangband_special_attack(player_type *creature_ptr);
void set_zangband_special_defense(player_type *creature_ptr);
void set_zangband_action(player_type *creature_ptr);
void set_zangband_visited_towns(player_type *creature_ptr);
void set_zangband_quest(player_type *creature_ptr, quest_type *const q_ptr, int loading_quest_index, const QUEST_IDX old_inside_quest);
void set_zangband_class(player_type *creature_ptr);
void set_zangband_learnt_spells(player_type *creature_ptr);
void set_zangband_pet(player_type *creature_ptr);
