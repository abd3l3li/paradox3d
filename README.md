# paradox3d

paradox3d is a simple 3D rendering project based on raycasting, inspired by early FPS games like Wolfenstein 3D.
The goal is to simulate a 3D environment from a 2D map using C and basic graphics concepts.

## Features
- Real-time 3D projection using raycasting
- Textured walls
- Player movement and rotation
- Collision detection
- Configurable maps and textures

## Controls
- W, A, S, D : move the player
- Arrow keys : rotate the camera
- ESC : exit the program

## Map
The map is defined in a .cub file and includes:
- Wall layout
- Player spawn position
- Texture paths
- Floor and ceiling colors

## Compilation & Run
```
make
./paradox3d maps/midnight_map.cub
```
## Project Goal
This project focuses on:
- Understanding raycasting
- Working with graphics libraries
- Improving math and game logic skills
- Writing clean, structured C code
