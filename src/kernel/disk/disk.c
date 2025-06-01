#include "disk.h"
#include <stdint.h>
#include <stddef.h>
#include <io/io.h>

// ATA Primary bus I/O ports
#define ATA_IO_BASE     0x1F0
#define ATA_REG_DATA    (ATA_IO_BASE + 0)
#define ATA_REG_ERROR   (ATA_IO_BASE + 1)
#define ATA_REG_SECCNT  (ATA_IO_BASE + 2)
#define ATA_REG_LBA_LO  (ATA_IO_BASE + 3)
#define ATA_REG_LBA_MID (ATA_IO_BASE + 4)
#define ATA_REG_LBA_HI  (ATA_IO_BASE + 5)
#define ATA_REG_DRIVE   (ATA_IO_BASE + 6)
#define ATA_REG_STATUS  (ATA_IO_BASE + 7)
#define ATA_REG_CMD     (ATA_IO_BASE + 7)
#define ATA_REG_CTRL    0x3F6

#define ATA_CMD_READ_SECTORS  0x20
#define ATA_CMD_WRITE_SECTORS 0x30

#define ATA_SR_BSY  0x80
#define ATA_SR_DRQ  0x08

static disk_info_t info = {
    .sector_count = 1024 * 1024, // .5 GiB
    .sector_size = 512           // Standard sector size
};

static void ata_wait_bsy() {
    while (inb(ATA_REG_STATUS) & ATA_SR_BSY);
}

static void ata_wait_drq() {
    while (!(inb(ATA_REG_STATUS) & ATA_SR_DRQ));
}

void disk_init() {
    // For future use with not ATI PIO
}

int disk_read(uint32_t lba, uint8_t* buffer, uint32_t count) {
    if (!buffer || lba + count > info.sector_count) return -1;
    for (uint32_t sector = 0; sector < count; ++sector) {
        ata_wait_bsy();
        outb(ATA_REG_CTRL, 0x00); // nIEN = 0 (enable interrupts)
        outb(ATA_REG_SECCNT, 1); // Sector count
        outb(ATA_REG_LBA_LO, (uint8_t)((lba + sector) & 0xFF));
        outb(ATA_REG_LBA_MID, (uint8_t)(((lba + sector) >> 8) & 0xFF));
        outb(ATA_REG_LBA_HI, (uint8_t)(((lba + sector) >> 16) & 0xFF));
        outb(ATA_REG_DRIVE, 0xE0 | (((lba + sector) >> 24) & 0x0F)); // master, LBA mode
        outb(ATA_REG_CMD, ATA_CMD_READ_SECTORS);

        ata_wait_bsy();
        ata_wait_drq();

        // Read 256 words (512 bytes)
        for (uint32_t i = 0; i < 256; ++i) {
            uint16_t data = inw(ATA_REG_DATA);
            buffer[sector * info.sector_size + i * 2] = data & 0xFF;
            buffer[sector * info.sector_size + i * 2 + 1] = (data >> 8) & 0xFF;
        }
    }
    return 0;
}

int disk_write(uint32_t lba, const uint8_t* buffer, uint32_t count) {
    if (!buffer || lba + count > info.sector_count) return -1;
    for (uint32_t sector = 0; sector < count; ++sector) {
        ata_wait_bsy();
        outb(ATA_REG_CTRL, 0x00); // nIEN = 0 (enable interrupts)
        outb(ATA_REG_SECCNT, 1); // Sector count
        outb(ATA_REG_LBA_LO, (uint8_t)((lba + sector) & 0xFF));
        outb(ATA_REG_LBA_MID, (uint8_t)(((lba + sector) >> 8) & 0xFF));
        outb(ATA_REG_LBA_HI, (uint8_t)(((lba + sector) >> 16) & 0xFF));
        outb(ATA_REG_DRIVE, 0xE0 | (((lba + sector) >> 24) & 0x0F)); // master, LBA mode
        outb(ATA_REG_CMD, ATA_CMD_WRITE_SECTORS);

        ata_wait_bsy();
        ata_wait_drq();

        // Write 256 words (512 bytes)
        for (uint32_t i = 0; i < 256; ++i) {
            uint16_t data = buffer[sector * info.sector_size + i * 2] |
                            (buffer[sector * info.sector_size + i * 2 + 1] << 8);
            outw(ATA_REG_DATA, data);
        }
    }
    return 0;
}

disk_info_t disk_get_info() {
    return info;
}