/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** IObject.hpp
*/

#ifndef IOBJECT_HPP
    #define IOBJECT_HPP
    #include <array>

    #include "Ray.hpp"
    #include "Point3D.hpp"
    #include "Vector3D.hpp"

namespace raytracer {

class IObject {
public:
    virtual ~IObject() = default;

    virtual std::array<int, 3> GetColor() const = 0;
    virtual math::Point3D GetPosition() const = 0;
    virtual bool Intersect(const core::Ray& ray, double& distance) const = 0;
    virtual math::Vector3D GetNormal(const math::Point3D& point) const = 0;
};

}

#endif
