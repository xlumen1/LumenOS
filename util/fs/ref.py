import struct

SECTOR_SIZE = 512
ENTRY_SIZE = 49
ENTRIES_PER_SECTOR = SECTOR_SIZE // ENTRY_SIZE

def pack_entry(name, size, isfile, start_block, end_block, start_byte, end_byte):
    name_bytes = name.encode('utf-8')[:32]
    name_bytes += b'\x00' * (32 - len(name_bytes))
    return struct.pack('<32sIBIIHH', name_bytes, size, isfile,
                       start_block, end_block, start_byte, end_byte)

def write_sector(f, sector_num, data):
    f.seek(sector_num * SECTOR_SIZE)
    f.write(data.ljust(SECTOR_SIZE, b'\x00'))

def write_file_data(f, start_sector, data):
    size = len(data)
    sectors_needed = (size + SECTOR_SIZE - 1) // SECTOR_SIZE
    for i in range(sectors_needed):
        chunk = data[i*SECTOR_SIZE:(i+1)*SECTOR_SIZE]
        write_sector(f, start_sector + i, chunk)
    return sectors_needed

def makefs():
    f = open('fs.img', 'wb')

    write_sector(f, 0, b'')

    next_sector = 1

    hello_data = b'Hello World!\n'
    hello_start = next_sector
    hello_sectors = write_file_data(f, hello_start, hello_data)
    hello_end = hello_start + hello_sectors - 1
    next_sector += hello_sectors

    readme_data = b'# Readme\nThis is LuFS.\n'
    readme_start = next_sector
    readme_sectors = write_file_data(f, readme_start, readme_data)
    readme_end = readme_start + readme_sectors - 1
    next_sector += readme_sectors

    docs_entries = [
        pack_entry(
            'readme.md',
            len(readme_data),
            1,
            readme_start,
            readme_end,
            0,
            (len(readme_data) - 1) % SECTOR_SIZE
        )
    ]
    docs_sector = next_sector
    next_sector += 1
    docs_data = b''.join(docs_entries).ljust(SECTOR_SIZE, b'\x00')
    write_sector(f, docs_sector, docs_data)

    root_entries = [
        pack_entry(
            'hello.txt',
            len(hello_data),
            1,
            hello_start,
            hello_end,
            0,
            (len(hello_data) - 1) % SECTOR_SIZE
        ),
        pack_entry(
            'docs',
            len(docs_entries),
            0,
            docs_sector,
            docs_sector,
            0,
            SECTOR_SIZE - 1
        )
    ]
    root_data = b''.join(root_entries).ljust(SECTOR_SIZE, b'\x00')
    write_sector(f, 1, root_data)

    f.close()
    print("LuFS image 'lufstest.img' created successfully.")

if __name__ == '__main__':
    makefs()
