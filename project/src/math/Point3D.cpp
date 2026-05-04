#include "math/Point3D.hpp"

namespace raytracer::math {

Point3D::Point3D(double xValue, double yValue, double zValue)
    : x(xValue), y(yValue), z(zValue)
{
}

Point3D Point3D::operator+(const Vector3D &vector) const
{
    return Point3D(x + vector.x, y + vector.y, z + vector.z);
}

Point3D &Point3D::operator+=(const Vector3D &vector)
{
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}

Point3D Point3D::operator-(const Vector3D &vector) const
{
    return Point3D(x - vector.x, y - vector.y, z - vector.z);
}

Point3D &Point3D::operator-=(const Vector3D &vector)
{
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    return *this;
}

Vector3D Point3D::operator-(const Point3D &other) const
{
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

} // namespace raytracer::math
