#!/bin/bash
# Run script for terminal version

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Change to project root (parent of bash/)
cd "$SCRIPT_DIR/.." || exit 1

if [ ! -f "./switchback" ]; then
    echo "Game not built yet. Building..."
    bash/build_terminal.sh
    echo ""
fi

if [ -f "./switchback" ]; then
    echo "Starting SWITCHBACK..."
    echo ""
    ./switchback
else
    echo "Error: Unable to find or build game executable"
    exit 1
fi
