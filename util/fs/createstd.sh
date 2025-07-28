#!/bin/bash

cd "$(dirname "$0")"

tar -xzf fs.tar.gz

cd ../elf
make clean
make all
cp ./test.elf ../fs/LumenFS/bin/test.elf
cd ../fs
python3 ./mkfs.py LumenFS fs.img
python3 ./rdfs.py fs.img
rm -r LumenFS
echo "LumenOS initramfs(install disk) filesystem generated"
