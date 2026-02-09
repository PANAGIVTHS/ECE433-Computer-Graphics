# ECE433 Computer Graphics

This collection contains coursework for the **ECE433 Computer Graphics** course at the **University of Thessaly** (Fall 2025).

While there are several exercises here, the most interesting project of this repo is **Project 3: Visiting a 3D House**. It's a fully interactive 3D graphics engine built from scratch using C++ and OpenGL.

## 3D House Simulation

For the final project we built a modular mini-engine to render a fully furnished house, a surrounding forest, and a dynamic environment. You can walk (or fly) through the house, toggle lights, and watch the day turn into night.

### Features

- **Interactive exploration:** First-person camera control (WASD + Mouse).
- **Day/Night cycle:** The sun and moon actually orbit the scene. As they move, the sky color changes from blue to sunset orange to dark blue, and the lighting shifts accordingly.
- **House:** Fully furnished with bedrooms, a kitchen, a living room with a TV, and a garage containing a 3D model of a futuristic car.
- **Outdoor environment:** Procedural terrain with a forest, mountains generated using **NURBS/Splines**, and a *Minecraft* Nether Portal with a light source attached to it.
- **Very extinsible classes (`Object.h`):** More complex classes like the Object class describing the objects in the world and providing features like full state description, tree-like structures and sub-classes.
- **Custom asset system:** We wrote our own `.txt` parser. Instead of hardcoding every vertex, we load furniture and layout data from text files, making it easy to "redecorate" the house without recompiling code.
- **Lighting:** Support for ambient, diffuse, and specular lighting with multiple light sources including a flashlight moving along the player.

## Controls

The simulation uses standard FPS controls. You can choose to use the mouse to look around (recommended) or stick to the keyboard.

| Action | Key (Mouse Mode) | Key (Keyboard Only) |
| --- | --- | --- |
| **Move** | `W`, `A`, `S`, `D` | `W`, `A`, `S`, `D` |
| **Fly Up / Down** | `Space` / `Z` | `Space` / `Z` |
| **Look Around** | Mouse Movement | `I` (Up), `K` (Down), `J` (Left), `L` (Right) |
| **Flashlight** | `E` | `E` |
| **Toggle Flight Mode** | `F` | `F` |
| **Reload Assets** | `R` | `R` |
| **Fullscreen** | `F11` | `F11` |
| **Quit** | `ESC` or `Q` | `ESC` or `Q` |

## Build & Run

The project is designed for **Linux**. We've included a `Makefile` to handle compilation.

### Prerequisites

Make sure you have `freeglut` and `OpenGL` libraries installed on your system.

### Compilation

To compile with **Mouse Control enabled** (recommended):

**Linux:**

```⁩bash
cd Hw3/exercise/src
make all -j $(nproc)

⁨```

To compile with **Keyboard Control only** (if mouse warping issues occur):
Add `USE_MOUSE=0` to the end of your make command.

### Running the game

Once compiled, run the executable:

```⁩bash
./visiting3Dhouse

⁨```

---

## Authors

* **Ioannis Iason Nikas**
* **Anastasios Kalousis**
* **Panagiotis Nikolaos Tsogkas**

*Department of Electrical and Computer Engineering, University of Thessaly.*⁩
