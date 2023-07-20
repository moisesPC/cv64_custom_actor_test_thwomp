#ifndef CV64_H
#define CV64_H

// Here go struct definitions, enumerations and function declarations that don't have a proper header yet.

#include "math.h"
#include <ultratypes.h>

typedef struct {
    u8 R;
    u8 G;
    u8 B;
    u8 A;
} Color8;

typedef union {
    u32 color_32;
    Color8 color_8;
} color_union;

extern void* ptr_array_8018CDE0[3];             // Needed for modelInfo_createRootNode
extern u8 play_sound_in_position(s16, vec3f*);

#endif
