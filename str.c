#include "str.h"
#include "arena.h"
#include "slice.h"
#include "types.h"
#include <string.h>

Str StrFrom(const byte *xs) { return (Str){.data = xs, .len = strlen(xs)}; }

StrCut StrCutAt(Str xs, usize pos) {
  if (pos >= xs.len) {
    return (StrCut){
        .left = xs,
        .right = {.data = 0, .len = 0},
    };
  }
  return (StrCut){
      .left = {.data = xs.data, .len = pos},
      .right = {.data = xs.data + pos, .len = xs.len - pos},
  };
}

StrCut StrCutBy(Str xs, byte by) {
  for (usize i = 0; i < xs.len; i++) {
    if (xs.data[i] == by)
      return StrCutAt(xs, i);
  }
  return StrCutAt(xs, xs.len);
}

StrSplit StrSplitBy(Arena *tmp, Str xs, byte by) {
  struct Slice {
    Str *data;
    usize len;
    usize cap;
  } slice = {0};

  StrCut cut = {0};
  do {
    cut = StrCutBy(xs, by);
    *slice_push(&slice, Str, tmp) = cut.left;
    xs = StrCutAt(cut.right, 1).right;
  } while (xs.len > 0);

  return (StrSplit){
      .data = slice.data,
      .len = slice.len,
  };
}

i32 StrCmp(Str left, Str right) {
  i32 diff = 0;
  if ((diff = left.len - right.len) != 0) {
    return diff;
  }
  for (usize i = 0; i < left.len; i++) {
    if ((diff = left.data[i] - right.data[i]) != 0) {
      return diff;
    }
  }
  return 0;
}

i32 StrCmpWith(Str left, const byte *right) {
  return StrCmp(left, StrFrom(right));
}

StrParseU32 StrToU32(Str xs) {
  u32 val = 0;

  usize i = 0;
  i32 diff = 0;

  // skip leading zeros
  for (; i < xs.len; i++) {
    if ((diff = xs.data[i] - '0') == 0) {
      continue;
    }
    if (diff < 0 || diff > 9) {
      return (StrParseU32){.err = 1};
    }
    val += diff;
    i++;
    break;
  }

  for (; i < xs.len; i++) {
    diff = xs.data[i] - '0';
    if (diff < 0 || diff > 9) {
      return (StrParseU32){.err = 1};
    }
    val = val * 10 + diff;
  }
  return (StrParseU32){.val = val};
}

Str StrTrimLeft(Str xs, byte val) {
  for (usize i = 0; i < xs.len; i++) {
    if (xs.data[i] == val) {
      continue;
    }
    return StrCutAt(xs, i).right;
  }
  return StrCutAt(xs, xs.len).right;
}

Str StrTrimRight(Str xs, byte val) {
  for (isize i = xs.len - 1; i >= 0; i--) {
    if (xs.data[i] == val) {
      continue;
    }
    return StrCutAt(xs, i + 1).left;
  }
  return StrCutAt(xs, xs.len).right;
}

Str StrTrim(Str xs, byte val) {
  return StrTrimRight(StrTrimLeft(xs, val), val);
}

void StrPrint(Str xs) {
  fprintf(stderr, "\"");
  for (usize i = 0; i < xs.len; i++) {
    fprintf(stderr, "%c", xs.data[i]);
  }
  fprintf(stderr, "\" ");
}
