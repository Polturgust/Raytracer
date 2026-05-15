#pragma once

#include <cmath>
#include <stdexcept>

namespace raytracer::math {

class Vector3D {
public:
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Vector3D() = default;
    Vector3D(double xValue, double yValue, double zValue) : x(xValue), y(yValue), z(zValue) {};

    double lengthSquared() const {return x * x + y * y + z * z;};
    double length() const {return std::sqrt(lengthSquared());};
    double dot(const Vector3D &other) const {return x * other.x + y * other.y + z * other.z;};
    Vector3D cross(const Vector3D &other) const { return Vector3D( y * other.z - z * other.y, z * other.x - x * other.z,  x * other.y - y * other.x);};
    Vector3D normalized() const {
        const double magnitude = length();

        if (magnitude == 0.0) {
            throw std::runtime_error("Cannot normalize a zero-length vector");
        }
        return *this / magnitude;
    };

    Vector3D operator+() const {return *this;};
    Vector3D operator-() const {return Vector3D(-x, -y, -z);};

    Vector3D operator+(const Vector3D &other) const {return Vector3D(x + other.x, y + other.y, z + other.z);};
    Vector3D &operator+=(const Vector3D &other) {x += other.x; y += other.y; z += other.z; return *this;};

    Vector3D operator-(const Vector3D &other) const {return Vector3D(x - other.x, y - other.y, z - other.z);};
    Vector3D &operator-=(const Vector3D &other) {x -= other.x; y -= other.y; z -= other.z; return *this;};

    Vector3D operator*(double value) const {return Vector3D(x * value, y * value, z * value);};
    Vector3D &operator*=(double value) {x *= value; y *= value; z *= value; return *this;};

    Vector3D operator/(double value) const {return Vector3D(x / value, y / value, z / value);};
    Vector3D &operator/=(double value) {x /= value; y /= value; z /= value; return *this;};
};

inline Vector3D operator*(double value, const Vector3D &vector) {return vector * value;};

} // namespace raytracer::math
