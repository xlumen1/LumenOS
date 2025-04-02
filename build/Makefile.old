.PHONY: build clean iso run

CC := gcc
AS := as
LD := ld

CCFLAGS := -m32 --static -ffreestanding -c -g
LDFLAGS := -m elf_i386 -T linker.ld -static -nostdlib

kernel := ../src/kernel
libk := $(kernel)/libk
build := .
out := $(build)/out
iso := iso

libk_src := $(wildcard $(libk)/**/*.c)
libk_objs := $(patsubst $(libk)/%.c,$(out)/libk/%.o,$(libk_src))

vga_src := $(wildcard $(libk)/vga/**/*.c)
vga_objs := $(patsubst $(libk)/vga/%.c,$(out)/libk/vga/%.o,$(vga_src))

gdt_src := $(wildcard $(kernel)/gdt/*.c)
gdt_objs := $(patsubst $(kernel)/gdt/%.c,$(out)/gdt/%.o,$(gdt_src))

time_src := $(wildcard $(libk)/time/*.c)
time_objs := $(patsubst $(libk)/time/%.c,$(out)/libk/time/%.o,$(time_src))

libk_a_src := $(wildcard $(libk)/**/*.s)
libk_a_objs := $(patsubst $(libk)/%.s,$(out)/libk/%.o,$(libk_a_src))

kernel_src := $(kernel)/kernel.c
kernel_objs := $(out)/kernel.o

all_objs := $(out)/boot.o $(kernel_objs) $(libk_objs) $(libk_a_objs) $(gdt_objs) $(vga_objs)

build: $(all_objs)
	$(LD) $(LDFLAGS) -o $(build)/kernel.bin $(all_objs)

$(out)/libk/%.o: $(libk)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $< -o $@

$(out)/boot.o: $(kernel)/boot.s
	@mkdir -p $(out)
	$(AS) --32 $< -o $@

$(out)/libk/util/%.o: $(libk)/util/%.s
	@mkdir -p $(dir $@)
	$(AS) --32 $< -o $@

$(out)/libk/vga/%.o: $(libk)/vga/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $< -o $@

$(out)/gdt/%.o: $(kernel)/gdt/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $< -o $@

$(out)/libk/time/%.o: $(libk)/time/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $< -o $@

$(out)/kernel.o: $(kernel_src)
	$(CC) $(CCFLAGS) $< -o $@

clean:
	rm -fr $(build)/out

iso: build
	@mkdir -p $(iso)/boot
	cp $(build)/kernel.bin $(iso)/boot/kernel.bin
	grub-mkrescue -o $(build)/os.iso $(iso)

run: build
	qemu-system-i386 --kernel $(build)/kernel.bin

debug: 
	qemu-system-i386 -s -S --kernel $(build)/kernel.bin