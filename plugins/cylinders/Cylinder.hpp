#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include <array>
#include <map>
#include <string>

#include "Error.hpp"
#include "SoType.hpp"
#include "AObject.hpp"
#include "Ray.hpp"
#include "Point3D.hpp"
#include "Vector3D.hpp"

namespace raytracer::plugins {

class Cylinder : public AObject {
private:
    math::Point3D _center;
    math::Vector3D _axis;
    double _radius;
    double _height;
    std::array<int,3> _color;

public:
    Cylinder(const math::Point3D& center, const math::Vector3D& axis, double radius, double height, const std::array<int,3>& color, const Material& material = {});
    ~Cylinder() override = default;

    std::array<int,3> GetColor() const override;
    math::Point3D GetPosition() const override;
    bool Intersect(const core::Ray& ray, double& distance) const override;
    math::Vector3D GetNormal(const math::Point3D& point) const override;
};

} // namespace

#endif
