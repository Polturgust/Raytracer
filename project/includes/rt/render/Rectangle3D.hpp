#pragma once

#include "rt/math/Point3D.hpp"
#include "rt/math/Vector3D.hpp"

namespace raytracer::render {

class Rectangle3D {
public:
    math::Point3D origin;
    math::Vector3D widthVector;
    math::Vector3D heightVector;

    Rectangle3D() = default;
    Rectangle3D(
        const math::Point3D &originPoint,
        const math::Vector3D &width,
        const math::Vector3D &height);

    math::Point3D pointAt(double u, double v) const;
};

} // namespace raytracer::render
