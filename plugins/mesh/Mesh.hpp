/*
** EPITECH PROJECT, 2025
** raytracer mesh plugin
** File description:
** Mesh.hpp
*/

#ifndef MESH_HPP
    #define MESH_HPP

    #include <vector>
    #include <map>
    #include <string>
    #include <memory>

    #include "IObject.hpp"
    #include "Point3D.hpp"
    #include "Vector3D.hpp"
    #include "Ray.hpp"

namespace raytracer::plugins {

class Triangle {
public:
    math::Point3D v0, v1, v2;
    
    Triangle(const math::Point3D& a, const math::Point3D& b, const math::Point3D& c)
        : v0(a), v1(b), v2(c) {}
    
    bool intersect(const core::Ray& ray, double& distance, math::Vector3D& normal) const;
    math::Vector3D getNormal() const;
};

class Mesh : public raytracer::IObject {
private:
    std::vector<Triangle> _triangles;
    std::array<int, 3> _color;
    math::Point3D _position;
    math::Vector3D _rotation; // Euler rotations in degrees (x, y, z)

public:
    Mesh(const std::array<int, 3>& color, const math::Point3D& position = {0, 0, 0}, const math::Vector3D& rotation = {0, 0, 0});
    ~Mesh() override = default;

    void addTriangle(const Triangle& triangle);
    
    std::array<int, 3> GetColor() const override;
    math::Point3D GetPosition() const override;
    math::Vector3D GetRotation() const;
    bool Intersect(const core::Ray& ray, double& distance) const override;
    math::Vector3D GetNormal(const math::Point3D& point) const override;
};

}

#endif
