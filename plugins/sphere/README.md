# Sphere Plugin

A simple geometric primitive plugin that renders spheres in the raytracer using ray-sphere intersection.

## Parameters

The sphere plugin accepts the following parameters in the configuration file:

| Parameter | Type | Description | Example |
|-----------|------|-------------|---------|
| `name` | string | **Required.** Must be `"sphere"` to load this plugin | `"sphere"` |
| `position.x` | number | X coordinate of sphere center | `0` |
| `position.y` | number | Y coordinate of sphere center | `5` |
| `position.z` | number | Z coordinate of sphere center | `10` |
| `radius` | number | Sphere radius in scene units | `2.0` |
| `color.r` | integer | Red channel (0-255) | `255` |
| `color.g` | integer | Green channel (0-255) | `100` |
| `color.b` | integer | Blue channel (0-255) | `50` |

## Configuration Example

```libconfig
primitives = [
    {
        name = "sphere";
        position = { x = 0; y = 0; z = 0; };
        radius = 2.0;
        color = { r = 255; g = 100; b = 100; };
    },
    {
        name = "sphere";
        position = { x = -4; y = 1; z = 2; };
        radius = 1.5;
        color = { r = 100; g = 255; b = 100; };
    },
    {
        name = "sphere";
        position = { x = 4; y = 0; z = 1; };
        radius = 1.0;
        color = { r = 100; g = 100; b = 255; };
    }
];
```

## Implementation Details

- **Ray-Sphere Intersection**: Uses quadratic formula to compute intersection with sphere
- **Normal Calculation**: Computed as normalized vector from sphere center to hit point
- **Shading**: Diffuse lighting with hard-coded directional light
- **Precision**: Ignores intersections closer than 0.001 units to avoid self-intersection artifacts

## Notes

- The sphere center is defined in world space coordinates
- All coordinates and radii use the same unit system as the scene
- Color values are clamped to 0-255 range automatically
- Multiple spheres can be defined in the `primitives` array
