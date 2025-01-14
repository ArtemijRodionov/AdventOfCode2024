CFLAGS = -std=c11 -Werror -Wall -Wextra -Wpedantic
EXTRA = -fsanitize=address,undefined,leak

run: run.c arena.h arena.c str.h str.c slice.h types.h $(wildcard solutions/*.h)
	cc $(CFLAGS) $(EXTRA) run.c arena.c str.c -o run

test: str.c arena.c arena.h slice.h test.c
	cc $(CFLAGS) $(EXTRA) test.c str.c arena.c -Wno-format -o test && ./test


