#pragma once

#include "Point3D.hpp"
#include "Vector3D.hpp"

namespace raytracer::core {

class Ray {
public:
    math::Point3D origin;
    math::Vector3D direction;

    Ray() = default;
    Ray(const math::Point3D &originPoint, const math::Vector3D &directionVector) : origin(originPoint), direction(directionVector) {};

    math::Point3D at(double t) const {return origin + (direction * t);};
};

} // namespace raytracer::core
