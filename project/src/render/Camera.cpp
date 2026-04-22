#include "render/Camera.hpp"

namespace raytracer::render {

Camera::Camera()
    : origin(0.0, 0.0, 0.0),
      viewport(
          math::Point3D(-1.0, -1.0, -1.0),
          math::Vector3D(2.0, 0.0, 0.0),
          math::Vector3D(0.0, 2.0, 0.0))
{
}

Camera::Camera(const math::Point3D &originPoint, const Rectangle3D &viewportRectangle)
    : origin(originPoint), viewport(viewportRectangle)
{
}

core::Ray Camera::ray(double u, double v) const
{
    const math::Point3D pixelPoint = viewport.pointAt(u, v);
    return core::Ray(origin, pixelPoint - origin);
}

} // namespace raytracer::render
