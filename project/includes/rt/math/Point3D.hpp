#pragma once

#include "rt/math/Vector3D.hpp"

namespace raytracer::math {

class Point3D {
public:
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Point3D() = default;
    Point3D(double xValue, double yValue, double zValue);

    Point3D operator+(const Vector3D &vector) const;
    Point3D &operator+=(const Vector3D &vector);

    Point3D operator-(const Vector3D &vector) const;
    Point3D &operator-=(const Vector3D &vector);

    Vector3D operator-(const Point3D &other) const;
};

} // namespace raytracer::math
