#!/bin/bash
# Run script for DTEK-V board version

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Change to project root (parent of bash/)
cd "$SCRIPT_DIR/.." || exit 1

if [ ! -f "./main.bin" ]; then
    echo "Board binary not built yet. Building..."
    bash/build_board.sh
    echo ""
fi

if [ -f "./main.bin" ]; then
    echo "Uploading to DTEK-V board..."
    echo ""
    make run
else
    echo "Error: Unable to find or build main.bin"
    exit 1
fi
