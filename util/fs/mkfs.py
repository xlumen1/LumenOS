import struct
import sys
import os
import math

SECTOR_SIZE = 512
ENTRY_SIZE = 49

class Entry:
    def __init__(self, name, isfile, parent, content=b''):
        self.name = name  # str
        self.isfile = isfile  # bool
        self.parent = parent  # str
        self.content = content  # bytes
        self.size = len(content) if isfile else 0

def get_entries_from_dir(root_dir, parent=""):
    """
    Recursively collects Entry objects for all files and directories under root_dir.
    """
    rel_path = os.path.relpath(root_dir, sys.argv[1])
    if rel_path == ".":
        rel_path = ""
    # Always add directory entry (including root)
    entries.append(Entry(
        name=os.path.basename(root_dir) if rel_path != "" else "",
        isfile=False,
        parent=parent,
        content=b''
    ))
    # If directory is empty, still need to scan (do nothing here, entry already added)
    items = sorted(os.listdir(root_dir))
    if not items:
        return  # Early return: empty dir, entry already added
    for item in items:
        entry_path = os.path.join(root_dir, item)
        if os.path.isfile(entry_path):
            with open(entry_path, "rb") as f:
                content = f.read()
            entries.append(Entry(
                name=item,
                isfile=True,
                parent=rel_path,
                content=content
            ))
        elif os.path.isdir(entry_path):
            get_entries_from_dir(entry_path, rel_path)

def pack_entry(name, size, isfile, start_block, end_block, start_byte, end_byte):
    name_bytes = name.encode()[:32].ljust(32, b'\x00')
    return struct.pack('<32sIBIIHH', name_bytes, size, int(isfile), start_block, end_block, start_byte, end_byte)

def write_sector(f, data):
    global sector
    f.seek(sector * SECTOR_SIZE)
    f.write(data.ljust(SECTOR_SIZE, b'\x00'))
    sector += 1

def makefs():
    get_entries_from_dir(sys.argv[1])

    # Build directory tree: parent -> [entries]
    dir_tree = {}
    for entry in entries:
        dir_tree.setdefault(entry.parent, []).append(entry)

    # Assign directory table offsets
    dir_order = []
    visited = set()
    def collect_dirs(parent):
        for entry in dir_tree.get(parent, []):
            if not entry.isfile:
                # Always use the full relative path for dpath (including root)
                dpath = os.path.join(parent, entry.name) if parent != "" else entry.name
                if dpath in visited:
                    continue  # Prevent infinite recursion
                visited.add(dpath)
                dir_order.append((dpath, entry))
                collect_dirs(dpath)
    # Start with root directory (empty string as rel_path)
    collect_dirs("")

    # Map: dir_rel_path -> (offset, size)
    dir_offsets = {}
    cur_offset = SECTOR_SIZE  # after header
    for dpath, dentry in dir_order:
        count = len(dir_tree.get(dpath, []))
        size = count * ENTRY_SIZE
        dir_offsets[dpath] = (cur_offset, size)
        cur_offset += size

    # Calculate file data offset (sector aligned)
    file_data_offset = math.ceil(cur_offset / SECTOR_SIZE) * SECTOR_SIZE

    # Assign file data offsets
    file_offsets = {}
    file_data = b''
    for entry in entries:
        if entry.isfile:
            offset = file_data_offset + len(file_data)
            start_block = offset // SECTOR_SIZE
            end_block = (offset + entry.size - 1) // SECTOR_SIZE if entry.size > 0 else start_block
            start_byte = offset % SECTOR_SIZE
            end_byte = (offset + entry.size - 1) % SECTOR_SIZE if entry.size > 0 else start_byte
            file_offsets[(entry.parent, entry.name)] = (start_block, end_block, start_byte, end_byte)
            file_data += entry.content

    # Write header
    mem_disk = struct.pack("<4sI", b"LuFS", len(entries))
    mem_disk += b'\x00' * (SECTOR_SIZE - struct.calcsize("<4sI"))

    # Write directory tables
    for dpath, dentry in dir_order:
        table = b''
        entries_in_dir = dir_tree.get(dpath, [])
        for entry in entries_in_dir:
            if entry.isfile:
                sb, eb, sbt, ebt = file_offsets[(dpath, entry.name)]
                table += pack_entry(entry.name, entry.size, True, sb, eb, sbt, ebt)
            else:
                child_path = os.path.join(dpath, entry.name) if dpath != "" else entry.name
                child_count = len(dir_tree.get(child_path, []))
                if child_count == 0:
                    table += pack_entry(entry.name, 0, False, 0, 0, 0, 0)
                else:
                    offset, size = dir_offsets[child_path]
                    sb = offset // SECTOR_SIZE
                    eb = (offset + size - 1) // SECTOR_SIZE
                    # If this is the root entry, set start_byte to ENTRY_SIZE
                    if dpath == "" and entry.name == "":
                        sbt = ENTRY_SIZE
                    else:
                        sbt = offset % SECTOR_SIZE
                    ebt = (offset + size - 1) % SECTOR_SIZE
                    table += pack_entry(entry.name, 0, False, sb, eb, sbt, ebt)
        mem_disk += table

    # Pad to file data offset
    if len(mem_disk) < file_data_offset:
        mem_disk += b'\x00' * (file_data_offset - len(mem_disk))

    # Append file data
    mem_disk += file_data

    # Write to output file sector by sector
    chunks = [mem_disk[i:i + SECTOR_SIZE] for i in range(0, len(mem_disk), SECTOR_SIZE)]
    with open(sys.argv[2], 'wb') as f:
        global sector
        sector = 0
        for chunk in chunks:
            write_sector(f, chunk)

if __name__ == "__main__":
    entries = []
    sector = 0
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <directory> <output_image>")
        exit(1)
    makefs()
