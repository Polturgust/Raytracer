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
    #include "IManager.hpp"
    #include "Error.hpp"

    #include <libconfig.h++>
    #include "Tile.hpp"

namespace raytracer {

class IManager;

class Core {
    std::size_t _x;
    std::size_t _y;
    std::unique_ptr<IManager> _manager;
    std::string _sceneFile;

public:
    std::vector<std::vector<Tile>> map;

    Core(std::size_t x, std::size_t y, std::unique_ptr<IManager> manager, const std::string& sceneFile);
    ~Core();

    Core(const Core&) = delete;
    Core& operator=(const Core&) = delete;
    Core(Core&&) noexcept = default;
    Core& operator=(Core&&) noexcept = default;

    static std::pair<std::size_t, std::size_t> LoadResolution(const std::string& sceneFile);
    void Run();
};

}

#endif
