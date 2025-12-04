# Data Structures in the Arcade Game Collection

This repository contains a small C project that shows how basic data structures are used in simple 2D games. The focus is not on fancy graphics, but on how arrays, structs, and simple state management can be combined to build playable games.

The main example is the `Arcade Game Collection`, which currently includes two games:
- `Dino Run`
- `Flappy Bird`

Everything is written in C and organized so you can see where and how each piece of game state is stored and updated.

## Project Layout

```
Data-Structures-and-Algorithms/
├── README.md
└── Arcade Game Collection/
	├── main.c          // menu / game launcher
	├── common.h
	├── common.c        // shared helpers and data
	├── dino_run.h
	├── dino_run.c      // Dino Run implementation
	├── flappy_bird.h
	├── flappy_bird.c   // Flappy Bird implementation
```

## How Data Structures Are Used

### Common Module (`common.c` / `common.h`)

- Uses C `struct`s to represent reusable concepts (for example, a generic game object or configuration).
- Centralizes shared data that is used by both games (for example, screen dimensions, input handling helpers, or random number utilities).

### Dino Run (`dino_run.c` / `dino_run.h`)

- **Arrays of obstacles**: Stores the active obstacles in an array so the game loop can iterate over them each frame, move them, and check for collisions with the player.
- **Player state struct**: Groups together the dino's position, velocity, jump state, and other attributes into a single `struct` so it can be passed around and updated cleanly.
- **Game state variables**: Tracks whether the game is running, paused, or over using a small set of state variables (a simple state machine).
- **Score tracking**: Keeps the current score and possibly high score in basic numeric variables that are updated when the player survives longer or passes obstacles.

### Flappy Bird (`flappy_bird.c` / `flappy_bird.h`)

- **Arrays of pipes**: Stores pipe positions and gaps in an array so each frame can update positions and detect when the bird collides or successfully passes through a gap.
- **Bird state struct**: Similar to Dino Run, groups the bird's position, velocity, and alive/dead state into a `struct`.
- **Game loop state**: Maintains a simple state machine (ready, playing, game over) using variables or enums to control which logic to run.
- **Shared helpers**: Reuses functions from `common.c` to avoid duplicating utility logic between games.

Overall, the project shows how even small games naturally organize their data into arrays, structs, and simple state machines so the main game loop can iterate, update, and render consistently.

## Building and Running

This project is designed to be built with `make` from within the `Arcade Game Collection` folder.

From the root of the repository:

```bash
cd "Arcade Game Collection"
make
```

This should produce a single executable (commonly named something like `arcade` or `main`, depending on your `Makefile`).

To run the compiled game collection:

```bash
./arcade    # or ./main, depending on the Makefile
```

If your `Makefile` has a specific target for running everything at once, you can also use:

```bash
make run
```

## Cleaning Build Artifacts

To remove the compiled executable and object files after building, run:

```bash
cd "Arcade Game Collection"
make clean
```

This will delete temporary build outputs so you can rebuild from a clean state.

## What You Can Learn Here

- How to represent game entities (player, obstacles, pipes) with C `struct`s.
- How to store multiple entities in arrays and loop over them each frame.
- How to keep game state (scores, modes, flags) in a small set of well-defined variables.
- How to organize a small C project into multiple `.c` / `.h` files.
