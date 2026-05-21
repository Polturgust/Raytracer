/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** ATexture.hpp
*/

#ifndef ATEXTURE_HPP
    #define ATEXTURE_HPP
    #include <array>

    #include "ITexture.hpp"

namespace raytracer {

class ATexture : public ITexture {
    std::array<int, 4> color; //rgba
    double reflextion; //1.00 100% 0.00 0%
public:
    ATexture(std::array<int, 4> color, double reflextion): color(color), reflextion(reflextion) {};
    ~ATexture() {};

    //geters for maths
};

}

#endif
