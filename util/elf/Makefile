CC = gcc
CFLAGS = -nostdlib -static -ffreestanding -m32 -Wall -Wl,-N -T link.ld -e _start -g
SRC = test.c
ELF = test.elf
HEADER = test_elf.h

all: $(ELF)

$(ELF): $(SRC)
	$(CC) $(CFLAGS) -o $(ELF) $(SRC)

clean:
	rm -f $(ELF) $(HEADER)