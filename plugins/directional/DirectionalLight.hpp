/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** DirectionalLight.hpp
*/

#ifndef DIRECTIONALLIGHT_HPP
    #define DIRECTIONALLIGHT_HPP
    #include <array>
    #include <map>
    #include <string>
    #include "ILight.hpp"

namespace raytracer {

    class DirectionalLight : public ILight {
        private:
            std::array<double, 3> _direction;
            double _intensity;
            std::array<int, 3> _color;

        public:
            DirectionalLight(std::array<double, 3> direction, double intensity, std::array<int, 3> color);
            ~DirectionalLight() = default;

            LightType GetType() const override;
            std::array<double, 3> GetDirection() const override;
            double GetIntensity() const override;
            std::array<int, 3> GetColor() const override;
    };

}

#endif