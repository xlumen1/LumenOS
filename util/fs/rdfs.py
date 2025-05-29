import struct
import sys
import os

SECTOR_SIZE = 512
ENTRY_SIZE = 49

def unpack_entry(entry_bytes):
    name, size, isfile, sb, eb, sbt, ebt = struct.unpack('<32sIBIIHH', entry_bytes)
    name = name.rstrip(b'\x00').decode(errors='replace')
    return {
        'name': name,
        'size': size,
        'isfile': bool(isfile),
        'start_block': sb,
        'end_block': eb,
        'start_byte': sbt,
        'end_byte': ebt,
    }

def read_sector(f, sector_num):
    f.seek(sector_num * SECTOR_SIZE)
    data = f.read(SECTOR_SIZE)
    print(f"[DEBUG] Read sector {sector_num} (offset {sector_num*SECTOR_SIZE}), {len(data)} bytes")
    return data

def read_file_data(f, entry):
    size = entry['size']
    start = entry['start_block'] * SECTOR_SIZE + entry['start_byte']
    f.seek(start)
    data = f.read(size)
    print(f"[DEBUG] Reading file '{entry['name']}' data from {start} (size {size})")
    return data

def read_dir_table(f, offset, count):
    f.seek(offset)
    print(f"[DEBUG] Reading directory table at offset {offset} for {count} entries")
    entries = []
    for i in range(count):
        entry_bytes = f.read(ENTRY_SIZE)
        if len(entry_bytes) != ENTRY_SIZE:
            print(f"[ERROR] Could not read full entry {i} at offset {offset + i*ENTRY_SIZE}")
            break
        entry = unpack_entry(entry_bytes)
        print(f"[DEBUG] Entry {i}: {entry}")
        entries.append(entry)
    return entries

def print_tree(f, offset, count, indent=0, path=""):
    print(f"{'  '*indent}[DEBUG] print_tree at offset {offset}, count {count}, path '{path}'")
    entries = read_dir_table(f, offset, count)
    for entry in entries:
        prefix = "[D]" if not entry['isfile'] else "[F]"
        print("  " * indent + f"{prefix} {entry['name']}")
        full_path = os.path.join(path, entry['name'])
        if not entry['isfile'] and entry['size'] == 0 and (entry['start_block'] != 0 or entry['start_byte'] != 0):
            child_offset = entry['start_block'] * SECTOR_SIZE + entry['start_byte']
            end_offset = entry['end_block'] * SECTOR_SIZE + entry['end_byte']
            total_bytes = end_offset - child_offset + 1
            child_count = total_bytes // ENTRY_SIZE
            print(f"{'  '*(indent+1)}[DEBUG] Descending into directory '{entry['name']}' at offset {child_offset} with {child_count} entries")
            print_tree(f, child_offset, child_count, indent + 1, full_path)

def extract_all(f, offset, count, base_path):
    print(f"[DEBUG] extract_all at offset {offset}, count {count}, base_path '{base_path}'")
    entries = read_dir_table(f, offset, count)
    for entry in entries:
        full_path = os.path.join(base_path, entry['name'])
        if entry['isfile']:
            data = read_file_data(f, entry)
            os.makedirs(os.path.dirname(full_path), exist_ok=True)
            print(f"[DEBUG] Extracting file '{full_path}', size {len(data)} bytes")
            with open(full_path, "wb") as out:
                out.write(data)
        else:
            if entry['size'] == 0 and (entry['start_block'] != 0 or entry['start_byte'] != 0):
                child_offset = entry['start_block'] * SECTOR_SIZE + entry['start_byte']
                end_offset = entry['end_block'] * SECTOR_SIZE + entry['end_byte']
                total_bytes = end_offset - child_offset + 1
                child_count = total_bytes // ENTRY_SIZE
                print(f"[DEBUG] Descending into directory '{entry['name']}' at offset {child_offset} with {child_count} entries for extraction")
                os.makedirs(full_path, exist_ok=True)
                extract_all(f, child_offset, child_count, full_path)

def main():
    if len(sys.argv) < 2:
        print("Usage: rdfs.py <image> [--extract <outdir>]")
        return

    image_path = sys.argv[1]
    extract = '--extract' in sys.argv
    outdir = sys.argv[sys.argv.index('--extract') + 1] if extract else None

    with open(image_path, "rb") as f:
        magic = f.read(4)
        if magic != b"LuFS":
            print("Not a valid LuFS image.")
            return

        entry_count = struct.unpack("<I", f.read(4))[0]
        root_table_offset = SECTOR_SIZE
        print(f"[DEBUG] LuFS image detected with {entry_count} entries at offset {root_table_offset}")
        print("Filesystem contents:")
        print_tree(f, root_table_offset, entry_count)

        if extract:
            print(f"\nExtracting to: {outdir}")
            os.makedirs(str(outdir), exist_ok=True)
            extract_all(f, root_table_offset, entry_count, outdir)

if __name__ == "__main__":
    main()