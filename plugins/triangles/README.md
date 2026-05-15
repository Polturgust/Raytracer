# Triangle Plugin

## Description
A 3D triangle object for ray tracing. Implements ray-triangle intersection using the Möller–Trumbore algorithm.

## Parameters

All parameters are provided as a `std::map<std::string, std::string>` from the configuration file.

### Required Parameters

| Parameter | Type | Description | Default |
|-----------|------|-------------|---------|
| `v0.x` | double | X coordinate of first vertex | 0.0 |
| `v0.y` | double | Y coordinate of first vertex | 0.0 |
| `v0.z` | double | Z coordinate of first vertex | 0.0 |
| `v1.x` | double | X coordinate of second vertex | 1.0 |
| `v1.y` | double | Y coordinate of second vertex | 0.0 |
| `v1.z` | double | Z coordinate of second vertex | 0.0 |
| `v2.x` | double | X coordinate of third vertex | 0.0 |
| `v2.y` | double | Y coordinate of third vertex | 1.0 |
| `v2.z` | double | Z coordinate of third vertex | 0.0 |
| `color.r` | int | Red component (0-255) | 255 |
| `color.g` | int | Green component (0-255) | 255 |
| `color.b` | int | Blue component (0-255) | 255 |

## Example Configuration

```libconfig
triangle = (
    {
        name = "triangle";
        v0 = { x = 0; y = 0; z = 5; };
        v1 = { x = 2; y = 0; z = 5; };
        v2 = { x = 1; y = 2; z = 5; };
        color = { r = 0; g = 255; b = 0; };
    }
);
```

## Notes

- Vertices must be defined as `v0`, `v1`, `v2` (not `a`, `b`, `c`)
- The triangle uses flat shading with normal computed as the cross product of edges
- Intersection uses barycentric coordinates for accuracy
- Normal points consistently based on vertex winding order (right-hand rule)
- Color values should be in range [0, 255]
