#ifndef UTIL_H_
#define UTIL_H_

#include "linalg/linalg.h"

const double PI = 3.141592653589793238463;

using vec = linalg::aliases::double3;

template<typename T>
inline T clamp(T max, T min, T value) {
    return std::max(max, std::min(min, value));
}



#endif
