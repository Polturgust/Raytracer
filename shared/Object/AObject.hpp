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
    Material _material;
public:
    AObject(std::unique_ptr<ITexture> Texture, const Material& material = {}) : Texture(std::move(Texture)), _material(material) {};
    ~AObject() override = default;

    Material GetMaterial() const override { return _material; }
};

}

#endif
