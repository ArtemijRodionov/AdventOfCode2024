#include "arena.h"
#include "str.h"
#include <assert.h>
#include <stdio.h>

#define test_eq(l, r)                                                          \
  do {                                                                         \
    b32 res = ((l) == (r));                                                    \
    if (res) {                                                                 \
      printf("OK - %s == %s\n", #l, #r);                                       \
    } else {                                                                   \
      fprintf(stderr, "ERR - %s == %s", #l, #r);                               \
      fprintf(stderr, " (%ld == %ld) == %d\n", l, r, res);                     \
    }                                                                          \
  } while (0);

#define test_lt(l, r)                                                          \
  do {                                                                         \
    b32 res = ((l) < (r));                                                     \
    if (res) {                                                                 \
      printf("OK - %s < %s\n", #l, #r);                                        \
    } else {                                                                   \
      fprintf(stderr, "ERR - %s < %s", #l, #r);                                \
      fprintf(stderr, " (%ld < %ld) == %d\n", l, r, res);                      \
    }                                                                          \
  } while (0);

#define test_gt(l, r)                                                          \
  do {                                                                         \
    b32 res = ((l) > (r));                                                     \
    if (res) {                                                                 \
      printf("OK - %s > %s\n", #l, #r);                                        \
    } else {                                                                   \
      fprintf(stderr, "ERR - %s > %s", #l, #r);                                \
      fprintf(stderr, " (%ld > %ld) == %d\n", l, r, res);                      \
    }                                                                          \
  } while (0);

int main(void) {
  Str str = StrFrom("hello");
  test_eq(str.len, 5);
  test_eq(StrCmp(str, str), 0);
  test_eq(StrCmpWith(str, "hello"), 0);
  test_lt(StrCmpWith(str, "hello1"), 0);
  test_gt(StrCmpWith(str, "hell"), 0);

  StrCut cut = StrCutAt(str, 2);
  test_eq(cut.left.len, 2);
  test_eq(cut.right.len, 3);
  test_eq(StrCmpWith(cut.left, "he"), 0);
  test_eq(StrCmpWith(cut.right, "llo"), 0);

  cut = StrCutAt(str, 4);
  test_eq(cut.left.len, 4);
  test_eq(cut.right.len, 1);
  test_eq(StrCmpWith(cut.left, "hell"), 0);
  test_eq(StrCmpWith(cut.right, "o"), 0);

  cut = StrCutBy(str, 'l');
  test_eq(cut.left.len, 2);
  test_eq(cut.right.len, 3);
  test_eq(StrCmpWith(cut.left, "he"), 0);
  test_eq(StrCmpWith(cut.right, "llo"), 0);

  cut = StrCutBy(str, 5);
  test_eq(cut.left.len, 5);
  test_eq(cut.right.len, 0);
  test_eq(StrCmpWith(cut.left, "hello"), 0);
  test_eq(StrCmpWith(cut.right, ""), 0);

  Str num = StrFrom("31");
  StrParseU32 parsed = StrToU32(num);
  test_eq(parsed.err, 0);
  test_eq(parsed.val, 31);

  num = StrFrom("031");
  parsed = StrToU32(num);
  test_eq(parsed.err, 0);
  test_eq(parsed.val, 31);

  num = StrFrom("3");
  parsed = StrToU32(num);
  test_eq(parsed.err, 0);
  test_eq(parsed.val, 3);

  num = StrFrom(" 031");
  parsed = StrToU32(num);
  test_eq(parsed.err, 1);

  num = StrFrom("031 ");
  parsed = StrToU32(num);
  test_eq(parsed.err, 1);

  num = StrFrom("   031");
  Str skipped = StrTrimLeft(num, ' ');
  test_eq(StrCmp(skipped, StrFrom("031")), 0);

  num = StrFrom("   031   ");
  skipped = StrTrim(num, ' ');
  test_eq(StrCmp(skipped, StrFrom("031")), 0);

  Arena a = arena_new(2 << 6);
  Str toSplit = StrFrom("1 2\n3 4\n5 6");
  StrSplit splitted = StrSplitBy(&a, toSplit, '\n');
  test_eq(splitted.len, 3);
  test_eq(StrCmp(splitted.data[0], StrFrom("1 2")), 0);
  test_eq(StrCmp(splitted.data[1], StrFrom("3 4")), 0);
  test_eq(StrCmp(splitted.data[2], StrFrom("5 6")), 0);
  arena_free(&a);
}
