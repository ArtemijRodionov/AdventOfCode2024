#include "../arena.h"
#include "../math.h"
#include "../slice.h"
#include "../str.h"
#include "../types.h"

static b32 s02check(StrSplit levels, usize skip) {
  typedef enum OrderDirection {
    ORDER_UNKNOWN,
    ORDER_INCR,
    ORDER_DECR,
  } OrderDirection;

  usize begin = 0;

  if (skip == begin)
    begin++;

  StrParseU32 parsed = StrToU32(levels.data[begin]);
  assert(!parsed.err);
  u32 prev = parsed.val;

  OrderDirection dir = ORDER_UNKNOWN;

  for (usize j = begin + 1; j < levels.len; j++) {
    if (j == skip)
      continue;

    StrParseU32 parsed = StrToU32(levels.data[j]);
    u32 cur = parsed.val;

    OrderDirection levelDir = prev < cur ? ORDER_INCR : ORDER_DECR;
    if (dir != ORDER_UNKNOWN && dir != levelDir) {
      return 0;
    }

    u32 diff = AbsU32(cur, prev);
    if (diff < 1 || diff > 3) {
      return 0;
    }

    prev = cur;
    dir = levelDir;
  }

  return 1;
}

static void s02AllowSkip(Str xs, b32 allowedSkips) {
  Arena t = arena_new(2 << 20);
  StrSplit lines = StrSplitBy(&t, xs, '\n');

  usize safeReportsCount = 0;

  for (usize i = 0; i < lines.len; i++) {
    StrSplit levels = StrSplitBy(&t, lines.data[i], ' ');

    if (s02check(levels, levels.len)) {
      safeReportsCount++;
    } else if (allowedSkips) {
      for (usize i = 0; i < levels.len; i++) {
        if (s02check(levels, i)) {
          safeReportsCount++;
          break;
        }
      }
    }
  }
  printf("%ld\n", safeReportsCount);
}

static void s02(Str xs) {
  s02AllowSkip(xs, 0);
  s02AllowSkip(xs, 1);
}
