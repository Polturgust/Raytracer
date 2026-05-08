/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Core.cpp
*/

#include "Core.hpp"

namespace raytracer {

Core::Core(std::unique_ptr<IReader> reader, std::unique_ptr<IManager> manager, const std::string& sceneFile)
    : _x(0), _y(0), _manager(std::move(manager)), _reader(std::move(reader)), _sceneFile(sceneFile) {
    if (_reader) {
        std::pair<size_t, size_t> size = _reader->GetCameraResolution();
        _x = size.first;
        _y = size.second;
    }
    map.resize(_y);
    for (auto& row : map)
        row.assign(_x, Tile({0, 0, 0}));
    sfml.emplace(_x, _y);
}

void Core::Run() {
    while (_manager->GetState() != FINISH && sfml->isOpen()) {
        if (!sfml->pollEvents()) {
            break;
        }
        _manager->Update(Objects, Lights, map);
        sfml->render(map);
    }
    ppmconvertor.Draw(_sceneFile.substr(0, _sceneFile.rfind('.')) + ".ppm", map);
}

}
