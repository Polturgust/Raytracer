#pragma once

#include "core/Ray.hpp"
#include "math/Point3D.hpp"
#include "render/Rectangle3D.hpp"

namespace raytracer::render {

class Camera {
public:
    math::Point3D origin;
    Rectangle3D viewport;

    Camera();
    Camera(const math::Point3D &originPoint, const Rectangle3D &viewportRectangle);

    core::Ray ray(double u, double v) const;
};

} // namespace raytracer::render
