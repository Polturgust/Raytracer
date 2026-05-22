# Mesh/OBJ Plugin

## Description
Loads 3D mesh geometry from Wavefront OBJ files. Parses vertices and faces, converts polygons to triangles, and handles ray-mesh intersection.

## Parameters

All parameters are provided as a `std::map<std::string, std::string>` from the configuration file.

### Required Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `file` | string | Path to the OBJ file (relative to working directory) |

### Optional Parameters

| Parameter | Type | Description | Default |
|-----------|------|-------------|---------|
| `position.x` | double | X coordinate offset for entire mesh | 0.0 |
| `position.y` | double | Y coordinate offset for entire mesh | 0.0 |
| `position.z` | double | Z coordinate offset for entire mesh | 0.0 |
| `scale` | double | Uniform scale factor for all vertices | 1.0 |
| `rotation.x` | double | Rotation around X axis in degrees | 0.0 |
| `rotation.y` | double | Rotation around Y axis in degrees | 0.0 |
| `rotation.z` | double | Rotation around Z axis in degrees | 0.0 |
| `color.r` | int | Red component (0-255) | 200 |
| `color.g` | int | Green component (0-255) | 200 |
| `color.b` | int | Blue component (0-255) | 200 |

## Example Configuration

```libconfig
mesh = (
    {
        file = "scenes/cube.obj";
        position = { x = 0; y = 0; z = 5; };
        scale = 2.0;
        color = { r = 100; g = 100; b = 100; };
    }
);
```

## OBJ File Support

- **Vertices** (`v`): 3D coordinates
- **Faces** (`f`): Triangles and polygons (automatically triangulated using fan triangulation)
- **Texture coordinates** (`vt`): Parsed but ignored
- **Normals** (`vn`): Parsed but ignored (computed per-triangle)

File format example:
```
v 1.0 1.0 -1.0
v 1.0 -1.0 -1.0
...
f 1 2 3
f 1 3 4
```

## Notes

- Position offset is applied after scaling
- Mesh subdivision follows fan triangulation (first vertex + each edge)
- Ray intersection returns the closest hit among all internal triangles
- Normal is computed per-triangle using cross product of edges
- OBJ indices are 1-based; the plugin converts to 0-based internally
