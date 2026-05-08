/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** DefaultManager.hpp
*/

#ifndef DEFAULTMANAGER_HPP
    #define DEFAULTMANAGER_HPP

    #include "AManager.hpp"
    #include "printer.hpp"

namespace raytracer {

class DefaultManager : public AManager {
    std::size_t CalculatedRow = 0;
    Printer _Printer;
public:
    DefaultManager() = default;
    ~DefaultManager() = default;

    void Update(const std::vector<IObject> objects, const std::vector<ILight> lights, std::vector<std::vector<Tile>>& map);
};

}

#endif