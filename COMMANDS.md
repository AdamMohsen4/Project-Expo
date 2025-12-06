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

