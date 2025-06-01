#include "elf.h"
#include <stdio.h> // For printf

int elf_load(const uint8_t *elf_data, size_t elf_size) {
    printf("[ELF] Loading ELF, size: %d bytes\n", elf_size);

    if (elf_size < sizeof(Elf32_Ehdr_t)) {
        printf("[ELF] Error: ELF header too small\n");
        return -1;
    }

    const Elf32_Ehdr_t *ehdr = (const Elf32_Ehdr_t *)elf_data;

    // ✨MAGIC✨
    if (ehdr->e_ident[0] != 0x7F || ehdr->e_ident[1] != 'E' ||
        ehdr->e_ident[2] != 'L' || ehdr->e_ident[3] != 'F') {
        printf("[ELF] Error: Not an ELF file\n");
        return -1;
    }

    printf("[ELF] ELF magic OK. Entry: %x, PH num: %d, PH off: %x\n",
        (unsigned)ehdr->e_entry, ehdr->e_phnum, (unsigned)ehdr->e_phoff);

    // Check program header table fits
    if (ehdr->e_phoff + ehdr->e_phnum * sizeof(Elf32_Phdr_t) > elf_size) {
        printf("[ELF] Error: Program header table out of bounds\n");
        return -1;
    }

    const Elf32_Phdr_t *phdr = (const Elf32_Phdr_t *)(elf_data + ehdr->e_phoff);
    for (uint16_t i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            printf("[ELF] Loading segment %d: vaddr=%x, filesz=%d, memsz=%d, offset=%x\n",
                i, (unsigned)phdr[i].p_vaddr, (unsigned)phdr[i].p_filesz,
                (unsigned)phdr[i].p_memsz, (unsigned)phdr[i].p_offset);

            // Check segment fits in file
            if (phdr[i].p_offset + phdr[i].p_filesz > elf_size) {
                printf("[ELF] Error: Segment %d out of bounds\n", i);
                return -1;
            }
            memcpy((void *)phdr[i].p_vaddr, elf_data + phdr[i].p_offset, phdr[i].p_filesz);
            // Zero out .bss
            memset((void *)(phdr[i].p_vaddr + phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
        }
    }

    printf("[ELF] Jumping to entry point: %x\n", (unsigned)ehdr->e_entry);
    ((void (*)())ehdr->e_entry)();

    printf("[ELF] Returned from entry point!\n");
    return 0;
}