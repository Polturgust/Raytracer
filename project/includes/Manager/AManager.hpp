/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** AManager.hpp
*/

#ifndef AMANAGER_HPP
    #define AMANAGER_HPP
    #include "IManager.hpp"

namespace raytracer {

class AManager : public IManager {
    std::size_t LastX = 0;
    std::size_t LastY = 0;
public:
    AManager() {};
    ~AManager() = default;

    void Update(std::vector<std::vector<Tile>>& map) {(void)map; return;};
};

}

#endif
