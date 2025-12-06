#!/bin/bash
# Build script for terminal version

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Change to project root (parent of bash/)
cd "$SCRIPT_DIR/.." || exit 1

echo "================================"
echo "Building SWITCHBACK (Terminal)"
echo "================================"
echo ""

make -f Makefile.terminal clean
make -f Makefile.terminal

if [ $? -eq 0 ]; then
    echo ""
    echo "Build successful!"
    echo "Run with: ./switchback"
    echo "Or: bash/run_terminal.sh"
else
    echo ""
    echo "Build failed!"
    exit 1
fi
