.PHONY: build clean iso run

CC := gcc
AS := as
LD := ld

CCFLAGS := -m32 --static -ffreestanding -c
LDFLAGS := -m elf_i386 -T linker.ld -static -nostdlib

kernel := ../src/kernel
libk := $(kernel)/libk
build := .
out := $(build)/out
iso := iso

libk_src := $(wildcard $(libk)/**/*.c)
libk_objs := $(patsubst $(libk)/%.c,$(out)/libk/%.o,$(libk_src))

kernel_src := $(kernel)/kernel.c
kernel_objs := $(out)/kernel.o

all_objs := $(out)/boot.o $(kernel_objs) $(libk_objs)

build: $(all_objs)
	$(LD) $(LDFLAGS) -o $(build)/kernel.bin $(all_objs)

$(out)/libk/%.o: $(libk)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $< -o $@

$(out)/boot.o: $(kernel)/boot.s
	@mkdir -p $(out)
	$(AS) --32 $< -o $@

$(out)/kernel.o: $(kernel_src)
	$(CC) $(CCFLAGS) $< -o $@

clean:
	rm -rf $(build)

iso: build
	@mkdir -p $(iso)/boot
	cp $(build)/kernel.bin $(iso)/boot/kernel.bin
	grub-mkrescue -o $(build)/os.iso $(iso)

run: build
	qemu-system-i386 --kernel $(build)/kernel.bin
