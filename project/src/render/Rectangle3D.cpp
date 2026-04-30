#include "render/Rectangle3D.hpp"

namespace raytracer::render {

Rectangle3D::Rectangle3D(
    const math::Point3D &originPoint,
    const math::Vector3D &width,
    const math::Vector3D &height)
    : origin(originPoint), widthVector(width), heightVector(height)
{
}

math::Point3D Rectangle3D::pointAt(double u, double v) const
{
    return origin + (widthVector * u) + (heightVector * v);
}

} // namespace raytracer::render
