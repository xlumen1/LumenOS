.PHONY: build clean iso run debug all refresh

# Compiler and tools
CC := gcc
AS := as
LD := ld

# Directories
KERNEL := ../src/kernel
BUILD := .
OUT := $(BUILD)/out
ISO := $(BUILD)/iso

# Flags
CCFLAGS := -m32 --static -ffreestanding -c -g -nostdinc -I$(KERNEL)/libk -I$(KERNEL)
LDFLAGS := -m elf_i386 -T linker.ld -static -nostdlib

# Source files
C_SRC := $(shell find $(KERNEL) -type f -name "*.c")
S_SRC := $(shell find $(KERNEL) -type f -name "*.s")

# Object files
C_OBJ := $(patsubst $(KERNEL)/%.c,$(OUT)/%.o,$(C_SRC))
S_OBJ := $(patsubst $(KERNEL)/%.s,$(OUT)/%.o,$(S_SRC))

# All objects
ALL_OBJ := $(C_OBJ) $(S_OBJ)

all: iso run

refresh: clean build

# Build kernel
build: $(ALL_OBJ)
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) -o $(BUILD)/kernel.bin $(ALL_OBJ)

# Compile C source files
$(OUT)/%.o: $(KERNEL)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CCFLAGS) $< -o $@

# Assemble assembly source files
$(OUT)/%.o: $(KERNEL)/%.s
	@mkdir -p $(dir $@)
	@echo "Assembling $<..."
	$(AS) --32 $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -fr $(OUT) $(BUILD)/kernel.bin $(BUILD)/os.iso $(ISO)

# Build ISO
iso: build
	@echo "Creating ISO..."
	@mkdir -p $(ISO)/boot/grub
	cp $(BUILD)/kernel.bin $(ISO)/boot/kernel.bin
	grub-mkrescue -o $(BUILD)/os.iso $(ISO)

# Run kernel in QEMU
run: build
	@echo "Running kernel in QEMU..."
	qemu-system-i386 --kernel $(BUILD)/kernel.bin

# Debug kernel in QEMU
debug: build
	@echo "Debugging kernel in QEMU..."
	qemu-system-i386 -s -S --kernel $(BUILD)/kernel.bin