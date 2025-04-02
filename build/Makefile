.PHONY: build clean iso run

CC := gcc
AS := as
LD := ld

CCFLAGS := -m32 --static -ffreestanding -c -g
LDFLAGS := -m elf_i386 -T linker.ld -static -nostdlib

KERNEL := ../src/kernel
LIBK := $(kernel)/libk
BUILD := .
OUT := $(BUILD)/out
ISO := iso

C_SRC := $(wildcard $(KERNEL)/**/*.c)
C_OBJ := $(patsubst $(KERNEL)/%.c,$(OUT)/%.o,$(C_SRC))

S_SRC := $(wildcard $(KERNEL)/**/*.s)
S_OBJ := $(patsubst $(KERNEL)/%.s,$(OUT)/%.o,$(S_SRC))

ALL_OBJ := $(C_OBJ) $(S_OBJ)

build: $(ALL_OBJ)
	$(LD) $(LDFLAGS) -o $(BUILD)/kernel.bin $(ALL_OBJ)

$(C_OBJ): $(C_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $< -o $@

$(S_OBJ): $(S_SRC)
	@mkdir -p $(OUT)
	$(AS) --32 $< -o $@

clean:
	rm -fr $(OUT)

iso: build
	@mkdir -p $(ISO)/boot
	cp $(BUILD)/kernel.bin $(ISO)/boot/kernel.bin
	grub-mkrescue -o $(BUILD)/os.iso $(ISO)

run: build
	qemu-system-i386 --kernel $(BUILD)/kernel.bin

debug: 
	qemu-system-i386 -s -S --kernel $(BUILD)/kernel.bin