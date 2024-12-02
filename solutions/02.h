#include "../arena.h"
#include "../slice.h"
#include "../str.h"
#include "../types.h"

static void s02a() {}

static void s02b() {}

typedef enum OrderDirection {
  ORDER_UNKNOWN,
  ORDER_INCR,
  ORDER_DECR,
} OrderDirection;

static void s02(Str xs) {
  Arena t = arena_new(2 << 20);
  StrSplit lines = StrSplitBy(&t, xs, '\n');

  usize safeReportsCount = 0;

  for (usize i = 0; i < lines.len; i++) {
    StrSplit levels = StrSplitBy(&t, lines.data[i], ' ');

    b32 first = 1;
    b32 safe = 1;
    u32 prev = 0;
    OrderDirection dir = ORDER_UNKNOWN;

    for (usize j = 0; j < levels.len; j++) {
      StrParseU32 parsed = StrToU32(levels.data[j]);
      assert(!parsed.err);
      u32 cur = parsed.val;
      if (first) {
        first = 0;
        prev = cur;
        continue;
      }
      if (dir == ORDER_UNKNOWN) {
        dir = prev < cur ? ORDER_INCR : ORDER_DECR;
      }
      u32 diff = 0;
      if (dir == ORDER_INCR) {
        if (prev >= cur) {
          safe = 0;
          break;
        }
        diff = cur - prev;
      }
      if (dir == ORDER_DECR) {
        if (prev <= cur) {
          safe = 0;
          break;
        }
        diff = prev - cur;
      }
      if (diff < 1 || diff > 3) {
        safe = 0;
        break;
      }
      prev = cur;
    }

    if (safe) {
      safeReportsCount++;
    }
  }
  printf("%ld\n", safeReportsCount);
}
