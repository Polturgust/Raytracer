/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** defaultManager.cpp
*/

#include "DefaultManager.hpp"

namespace raytracer {

void DefaultManager::Update(const std::vector<IObject> objects, const std::vector<ILight> lights, std::vector<std::vector<Tile>>& map) {
    if (CalculatedRow == map.size()) {
        _state = FINISH;
        return;
    }
    _state = WORKING;
    if (_multiThread.Compute(objects, lights, map[CalculatedRow], CalculatedRow)) {
        CalculatedRow++;
    }
}

}
