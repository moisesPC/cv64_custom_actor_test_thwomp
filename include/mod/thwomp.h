#ifndef THWOMP_H
#define THWOMP_H

#include "model_info.h"
#include "module.h"
#include "atari_work.h"
#include "thwomp_constants.h"
#include "actor_settings.h"

#define SIZE_MULTIPLIER self->settings->variable_3

typedef struct {
    ModuleHeader header;
    u8 field_0x20[4];               // Necessary padding because "model" should be field 0x24
    model_info* model;
    atari_data_work* collider;
    u32 timer;
    f32 fallingSpeed;
    u32 movingTime;
    u32 waitTime;
    u8 field_0x3C[0x70 - 0x3C];     // Necessary padding because "settings" should be field 0x70
    actor_settings* settings;
} thwomp;

void thwomp_entrypoint(thwomp* self);
void thwomp_init(thwomp* self);
void thwomp_rise_up(thwomp* self);
void thwomp_stay_on_top(thwomp* self);
void thwomp_fall(thwomp* self);
void thwomp_hit_ground(thwomp* self);

void (*thwomp_functions[])(thwomp* self) = {
    thwomp_init,
    thwomp_rise_up,
    thwomp_stay_on_top,
    thwomp_fall,
    thwomp_hit_ground,
};

enum thwomp_functions_enum {
    THWOMP_INIT,
    THWOMP_RISE_UP,
    THWOMP_STAY_ON_TOP,
    THWOMP_FALL,
    THWOMP_HIT_GROUND,
};

#endif
