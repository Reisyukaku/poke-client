#pragma once

#include "common.hpp"

namespace exl::diag {

    NX_INLINE bool DetectDebugger(void) {
        u64 tmp = 0;
        Result rc = svcGetInfo(&tmp, InfoType_DebuggerAttached, INVALID_HANDLE, 0);
        return R_SUCCEEDED(rc) && tmp != 0;
    }

    NX_INLINE void DebugBreak(void) {
        if(DetectDebugger()) {
            svcBreak(BreakReason_Panic, 0, 0);
        }
    }

    NX_INLINE void DebugBreak(u64 arg1, u64 arg2) {
        if(DetectDebugger()) {
            svcBreak(BreakReason_Panic, arg1, arg2);
        }
    }

}