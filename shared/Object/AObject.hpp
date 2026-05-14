/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** AObject.hpp
*/

#ifndef AOBJECT_HPP
    #define AOBJECT_HPP
    #include <memory>

    #include "IObject.hpp"
    #include "ITexture.hpp"

namespace raytracer {

class AObject : public IObject {
    std::unique_ptr<ITexture> Texture;
public:
    AObject(std::unique_ptr<ITexture> Texture): Texture(std::move(Texture)) {};
    ~AObject() override = default;
};

}

#endif
