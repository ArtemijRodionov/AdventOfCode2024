
#include "arena.h"
#include "types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Arena arena_new(isize size) {
  assert(size > 0);
  void *mem = malloc(size);
  if (mem == NULL) {
    fprintf(stderr, "can't alloc mem for arena\n");
    exit(1);
  }
  return (Arena){.ptr = mem, .pos = 0, .size = size};
}

void arena_free(Arena *arena) {
  if (arena == NULL || arena->ptr == NULL) {
    return;
  }
  free(arena->ptr);
  arena->ptr = NULL;
  arena->pos = 0;
  arena->size = 0;
}

void arena_clear(Arena *arena) {
  if (arena == NULL || arena->ptr == NULL) {
    return;
  }
  memset(arena->ptr, 0, arena->pos);
  arena->pos = 0;
}

void *arena_alloc_padded(Arena *arena, usize count, usize size, isize align) {
  if (arena == NULL || arena->ptr == NULL) {
    return NULL;
  }

  void *start = (char *)arena->ptr + arena->pos;
  isize padding = -(uintptr_t)start & (align - 1);
  isize occupy = count * size;

  if (arena->pos + padding + occupy > arena->size) {
    fprintf(stderr, "arena has no space left for this allocation: %zu > %zu\n",
            arena->pos + padding + occupy, arena->size);
    exit(1);
  }

  arena->pos += padding + occupy;
  void *padded_start = (char *)start + padding;
  return memset(padded_start, 0, occupy);
}
