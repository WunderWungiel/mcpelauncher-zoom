#pragma once
#include <cmath>

// Zoomify's EASE_OUT_EXP transition (utils/TransitionType.kt):
//   1 - 2^(10 - log2(1023) - 10t) + 1/1023
// written without the log2 constant so the endpoints land exactly on 0 and 1.
inline double easeOutExp(double t) {
    if(t <= 0.0) {
        return 0.0;
    }
    if(t >= 1.0) {
        return 1.0;
    }
    return 1.0 - (1024.0 / 1023.0) * std::pow(2.0, -10.0 * t) + 1.0 / 1023.0;
}
