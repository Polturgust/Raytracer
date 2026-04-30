/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** IManager.hpp
*/

#ifndef IMANAGER_HPP
    #define IMANAGER_HPP
    #include <vector>

    #include "Tile.hpp"

namespace raytracer {

class IManager {
public:
    ~IManager() = default;

    virtual void Update(std::vector<std::vector<Tile>>& map) = 0;
};

}

#endif
