#ifndef AOC_SLICE
#define AOC_SLICE

#include "arena.h"
#include "types.h"

static void slice_grow(void *slice, usize size, isize align, Arena *a) {
  struct {
    void *data;
    usize len;
    usize cap;
  } replica;
  memcpy(&replica, slice, sizeof(replica));

  if (!replica.data) {
    replica.cap = 1;
    replica.data = arena_alloc_padded(a, replica.cap, 2 * size, align);
  } else if ((char *)a->ptr == (char *)replica.data + size * replica.cap) {
    arena_alloc_padded(a, replica.cap, size, align);
  } else {
    void *data = arena_alloc_padded(a, replica.cap, 2 * size, align);
    memcpy(data, replica.data, size * replica.len);
    replica.data = data;
  }

  replica.cap *= 2;
  memcpy(slice, &replica, sizeof(replica));
}

#define slice_push(s, t, arena)                                                \
  ((s)->len >= (s)->cap   ? slice_grow(s, sizeof(t), _Alignof(t), arena),      \
   (s)->data + (s)->len++ : (s)->data + (s)->len++)

#endif // !AOC_SLICE
