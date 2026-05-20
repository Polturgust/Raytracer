/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** ObjReader.hpp
*/

#ifndef OBJREADER_HPP
    #define OBJREADER_HPP

    #include <fstream>
    #include <sstream>

    #include "AReader.hpp"

namespace raytracer {

class ObjReader : public AReader {
private:
    bool _hasVertices = false;
    bool _hasFaces = false;

public:
    explicit ObjReader(std::string nameFile): AReader(std::move(nameFile)) { SetUp(); }
    ~ObjReader() = default;

    void SetUp() override;

    std::pair<std::size_t, std::size_t> GetCameraResolution() override;
    std::array<int, 3> GetCameraPosition() override;
    std::array<int, 3> GetCameraRotation() override;
    double GetCameraFieldOfView() override;

    std::vector<std::unique_ptr<IObject>> GetObjects() override;
    std::vector<std::unique_ptr<ILight>> GetLights() override;
};

}

#endif
