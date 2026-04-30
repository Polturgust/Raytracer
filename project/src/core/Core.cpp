/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Core.cpp
*/

#include "Core.hpp"

namespace raytracer {

Core::Core(std::size_t x, std::size_t y, std::unique_ptr<IManager> manager, const std::string& sceneFile)
    : _x(x), _y(y), _manager(std::move(manager)), _sceneFile(sceneFile) {
    map.resize(_y);
    for (auto& row : map)
        row.assign(_x, Tile({0, 0, 0, 255}));
}

Core::~Core() = default;

std::pair<std::size_t, std::size_t> Core::LoadResolution(const std::string& sceneFile)
{
    libconfig::Config cfg;

    try {
        cfg.readFile(sceneFile.c_str());
    } catch (const libconfig::FileIOException&) {
        throw Error("Core: cannot read scene file '\"" + sceneFile + "\"'.\n");
    } catch (const libconfig::ParseException& ex) {
        throw Error("Core: parse error in scene file '\"" + sceneFile + "\"' at line "
            + std::to_string(ex.getLine()) + ": " + ex.getError() + "\n");
    }

    const libconfig::Setting& root = cfg.getRoot();
    if (!root.exists("camera"))
        throw Error("Core: missing 'camera' section in scene file.\n");
    const libconfig::Setting& camera = root["camera"];
    if (!camera.exists("resolution"))
        throw Error("Core: missing 'camera.resolution' section in scene file.\n");
    const libconfig::Setting& resolution = camera["resolution"];

    int width = 0;
    int height = 0;
    if (!resolution.lookupValue("width", width) || !resolution.lookupValue("height", height))
        throw Error("Core: resolution width/height are missing in scene file.\n");
    if (width <= 0 || height <= 0)
        throw Error("Core: resolution width/height must be positive values.\n");

    return {static_cast<std::size_t>(width), static_cast<std::size_t>(height)};
}

void Core::Run()
{
    while (true) {
        _manager->Update(map);
    }
}

}