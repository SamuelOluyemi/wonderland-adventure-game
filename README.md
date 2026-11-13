# Wonderland Adventure Game

A C++ text-based adventure game developed as the final project for COMP 206 at Athabasca University.

The game takes players on an interactive journey through Wonderland, where they explore locations, interact with characters, collect items, solve riddles, complete challenges, and navigate their way toward the final destination.

## Project Overview

Wonderland is a console-based adventure game built using C++. It features an interconnected game world, command-based interactions, character and inventory management, riddles, and dynamic game progression.

The application uses a modular design, with game components organized into separate header files. External text files are used to store game content, allowing locations, characters, items, riddles, jokes, and instructions to be loaded independently of the main application logic.

## Features

- **Interactive Gameplay:** Command-based interaction with the game environment.
- **Location Navigation:** Explore interconnected locations using directional commands and aliases.
- **Character Interactions:** Encounter and interact with characters throughout Wonderland.
- **Inventory Management:** Collect, use, consume, wear, and drop available items.
- **Riddles and Challenges:** Solve riddles to progress through different stages of the game.
- **Randomized Content:** Random selection of riddles, jokes, and other game elements.
- **Game State Management:** Track game progression, player status, lives, score, and completion conditions.
- **Input Validation:** Handle unsupported commands, invalid actions, and unavailable interactions.
- **Replay Functionality:** Restart the game after successful completion.
- **External Data Loading:** Load game content from text files rather than hardcoding all content into the application.

## Technologies and Concepts

| Category | Technologies / Concepts |
|---|---|
| Programming Language | C++ |
| Standard | C++17 |
| Programming Paradigm | Object-Oriented Programming |
| Data Structures | Vectors, Structures, Enumerations |
| Generic Programming | Function Templates |
| Memory Concepts | Pointers and References |
| File Handling | Input File Streams (`ifstream`) |
| String Processing | String manipulation and command parsing |
| Software Design | Modular programming and separation of responsibilities |
| Development Tools | Terminal, `vi`, GNU C++ Compiler (g++) |

## Project Structure

```text
wonderland-adventure-game/
│
├── Wonderland.cpp
├── Wonderland.h
├── Actions.h
├── Characters.h
├── Control.h
├── Directions.h
├── Inventory.h
├── Items.h
├── Locations.h
├── RiddlesAndJokes.h
├── Utilities.h
│
├── game_files/
│   ├── action_words.txt
│   ├── characters.txt
│   ├── dad_jokes.txt
│   ├── game_riddles.txt
│   ├── instructions.txt
│   ├── items.txt
│   ├── king_riddles.txt
│   ├── loc_desc.txt
│   ├── playroom_riddles.txt
│   └── queen_riddles.txt
│
├── Wonderland Game Map & Locations.png
├── .gitignore
└── README.md
```

### Architecture and Responsibilities

| File | Responsibility |
|---|---|
| `Wonderland.cpp` | Application entry point |
| `Wonderland.h` | Main game class and overall game coordination |
| `Control.h` | Processes player commands and coordinates game actions |
| `Actions.h` | Defines and validates available actions |
| `Characters.h` | Character definitions and main player attributes |
| `Directions.h` | Direction handling and navigation aliases |
| `Inventory.h` | Player inventory operations |
| `Items.h` | Item definitions and item management |
| `Locations.h` | Game locations and location management |
| `RiddlesAndJokes.h` | Riddle and joke functionality |
| `Utilities.h` | Shared utility functions |

### External Game Resources

The `game_files/` directory contains the external data used by the application, including:

- Character information.
- Item definitions.
- Location descriptions.
- Available action words.
- Game instructions.
- General, King, Queen, and Playroom riddles.
- Dad jokes.

These resources are loaded by the relevant game components at runtime.

## Game Map

The following map illustrates the Wonderland game environment and its interconnected locations.

![Wonderland Game Map and Locations](<Wonderland Game Map & Locations.png>)

## Prerequisites

To compile and run the application, you need:

- A C++ compiler supporting C++17.
- A terminal or command-line environment.
- The complete project source code and `game_files/` directory.

The project can be built using GNU g++ on macOS, Linux, or another compatible environment.

## Compilation and Execution

### 1. Clone the repository

```bash
git clone https://github.com/SamuelOluyemi/wonderland-adventure-game.git
```

### 2. Navigate to the project directory

```bash
cd wonderland-adventure-game
```

### 3. Compile the application

```bash
g++ -std=c++17 -Wall -Wextra -pedantic Wonderland.cpp -o Wonderland
```
    or simply use

```bash
g++ Wonderland.cpp -o Wonderland
```

### 4. Run the game

```bash
./Wonderland
```

**Important:** Run the executable from the project root directory. The application loads its external game resources using relative paths under `game_files/`.

## Gameplay

When launched, the game welcomes the player and offers the opportunity to view the instructions before beginning.

Players navigate Wonderland using available directional commands and interact with their surroundings through supported actions.

Throughout the journey, players can:

1. Explore different locations.
2. Interact with characters.
3. Discover and collect items.
4. Manage their inventory.
5. Solve riddles and complete challenges.
6. Track their score and remaining lives.
7. Reach Wonderland and complete the adventure.

The game also supports restarting after successful completion.

## Learning Outcomes

Developing this project provided practical experience in:

- Designing and implementing a multi-component C++ application.
- Applying object-oriented programming principles.
- Organizing code into reusable modules and header files.
- Working with C++ standard library containers.
- Implementing file input and external data loading.
- Using pointers and references to manage relationships between game objects.
- Developing reusable generic functions using templates.
- Processing user input and validating commands.
- Managing application state and game progression.
- Debugging, testing, and refining application functionality.

## Academic Context

**Course:** COMP 206 – Introduction to Programming (C++)

**Institution:** Athabasca University

**Project:** Final Course Project – Wonderland Adventure Game

This project was developed as an academic programming project to apply C++ programming concepts to the design and implementation of an interactive application.

## Author

**Samuel Oluyemi**

Software Developer | C++ | JavaScript | React | Node.js | Python

GitHub: https://github.com/SamuelOluyemi

---

*Developed for COMP 206 at Athabasca University.*
