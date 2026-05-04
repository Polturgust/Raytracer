/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** MultiThread.hpp
*/

#ifndef MULTITHREAD_HPP
    #define MULTITHREAD_HPP
    #include <vector>
    #include <future>
    #include <chrono>

    #include "Tile.hpp"
    #include "IObject.hpp"
    #include "ILight.hpp"

namespace raytracer {

class MultiThread {
    std::vector<std::future<void>> thread = {};
public:
    MultiThread() {};
    ~MultiThread() = default;

    bool isEnd(void);
    int Compute(const std::vector<IObject> objects, const std::vector<ILight> lights, std::vector<Tile>& line, std::size_t y);
};

}

#endif