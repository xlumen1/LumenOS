CC = gcc
CFLAGS = -nostdlib -static -ffreestanding -m32 -Wall -Wextra -g -c
LDFLAGS = -m32 -nostdlib -static -T link.ld -e _start -Wl,-N

SRC = test.c
OBJ = test.o
START_SRC = _start.c
START_OBJ = _start.o
ELF = test.elf
HEADER = test_elf.h

all: $(ELF)

# Compile .c files into .o files
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ)

$(START_OBJ): $(START_SRC)
	$(CC) $(CFLAGS) $(START_SRC) -o $(START_OBJ)

# Link the final ELF, putting _start.o first
$(ELF): $(START_OBJ) $(OBJ)
	$(CC) $(LDFLAGS) -o $(ELF) $(START_OBJ) $(OBJ)

clean:
	rm -f $(ELF) $(OBJ) $(START_OBJ) $(HEADER)
