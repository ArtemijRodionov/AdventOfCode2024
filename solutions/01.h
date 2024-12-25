#include "../arena.h"
#include "../math.h"
#include "../slice.h"
#include "../str.h"
#include "../types.h"
#include <stdio.h>
#include <stdlib.h>

static int CmpU32(const void *left, const void *right) {
  u32 f = *((u32 *)left);
  u32 s = *((u32 *)right);
  if (f > s)
    return 1;
  if (f < s)
    return -1;
  return 0;
}

typedef struct SliceU32 SliceU32;
struct SliceU32 {
  u32 *data;
  usize len;
  usize cap;
};

static void s01a(Arena *tmp, SliceU32 left, SliceU32 right) {
  assert(left.len == right.len);
  u32 *leftCopy = arena_alloc_n(tmp, u32, left.len * sizeof(u32));
  u32 *rightCopy = arena_alloc_n(tmp, u32, right.len * sizeof(u32));
  memcpy(leftCopy, left.data, left.len * sizeof(u32));
  memcpy(rightCopy, right.data, right.len * sizeof(u32));
  qsort(leftCopy, left.len, sizeof(u32), CmpU32);
  qsort(rightCopy, right.len, sizeof(u32), CmpU32);

  u32 result = 0;
  for (usize i = 0; i < left.len; i++) {
    result += AbsU32(leftCopy[i], rightCopy[i]);
  }
  printf("%d\n", result);
}

typedef struct hashmap hashmap;
struct hashmap {
  hashmap *child[4];
  u32 key;
  usize value;
};

static u64 hash(u64 s) {
  u64 h = (0x100 ^ s) * 1111111111111111111u;
  return h;
}

static usize *upsert(hashmap **m, u32 key, Arena *perm) {
  for (u64 h = hash(key); *m; h <<= 2) {
    if (key == (*m)->key) {
      return &(*m)->value;
    }
    m = &(*m)->child[h >> 62];
  }
  if (!perm) {
    return 0;
  }
  *m = arena_alloc(perm, hashmap);
  (*m)->key = key;
  return &(*m)->value;
}

static void s01b(Arena *tmp, SliceU32 left, SliceU32 right) {
  assert(left.len == right.len);

  hashmap *h = arena_alloc(tmp, hashmap);
  for (usize i = 0; i < right.len; i++) {
    usize *val = upsert(&h, right.data[i], tmp);
    *val += 1;
  }

  u64 result = 0;
  for (usize i = 0; i < left.len; i++) {
    usize *val = upsert(&h, left.data[i], tmp);
    result += left.data[i] * (*val);
  }

  printf("%ld\n", result);
}

static void s01(Str xs) {
  SliceU32 left = {0};
  SliceU32 right = {0};

  Arena tmp = arena_new(2 << 17);
  StrSplit sp = StrSplitBy(&tmp, xs, '\n');
  for (usize i = 0; i < sp.len; i++) {
    Str line = sp.data[i];
    StrCut cut = StrCutBy(line, ' ');

    StrParseU32 parse = StrToU32(cut.left);
    assert(!parse.err && "Failed parse left");
    // TODO init slice ahead of time
    *slice_push(&left, u32, &tmp) = parse.val;

    parse = StrToU32(StrTrimLeft(cut.right, ' '));
    assert(!parse.err && "Failed parse right");
    *slice_push(&right, u32, &tmp) = parse.val;
  }
  s01a(&tmp, left, right);
  s01b(&tmp, left, right);
  arena_free(&tmp);
}
