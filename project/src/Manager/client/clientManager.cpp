/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** clientManager.cpp
*/

#include "ClientManager.hpp"

namespace raytracer {

TcpSocket::~TcpSocket() {
    Close();
}

TcpSocket::TcpSocket(TcpSocket&& other) noexcept : _fd(other._fd) {
    other._fd = -1;
}

TcpSocket& TcpSocket::operator=(TcpSocket&& other) noexcept {
    if (this != &other) {
        Close();
        _fd = other._fd;
        other._fd = -1;
    }
    return *this;
}

void TcpSocket::Close() {
    if (_fd != -1) {
        ::close(_fd);
        _fd = -1;
    }
}

void TcpSocket::Connect(const std::string& ip, std::uint16_t port) {
    Close();
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        throw Error("Client: cannot create socket.\n");

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) != 1) {
        Close();
        throw Error("Client: invalid ip address.\n");
    }
    if (::connect(_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        Close();
        throw Error("Client: cannot connect to server.\n");
    }
}

std::size_t TcpSocket::Send(const std::string& message) const {
    if (_fd == -1)
        throw Error("Client: socket is not connected.\n");
    const ssize_t size = ::send(_fd, message.c_str(), message.size(), 0);
    if (size < 0)
        throw Error("Client: cannot send data.\n");
    return static_cast<std::size_t>(size);
}

std::string TcpSocket::Receive() const {
    if (_fd == -1)
        throw Error("Client: socket is not connected.\n");

    char buffer[4096];
    const ssize_t size = ::recv(_fd, buffer, sizeof(buffer), 0);
    if (size < 0)
        throw Error("Client: cannot receive data.\n");
    if (size == 0)
        throw Error("Client: connection closed by server.\n");
    return std::string(buffer, static_cast<std::size_t>(size));
}

void Poller::Clear() {
    _polls.clear();
}

void Poller::Watch(int fd, short events) {
    _polls.push_back(pollfd{fd, events, 0});
}

int Poller::Wait(int timeout) {
    if (_polls.empty())
        return 0;
    return ::poll(_polls.data(), static_cast<nfds_t>(_polls.size()), timeout);
}

short Poller::Revents(std::size_t index) const {
    return _polls.at(index).revents;
}

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

void ClientManager::Update(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& map)
{
    (void)map;
    (void)objects;
    (void)lights;
    (void)camera;
    ConnectIfNeeded();
    _poller.Clear();
    _poller.Watch(_socket.Fd(), static_cast<short>(POLLIN | (_outgoing.empty() ? 0 : POLLOUT)));
    _poller.Watch(STDIN_FILENO, POLLIN);
    if (_poller.Wait(-1) < 0)
        throw Error("Client: poll failed.\n");
    if (_poller.Revents(1) & POLLIN) {
        std::string input;
        if (std::getline(std::cin, input) && !input.empty())
            QueueMessage(input);
    }
    if ((_poller.Revents(0) & POLLOUT) && !_outgoing.empty()) {
        std::string message = _outgoing.front();
        if (!message.empty() && message.back() != '\n')
            message += '\n';
        _socket.Send(message);
        _outgoing.pop();
    }
    if (_poller.Revents(0) & POLLIN) {
        const std::string message = _socket.Receive();
        _incoming.push(message);
        std::cout << message;
        std::cout.flush();
    }
}

void ClientManager::InitCore(Core& core) {
    (void)core;
    return;
}

}
