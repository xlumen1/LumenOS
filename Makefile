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
CCFLAGS := -m32 --static -ffreestanding -c -g -nostdinc -I$(KERNEL)/libk -I$(KERNEL) -I$(STDLIB)
LDFLAGS := -m elf_i386 -T $(BUILD)/linker.ld -static -nostdlib -g

# Build files
SRC := $(shell find $(KERNEL) -type f -name "*.c") $(shell find $(KERNEL) -type f -name "*.s")
OBJ := $(patsubst $(KERNEL)/%.c,$(OUT)/%.o,$(filter %.c,$(SRC))) \
       $(patsubst $(KERNEL)/%.s,$(OUT)/%.o,$(filter %.s,$(SRC)))

all: iso run

build: $(OBJ)
	$(LD) $(LDFLAGS) -o $(BUILD)/kernel.bin $(OBJ)

$(OUT)/%.o: $(KERNEL)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $< -o $@

$(OUT)/%.o: $(KERNEL)/%.s
	$(AS) --32 -g $< -o $@

clean:
	rm -fr $(OUT) $(BUILD)/kernel.bin $(BUILD)/os.iso

iso: build
	mkdir -p $(ISO)/boot/grub
	cp $(BUILD)/kernel.bin $(ISO)/boot/kernel.bin
	$(GRUB) -o $(BUILD)/os.iso $(ISO)

run: build
	qemu-system-i386 --kernel $(BUILD)/kernel.bin &

debug: build
	qemu-system-i386 -s -S --kernel $(BUILD)/kernel.bin &
	gdb ./kernel.bin --silent -ex "target remote localhost:1234"