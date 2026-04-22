/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Poll.cpp
*/

#include "ServerManagr.hpp"

namespace raytracer {

ServerPoll::ServerPoll(int port): _fd(-1) {
    const int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    const int opt = 1;
    sockaddr_in addr{};

    if (sockfd < 0)
        throw Error("Cannot create socket.\n");
    if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        ::close(sockfd);
        throw Error("Cannot configure socket reuse option.\n");
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<std::uint16_t>(port));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (::bind(sockfd, static_cast<sockaddr *>(static_cast<void *>(&addr)), sizeof(addr)) < 0) {
        ::close(sockfd);
        throw Error("Cannot access port: " + std::to_string(port) + ".\n");
    }
    if (::listen(sockfd, 10) < 0) {
        ::close(sockfd);
        throw Error(std::to_string(port) + " is not a free port.\n");
    }
    _fd = sockfd;
}

void ServerPoll::paul(std::vector<Client>& cliList) {
    std::vector<pollfd> pollList;
    pollList.reserve(cliList.size() + 1);
    pollList.push_back(pollfd{_fd, POLLIN, 0});

    for (const auto& client : cliList)
        pollList.push_back(pollfd{client.fd, static_cast<short>(client.doWrite ? POLLIN | POLLOUT : POLLIN), 0});

    if (::poll(pollList.data(), static_cast<nfds_t>(pollList.size()), -1) < 0)
        throw Error("Poll failed.\n");
    if (pollList[0].revents & POLLIN) {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        const int clientFd = ::accept(_fd, static_cast<sockaddr *>(static_cast<void *>(&clientAddr)), &clientLen);
        if (clientFd >= 0)
            cliList.emplace_back(clientFd);
        else
            throw Warning("Cannot accept a new client.\n");
    }

    std::vector<std::size_t> toRemove;
    for (std::size_t idx = 1; idx < pollList.size(); ++idx) {
        Client& client = cliList[idx - 1];

        client.doRead = false;
        if (pollList[idx].revents & (POLLERR | POLLHUP | POLLNVAL)) {
            toRemove.push_back(idx - 1);
            continue;
        }
        if (pollList[idx].revents & POLLIN) {
            char buffer[4096];
            const ssize_t size = ::recv(client.fd, buffer, sizeof(buffer), 0);
            if (size <= 0) {
                toRemove.push_back(idx - 1);
                continue;
            }
            client.cmd.append(buffer, static_cast<std::size_t>(size));
            client.doRead = true;
        }
    }
    for (auto it = toRemove.rbegin(); it != toRemove.rend(); ++it)
        cliList.erase(cliList.begin() + static_cast<std::vector<Client>::difference_type>(*it));
}

}

