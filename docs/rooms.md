# Adventure Game - Complete Guide

## Game Overview

You awaken in an ancient dungeon complex. Your goal is to explore the interconnected chambers, collect mysterious artifacts, and find a way to escape through the Sealed Gate.

## Win Condition

To win the game, you must:
1. Collect all three ARTIFACTS (A1, A2, A3) which will automatically fuse into a MASTER KEY
2. Navigate to the Sealed Gate (Room 6)
3. Use the MASTER KEY to unlock the gate and escape

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

### Directional Map with Details

```
                    [4] Dark Chamber
                    (ARTIFACT A2 - requires lit lamp)
                         |
                       south
                         |
[0] Entrance ----east---[1] Hallway ----east---[3] Supply Closet
(LAMP)            |                                 (KEY)
  |             north                                |     |
south         (locked)                             north east
  |               |                                  |     |
[7] Hidden      [2] Locked Chamber              [4] Dark [5] Maintenance
   Alcove      (ARTIFACT A1 - requires KEY)      Chamber  (BATTERY)
(ARTIFACT A3)       |                                        |
                  south                                    north
                    |                                        |
                 [6] Sealed Gate                      [8] Control Room
              (EXIT - requires MASTER KEY)           (no items)
```

## Room Details

### Room 0: Entrance
- **Description**: A dusty stone entrance hall. An old LAMP rests on the floor near crumbling pillars. Passages lead east and south into darkness.
- **Item**: LAMP
- **Exits**: East (to Hallway), South (to Hidden Alcove)
- **Special**: Starting location

### Room 1: Hallway
- **Description**: A long corridor with weathered stone walls. To the north, a massive sealed door shows signs of an ancient lock mechanism. Doorways branch east and west.
- **Item**: None
- **Exits**: North (to Locked Chamber - requires KEY), East (to Supply Closet), South (to Entrance), West (to Entrance)
- **Special**: Central hub connecting multiple areas

### Room 2: Locked Chamber
- **Description**: A sealed chamber marked with protective symbols. The air hums with residual energy. An ancient ARTIFACT (A1) hovers on a pedestal, pulsing with blue light.
- **Item**: ARTIFACT A1
- **Exits**: North (to Sealed Gate), South (to Hallway)
- **Special**: **LOCKED** - Requires KEY from Supply Closet to enter

### Room 3: Supply Closet
- **Description**: A cluttered storage room filled with scattered equipment and broken crates. Among the debris, a brass KEY hangs from a rusted hook on the wall.
- **Item**: KEY
- **Exits**: North (to Dark Chamber), East (to Maintenance), West (to Hallway)
- **Special**: Contains the KEY needed to unlock Locked Chamber

### Room 4: Dark Chamber
- **Description**: Complete darkness engulfs everything. Cold stone walls radiate an ancient chill. You sense something valuable is here, but cannot see without light. [ARTIFACT A2 hidden in darkness]
- **Item**: ARTIFACT A2
- **Exits**: South (to Supply Closet)
- **Special**: **DARK ROOM** - Cannot enter without lit lamp. Must combine LAMP + BATTERY first

### Room 5: Maintenance
- **Description**: A technical room with rusty pipes and old machinery. On a metal shelf, you spot a BATTERY PACK still sealed in weathered packaging.
- **Item**: BATTERY
- **Exits**: North (to Control Room), West (to Supply Closet)
- **Special**: Contains BATTERY needed to power the lamp

### Room 6: Sealed Gate
- **Description**: The final chamber. A towering gate blocks the exit to freedom. At its center, an intricate mechanism awaits a MASTER KEY to unlock your escape.
- **Item**: None
- **Exits**: South (to Locked Chamber)
- **Special**: **EXIT ROOM** - Win condition achieved here with MASTER KEY

### Room 7: Hidden Alcove
- **Description**: A narrow alcove hidden from the main path. Ancient markings cover the walls in faded script. Something glints in the shadows - an ARTIFACT (A3) embedded in the stone.
- **Item**: ARTIFACT A3
- **Exits**: North (to Entrance)
- **Special**: Easy-to-reach artifact

### Room 8: Control Room
- **Description**: An abandoned control center with dead terminals and darkened screens. A flickering holographic map shows the dungeon layout, casting eerie blue light across dust-covered consoles.
- **Item**: None
- **Exits**: South (to Maintenance)
- **Special**: Optional exploration area, no items

## Items & Inventory

### Collectible Items

| Item | Location | Purpose |
|------|----------|---------|
| **LAMP** | Entrance (Room 0) | Required to see in dark rooms |
| **KEY** | Supply Closet (Room 3) | Unlocks the Locked Chamber (Room 2) |
| **BATTERY** | Maintenance (Room 5) | Powers the lamp when used together |
| **ARTIFACT A1** | Locked Chamber (Room 2) | Combine with A2 & A3 to create MASTER KEY |
| **ARTIFACT A2** | Dark Chamber (Room 4) | Combine with A1 & A3 to create MASTER KEY |
| **ARTIFACT A3** | Hidden Alcove (Room 7) | Combine with A1 & A2 to create MASTER KEY |
| **MASTER KEY** | Auto-created | Unlocks the Sealed Gate for escape |

### Special Item Mechanics

- **Lamp + Battery**: Use the "USE" action when you have both items. The battery will be consumed and the lamp will be permanently lit.
- **Three Artifacts**: When you collect all three artifacts (A1, A2, A3), they automatically fuse together to create the MASTER KEY.

## Game Mechanics

### Actions
- **MOVE**: Navigate between rooms using directional commands (north, south, east, west)
- **TAKE**: Pick up items in the current room
- **USE**: Combine lamp and battery to light the lamp
- **Inventory**: View collected items

### Puzzles

#### Puzzle 1: Dark Chamber Access
- **Challenge**: Room 4 is pitch black and cannot be entered
- **Solution**:
  1. Collect LAMP from Entrance
  2. Collect BATTERY from Maintenance
  3. Use the USE action to light the lamp
  4. Now you can enter the Dark Chamber and collect ARTIFACT A2

#### Puzzle 2: Locked Chamber Access
- **Challenge**: Room 2 is locked and cannot be entered
- **Solution**:
  1. Collect KEY from Supply Closet
  2. Return to Hallway
  3. Move north - the KEY will automatically unlock the door
  4. Collect ARTIFACT A1

#### Puzzle 3: Escape Assembly
- **Challenge**: Need MASTER KEY to escape through Sealed Gate
- **Solution**:
  1. Collect all three artifacts (A1, A2, A3)
  2. They automatically fuse into MASTER KEY
  3. Navigate to Sealed Gate (Room 6)
  4. Victory is achieved automatically when you reach Room 6 with MASTER KEY

## Walkthrough

### Optimal Solution Path

1. **Start** at Entrance (Room 0)
2. **TAKE** the LAMP
3. **Move SOUTH** to Hidden Alcove (Room 7)
4. **TAKE** ARTIFACT A3
5. **Move NORTH** back to Entrance (Room 0)
6. **Move EAST** to Hallway (Room 1)
7. **Move EAST** to Supply Closet (Room 3)
8. **TAKE** the KEY
9. **Move EAST** to Maintenance (Room 5)
10. **TAKE** the BATTERY
11. **USE** to light the lamp (combines LAMP + BATTERY)
12. **Move WEST** back to Supply Closet (Room 3)
13. **Move NORTH** to Dark Chamber (Room 4) - now possible with lit lamp
14. **TAKE** ARTIFACT A2
15. **Move SOUTH** back to Supply Closet (Room 3)
16. **Move WEST** to Hallway (Room 1)
17. **Move NORTH** to Locked Chamber (Room 2) - KEY unlocks door
18. **TAKE** ARTIFACT A1 - *MASTER KEY auto-created!*
19. **Move NORTH** to Sealed Gate (Room 6)
20. **Victory!** - Escape achieved with MASTER KEY

### Alternative Exploration

Players can explore rooms in any order, but must complete the three puzzles to access all artifacts:
- Easiest: ARTIFACT A3 (just move south from start)
- Medium: ARTIFACT A2 (need LAMP + BATTERY, then USE action)
- Hardest: ARTIFACT A1 (need KEY first, navigate to locked room)

## Tips & Hints

- Explore thoroughly - not all rooms contain items, but they may provide context
- The Control Room (Room 8) is optional but adds to the story
- You cannot drop items - inventory is permanent
- The BATTERY is consumed when lighting the lamp, but the lit lamp is permanent
- Once a door is unlocked, it stays unlocked
- The game displays available exits in each room
- Pay attention to room descriptions - they hint at items and obstacles

## Technical Notes

- Maximum inventory: 10 items
- Total rooms: 9
- Required items for completion: 6 (LAMP, BATTERY, KEY, A1, A2, A3)
- Minimum moves to complete: ~20 actions
