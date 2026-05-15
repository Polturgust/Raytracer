# Plane Plugin

## Description
An infinite 3D plane object for ray tracing. Implements ray-plane intersection using the plane equation method.

## Parameters

All parameters are provided as a `std::map<std::string, std::string>` from the configuration file.

### Required Parameters

| Parameter | Type | Description | Default |
|-----------|------|-------------|---------|
| `position.x` | double | X coordinate of a point on the plane | 0.0 |
| `position.y` | double | Y coordinate of a point on the plane | 0.0 |
| `position.z` | double | Z coordinate of a point on the plane | 0.0 |
| `normal.x` | double | X component of plane normal vector | 0.0 |
| `normal.y` | double | Y component of plane normal vector | 1.0 |
| `normal.z` | double | Z component of plane normal vector | 0.0 |
| `color.r` | int | Red component (0-255) | 255 |
| `color.g` | int | Green component (0-255) | 255 |
| `color.b` | int | Blue component (0-255) | 255 |

## Example Configuration

```libconfig
plane = (
    {
        position = { x = 0; y = -3; z = 0; };
        normal = { x = 0; y = 1; z = 0; };
        color = { r = 128; g = 128; b = 128; };
    }
);
```

## Notes

- The plane extends infinitely in all directions perpendicular to the normal
- Normal vector is automatically normalized during object creation
- A point on the plane can be any position; rays are intersected using the plane equation
- Color values should be in range [0, 255]
- The normal direction determines which side is "front-facing" for shading purposes
