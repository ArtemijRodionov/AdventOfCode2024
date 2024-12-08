#ifndef AOC_MATH
#define AOC_MATH
#include "types.h"

static u32 AbsU32(u32 left, u32 right) {
  if (left < right)
    return right - left;
  return left - right;
}

#endif
