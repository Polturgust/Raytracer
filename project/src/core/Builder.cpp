/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Builder.cpp
*/

#include "Builder.hpp"
#include "IManager.hpp"
#include "Default/DefaultManager.hpp"
#include "ServerManagr.hpp"
#include "ClientManager.hpp"

namespace raytracer {

CoreBuilder& CoreBuilder::SetMode(raytracer_type mode) {
    _mode = mode;
    return *this;
}

CoreBuilder& CoreBuilder::SetSceneFile(const std::string& sceneFile) {
    _sceneFile = sceneFile;
    return *this;
}

CoreBuilder& CoreBuilder::SetClientConnection(const std::string& ip, std::uint16_t port) {
    _ip = ip;
    _port = port;
    return *this;
}

CoreBuilder& CoreBuilder::SetPort(std::uint16_t port) {
    _port = port;
    return *this;
}

std::unique_ptr<IManager> CoreBuilder::BuildManager() const {
    if (_mode == SERVER)
        return std::make_unique<ServerManager>(static_cast<int>(_port), _sceneFile);
    if (_mode == CLIENT)
        return std::make_unique<ClientManager>(_ip, _port);
    return std::make_unique<DefaultManager>();
}

Core CoreBuilder::Build() const {
    std::size_t width = 1;
    std::size_t height = 1;

    if (!_sceneFile.empty()) {
        const auto resolution = Core::LoadResolution(_sceneFile);
        width = resolution.first;
        height = resolution.second;
    }

    return Core(width, height, BuildManager(), _sceneFile);
}

}