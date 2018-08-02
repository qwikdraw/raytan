# Raytan
Advanced Raytracer in C++

## Dependencies
- lodepng
- glm
- Qt 5.11

## Building
### Requires
- clang
- Qt's `moc`
- make
- pkg-config

### Instructions
`make`

## Authors
- Logan Kaser
- Theo Walton
- Brendan Pierce

## About
Raytan is a fast raytracer written in C++ with a focus on practical effects. It supports primitive spheres, cones, cylinders, elipsoids, planes, cubes and sheets. It also supports compound shapes that result from addition and subtraction of 2 shapes. The color, material properties (diffuse, reflection, refraction, refractive index) and normals can all be changed using textures, which lets you create realistic 3D sprites that can seamlessly fit into scenes.

Raytan has a GUI written with QT that gives you control over ambient, shadow softness, light samples, recursion level and antialiasing. The GUI also lets you choose various post effects such as cartoon, tint + saturation, motion blur, edge detection and 3D, and lets you save your image.
