#pragma once

#include <cmath>

namespace raytracer::math {

class Vector3D {
public:
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Vector3D() = default;
    Vector3D(double xValue, double yValue, double zValue);

    double lengthSquared() const;
    double length() const;
    double dot(const Vector3D &other) const;
    Vector3D cross(const Vector3D &other) const;
    Vector3D normalized() const;

    Vector3D operator+() const;
    Vector3D operator-() const;

    Vector3D operator+(const Vector3D &other) const;
    Vector3D &operator+=(const Vector3D &other);

    Vector3D operator-(const Vector3D &other) const;
    Vector3D &operator-=(const Vector3D &other);

    Vector3D operator*(double value) const;
    Vector3D &operator*=(double value);

    Vector3D operator/(double value) const;
    Vector3D &operator/=(double value);
};

Vector3D operator*(double value, const Vector3D &vector);

} // namespace raytracer::math
