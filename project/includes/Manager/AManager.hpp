/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** AManager.hpp
*/

#ifndef AMANAGER_HPP
    #define AMANAGER_HPP
    #include "IManager.hpp"
    #include "MultiThread.hpp"
namespace raytracer {

class AManager : public IManager {
protected:
    State _state = WAIT;
    MultiThread _multiThread;
public:
    AManager() {};
    ~AManager() = default;

    void InitCore(Core& core) {(void)core;};
    void Update(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, std::vector<std::vector<Tile>>& map) = 0;
    State GetState(void) {return _state;};
};

}

#endif
