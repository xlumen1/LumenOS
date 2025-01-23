.PHONY: build clean iso run

CCFLAGS := -m32 --static -ffreestanding -c
LDFLAGS := -m elf_i386 -T linker.ld -static -nostdlib

kernel := ../src/kernel
build := .
out := out
iso := iso/



build:
	mkdir -p $(out)
	$(AS) --32 $(kernel)/boot.s -o $(out)/boot.o
	$(CC) $(CCFLAGS) $(kernel)/kernel.c -o $(out)/kernel.o
	$(LD) $(LDFLAGS) -o $(build)/kernel.bin $(out)/boot.o $(out)/kernel.o
	$(MAKE) clean

clean:
	rm -f $(out)/*.o
	rmdir $(out)

iso: build
	cp $(build)/kernel.bin $(iso)/boot/kernel.bin
	grub-mkrescue -o $(build)/os.iso iso

run:
	qemu-system-i386 --kernel kernel.bin