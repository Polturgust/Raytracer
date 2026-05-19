/*
** EPITECH PROJECT, 2025
** raytracer triangle plugin
** File description:
** Triangle.hpp
*/

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <array>
#include <map>
#include <string>

#include "AObject.hpp"
#include "Ray.hpp"
#include "Point3D.hpp"
#include "Vector3D.hpp"

namespace raytracer::plugins {

class Triangle : public AObject {
private:
    math::Point3D _v0;  // First vertex
    math::Point3D _v1;  // Second vertex
    math::Point3D _v2;  // Third vertex
    math::Vector3D _normal;  // Face normal (for flat shading)
    std::array<int, 3> _color;

public:
    Triangle(const math::Point3D& v0, const math::Point3D& v1, const math::Point3D& v2, 
             const std::array<int, 3>& color);
    ~Triangle() override = default;

    std::array<int, 3> GetColor() const override;
    math::Point3D GetPosition() const override;  // Returns centroid
    bool Intersect(const core::Ray& ray, double& distance) const override;
    math::Vector3D GetNormal(const math::Point3D& point) const override;
};

} // namespace raytracer::plugins

#endif
