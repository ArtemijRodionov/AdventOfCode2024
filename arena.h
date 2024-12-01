#ifndef AOC_ARENA
#define AOC_ARENA
#include "types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Arena {
  void *ptr;
  isize pos;
  isize size;
} Arena;

Arena arena_new(isize size);
void arena_free(Arena *arena);
void arena_clear(Arena *arena);
void *arena_alloc_padded(Arena *arena, usize count, usize size, isize align);

#define arena_alloc(a, t) (t *)arena_alloc_padded(a, 1, sizeof(t), _Alignof(t))

#define arena_alloc_n(a, t, n)                                                 \
  (t *)arena_alloc_padded(a, n, sizeof(t), _Alignof(t))

#define arena_alloc_from(a, t, v) (t *)memcpy(arena_alloc(a, t), &v, sizeof(v))

#endif // !AOC_ARENA
