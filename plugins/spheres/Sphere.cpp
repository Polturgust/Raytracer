/*
** EPITECH PROJECT, 2025
** raytracer sphere plugin
** File description:
** Sphere.cpp
*/

#include "Sphere.hpp"

namespace raytracer::plugins {

Sphere::Sphere(const math::Point3D& center, double radius, const std::array<int, 3>& color)
    : AObject(nullptr), _center(center), _radius(radius), _color(color)
{
    if (radius < 0.0)
        throw Error("Sphere radius cannot be negative");
}

std::array<int, 3> Sphere::GetColor() const
{
    return _color;
}

math::Point3D Sphere::GetPosition() const
{
    return _center;
}

bool Sphere::Intersect(const core::Ray& ray, double& distance) const
{
    const math::Vector3D oc = ray.origin - _center;
    const double a = ray.direction.dot(ray.direction);
    const double b = 2.0 * oc.dot(ray.direction);
    const double c = oc.dot(oc) - _radius * _radius;
    const double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0.0)
        return false;

    const double sqrt_disc = std::sqrt(discriminant);
    const double t1 = (-b - sqrt_disc) / (2.0 * a);
    const double t2 = (-b + sqrt_disc) / (2.0 * a);

    if (t1 > 0.001) {
        distance = t1;
        return true;
    }
    if (t2 > 0.001) {
        distance = t2;
        return true;
    }
    return false;
}

math::Vector3D Sphere::GetNormal(const math::Point3D& point) const
{
    return (point - _center).normalized();
}

}

extern "C" {

    using namespace raytracer::plugins;
    using namespace raytracer;

    raytracer::SoTypeEnum getSOType()
    {
        return raytracer::SoTypeEnum::OBJECT;
    }

    raytracer::IObject* getObject(std::map<std::string, std::string> params)
    {
        double x = 0.0, y = 0.0, z = 0.0;
        double radius = 1.0;
        int r = 255, g = 255, b = 255;

        try {
            if (params.count("position.x") && !params.at("position.x").empty())
                x = std::stod(params.at("position.x"));
            if (params.count("position.y") && !params.at("position.y").empty())
                y = std::stod(params.at("position.y"));
            if (params.count("position.z") && !params.at("position.z").empty())
                z = std::stod(params.at("position.z"));
            if (params.count("radius") && !params.at("radius").empty())
                radius = std::stod(params.at("radius"));
            if (params.count("color.r") && !params.at("color.r").empty())
                r = std::stoi(params.at("color.r"));
            if (params.count("color.g") && !params.at("color.g").empty())
                g = std::stoi(params.at("color.g"));
            if (params.count("color.b") && !params.at("color.b").empty())
                b = std::stoi(params.at("color.b"));

            return new raytracer::plugins::Sphere(math::Point3D(x, y, z), radius, {r, g, b});
        } catch (const std::exception& e) {
            throw Error(std::string("Error creating sphere: ") + e.what());
            return nullptr;
        }
    }

}
