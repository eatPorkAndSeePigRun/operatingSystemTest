all: main

main: main.c factorial.c
	$(CC) $^ $(CCFLAGS) -o $@

clean:
	-rm main

.PHONY: clean

CCFLAGS = -w

