# Quick Command Reference

## Terminal Version (Play on Your Computer)

### Quick Start
```bash
bash/run_terminal.sh       # Easiest - builds and runs automatically
```

### Build Commands
```bash
bash/build_terminal.sh     # Build using convenience script
make -f Makefile.terminal  # Build using make
```

### Run Commands
```bash
./switchback               # Run after building
make -f Makefile.terminal run  # Build and run with make
```

### Clean Commands
```bash
make -f Makefile.terminal clean  # Remove build files
```

---

## DTEK-V Board Version (Play on Hardware)

### Quick Start
```bash
bash/run_board.sh          # Easiest - builds and uploads automatically
```

### Build Commands
```bash
bash/build_board.sh        # Build using convenience script
make build                 # Build using make
```

### Upload & Run Commands
```bash
make run                   # Upload to board (after building)
```

### Clean Commands
```bash
make clean                 # Remove build files
```

---

## In-Game Commands

When playing, you'll see a menu like this:

```
--- Commands ---
[1] Go east          ← Movement options (varies by room)
[2] Go south
[3] Look around      ← Re-read room description
[4] Take item        ← Pick up items
[5] Use item         ← Combine/activate items
[6] Quit game        ← Exit
```

Just type the number and press ENTER (terminal) or use switches + BTN2 (board).

---

## File Structure

```
Project Structure:
  bash/
    build_terminal.sh         - Terminal build script
    run_terminal.sh           - Terminal run script
    build_board.sh            - Board build script
    run_board.sh              - Board run script

  src/terminal/
    board_terminal.c          - Terminal board simulation
    ui_terminal.c             - Terminal UI
    input_terminal.c          - Keyboard input
    main_terminal.c           - Entry point

  src/
    board.c                   - DTEK-V hardware
    ui.c                      - DTEK-V UI
    input.c                   - Switch/button input
    main.c                    - Board entry point
    world.c                   - Room & puzzle logic (shared)
    game.c                    - Game loop & menus (shared)
```

---

## Troubleshooting

### Terminal Version Won't Build
```bash
# Make scripts executable
chmod +x bash/*.sh

# Check gcc is installed
gcc --version

# Try manual build
make -f Makefile.terminal clean
make -f Makefile.terminal
```

### Board Version Won't Build
```bash
# Check RISC-V toolchain is installed
riscv32-unknown-elf-gcc --version

# Clean and rebuild
make clean
make build
```

### Game Won't Run
```bash
# Terminal: Make sure it's executable
chmod +x switchback
./switchback

# Board: Check USB connection and tools directory
ls tools/
```

### Scripts Won't Execute
```bash
# Make all scripts executable
chmod +x bash/*.sh

# Run with explicit bash
bash bash/run_terminal.sh
```
