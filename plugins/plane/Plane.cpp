/*
** EPITECH PROJECT, 2025
** raytracer plane plugin
** File description:
** Plane.cpp
*/

#include "Plane.hpp"

#include <cmath>
#include <iostream>
#include <map>

#include "Error/Error.hpp"

#include "SoType.hpp"

namespace raytracer::plugins {

Plane::Plane(const math::Point3D& origin, const math::Vector3D& normal, const std::array<int, 3>& color)
    : AObject(nullptr), _origin(origin), _normal(normal.normalized()), _color(color)
{
}

std::array<int, 3> Plane::GetColor() const
{
    return _color;
}

math::Point3D Plane::GetPosition() const
{
    return _origin;
}

bool Plane::Intersect(const core::Ray& ray, double& distance) const
{
    const double denom = ray.direction.dot(_normal);

    if (std::abs(denom) < 0.0001)
        return false;

    const math::Vector3D oc = _origin - ray.origin;
    const double t = oc.dot(_normal) / denom;

    if (t > 0.001) {
        distance = t;
        return true;
    }
    return false;
}

math::Vector3D Plane::GetNormal(const math::Point3D& point) const
{
    (void)point;
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
    double origin_x = 0.0, origin_y = 0.0, origin_z = 0.0;
    double normal_x = 0.0, normal_y = 1.0, normal_z = 0.0;
    int r = 255, g = 255, b = 255;

    try {
        if (params.count("position.x") && !params.at("position.x").empty())
            origin_x = std::stod(params.at("position.x"));
        if (params.count("position.y") && !params.at("position.y").empty())
            origin_y = std::stod(params.at("position.y"));
        if (params.count("position.z") && !params.at("position.z").empty())
            origin_z = std::stod(params.at("position.z"));
        if (params.count("normal.x") && !params.at("normal.x").empty())
            normal_x = std::stod(params.at("normal.x"));
        if (params.count("normal.y") && !params.at("normal.y").empty())
            normal_y = std::stod(params.at("normal.y"));
        if (params.count("normal.z") && !params.at("normal.z").empty())
            normal_z = std::stod(params.at("normal.z"));
        if (params.count("color.r") && !params.at("color.r").empty())
            r = std::stoi(params.at("color.r"));
        if (params.count("color.g") && !params.at("color.g").empty())
            g = std::stoi(params.at("color.g"));
        if (params.count("color.b") && !params.at("color.b").empty())
            b = std::stoi(params.at("color.b"));

        return new Plane(math::Point3D(origin_x, origin_y, origin_z),
                        math::Vector3D(normal_x, normal_y, normal_z),
                        {r, g, b});
    } catch (const std::exception& e) {
        throw Error(std::string("Error creating plane: ") + e.what());
        return nullptr;
    }
}

}
