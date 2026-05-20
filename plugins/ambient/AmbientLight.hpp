/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** AmbientLight.hpp
*/

#ifndef AMBIENTLIGHT_HPP
    #define AMBIENTLIGHT_HPP
    #include <array>
    #include <map>
    #include <string>
    #include "ILight.hpp"

namespace raytracer {

    class AmbientLight : public ILight {
        private:
            double _intensity;
            std::array<int, 3> _color;

        public:
            AmbientLight(double intensity, std::array<int, 3> color);
            ~AmbientLight() = default;

            LightType GetType() const override;
            std::array<double, 3> GetDirection() const override;
            double GetIntensity() const override;
            std::array<int, 3> GetColor() const override;
    };

}

#endif