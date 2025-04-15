.PHONY: build clean iso run debug all refresh

# Compiler and tools
CC := gcc
AS := as
LD := ld
GRUB := grub-mkrescue

# Directories
KERNEL := ../src/kernel
BUILD := .
OUT := $(BUILD)/out
ISO := $(BUILD)/iso

# Flags
CCFLAGS := -m32 --static -ffreestanding -c -g -nostdinc -I$(KERNEL)/libk -I$(KERNEL)
LDFLAGS := -m elf_i386 -T linker.ld -static -nostdlib -g

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
	@$(LD) $(LDFLAGS) -o $(BUILD)/kernel.bin $(ALL_OBJ)

# Compile C source files
$(OUT)/%.o: $(KERNEL)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CCFLAGS) $< -o $@

# Assemble assembly source files
$(OUT)/%.o: $(KERNEL)/%.s
	@mkdir -p $(dir $@)
	@echo "Assembling $<..."
	$(AS) --32 -g $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -fr $(OUT) $(BUILD)/kernel.bin $(BUILD)/os.iso

# Build ISO
iso: build
	@echo "Creating ISO..."
	@mkdir -p $(ISO)/boot/grub
	cp $(BUILD)/kernel.bin $(ISO)/boot/kernel.bin
	$(GRUB) -o $(BUILD)/os.iso $(ISO)

# Run kernel in QEMU
run: build
	@echo "Running kernel in QEMU..."
	qemu-system-i386 --kernel $(BUILD)/kernel.bin &

# Debug kernel in QEMU
debug: build
	@echo "Debugging kernel in QEMU..."
	@qemu-system-i386 -s -S --kernel $(BUILD)/kernel.bin &
	@gdb ./kernel.bin --silent -ex "target remote localhost:1234"

testiso: iso
	@echo "Testing ISO..."
	@qemu-system-i386 -cdrom $(BUILD)/os.iso -boot d -m 512