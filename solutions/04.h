#include "../arena.h"
#include "../slice.h"
#include "../str.h"
#include "../types.h"

static void s04a(Str xs) {
  Arena tmp = arena_new(2 << 12);
  StrSplit lines = StrSplitBy(&tmp, xs, '\n');

  byte xmas[] = {'X', 'M', 'A', 'S'};
  isize lenY = lines.len;
  isize lenX = lines.data[0].len;

  usize count = 0;
  for (isize y = 0; y < lenY; y++) {
    for (isize x = 0; x < lenX; x++) {
      for (isize dy = -1; 1 >= dy; dy++) {
        for (isize dx = -1; 1 >= dx; dx++) {
          isize newX = x;
          isize newY = y;

          b32 valid = 1;
          for (isize ci = 0, clen = sizeof(xmas) / sizeof(xmas[0]); ci < clen;
               ci++) {
            if (newX < 0 || newY < 0 || newX >= lenX || newY >= lenY ||
                xmas[ci] != lines.data[newY].data[newX]) {
              valid = 0;
              break;
            }
            newY += dy;
            newX += dx;
          }
          if (valid) {
            count++;
          }
        }
      }
    }
  }
  printf("%zu\n", count);
  arena_free(&tmp);
}

static b32 s04bcheck(StrSplit lines, isize lenX, isize lenY, isize x, isize y,
                     byte expected) {
  return x >= 0 && y >= 0 && x < lenX && y < lenY &&
         lines.data[y].data[x] == expected;
}

static void s04b(Str xs) {
  Arena tmp = arena_new(2 << 12);
  StrSplit lines = StrSplitBy(&tmp, xs, '\n');

  isize lenY = lines.len;
  isize lenX = lines.data[0].len;

  usize count = 0;
  for (isize y = 0; y < lenY; y++) {
    for (isize x = 0; x < lenX; x++) {
      b32 lmas = s04bcheck(lines, lenX, lenY, x, y, 'M') &&
                 s04bcheck(lines, lenX, lenY, x + 1, y + 1, 'A') &&
                 s04bcheck(lines, lenX, lenY, x + 2, y + 2, 'S');
      b32 lsam = s04bcheck(lines, lenX, lenY, x, y, 'S') &&
                 s04bcheck(lines, lenX, lenY, x + 1, y + 1, 'A') &&
                 s04bcheck(lines, lenX, lenY, x + 2, y + 2, 'M');

      if (!(lmas || lsam))
        continue;

      b32 rmas = s04bcheck(lines, lenX, lenY, x + 2, y, 'M') &&
                 s04bcheck(lines, lenX, lenY, x + 1, y + 1, 'A') &&
                 s04bcheck(lines, lenX, lenY, x, y + 2, 'S');
      b32 rsam = s04bcheck(lines, lenX, lenY, x + 2, y, 'S') &&
                 s04bcheck(lines, lenX, lenY, x + 1, y + 1, 'A') &&
                 s04bcheck(lines, lenX, lenY, x, y + 2, 'M');

      if (!(rmas || rsam))
        continue;

      count++;
    }
  }
  printf("%zu\n", count);
  arena_free(&tmp);
}

static void s04(Str xs) {
  s04a(xs);
  s04b(xs);
}
