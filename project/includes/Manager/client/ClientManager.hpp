/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** ClientManager.hpp
*/

#ifndef CLIENTMANAGER_HPP
    #define CLIENTMANAGER_HPP
    #include <cstdint>
    #include <queue>
    #include <string>
    #include <iostream>

    #include "AManager.hpp"
    #include "Net.hpp"
    #include "Error.hpp"
    #include "printer.hpp"

namespace raytracer {

class ClientManager : public AManager {
    TcpSocket _socket;
    Poller _poller;
    std::queue<std::string> _outgoing;
    std::queue<std::string> _incoming;
    std::string _ip;
    std::uint16_t _port;
    bool _connected = false;

    void ConnectIfNeeded();
    void DoPoll();

public:
    ClientManager(const std::string& ip, std::uint16_t port);
    ~ClientManager() = default;

    void QueueMessage(const std::string& message);
    bool HasMessage() const;
    std::string PopMessage();

    void Update(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& map);
    void InitCore(Core& core);
};

}

#endif
