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
    #include <vector>
    #include <poll.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <cstring>
    #include <iostream>

    #include "AManager.hpp"
    #include "Error.hpp"

namespace raytracer {

class TcpSocket {
    int _fd = -1;

public:
    TcpSocket() = default;
    ~TcpSocket();

    TcpSocket(const TcpSocket&) = delete;
    TcpSocket& operator=(const TcpSocket&) = delete;
    TcpSocket(TcpSocket&& other) noexcept;
    TcpSocket& operator=(TcpSocket&& other) noexcept;

    void Connect(const std::string& ip, std::uint16_t port);
    std::size_t Send(const std::string& message) const;
    std::string Receive() const;
    int Fd() const {return _fd;};
    bool IsOpen() const {return _fd != -1;};
    void Close();
};

class Poller {
    std::vector<pollfd> _polls;

public:
    Poller() = default;

    void Clear();
    void Watch(int fd, short events);
    int Wait(int timeout);
    short Revents(std::size_t index) const;
    std::size_t Size() const {return _polls.size();};
};

class ClientManager : public AManager {
    TcpSocket _socket;
    Poller _poller;
    std::queue<std::string> _outgoing;
    std::queue<std::string> _incoming;
    std::string _ip;
    std::uint16_t _port;
    bool _connected = false;

    void ConnectIfNeeded();

public:
    ClientManager(const std::string& ip, std::uint16_t port);
    ~ClientManager() = default;

    void QueueMessage(const std::string& message);
    bool HasMessage() const;
    std::string PopMessage();
    void Update(std::vector<std::vector<Tile>>& map) override;
};

}

#endif