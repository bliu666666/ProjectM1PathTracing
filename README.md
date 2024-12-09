# ProjectM1PathTracing

## Intro

This projet aims to implement a simple 3d renderer using the path tracing technique.
It try to intent a realistic simulation of light in a scene by applying the Monte Carlo
method to solve the redering equation.


## Steps for implementations

- [x] Implement a camera system that can compute rays
- [x] Implement ray-spheres and ray-boxes intersection detection, as well as first collision detection
- [x] Validate a first ray casting renderer on simple scenes
- [X] Implement BRDF with Lambertian reflectance
- [ ] Generalize the ray casting renderer to path tracing

## Explanation

### vec3 class

Almost all graphics programs have a vec3 class. She takes 3 cordonates (x, y, z) in enter and define all vector operation
we can do.We’ll use the same class vec3 for colors, locations, directions,etc.

### color 

using the ´vec3´ class, we'll create a ´color.h´ and define a utility function that writes a single pixel's color out to the standard output
stream (in this case we use ppm standard).

### ray class

The one thing that all ray tracers have is a ray class and a computation of what color is seen along a ray.
𝐏(𝑡)=𝐀+𝑡𝐛. Here 𝐏 is a 3D position along a line in 3D. 𝐀 is the ray origin and 𝐛 is the ray direction. The ray parameter 𝑡
is a real number (double in the code). Plug in a different 𝑡 and 𝐏(𝑡) moves the point along the ray. dd in negative 𝑡
values and you can go anywhere on the 3D line

### viewport 

![image info](/image/readme/viewport.jpeg)

### First rendering

rendering a blue-to-white background.

![image info](/image/rendering/first.png)

### First Object

![image info](/image/rendering/image_sphere.png)

### Shading effect


![image info](/image/rendering/sphere_shading.png)

### Anti-aliasing

![image info](/image/rendering/Antialiasing.png)

## Lambertian

![image info](/image/readme/image_sphere_lambertian.png)
