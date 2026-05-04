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
    #include "IObject.hpp"
    #include "ILight.hpp"

namespace raytracer {

typedef enum {
    WORKING,
    FINISH,
    WAIT,
} State;

class IManager {
public:
    ~IManager() = default;

    virtual void Update(const std::vector<IObject> objects, const std::vector<ILight> lights, std::vector<std::vector<Tile>>& map) = 0;
    virtual State GetState(void) = 0;
};

}

#endif
