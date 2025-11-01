# Timber Remake (C++ / SFML)

A remake of the classic **Timber!** game, developed in **C++** using the **SFML** library.  
This project began as a hands-on way for me to understand the fundamentals of **game development**, but quickly evolved into my own implementation with optimizations, cleaner logic, and improved structure beyond the original tutorial.

---

## Purpose

This side project serves as a learning platform for:
- Understanding the **game loop**, **rendering pipeline**, and **real-time input handling**
- Building a playable and optimized 2D game in **C++**
- Applying new programming concepts learned from my coursework and self-study
- Experimenting with **SFML (Simple and Fast Multimedia Library)** for graphics, input, and sound

I wanted to go beyond simply following a tutorial — rewriting and improving logic to understand **why** things work, not just **how**.

---

## Tech Stack

| Component | Technology |
|------------|-------------|
| Language | C++ |
| Graphics / Audio | SFML (Simple and Fast Multimedia Library) |
| IDE | Visual Studio / VS Code |
| Version Control | Git + GitHub |
| Platform | Windows (can be ported to Linux) |

---

## Features Implemented

- Real-time **player movement and input detection**
- Optimized **game loop** with clean delta time usage
- Custom logic for **branch spawning**, **score system**, and **time bar**
- **Collision detection** and game-over state management
- Object-oriented structure for improved readability and scalability
- Memory-safe implementation (avoiding unnecessary heap allocations)
- Modularized updates for better maintainability

---

## Planned Improvements

- Add animations and smoother transitions  
- Implement sound effects and background music using SFML Audio  
- Add menus (start, pause, game over)  
- Refactor repeated logic into reusable classes  
- Experiment with entity-based design (ECS pattern)

---

## Learning Goals

Through this project, I’m strengthening:
- **C++ fundamentals** — pointers, references, memory management, and object design  
- Understanding of **game engine architecture** and event-driven loops  
- Proficiency with **SFML’s graphics, input, and audio systems**  
- Writing **clean, efficient, and modular code** suitable for larger projects

---

## Build Instructions

1. Install [SFML](https://www.sfml-dev.org/)
2. Clone this repository:
   ```bash
   git clone https://github.com/acruzgarcia21/TimberRemake.git
