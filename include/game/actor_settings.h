#ifndef ACTOR_SETTINGS_H
#define ACTOR_SETTINGS_H

#include "math.h"
#include <ultratypes.h>

#define END_ACTOR_LIST 0xFFFF

typedef struct {
    vec3s position;
    s16 actor_ID;
    u16 variable_1;
    u16 variable_2;
    u16 variable_3;
    u8 difficulty__spawn_setting_1;
    u8 spawn_setting_2;
} actor_settings; 

#endif
