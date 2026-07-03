# Legends of Valor

A console-based Role-Playing Game (RPG) built in C++

## Overview

Legends of Valor lets a player create a custom character, choose a class (Warrior, Mage, or Rogue), and progress through a dynamic game world involving exploration, turn-based combat, inventory management, and character growth. The project is built around a class hierarchy modeling core game entities — Player, Character subclasses, Items, Enemies, and the Game Engine — and demonstrates the practical application of core OOP concepts in a modular, extensible codebase.

## OOP Concepts Demonstrated

- Constructors, destructors, and copy constructors (deep copy of nested objects)
- Inheritance and polymorphism (base `Character` class extended by `Warrior`, `Mage`, `Rogue`, and `Enemy` types)
- Virtual functions (class-specific skill and combat behavior)
- Operator overloading
- Friend classes / friend functions
- Static data members and functions (e.g. tracking total player instances)
- Const accessor (getter) functions for read-only, encapsulated access
- Exception handling for invalid input and corrupted save data

## Features

| Module | Description |
|---|---|
| Character Creation | Custom name and class selection with unique base stats and skill trees |
| Combat System | Turn-based battles: attack, use skill, defend, or use item; agility-based turn order; multi-phase boss fights |
| Inventory | Add/remove items, category filtering (potions, weapons, armor, quest items), stack merging, size limits |
| Exploration | Map/graph-based movement across areas with random events and locked zones |
| Progression | XP gain, leveling, stat upgrades, and class evolutions |
| Save/Load | Persist and restore full game state, with corrupted-file handling |
| Shop | Buy weapons, potions, and gear with in-game currency |
| Reporting | Battle history log and end-of-game performance report |

## Project Structure

class hierarchy:

```
Character (abstract base)
 ├── Warrior
 ├── Mage
 └── Rogue

Enemy (abstract base)
 ├── NormalEnemy
 └── Boss

Item (abstract base)
 ├── Potion
 ├── Weapon
 ├── Armor
 └── QuestItem

Inventory
Map / Area
GameEngine
```

## Build Instructions

1. Open the project folder in MS Visual Studio (or any C++ IDE supporting C++11 or later).
2. Build the solution.

Or compile via command line (g++):

```bash
g++ -std=c++17 *.cpp -o legends_of_valor
./legends_of_valor
```


## Modules Completed

*(List which modules are implemented and working, e.g.)*
- [x] Create Player
- [x] Show Player Stats
- [x] Inventory (Add/Remove/Show)
- [x] Explore Map
- [x] Combat (Fight Enemy, Use Skill)
- [ ] Boss Fight
- [ ] Save/Load Game
- [ ] Final Score Report

## Authors

-24F-0634

## Notes

- All naming conventions follow the assignment spec: classes start with a capital letter, variables/identifiers use camelCase.
- Exception handling is used throughout to prevent crashes from invalid input or corrupted data.
