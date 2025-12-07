# GET OUT! - Text Adventure Game

An interactive text-based dungeon adventure for the DTEK-V board.

## Setup & Start (DTEK-V Board)

1. **Connect the DTEK-V board** via USB to your computer
2. **Configure JTAG** 
   ```bash
   jtagconfig 
   ```
   Note: In vm terminal
3. **Build and upload** the game:

   ```bash
   make clean 
   make
   ```

4. **Open a terminal** to see the game output (text will display via serial connection)

The game will start automatically and display the introduction.

## How to Play

### Controls
- **Switches (SW0-SW4):** Select a menu option by setting the switch number (e.g., SW3 ON for option 3)
- **Button (BTN2):** Confirm your selection
- **7-Segment Displays:** Show your current choice, inventory count, and room number
- **LEDs:** Display your inventory items (key, lamp, artifacts, etc.)

### Gameplay
Each room presents a menu with numbered options:
- **Movement:** Choose from available exits (north, east, south, west)
- **Look around:** Re-read the room description
- **Take item:** Pick up items in the room (shown in CAPITAL LETTERS)
- **Use item:** Combine or activate items (e.g., light the lamp with matches)
- **Quit game:** Exit

Navigate through 9 connected rooms, collecting items and solving puzzles.

### Key Puzzles
1. **Dark Chamber:** Need a lit lamp to enter (requires LAMP + MATCHES, then use "Use item")
2. **Locked Chamber:** Need the KEY to unlock the door
3. **Master Key:** Collect all 3 ARTIFACTS (A1, A2, A3) - they automatically fuse into a MASTER KEY

## Win Condition

To win the game:
1. Find and collect all **3 ARTIFACTS** hidden throughout the dungeon
2. The artifacts will automatically fuse into a **MASTER KEY**
3. Reach the **Sealed Gate** (room 6) with the MASTER KEY
4. Defeat the **boss fight** (fire-extinguishing challenge using switches and button)
5. Successfully escape to achieve victory!

