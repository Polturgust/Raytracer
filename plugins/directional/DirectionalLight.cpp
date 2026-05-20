/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** DirectionalLight.cpp
*/

#include "DirectionalLight.hpp"
#include "SoType.hpp"
#include <map>
#include <string>
#include <stdexcept>

namespace raytracer {

    DirectionalLight::DirectionalLight(std::array<double, 3> direction, double intensity, std::array<int, 3> color)
        : _direction(direction), _intensity(intensity), _color(color)
    {
    }

    LightType DirectionalLight::GetType() const 
    {
        return LightType::Directional;
    }

    std::array<double, 3> DirectionalLight::GetDirection() const 
    {
        return _direction;
    }

    double DirectionalLight::GetIntensity() const
    {
        return _intensity;
    }

    std::array<int, 3> DirectionalLight::GetColor() const
    {
        return _color;
    }

}

static double parseDouble(const std::map<std::string, std::string>& p,
                          const std::string& key, double def = 0.0)
{
    auto it = p.find(key);
    if (it == p.end())
        return def;
    try {
        return std::stod(it->second);
    }
    catch (...) {
        return def;
    }
}

static int parseInt(const std::map<std::string, std::string>& p,
                    const std::string& key, int def = 255)
{
    auto it = p.find(key);
    if (it == p.end())
        return def;
    try {
        return std::stoi(it->second);
    }
    catch (...) {
        return def;
    }
}

extern "C" {

    raytracer::SoTypeEnum getSOType()
    {
        return raytracer::LIGHT;
    }

    raytracer::ILight* getLight(std::map<std::string, std::string> params)
    {
        std::array<double, 3> dir = {
            parseDouble(params, "x",  0.0),
            parseDouble(params, "y", -1.0),
            parseDouble(params, "z",  0.0)
        };
        double intensity = parseDouble(params, "intensity", 1.0);
        std::array<int, 3> color = {
            parseInt(params, "color.r", 255),
            parseInt(params, "color.g", 255),
            parseInt(params, "color.b", 255)
        };
        return new raytracer::DirectionalLight(dir, intensity, color);
    }

}