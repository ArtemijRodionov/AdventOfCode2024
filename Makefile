CFLAGS =

run: run.c arena.h arena.c str.h str.c slice.h types.h $(wildcard solutions/*.h)
	cc $(CFLAGS) -std=c11 run.c arena.c str.c -o run

test: str.c arena.c arena.h slice.h test.c
	cc -std=c11 test.c str.c arena.c -Wno-format -o test && ./test


