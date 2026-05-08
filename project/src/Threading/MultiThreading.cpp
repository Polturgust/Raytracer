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

int MultiThread::Compute(const std::vector<IObject> objects, const std::vector<ILight> lights, std::vector<Tile>& line, std::size_t y) {
    if (!isEnd())
        return 0;
    for (auto& t: thread)
        t.get();
    thread.clear();
    int numThreads = std::thread::hardware_concurrency();
    int chunkSize  = line.size() / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        std::size_t start = i * chunkSize;
        std::size_t end = (i == numThreads - 1) ? line.size() : start + chunkSize;

        thread.push_back(std::async(std::launch::async, [objects, lights, start, end, &line, y]() {
            for (std::size_t i = start; i < end; ++i) {
                Tile &tile = line[i];
                SampleCalcul(objects, lights, tile, i, y, line.size());
            }
        }));
    }
    return 1;
}

}