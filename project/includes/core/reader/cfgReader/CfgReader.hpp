/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** CfgReader.hpp
*/

#ifndef CFGREADER_HPP
    #define CFGREADER_HPP
    #include <libconfig.h++>

    #include "AReader.hpp"

namespace raytracer {

class CfgReader : public AReader {
    libconfig::Config _cfg;

    const libconfig::Setting& CameraExist();
public:
    CfgReader(std::string NameFile): AReader(NameFile) {SetUp();};
    ~CfgReader() = default;

    void SetUp();
    std::pair<std::size_t, std::size_t> GetCameraResolution();
    std::array<int, 3> GetCameraPosition();
    std::array<int, 3> GetCameraRotation();
    double GetCameraFieldOfView();
};

}

#endif