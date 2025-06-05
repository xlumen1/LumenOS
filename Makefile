.PHONY: build clean iso run debug all

# Tools
CC := gcc
AS := as
LD := ld
GRUB := grub-mkrescue

# Directories
KERNEL := src/kernel
BUILD := build
STDLIB := src/stdlib
OUT := $(BUILD)/out
ISO := $(BUILD)/iso

# Flags
CCFLAGS := -m32 --static -ffreestanding -c -g -nostdinc -I$(KERNEL) -I$(STDLIB) -Wextra -Wall
LDFLAGS := -m elf_i386 -T $(BUILD)/linker.ld -static -nostdlib -g

# Build files
SRC :=  $(shell find $(KERNEL) -type f -name "*.c") $(shell find $(STDLIB) -type f -name "*.c") $(shell find $(KERNEL) -type f -name "*.s")
OBJ :=  $(patsubst %.c,$(OUT)/%.o,$(filter %.c,$(SRC))) \
    	$(patsubst %.s,$(OUT)/%.o,$(filter %.s,$(SRC)))

build: $(OBJ) fs
	$(LD) $(LDFLAGS) -o $(BUILD)/kernel.bin $(OBJ)

$(OUT)/$(KERNEL)/%.o: $(KERNEL)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $< -o $@

$(OUT)/$(STDLIB)/%.o: $(STDLIB)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $< -o $@

$(OUT)/$(KERNEL)/%.o: $(KERNEL)/%.s
	mkdir -p $(dir $@)
	$(AS) --32 -g $< -o $@

clean:
	rm -fr $(OUT) $(BUILD)/kernel.bin $(BUILD)/install.iso $(BUILD)/iso/boot/kernel.bin $(BUILD)/iso/boot/fs.img

iso: build
	mkdir -p $(ISO)/boot/grub
	cp $(BUILD)/kernel.bin $(ISO)/boot/kernel.bin
	cp $(OUT)/fs.img $(ISO)/boot/fs.img
	$(GRUB) -o $(BUILD)/install.iso $(ISO)

run: iso
	qemu-system-i386 --cdrom $(BUILD)/os.iso -serial stdio

all: build run

debug: iso
	qemu-system-i386 -s -S --cdrom $(BUILD)/os.iso &
	gdb $(BUILD)/kernel.bin --silent -ex "target remote localhost:1234"

fs: $(OUT)/fs.img

$(OUT)/fs.img:
	mkdir -p $(OUT)
	util/fs/createstd.sh
	mv util/fs/fs.img $@