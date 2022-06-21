#pragma once

#include "types.h"

u32 clamp(u32 value, u32 low, u32 high) {
    if (value < low) {
        return low;
    }
    if (value > high) {
        return high;
    }
    return value;
}