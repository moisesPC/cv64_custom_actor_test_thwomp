#ifndef ULTRA64_H
#define ULTRA64_H

#include "ultra64/ultratypes.h"
#include "ultra64/exception.h"
#include "ultra64/rcp.h"
#include "ultra64/thread.h"
#include "ultra64/convert.h"
#include "ultra64/time.h"
#include "ultra64/message.h"
#include "ultra64/sptask.h"
#include "ultra64/gu.h"
#include "ultra64/vi.h"
#include "ultra64/pi.h"
#include "ultra64/controller.h"
#include "ultra64/printf.h"
#include "ultra64/mbi.h"
#include "ultra64/pfs.h"
#include "ultra64/motor.h"
#include "ultra64/R4300.h"
#include "ultra64/ucode.h"

#define VTX(x,y,z,s,t,crnx,cgny,cbnz,a) { { { x, y, z }, 0, { s, t }, { crnx, cgny, cbnz, a } } }

#endif
