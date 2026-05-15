# Sphere Plugin

## Description
A 3D sphere object for ray tracing. Implements ray-sphere intersection using the quadratic discriminant method.

## Parameters

All parameters are provided as a `std::map<std::string, std::string>` from the configuration file.

### Required Parameters

| Parameter | Type | Description | Default |
|-----------|------|-------------|---------|
| `position.x` | double | X coordinate of sphere center | 0.0 |
| `position.y` | double | Y coordinate of sphere center | 0.0 |
| `position.z` | double | Z coordinate of sphere center | 0.0 |
| `radius` | double | Sphere radius | 1.0 |
| `color.r` | int | Red component (0-255) | 255 |
| `color.g` | int | Green component (0-255) | 255 |
| `color.b` | int | Blue component (0-255) | 255 |

## Example Configuration

```libconfig
sphere = (
    {
        name = "sphere";
        position = { x = 0; y = 0; z = 5; };
        radius = 2.5;
        color = { r = 255; g = 0; b = 0; };
    }
);
```

## Notes

- Radius must be non-negative
- The sphere uses flat shading with its normal computed as the normalized vector from center to intersection point
- Color values should be in range [0, 255] (clamped if out of range)
