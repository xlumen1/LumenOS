#!/bin/sh
awk -f "$(dirname "$0")/fmt_s.awk" "$1" > "$1.tmp" && mv "$1.tmp" "$1"
