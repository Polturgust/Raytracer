/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** AmbientLight.cpp
*/

#include "AmbientLight.hpp"
#include "SoType.hpp"
#include <map>
#include <string>

namespace raytracer {

    AmbientLight::AmbientLight(double intensity, std::array<int, 3> color)
        : _intensity(intensity), _color(color)
    {
    }

    LightType AmbientLight::GetType() const {
        return LightType::Ambient;
    }

    std::array<double, 3> AmbientLight::GetDirection() const {
        return {0.0, 0.0, 0.0};
    }

    double AmbientLight::GetIntensity() const {
        return _intensity;
    }

    std::array<int, 3> AmbientLight::GetColor() const {
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
        double intensity = parseDouble(params, "intensity", 1.0);
        std::array<int, 3> color = {
            parseInt(params, "color.r", 255),
            parseInt(params, "color.g", 255),
            parseInt(params, "color.b", 255)
        };
        return new raytracer::AmbientLight(intensity, color);
    }

}