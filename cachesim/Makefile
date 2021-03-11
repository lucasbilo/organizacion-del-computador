CC ?= gcc
CFLAGS ?= -g -O2 -std=gnu11 -Wall -Wextra -Wvla
LDLIBS := -lm

cachesim: cachesim.o args.o cache_op.o stats.o

.PHONY: clean