/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** PointLight.hpp
*/

#ifndef POINTLIGHT_HPP
    #define POINTLIGHT_HPP
    #include <array>
    #include <map>
    #include <string>
    #include "ILight.hpp"

namespace raytracer {

class PointLight : public ILight {
    std::array<double, 3> _position;
    double _intensity;
    std::array<int, 3> _color;

public:
    PointLight(std::array<double, 3> position, double intensity, std::array<int, 3> color);
    ~PointLight() = default;

    LightType GetType() const override;
    std::array<double, 3> GetDirection() const override;
    double GetIntensity() const override;
    std::array<int, 3> GetColor() const override;
};

}

#endif