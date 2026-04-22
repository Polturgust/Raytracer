#pragma once

#include "math/Point3D.hpp"
#include "math/Vector3D.hpp"

namespace raytracer::core {

class Ray {
public:
    math::Point3D origin;
    math::Vector3D direction;

    Ray() = default;
    Ray(const math::Point3D &originPoint, const math::Vector3D &directionVector);

    math::Point3D at(double t) const;
};

} // namespace raytracer::core
