# Raetan
Advanced Raytracer in C++

## Dependencies
...

## Authors
- Logan Kaser
- Theo Walton
- Michael Lu
- Brendan Pierce

## Design

### Ray
- `glm::dvec3 origin` Ray origin
- `glm::dvec3 direction` Ray Direction  
Maybe just `o` and `d` instead?

### Intersect
-	`glm::dvec3 position`
-	`glm::dvec3 normal`
-	`glm::dvec3 color`

### RawColor
Represents a raw pre gamma correction color
- `glm::dvec3 color`
- `double depth`

### Scene
Represents a collection of objects that can be rendered
- `Scene(std::string scenefile)` -> Constructs a scene object
- `trace(Ray& ray)` -> `Intersect` is pure

### Data flow
Camera -> Scene -> RawColor -> Filters -> Gamma -> Screen Buffer
