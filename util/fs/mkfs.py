import struct
import sys
import os
import math

SECTOR_SIZE = 512
ENTRY_SIZE = 49
ENTRIES_PER_SECTOR = SECTOR_SIZE // ENTRY_SIZE

class Entry:
    def __init__(self, name=b'', isfile=False, parent=b'', content=b''):
        self.name = name
        self.content = content
        self.isfile = isfile
        self.parent = parent
        self.size = len(content)

entries: list[Entry] = []
sector = 0

def get_dir(root_dir, parent=""):
    full_path = os.path.join(parent, root_dir) if parent else root_dir
    rel_path = os.path.relpath(full_path, sys.argv[1])
    
    nodes = os.listdir(full_path)

    for i in nodes:
        entry_path = os.path.join(full_path, i)
        rel_entry_path = os.path.relpath(entry_path, sys.argv[1])
        contents = b''
        if os.path.isfile(entry_path):
            with open(entry_path, "rb") as f:
                contents = f.read()
        entries.append(Entry(
            name=i.encode(),
            isfile=os.path.isfile(entry_path),
            parent=rel_path.encode(),
            content=contents
        ))
        if os.path.isdir(entry_path):
            get_dir(i, full_path)


def pack_entry(name: bytes, size: int, isfile: bool, start_block: int, end_block: int, start_byte: int, end_byte: int):
    name_bytes = name[:32]
    name_bytes += b'\x00' * (32 - len(name_bytes))
    return struct.pack('<32sIBIIHH', name_bytes, size, isfile, start_block, end_block, start_byte, end_byte)

def pad_sectors(sectors: bytes):
    return sectors + (b'\x00' * (SECTOR_SIZE - (SECTOR_SIZE * math.ceil((len(sectors) % SECTOR_SIZE) / SECTOR_SIZE))))

def write_sector(f, data):
    global sector
    f.seek(sector * SECTOR_SIZE)
    f.write(data.ljust(SECTOR_SIZE, b'\x00'))
    sector += 1

def makefs():
    get_dir(sys.argv[1])
    mem_disk = b''
    mem_disk += struct.pack("<4s", b"LuFS")
    mem_disk += struct.pack("<I", len(entries))
    mem_disk += b'\x00' * (SECTOR_SIZE - (struct.calcsize("<I") + struct.calcsize("<4s")))

    dir_entry_area = {}
    for entry in entries:
        parent = entry.parent.decode()
        if parent not in dir_entry_area:
            dir_entry_area[parent] = 1
        else:
            dir_entry_area[parent] += 1
        if not entry.isfile:
            dir_entry_area[entry.name.decode()] = 0

    dir_tables = {}
    for directory in dir_entry_area.keys():
        table = b''
        for entry in entries:
            if entry.parent.decode() != directory:
                continue
            table += b'\x00' * ENTRY_SIZE  # Placeholder
        dir_tables[directory] = table

    total_dir_table_bytes = sum(len(tbl) for tbl in dir_tables.values())
    total_header_bytes = SECTOR_SIZE
    file_data_offset = math.ceil((total_header_bytes + total_dir_table_bytes) / SECTOR_SIZE) * SECTOR_SIZE

    file_data = b''
    file_offsets = {}

    for entry in entries:
        if not entry.isfile:
            continue
        offset = file_data_offset + len(file_data)
        start_block = offset // SECTOR_SIZE
        end_block = (offset + entry.size - 1) // SECTOR_SIZE
        start_byte = offset % SECTOR_SIZE
        end_byte = (offset + entry.size - 1) % SECTOR_SIZE
        file_offsets[entry.name.decode()] = (start_block, end_block, start_byte, end_byte)
        file_data += entry.content
    
    print([entry.name.decode() for entry in entries])

    dir_offsets = {}
    cur_offset = 0
    for directory, table in dir_tables.items():
        dir_offsets[directory] = cur_offset
        cur_offset += len(table)

    for directory in dir_tables.keys():
        table = b''
        for entry in entries:
            if entry.parent.decode() != directory:
                continue
            if entry.isfile:
                sb, eb, sbt, ebt = file_offsets[entry.name.decode()]
                table += pack_entry(entry.name, entry.size, True, sb, eb, sbt, ebt)
            else:
                child_dir = entry.name.decode()
                if dir_entry_area[child_dir] == 0:
                    table += pack_entry(entry.name, 0, False, 0, 0, 0, 0)
                else:
                    offset = total_header_bytes + dir_offsets[child_dir]
                    size = dir_entry_area[child_dir] * ENTRY_SIZE
                    sb = offset // SECTOR_SIZE
                    eb = (offset + size - 1) // SECTOR_SIZE
                    sbt = offset % SECTOR_SIZE
                    ebt = (offset + size - 1) % SECTOR_SIZE
                    table += pack_entry(entry.name, 0, False, sb, eb, sbt, ebt)
        dir_tables[directory] = table

    dir_bytes = b''.join(dir_tables.values())
    mem_disk += dir_bytes

    actual_dir_end = total_header_bytes + len(dir_bytes)
    expected_file_data_start = file_data_offset

    if expected_file_data_start > actual_dir_end:
        padding = expected_file_data_start - actual_dir_end
        mem_disk += b'\x00' * padding

    mem_disk += file_data


    chunks = [mem_disk[i:i + SECTOR_SIZE] for i in range(0, len(mem_disk), SECTOR_SIZE)]
    with open(sys.argv[2], 'wb') as f:
        for chunk in chunks:
            write_sector(f, chunk)


if __name__ == "__main__":
    entries = []
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <directory> <output_image>")
        exit(1)
    makefs()