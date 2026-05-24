/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** ILight.hpp
*/

#ifndef ILIGHT_HPP
    #define ILIGHT_HPP
    #include <array>
 
namespace raytracer {
    enum class LightType {
        Directional,
        Ambient,
        Point,
    };
    
    class ILight {
        public:
            virtual ~ILight() = default;
        
            virtual LightType GetType() const = 0;
            virtual std::array<double, 3> GetDirection() const = 0;
            virtual double GetIntensity() const = 0;
            virtual std::array<int, 3> GetColor() const = 0;
        };
}
 
#endif