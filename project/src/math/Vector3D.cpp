#include "math/Vector3D.hpp"

#include <stdexcept>

namespace raytracer::math {

Vector3D::Vector3D(double xValue, double yValue, double zValue)
    : x(xValue), y(yValue), z(zValue)
{
}

double Vector3D::lengthSquared() const
{
    return x * x + y * y + z * z;
}

double Vector3D::length() const
{
    return std::sqrt(lengthSquared());
}

double Vector3D::dot(const Vector3D &other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Vector3D Vector3D::cross(const Vector3D &other) const
{
    return Vector3D(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x);
}

Vector3D Vector3D::normalized() const
{
    const double magnitude = length();

    if (magnitude == 0.0) {
        throw std::runtime_error("Cannot normalize a zero-length vector");
    }
    return *this / magnitude;
}

Vector3D Vector3D::operator+() const
{
    return *this;
}

Vector3D Vector3D::operator-() const
{
    return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::operator+(const Vector3D &other) const
{
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D &Vector3D::operator+=(const Vector3D &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3D Vector3D::operator-(const Vector3D &other) const
{
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D &Vector3D::operator-=(const Vector3D &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3D Vector3D::operator*(double value) const
{
    return Vector3D(x * value, y * value, z * value);
}

Vector3D &Vector3D::operator*=(double value)
{
    x *= value;
    y *= value;
    z *= value;
    return *this;
}

Vector3D Vector3D::operator/(double value) const
{
    return Vector3D(x / value, y / value, z / value);
}

Vector3D &Vector3D::operator/=(double value)
{
    x /= value;
    y /= value;
    z /= value;
    return *this;
}

Vector3D operator*(double value, const Vector3D &vector)
{
    return vector * value;
}

} // namespace raytracer::math
