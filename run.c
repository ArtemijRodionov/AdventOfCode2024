
#include "solutions/01.h"
#include "solutions/02.h"
#include "solutions/03.h"

#include "str.h"
#include "types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct SolutionSelector SolutionSelector;
typedef enum SolutionPart SolutionPart;

void solutionOneA(Str input) { printf("%s", input.data); }

typedef struct Solution Solution;
struct Solution {
  char *mark;
  void (*fn)(Str);
};

static Solution solutions[] = {
    {.mark = "01", .fn = s01},
    {.mark = "02", .fn = s02},
    {.mark = "03", .fn = s03},
};

Solution findSolution(const char *val) {
  Str lookingFor = StrFrom(val);
  for (usize i = 0; i < sizeof(solutions) / sizeof(Solution); i++) {
    if (StrCmp(StrFrom(solutions[i].mark), lookingFor) == 0) {
      return solutions[i];
    }
  }
  assert(0 && "Failed to find solution");
}

int main(int argc, char *argv[]) {
  assert(argc >= 2 && "specify number for solution");

  printf("Looking for %s solution\n", argv[1]);
  Solution s = findSolution(argv[1]);

  char defaultPath[100] = {0};
  snprintf(defaultPath, 100, "data/%s", s.mark);
  char *path = defaultPath;
  if (argc > 2) {
    path = argv[2];
  }

  FILE *file = fopen(path, "r");
  assert(file && "Failed to open file");
  fseek(file, 0, SEEK_END);
  u64 size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *string = malloc(size + 1);
  fread(string, size, 1, file);
  fclose(file);
  Str input = StrFrom(string);
  s.fn(input);
  free(string);
  return 0;
}
