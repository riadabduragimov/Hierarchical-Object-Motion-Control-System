<h1 align = center> Hierarchical Object Motion Control System </h1>

## Table of Contents
1. [Project Overview](#project-overview)
2. [Assignment Description](#assignment-description)
3. [Requirements](#requirements)
4. [How to Build and Run](#how-to-build-and-run)
5. [Input Files](#input-files)
6. [Animation Details](#animation-details)

---

## Project Overview
This project implements a simple **hierarchical articulated figure** consisting of a torso and two legs. The figure moves along a trajectory defined by control points using **Catmull-Rom spline interpolation**. The walking motion is generated using **forward kinematics** with sinusoidal leg swings and torso sway, creating a natural gait.

The animation demonstrates:
- Movement along a 3D path.
- Forward/backward lean and lateral sway of the torso.
- Leg motion synchronized to speed to avoid unnatural motion ("moon walking").

---

## Assignment Description
The goal of this assignment is to generate the motion of a simple articulated figure using hierarchical modeling techniques. The main requirements are:
- Figure must have a **torso and two legs**.
- Walking motion must follow a **trajectory** defined by control points.
- Leg motion can be **sinusoidal** or linearly interpolated.
- Forward kinematics is used to generate leg movement.
- Extensions (optional) include torso sway, hip tilt, and dynamic orientation of the figure along the path.

**Input:**
- Geometric data for the figure (torso, left leg, right leg) in text files.
- Control points file defining the walking trajectory (Catmull-Rom spline with `dt`).

**Output:**
- Animated 3D view of the figure moving along the path.

---

## Requirements
- **C++ Compiler** (Visual Studio recommended)
- **OpenGL / GLUT** libraries included in `lib/`
- Windows or Linux environment with OpenGL support
- Standard C++ libraries (`<vector>`, `<iostream>`, `<fstream>`, `<cmath>`)

---

## How to Build and Run
1. Open the project in Visual Studio (or your preferred C++ IDE).
2. Make sure the **GLUT library** is linked correctly:
   - Include directory: `lib/glut.h`
   - Library directory: `lib/glut32.lib`
3. Build the project.
4. Run the executable. The figure will move along the trajectory defined in `data/path.txt`.
5. Optional: Record the screen to generate a movie for submission.

---

## Input Files
The input files are located in the `data` folder:
- **Torso, Left Leg, Right Leg (`*.txt`)**: geometric dimensions of each part.
- **Path (`path.txt`)**: control points and spacing (`dt`) for the trajectory.  
You can check the exact content of the files in the `data` folder.

---

## Animation Details
- **Catmull-Rom Spline** used for smooth path interpolation.
- **Torso sway**: lateral and forward/backward motion for realism.
- **Leg swing**: sinusoidal motion with frequency proportional to speed.
- **Heading direction**: figure rotates along the path automatically.
- **Ground grid**: rendered as reference for movement in the 3D world.


