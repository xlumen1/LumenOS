#include "elf.h"
#include <stdio.h>
#include <idt/idt.h>


void *user_exit_return = 0;

void elf_jump(Elf32_Ehdr_t *ehdr) {
    // Set up the stack
    void* user_stack = malloc(0x4000); // 16KB stack
    if (!user_stack) {
        printf("[ELF] Error: Failed to allocate user stack\n");
        return -1;
    }

    uintptr_t old_stack_pointer;
    __asm__ volatile ("mov %%esp, %0" : "=r"(old_stack_pointer));
    uintptr_t stack_ptr = (uintptr_t)user_stack + 0x4000;
    const char *progname = "test.elf";
    size_t progname_len = strlen(progname) + 1; // +1 for null terminator
    stack_ptr -= progname_len;
    memcpy((void *)stack_ptr, progname, progname_len); // Copy program name to stack

    stack_ptr &= 0x3; // Align to 4 bytes

    // Argv
    char *argv[] = {(char *)stack_ptr, NULL}; // Null-terminated argv array
    stack_ptr -= sizeof(argv);
    memcpy((void *)stack_ptr, argv, sizeof(argv)); // Copy argv to stack

    // Envp
    stack_ptr -= sizeof(char*);
    *((char**)stack_ptr) = NULL; // Null-terminated envp

    // Argc
    stack_ptr -= sizeof(int);
    *((int*)stack_ptr) = 1; // argc = 1 (only the program name)

    stack_ptr -= 4;
    *((uint32_t*)stack_ptr) = (uint32_t)&&elf_return_address;

    __asm__ volatile (
        "mov %0, %%esp\n\t" :: "r"(user_stack) // Set stack pointer to top of stack
    );

    printf("[ELF] Jumping to entry point: %x with user-mode stack at %x\n", (unsigned)ehdr->e_entry, (unsigned)user_stack);

    ((void (*)())ehdr->e_entry)();
elf_return_address:
    __asm__ volatile ("mov %0, %%esp" :: "r"(old_stack_pointer));
    free(user_stack);
}

int elf_load(const uint8_t *elf_data, size_t elf_size) {
    printf("[ELF] Loading ELF, size: %d bytes\n", elf_size);

    if (elf_size < sizeof(Elf32_Ehdr_t)) {
        printf("[ELF] Error: ELF header too small\n");
        return -1;
    }

    const Elf32_Ehdr_t *ehdr = (const Elf32_Ehdr_t *)elf_data;

    // ✨MAGIC✨ (Note that the boxes are supposed to be sparkles, some computers may not render them)
    if (ehdr->e_ident[0] != 0x7F || ehdr->e_ident[1] != 'E' ||
        ehdr->e_ident[2] != 'L' || ehdr->e_ident[3] != 'F') {
        printf("[ELF] Error: Not an ELF file\n");
        return -1;
    }

    printf("[ELF] ELF magic OK. Entry: %x, PH num: %d, PH off: %x\n",
        (unsigned)ehdr->e_entry, ehdr->e_phnum, (unsigned)ehdr->e_phoff);
    if (ehdr->e_phoff + ehdr->e_phnum * sizeof(Elf32_Phdr_t) > elf_size) {
        printf("[ELF] Error: Program header table out of bounds\n");
        return -1;
    }

    const Elf32_Phdr_t *phdr = (const Elf32_Phdr_t *)(elf_data + ehdr->e_phoff);
    for (uint16_t i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            printf("[ELF] Segment %d: vaddr=%x, filesz=%d, memsz=%d, offset=%x\n",
                i, (unsigned)phdr[i].p_vaddr, (unsigned)phdr[i].p_filesz,
                (unsigned)phdr[i].p_memsz, (unsigned)phdr[i].p_offset);
            if (phdr[i].p_offset + phdr[i].p_filesz > elf_size) {
                printf("[ELF] Error: Segment %d out of bounds\n", i);
                return -1;
            }
            if (phdr[i].p_vaddr == 0 || phdr[i].p_vaddr < 0x10000) {
                printf("[ELF] Error: Segment %d has invalid vaddr %x\n", i, (unsigned)phdr[i].p_vaddr);
                return -1;
            }
            printf("[ELF] Memcpy to %x from offset %x, size %d\n",
                (unsigned)phdr[i].p_vaddr, (unsigned)phdr[i].p_offset, (unsigned)phdr[i].p_filesz);
            memcpy((void *)phdr[i].p_vaddr, elf_data + phdr[i].p_offset, phdr[i].p_filesz);

            // Zero out .bss
            printf("[ELF] Memset to %x, size %d\n",
                (unsigned)(phdr[i].p_vaddr + phdr[i].p_filesz),
                (unsigned)(phdr[i].p_memsz - phdr[i].p_filesz));
            memset((void *)(phdr[i].p_vaddr + phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
        }
    }

    elf_jump(ehdr);
    
    printf("[ELF] Returned from entry point!\n");
    return 0;
}