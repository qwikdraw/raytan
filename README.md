# Raytan
Advanced Raytracer in C++

## About
Raytan is a fast raytracer written in C++ with a focus on practical effects. It supports spheres, cones, cylinders, elipsoids, planes, cubes and sheets. It also supports compound shapes that result from addition and subtraction of two shapes.  
This is implemented using a distance field algorithm.  
The color, material properties (diffuse, reflection, refraction, refractive index) and normals can all be mapped to textures, to allow the creation of realistic 3D sprites that can seamlessly fit into scenes.

Raytan has a QT GUI that gives you control over ambient, shadow softness, light samples, recursion level and antialiasing. The GUI allows for the application of various post effects such as cartoon, tint + saturation, motion blur, edge detection without re-rendering.

# Screenshots
![gui](https://raw.githubusercontent.com/logankaser/Raytan/master/screenshots/gui.png)
![animation gif](https://raw.githubusercontent.com/logankaser/Raytan/master/screenshots/animation.gif)
![post-processing gif](https://raw.githubusercontent.com/logankaser/Raytan/master/screenshots/beach_cartoon.gif)
![sprite in scene](https://raw.githubusercontent.com/logankaser/Raytan/master/screenshots/sprite_showcase.png)

## Dependencies
- lodepng
- glm
- Qt 5.1

## Building
### Requires
- clang
- Qt's `moc`
- make
- pkg-config

### Instructions
Install dependencies, on macOS with brew you can do this with `make deps`

`make`

## Authors
- Logan Kaser
- Theo Walton
- Brendan Pierce
