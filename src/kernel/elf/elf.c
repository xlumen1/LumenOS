#include "elf.h"

int elf_load(const uint8_t *elf_data) {
    const Elf32_Ehdr_t *ehdr = (const Elf32_Ehdr_t *)elf_data;

    // ✨MAGIC✨
    if (ehdr->e_ident[0] != 0x7F || ehdr->e_ident[1] != 'E' ||
        ehdr->e_ident[2] != 'L' || ehdr->e_ident[3] != 'F') {
        return -1; // Oof, I don't think its an elf file
    }

    // Load each program header
    const Elf32_Phdr_t *phdr = (const Elf32_Phdr_t *)(elf_data + ehdr->e_phoff);
    for (uint16_t i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            memcpy((void *)phdr[i].p_vaddr, elf_data + phdr[i].p_offset, phdr[i].p_filesz);
            // Zero out .bss
            memset((void *)(phdr[i].p_vaddr + phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
        }
    }

    // Hmm yes this is why I love programming in C
    ((void (*)())ehdr->e_entry)();

    return 0;
}