#include "../str.h"
#include "../types.h"
#include <stdio.h>

typedef struct s03Parser s03Parser;
struct s03Parser {
  usize pos;
  Str text;
};

static b32 s03Consume(s03Parser *p, Str val) {
  for (usize i = 0; i < val.len; i++) {
    if (p->pos >= p->text.len)
      exit(1);
    if (p->text.data[p->pos] != val.data[i]) {
      return 0;
    }
    p->pos++;
  }
  return 1;
}

static b32 s03ConsumeWhole(s03Parser *p, Str val) {
  usize pos = p->pos;
  for (usize i = 0; i < val.len; i++) {
    if (pos >= p->text.len)
      exit(1);
    if (p->text.data[pos] != val.data[i]) {
      return 0;
    }
    pos++;
  }
  p->pos = pos;
  return 1;
}

static i32 s03GetDigit(s03Parser *p) {
  i32 val = 0;
  usize i = 0;
  for (; i < p->text.len; i++) {
    if (p->pos >= p->text.len)
      exit(1);
    i32 t = p->text.data[p->pos] - '0';
    if (t < 0 || t > 9) {
      break;
    }
    val = val * 10 + t;
    p->pos++;
  }
  return i == 0 ? -1 : val;
}

static void s03Parse(Str xs, b32 parseDo) {
  s03Parser p = {.text = xs};
  Str mul = StrFrom("mul(");
  Str comma = StrFrom(",");
  Str par = StrFrom(")");
  Str ido = StrFrom("do()");
  Str idont = StrFrom("don't()");

  b32 allowed = 1;

  u32 result = 0;
  while (p.pos + 8 < p.text.len) {
    if (parseDo) {
      if (s03ConsumeWhole(&p, ido)) {
        allowed = 1;
      } else if (s03ConsumeWhole(&p, idont)) {
        allowed = 0;
      }
    }

    if (!s03Consume(&p, mul)) {
      p.pos++;
      continue;
    }

    i32 left = s03GetDigit(&p);
    if (left == -1)
      continue;

    if (!s03Consume(&p, comma))
      continue;

    i32 right = s03GetDigit(&p);
    if (right == -1)
      continue;

    if (!s03Consume(&p, par))
      continue;

    if (!parseDo || allowed)
      result += left * right;
  }
  printf("%d\n", result);
}

static void s03(Str xs) {
  s03Parse(xs, 0);
  s03Parse(xs, 1);
}
