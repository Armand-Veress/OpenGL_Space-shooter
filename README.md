# OpenGL university project following the Universal Rendering Pipeline structure.

* **Project Objective:** OpenGL implementation of a Universal Real-Time Rendering Pipeline for an asteroid shooting space simulation.

## Project Overview
This project is an OpenGL 3.3+ application that demonstrates the core principles of the graphics pipeline. The player controls a spacecraft with the critical mission of defending Earth from an endless wave of incoming asteroids.

* **Start of the game:** Your ship starts at the margin of the galaxy. Travel fast back to earth to intercept incoming asteroid threats.
* **Survival Logic:** The game is an endless survival challenge.
* **Game Over Condition:** If an asteroid bypasses your defenses and strikes Earth, the simulation terminates.
* **Scoring System:** Your final performance metric is calculated and displayed in the terminal upon exit.
* **Game controls:**
    * The ship is **moving forward** at a constant speed
    * Control direction with **mouse cursor** or **WASD** keys
    * Press **Shift** or **Right click** for a speed boost
    * Press **Space bar** or **Left click** to shoot asteroids

 
## Technical Stack & Architecture

**Core Libraries**
* **GLAD:** The OpenGL Loader that serves as the interface between the application and the GPU drivers, dynamically loading modern OpenGL function pointers.
* **GLFW:** A multi-platform library used for window creation, OpenGL context management, and real-time input handling.
* **GLM (OpenGL Mathematics):** A header-only C++ mathematics library used for vector and matrix operations (MVP transformations).
* **stb_image (by Sean Barrett):** A lightweight, single-header image loading tool used to decode texture assets into raw pixel data.
* **Dear ImGui:** An immediate-mode GUI used for real-time debugging and on-screen simulation parameters.

**Rendering Pipeline Stages**
* **Application Stage:** Logic handling, including collision detection between asteroids and the ship/Earth, frame timing, and user input processing via GLFW.
* **Geometry Stage:** Processing of vertex data through Model-View-Projection (MVP) matrices to transform 3D coordinates into screen space.
* **Pixel Processing:** Final fragment output generated through custom GLSL Shaders (Vertex & Fragment) using 2D Texture Mapping.

## How to run
  1. Clone repository and navigate to root folder
     
* **Windows:**
  
  2\. Build & compile
  ```powershell
  mkdir build
  cd build
  cmake ..
  cmake --build .
  cd ..
  ```
  3\. Run the game
  ```powershell
  ./bin/Debug/OpenGL_SpaceShooter.exe
  ```
  
* **Linux:**
  
  2\. Install dependencies:
     * **Fedora distribution:**
     ```bash
     sudo dnf install @development-tools cmake gcc-cpp libX11-devel mesa-libGL-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel
     ```
     * **Ubuntu / Debian distributions:**
     ```bash
     sudo apt install build-essential cmake libx11-dev libgl1-mesa-dev libxrandr-dev
     ```
     
  3\. Build & compile
  ```bash
  mkdir build
  cd build
  cmake ..
  cmake --build .
  cd ..
  ```
  4\. Run the game
  ```bash
  ./bin/OpenGL_SpaceShooter
  ```

## Team members:
* Anghel Mihai-Alexandru
* Veress Armand
