#!/bin/bash

cd "$(dirname "$0")"

wget https://github.com/xlumen1/LumenStandardFs/archive/refs/heads/main.zip -O fs.zip
unzip fs.zip
rm fs.zip

# Get the files to be removed from the .empties file
while IFS= read -r file; do
    echo "Removing $file"
    rm "LumenStandardFS-main$file"
done < LumenStandardFS-main/.empties
rm LumenStandardFS-main/.empties

echo "Creating Filesystem..."
python3 ./mkfs.py LumenStandardFS-main fs.img

rm -rf LumenStandardFS-main
echo "Standard filesystem created as fs.img"
python3 ./rdfs.py fs.img