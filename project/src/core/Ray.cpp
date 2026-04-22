#include "core/Ray.hpp"

namespace raytracer::core {

Ray::Ray(const math::Point3D &originPoint, const math::Vector3D &directionVector)
    : origin(originPoint), direction(directionVector)
{
}

math::Point3D Ray::at(double t) const
{
    return origin + (direction * t);
}

} // namespace raytracer::core
