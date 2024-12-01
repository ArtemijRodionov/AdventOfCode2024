
#ifndef AOC_STR
#define AOC_STR
#include "arena.h"
#include "types.h"

typedef struct Str Str;
struct Str {
  const byte *data;
  usize len;
};

Str StrFrom(const byte *xs);

// cut
typedef struct StrCut StrCut;
struct StrCut {
  Str left;
  Str right;
};
StrCut StrCutAt(Str xs, usize pos);
StrCut StrCutBy(Str xs, byte by);

// split
typedef struct StrSplit StrSplit;
struct StrSplit {
  const Str *data;
  usize len;
};
StrSplit StrSplitBy(Arena *tmp, Str xs, byte by);

// cmp
i32 StrCmp(Str left, Str right);
i32 StrCmpWith(Str left, const byte *right);

// parse
typedef struct StrParseU32 StrParseU32;
struct StrParseU32 {
  u32 val;
  b32 err;
};
StrParseU32 StrToU32(Str xs);
Str StrTrim(Str xs, byte by);
Str StrTrimLeft(Str xs, byte by);
Str StrTrimRight(Str xs, byte by);

// DBG
void StrPrint(Str xs);
#endif
