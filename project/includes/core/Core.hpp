/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Core.hpp
*/

#ifndef CORE_HPP
    #define CORE_HPP
    #include <cstddef>
    #include <memory>
    #include <string>
    #include <utility>
    #include <vector>
    #include <libconfig.h++>

    #include "IManager.hpp"
    #include "Error.hpp"
    #include "Tile.hpp"
    #include "IReader.hpp"

namespace raytracer {

class IManager;

class Core {
    std::size_t _x;
    std::size_t _y;
    std::unique_ptr<IManager> _manager;
    std::unique_ptr<IReader> _reader;
    std::string _sceneFile;

public:
    std::vector<std::vector<Tile>> map;

    Core(std::unique_ptr<IReader> reader, std::unique_ptr<IManager> manager, const std::string& sceneFile);
    ~Core() = default;

    static std::pair<std::size_t, std::size_t> LoadResolution();
    void Run();
};

}

#endif
