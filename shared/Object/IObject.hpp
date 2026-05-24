/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** IObject.hpp
*/

#ifndef IOBJECT_HPP
    #define IOBJECT_HPP
    #include <array>
    #include <map>
    #include <string>

    #include "Ray.hpp"
    #include "Point3D.hpp"
    #include "Vector3D.hpp"

namespace raytracer {

struct Material {
    double transparency = 0.0;
    double refraction = 1.0;
    double reflection = 0.0;
};

inline Material ParseMaterial(const std::map<std::string, std::string>& params)
{
    Material material;

    const auto apply = [&](const std::string& key, double& value) {
        const auto it = params.find(key);
        if (it != params.end() && !it->second.empty())
            value = std::stod(it->second);
    };

    apply("material.transparency", material.transparency);
    apply("material.refraction", material.refraction);
    apply("material.reflection", material.reflection);
    apply("transparency", material.transparency);
    apply("refraction", material.refraction);
    apply("reflection", material.reflection);
    return material;
}

class IObject {
public:
    virtual ~IObject() = default;

    virtual std::array<int, 3> GetColor() const = 0;
    virtual Material GetMaterial() const = 0;
    virtual math::Point3D GetPosition() const = 0;
    virtual bool Intersect(const core::Ray& ray, double& distance) const = 0;
    virtual math::Vector3D GetNormal(const math::Point3D& point) const = 0;
};

}

#endif
