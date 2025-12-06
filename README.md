# Switchback - Adventure Game

An interactive text-based adventure game where you explore an ancient dungeon, solve puzzles, and escape through the Sealed Gate.

> **Quick Reference**: See [COMMANDS.md](COMMANDS.md) for a condensed command reference guide.

## Quick Start

### Terminal Version (Play on Your Computer)

**Easiest way - Use the convenience scripts:**

```bash
# Build and run in one step
bash/run_terminal.sh

# Or build first, then run
bash/build_terminal.sh
./switchback
```

**Manual method - Using make commands:**

```bash
# Build the game
make -f Makefile.terminal

# Run the game
./switchback

# Clean build files
make -f Makefile.terminal clean
```

### DTEK-V Board Version (Play on Hardware)

**Easiest way - Use the convenience scripts:**

```bash
# Build and upload to board in one step
bash/run_board.sh

# Or build first, then upload
bash/build_board.sh
make run
```

**Manual method - Using make commands:**

```bash
# Build for DTEK-V board
make clean
make build

# Upload and run on board
make run
```

## Command Reference

### Terminal Version Commands

| Command | Description |
|---------|-------------|
| `bash/run_terminal.sh` | **Recommended**: Build (if needed) and run the game |
| `bash/build_terminal.sh` | Build the terminal version |
| `./switchback` | Run the game (after building) |
| `make -f Makefile.terminal` | Build using make |
| `make -f Makefile.terminal run` | Build and run using make |
| `make -f Makefile.terminal clean` | Clean build files |

### DTEK-V Board Commands

| Command | Description |
|---------|-------------|
| `bash/run_board.sh` | **Recommended**: Build (if needed) and upload to board |
| `bash/build_board.sh` | Build the board version |
| `make build` | Build for DTEK-V (manual) |
| `make run` | Upload to board (after building) |
| `make clean` | Clean build files |

## How to Play

### Game Objective

You must:
1. Find 3 ARTIFACTS hidden in locked and dark chambers
2. The artifacts will automatically fuse into a MASTER KEY
3. Reach the Sealed Gate with the MASTER KEY to escape and win

### Controls (Terminal Version)

The game presents a clear menu for each room. When prompted with `>`, enter the number for your choice:

- **Movement Commands**: Listed first (e.g., "[1] Go east", "[2] Go south")
  - Each room shows available exits with compass directions
  - Simply enter the number to move in that direction

- **Action Commands**: Listed after movement options
  - **Look around**: Re-read current room description and see exits
  - **Take item**: Pick up items in the current room
  - **Use item**: Combine or activate items (e.g., light the lamp)
  - **Quit game**: Exit the game

### Example Gameplay

```
=== SWITCHBACK ===

You awaken in an ancient dungeon.
To escape, you must:

1. Find 3 ARTIFACTS hidden in
   locked and dark chambers
2. They will fuse into a MASTER KEY
3. Reach the Sealed Gate to escape

Explore, solve puzzles, collect
items. Good luck, adventurer.

Press BTN2 to begin...
[Press ENTER to continue]

================================
Entrance
================================
A dusty stone entrance hall. An old LAMP rests on the floor near
crumbling pillars. Passages lead east and south into darkness.

Exits: east, south

--- Commands ---
[1] Go east
[2] Go south
[3] Look around
[4] Take item
[5] Use item
[6] Quit game

> 4
Taken.

[Press ENTER to continue]

> 1
(Moves east to Hallway...)
```

### Game Tips

- **Read room descriptions carefully** - they tell you what items are available and where exits lead
- **Take the LAMP first** - you'll need it later
- **Collect the BATTERY** and use the USE action to light the lamp before entering dark rooms
- **Get the KEY** from the Supply Closet to unlock the Locked Chamber
- **All three artifacts are required** - explore thoroughly to find them all
- **Items mentioned in CAPITAL LETTERS** can usually be picked up
- You cannot drop items once collected
- Some rooms are locked or too dark to enter without the right items

## Puzzles

The game contains three main puzzles:

1. **Dark Chamber Access**: Need to light the lamp (requires LAMP + BATTERY + USE action)
2. **Locked Chamber Access**: Need the KEY to unlock the door
3. **Final Escape**: Need all 3 artifacts to create the MASTER KEY

## Game Map

```
        [4] Dark Chamber
             |
[0]---[1]---[3]---[5]---[8]
 |     |           |     Control Room
[7]   [2]      Maintenance
      Locked
      Chamber
        |
       [6]
    Sealed Gate
    (EXIT)
```

For a complete walkthrough and detailed room descriptions, see [docs/rooms.md](docs/rooms.md).

## Project Structure

```
Project-Expo/
├── src/
│   ├── world.c              - Game world logic and room definitions
│   ├── game.c               - Game loop and main game logic
│   ├── main.c               - Entry point for DTEK-V board
│   ├── main_terminal.c      - Entry point for terminal version
│   ├── board.c              - DTEK-V board hardware abstraction
│   ├── board_terminal.c     - Terminal board simulation
│   ├── ui.c                 - DTEK-V UI implementation
│   ├── ui_terminal.c        - Terminal UI implementation
│   ├── input.c              - DTEK-V input handling
│   └── input_terminal.c     - Terminal input handling
├── include/                 - Header files
├── docs/
│   └── rooms.md            - Complete game guide and walkthrough
├── Makefile                - Build for DTEK-V board
└── Makefile.terminal       - Build for native terminal
```

## Technical Details

- Written in C99
- 9 interconnected rooms to explore
- Inventory system supporting up to 10 items
- Dynamic room descriptions based on game state
- Puzzle system with locked doors and dark rooms
- Automatic artifact fusion mechanic

## Development

The game uses a hardware abstraction layer to support both embedded (DTEK-V) and native (terminal) platforms. The core game logic ([world.c](src/world.c), [game.c](src/game.c)) is platform-independent.

### Adding New Rooms

Edit [src/world.c](src/world.c) and modify the `world_init()` function to add or modify rooms.

### Modifying Puzzles

The puzzle logic is in [src/world.c](src/world.c):
- `world_try_move()` - Door locking and darkness checks
- `world_take_item()` - Item collection and artifact fusion
- `world_use_action()` - Lamp lighting logic

## License

Educational project for learning C programming and game development.

---

Enjoy your adventure! Can you escape the ancient dungeon?
