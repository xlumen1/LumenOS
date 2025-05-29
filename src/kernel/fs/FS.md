# File system internals

The LumenOS Filesystem (henceforth called LuFS) uses one key structure: the File System Entry.

```c
struct FsEntry {
    char name[32];
    uint32_t size;
    uint8_t isfile;
    uint32_t start_block; // Inclusive
    uint32_t end_block; // Inclusive
    uint16_t start_byte; // Inclusive
    uint16_t end_byte; // Inclusive
} __attribute__ ((packed));
```

## Constants

| Name                 | Value | Description                                    |
| -------------------- | ----- | ---------------------------------------------- |
| ENTRY_SIZE           |  49   | Size of one entry struct in bytes              |
| ENTRIES_PER_SECTOR   |  9    | Number of entries per sector (512 // 49 = 9)   |

## Disk Layout

**Disk Map:**
| 0        | 1       | 2    |
| -------- | ------- | ---- |
| Reserved | Entries | Data |

## Header

The first four bytes **must** spell out the string "LuFS", the next 4 bytes hold a `uint32` dictating the number of entries.

## Directories

A directory is effectively a file. When isFile == 0 and size == 0, the data (block and byte) pointers will be pointing to the sections within the entry block that contain the subfiles and subdirectories.
Subfile and subdirectory entries **must** all be in the same continuous area of data on disk. If a directory is empty, it **must** point to (0, 0, 0, 0);

## Notes

- Filename is limited to 32 UTF-8 characters
- All data is stored in little-endian