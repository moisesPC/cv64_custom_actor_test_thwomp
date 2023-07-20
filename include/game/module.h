#ifndef MODULE_H
#define MODULE_H

#include <ultratypes.h>

typedef struct {
    u8 timer;                      // Could also be "number of accesses to function"
    u8 function;                   // ID within the functions array
} functionInfo;                    // Size = 0x2

typedef struct {
    s16 ID;
    s16 flags;
    s16 field_0x04;
    s16 field_0x06;
    functionInfo current_function[3];
    s16 functionInfo_ID;
    void (*destroy)(void*);
    struct ModuleHeader* parent;
    struct ModuleHeader* next;
    struct ModuleHeader* child;
} ModuleHeader;                    // Size = 0x20

extern void goToNextFunc(u16 current_functionInfo[], s16* functionInfo_ID);
extern void goToFunc(u16 current_functionInfo[], s16* functionInfo_ID, s32 function);

#endif
