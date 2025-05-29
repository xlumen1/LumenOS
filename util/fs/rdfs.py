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

def read_dir_table(f, offset, count):
    f.seek(offset)
    entries = []
    for _ in range(count):
        entry_bytes = f.read(ENTRY_SIZE)
        if len(entry_bytes) != ENTRY_SIZE:
            break
        entry = unpack_entry(entry_bytes)
        entries.append(entry)
    return entries

def print_tree(f, offset, count, indent=0, path="", visited=None, skip_empty_root=True, max_entries=None, printed=[0]):
    if visited is None:
        visited = set()
    entries = read_dir_table(f, offset, count)
    # Track names at this level to avoid duplicate printing
    seen = set()
    for entry in entries:
        if max_entries is not None and printed[0] >= max_entries:
            return
        # Skip printing empty directory entries (name is empty)
        if entry['name'] == "":
            continue
        # Skip duplicate names at this level
        if entry['name'] in seen:
            continue
        seen.add(entry['name'])
        prefix = "[D]" if not entry['isfile'] else "[F]"
        print("  " * indent + f"{prefix} {entry['name']}")
        printed[0] += 1
        full_path = os.path.join(path, entry['name'])
        # Prevent infinite recursion by tracking visited directory offsets
        if not entry['isfile'] and entry['size'] == 0 and (entry['start_block'] != 0 or entry['start_byte'] != 0):
            child_offset = entry['start_block'] * SECTOR_SIZE + entry['start_byte']
            if child_offset in visited:
                continue
            visited.add(child_offset)
            end_offset = entry['end_block'] * SECTOR_SIZE + entry['end_byte']
            total_bytes = end_offset - child_offset + 1
            child_count = total_bytes // ENTRY_SIZE
            print_tree(f, child_offset, child_count, indent + 1, full_path, visited, skip_empty_root, max_entries, printed)

def read_file_data(f, entry):
    size = entry['size']
    start = entry['start_block'] * SECTOR_SIZE + entry['start_byte']
    f.seek(start)
    return f.read(size)

def extract_all(f, offset, count, base_path, visited=None, max_entries=None, extracted=[0]):
    if visited is None:
        visited = set()
    entries = read_dir_table(f, offset, count)
    seen = set()
    for entry in entries:
        if max_entries is not None and extracted[0] >= max_entries:
            return
        # Skip empty name entries
        if entry['name'] == "":
            continue
        # Skip duplicate names at this level
        if entry['name'] in seen:
            continue
        seen.add(entry['name'])
        full_path = os.path.join(base_path, entry['name'])
        if entry['isfile']:
            os.makedirs(os.path.dirname(full_path), exist_ok=True)
            data = read_file_data(f, entry)
            with open(full_path, "wb") as out:
                out.write(data)
            extracted[0] += 1
        else:
            os.makedirs(full_path, exist_ok=True)
            if entry['size'] == 0 and (entry['start_block'] != 0 or entry['start_byte'] != 0):
                child_offset = entry['start_block'] * SECTOR_SIZE + entry['start_byte']
                if child_offset in visited:
                    continue
                visited.add(child_offset)
                end_offset = entry['end_block'] * SECTOR_SIZE + entry['end_byte']
                total_bytes = end_offset - child_offset + 1
                child_count = total_bytes // ENTRY_SIZE
                extract_all(f, child_offset, child_count, full_path, visited, max_entries, extracted)

def main():
    if len(sys.argv) < 2 or '--help' in sys.argv or '-h' in sys.argv:
        print("Usage:")
        print("  rdfs.py <image>                # Display LuFS image as a tree")
        print("  rdfs.py <image> --extract <outdir>  # Extract all files to <outdir>")
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
        print("=== Filesystem metadata ===")
        print(f"Entry count: {entry_count}")
        if entry_count <= 0:
            print("No entries found in the filesystem.")
            return
        print("=== Filesystem contents ===")
        print_tree(f, root_table_offset, entry_count, max_entries=entry_count - 1) # Ignore root

        if extract:
            print(f"\nExtracting to: {outdir}")
            os.makedirs(str(outdir), exist_ok=True)
            extract_all(f, root_table_offset, entry_count, outdir, max_entries=entry_count - 1)

if __name__ == "__main__":
    main()