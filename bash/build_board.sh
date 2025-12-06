#!/bin/bash
# Build script for DTEK-V board version

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Change to project root (parent of bash/)
cd "$SCRIPT_DIR/.." || exit 1

echo "================================"
echo "Building SWITCHBACK (DTEK-V)"
echo "================================"
echo ""

make clean
make build

if [ $? -eq 0 ]; then
    echo ""
    echo "Build successful!"
    echo "Binary: main.bin"
    echo "Run on board with: make run"
    echo "Or: bash/run_board.sh"
else
    echo ""
    echo "Build failed!"
    exit 1
fi
