CC = gcc
CFLAGS = -nostdlib -static -ffreestanding -m32 -Wall -T link.ld -e _start
SRC = test.c
ELF = test.elf
HEADER = test_elf.h

all: $(ELF)

$(ELF): $(SRC)
	$(CC) $(CFLAGS) -o $(ELF) $(SRC)

clean:
	rm -f $(ELF) $(HEADER)