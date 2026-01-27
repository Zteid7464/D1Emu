SRC := src/main.c src/emu.c
HEADERS := src/emu.h

CFLAGS := -Wall -Wextra -std=c11

d1emu: $(SRC) $(HEADERS)
	gcc $(SRC) -o d1emu $(CFLAGS)