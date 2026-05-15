#pragma once

#include "Vector3D.hpp"

namespace raytracer::math {

class Point3D {
public:
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Point3D() = default;
    Point3D(double xValue, double yValue, double zValue) : x(xValue), y(yValue), z(zValue) {};

    Point3D operator+(const Vector3D &vector) const {return Point3D(x + vector.x, y + vector.y, z + vector.z);};
    Point3D &operator+=(const Vector3D &vector) {x += vector.x; y += vector.y; z += vector.z; return *this;};

    Point3D operator-(const Vector3D &vector) const {return Point3D(x - vector.x, y - vector.y, z - vector.z);};
    Point3D &operator-=(const Vector3D &vector) { x -= vector.x; y -= vector.y; z -= vector.z; return *this;};

    Vector3D operator-(const Point3D &other) const {return Vector3D(x - other.x, y - other.y, z - other.z);};
};

} // namespace raytracer::math
