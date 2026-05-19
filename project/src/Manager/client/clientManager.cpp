/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** clientManager.cpp
*/

#include "ClientManager.hpp"

namespace raytracer {

ClientManager::ClientManager(const std::string& ip, std::uint16_t port): _ip(ip), _port(port){}

void ClientManager::ConnectIfNeeded() {
    if (_connected)
        return;
    _socket.Connect(_ip, _port);
    _connected = true;
}

void ClientManager::QueueMessage(const std::string& message) {
    _outgoing.push(message);
}

bool ClientManager::HasMessage() const {
    return !_incoming.empty();
}

std::string ClientManager::PopMessage() {
    if (_incoming.empty())
        return "";
    std::string message = _incoming.front();
    _incoming.pop();
    return message;
}

void ClientManager::DoPoll() {
    ConnectIfNeeded();
    _poller.Clear();
    _poller.Watch(_socket.Fd(), static_cast<short>(POLLIN | (_outgoing.empty() ? 0 : POLLOUT)));
    _poller.Watch(STDIN_FILENO, POLLIN);
    if (_poller.Wait(-1) < 0)
        throw Error("Client: poll failed.");
    if (_poller.Revents(1) & POLLIN) {
        std::string input;
        if (std::getline(std::cin, input) && !input.empty())
            QueueMessage(input);
    }
    if ((_poller.Revents(0) & POLLOUT) && !_outgoing.empty()) {
        std::string message = _outgoing.front();
        while (!message.empty() && (message.back() == '\n' || message.back() == '\r'))
            message.pop_back();
        message += "\r\n";
        _socket.SendAll(message.data(), message.size());
        _outgoing.pop();
    }
    if (_poller.Revents(0) & POLLIN) {
        std::string message;
        try {
            message = _socket.Receive();
        } catch (...) {}
        if (message.empty())
            throw Warning("Client: connection closed by server.");
        _incoming.push(message);
        std::cout << Color::BLUE << "Reveive : " << Color::RESET << message;
        std::cout.flush();
    }
}

void ClientManager::Update(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& map)
{
    DoPoll();
    (void)map;
    (void)objects;
    (void)lights;
    (void)camera;
}

void ClientManager::InitCore(Core& core) {
    DoPoll();
    (void)core;
}

}
