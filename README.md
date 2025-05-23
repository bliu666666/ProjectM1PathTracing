# Path Tracing Renderer

This project is an implementation of a rendering engine using the path tracing technique, developed as part of Master 1. Path tracing is a physically realistic rendering method that simulates the behavior of light in a 3D scene.

## Features

- Rendering of 3D scenes with path tracing
- Support for geometric objects:
  - Spheres
  - Axis-Aligned Boxes (AABB)  
- Diffuse materials (Lambertian)
- Antialiasing by multiple sampling
- Gamma correction
- Export to PPM format
- Parallelized with OpenMp
- Support for different object materials:
  - Specular
  - Glossy
  - Dielectric
- Different rendering methods:
  - Path Tracing Monte Carlo
  - Metropolis Light Transport
- Light sources


## Prerequisites

- CMake (version 3.10 minimum)
- C++ compiler supporting C++11
- Python 3.x (for the PPM conversion script)

## Installation

1. Clone the repository :
```bash
git clone [repo_address]
cd ProjectM1PathTracing
```

2. Create and navigate to the build directory :
```bash
mkdir build
cd build
```

3. Generate the build files with CMake :
```bash
cmake ..
```

4. Compile the project :
```bash
make
```

## Use

The program takes several command line arguments :
```bash
./render [width] [height] [samples_per_pixel] [output_path] [max_depth][render_mode]
```

Exemple :
```bash
./render 800 600 100 output.ppm 50 0
```

After execution, you will be prompted to enter the camera parameters:
- Camera position (x,y,z)
- Aim point (x,y,z)
- Up vector (x,y,z)
- Vertical field of view (in degrees) 
- Number of iterations if you choose the mode MLT

### Converting PPM file

A Python script is provided to convert PPM files to other formats :
```bash
python3 convert_ppm.py [outputppm_path] [outputpng_path]
```

## Code Structure

- `src/` : Contains source files
  - `render.h/.cpp` : Main rendering functions
  - `sphere.h/.cpp` : Implementation of spheres
  - `obj.h` : Base class for objects
  - `material.h` : Base class for materials
  - `lambertian.h/.cpp` : Implementation of the diffuse material
  - `vec3.h/.cpp` : Class for 3D vectors
  - `ray.h/.cpp` : Class for rays
  - `camera.h/.cpp` : Camera Management
  - `dielectric.h/.cpp` : Implementation of dielectric (transparent) material
  - `glossy.h/.cpp` : Implementation of glossy material
  - `specular.h/.cpp` : Implementation of specular reflection
  - `emissive.h/.cpp` : Implementation of emissive (light-emitting) material
  - `firstCollision.h/.cpp` : Functions to compute the first ray-object intersection
  - `AABB.h/.cpp` : Axis-Aligned Bounding Box for spatial acceleration
  - `mlt_path.h/.cpp` : Functions related to path tracing using Metropolis Light Transport

## Project Architecture

The project uses an object-oriented architecture with:
1. A hierarchy of geometric objects (Object -> Sphere, AABB)
2. A hierarchy of materials (Material -> Lambertian,Specular,Glossy,Dielectric,Emissive)
3. A recursive ray tracing system for path tracing  
4. An integrated MLT rendering engine