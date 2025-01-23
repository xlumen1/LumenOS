.PHONY: build clean iso run

kernel := ../src/kernel
build := .
out := out

build:
	$(AS) --32 $(kernel)/boot.s -o $(out)/boot.o
	$(CC) -m32 --static -ffreestanding -c $(kernel)/kernel.c -o $(out)/kernel.o
	$(LD) -m elf_i386 -T linker.ld -o $(build)/kernel.bin -static -nostdlib $(out)/boot.o $(out)/kernel.o

clean:
	rm $(out)/*.o

iso:

run:
	qemu-system-i386 --kernel kernel.bin