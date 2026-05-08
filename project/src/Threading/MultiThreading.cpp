/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** MultiThreading.cpp
*/

#include "MultiThread.hpp"
//a retirer avec le sample
#include <cmath>
#define PI 3.14159265358979323846

namespace raytracer {

//juste un sample quoi
void SampleCalcul(const std::vector<IObject> objects, const std::vector<ILight> lights, Tile& Tile, const std::size_t x, const std::size_t y, const std::size_t width) {
    Tile.SetState(COMPUTING);
    (void)objects;
    (void)lights;
    //(void)camera;
    const double cycle = (width > 1) ? static_cast<double>(width) : 360.0;
    const double phase = 2.0 * PI * (static_cast<double>(x) + static_cast<double>(y)) / cycle;
    const int r = static_cast<int>((std::sin(phase) + 1.0) * 127.5);
    const int g = static_cast<int>((std::sin(phase + 2.0943951023931954923) + 1.0) * 127.5);
    const int b = static_cast<int>((std::sin(phase + 4.1887902047863909846) + 1.0) * 127.5);
    Tile.SetColor({r, g, b});
    Tile.SetState(COMPUTED);
}

bool MultiThread::isEnd(void) {
    for (auto& f : thread)
        if (f.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
            return 0;
    return 1;
}

int MultiThread::Compute(const std::vector<IObject> objects, const std::vector<ILight> lights, std::vector< std::vector<Tile>>& map, std::size_t start, std::size_t max) {
    if (!isEnd())
        return 0;
    for (auto& t: thread)
        t.get();
    thread.clear();

    std::cout << "Computing line " << start << " to " << max << "." << std::endl;
    for (std::size_t y = start; y < max; y++) {
        thread.push_back(std::async(std::launch::async, [objects, lights, &map, y]() {
            for (std::size_t i = 0; i < map[y].size(); ++i) {
                Tile &tile = map[y][i];
                SampleCalcul(objects, lights, tile, i, y, map[y].size());
            }
        }));
    }
    return 1;
}

}