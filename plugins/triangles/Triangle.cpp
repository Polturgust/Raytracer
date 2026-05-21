/*
** EPITECH PROJECT, 2025
** raytracer triangle plugin
** File description:
** Triangle.cpp
*/

#include "Triangle.hpp"

#include <cmath>
#include <iostream>
#include <map>

#include "Error.hpp"

#include "SoType.hpp"

namespace raytracer::plugins {

Triangle::Triangle(const math::Point3D& v0, const math::Point3D& v1, const math::Point3D& v2, 
                   const std::array<int, 3>& color, const Material& material)
    : AObject(nullptr, material), _v0(v0), _v1(v1), _v2(v2), _color(color)
{
    // Compute face normal using cross product: (v1 - v0) × (v2 - v0)
    const math::Vector3D edge1 = v1 - v0;
    const math::Vector3D edge2 = v2 - v0;
    _normal = edge1.cross(edge2).normalized();
}

std::array<int, 3> Triangle::GetColor() const
{
    return _color;
}

math::Point3D Triangle::GetPosition() const
{
    // Return centroid of triangle
    return math::Point3D(
        (_v0.x + _v1.x + _v2.x) / 3.0,
        (_v0.y + _v1.y + _v2.y) / 3.0,
        (_v0.z + _v1.z + _v2.z) / 3.0
    );
}

bool Triangle::Intersect(const core::Ray& ray, double& distance) const
{
    // Möller–Trumbore ray-triangle intersection algorithm
    const double EPSILON = 0.0001;

    math::Vector3D edge1 = _v1 - _v0;
    math::Vector3D edge2 = _v2 - _v0;
    math::Vector3D h = ray.direction.cross(edge2);
    double a = edge1.dot(h);

    // Ray is parallel to triangle plane
    if (std::abs(a) < EPSILON)
        return false;

    double f = 1.0 / a;
    math::Vector3D s = ray.origin - _v0;
    double u = f * s.dot(h);

    if (u < 0.0 || u > 1.0)
        return false;

    math::Vector3D q = s.cross(edge1);
    double v = f * ray.direction.dot(q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    double t = f * edge2.dot(q);

    if (t > EPSILON) {  // Ray intersection is in front of ray origin
        distance = t;
        return true;
    }
    return false;
}

math::Vector3D Triangle::GetNormal(const math::Point3D& point) const
{
    (void)point;  // Return flat face normal (can be extended to smooth shading with vertex normals)
    return _normal;
}

} // namespace raytracer::plugins

extern "C" {

using namespace raytracer::plugins;
using namespace raytracer;

SoTypeEnum getSOType()
{
    return OBJECT;
}

IObject* getObject(std::map<std::string, std::string> params)
{
    double v0_x = 0.0, v0_y = 0.0, v0_z = 0.0;
    double v1_x = 1.0, v1_y = 0.0, v1_z = 0.0;
    double v2_x = 0.0, v2_y = 1.0, v2_z = 0.0;
    int r = 255, g = 255, b = 255;

    try {
        // Parse first vertex
        if (params.count("v0.x") && !params.at("v0.x").empty())
            v0_x = std::stod(params.at("v0.x"));
        if (params.count("v0.y") && !params.at("v0.y").empty())
            v0_y = std::stod(params.at("v0.y"));
        if (params.count("v0.z") && !params.at("v0.z").empty())
            v0_z = std::stod(params.at("v0.z"));

        // Parse second vertex
        if (params.count("v1.x") && !params.at("v1.x").empty())
            v1_x = std::stod(params.at("v1.x"));
        if (params.count("v1.y") && !params.at("v1.y").empty())
            v1_y = std::stod(params.at("v1.y"));
        if (params.count("v1.z") && !params.at("v1.z").empty())
            v1_z = std::stod(params.at("v1.z"));

        // Parse third vertex
        if (params.count("v2.x") && !params.at("v2.x").empty())
            v2_x = std::stod(params.at("v2.x"));
        if (params.count("v2.y") && !params.at("v2.y").empty())
            v2_y = std::stod(params.at("v2.y"));
        if (params.count("v2.z") && !params.at("v2.z").empty())
            v2_z = std::stod(params.at("v2.z"));

        // Parse color
        if (params.count("color.r") && !params.at("color.r").empty())
            r = std::stoi(params.at("color.r"));
        if (params.count("color.g") && !params.at("color.g").empty())
            g = std::stoi(params.at("color.g"));
        if (params.count("color.b") && !params.at("color.b").empty())
            b = std::stoi(params.at("color.b"));
        const Material material = ParseMaterial(params);

        return new Triangle(math::Point3D(v0_x, v0_y, v0_z),
                           math::Point3D(v1_x, v1_y, v1_z),
                           math::Point3D(v2_x, v2_y, v2_z),
                           {r, g, b}, material);
    } catch (const std::exception& e) {
        throw Error(std::string("Error creating triangle: ") + e.what());
        return nullptr;
    }
}

}
