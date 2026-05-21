/*
** EPITECH PROJECT, 2025
** raytracer sphere plugin
** File description:
** Sphere.hpp
*/

#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <array>
#include <map>
#include <string>
#include <cmath>
#include <iostream>


#include "Error.hpp"
#include "SoType.hpp"
#include "AObject.hpp"
#include "Ray.hpp"
#include "Point3D.hpp"
#include "Vector3D.hpp"

namespace raytracer::plugins {

class Sphere : public AObject {
private:
    math::Point3D _center;
    double _radius;
    std::array<int, 3> _color;

public:
    Sphere(const math::Point3D& center, double radius, const std::array<int, 3>& color, const Material& material = {});
    ~Sphere() override = default;

    std::array<int, 3> GetColor() const override;
    math::Point3D GetPosition() const override;
    bool Intersect(const core::Ray& ray, double& distance) const override;
    math::Vector3D GetNormal(const math::Point3D& point) const override;
};

} // namespace raytracer::plugins

#endif
