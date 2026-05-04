/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** ServerManager.hpp
*/

#ifndef SERVERMANAGER_HPP
    #define SERVERMANAGER_HPP
    #include <cstdint>
    #include <algorithm>
    #include <cstring>
    #include <string>
    #include <vector>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <poll.h>
    #include <unistd.h>
    #include <iostream>

    #include "AManager.hpp"
    #include "Error.hpp"
    #include "Warning.hpp"

#include <sys/socket.h>

namespace raytracer {

class Client {
public:
    int fd;
    std::string cmd = "";
    bool doWrite = false;
    bool doRead = false;
    std::vector<std::string> MsgQueu;

    explicit Client(int fd): fd(fd) {};
    ~Client() {if (fd != -1) close(fd);}
};

class ServerPoll {
    int _fd;
public:
    ServerPoll(int port);
    ~ServerPoll() {if (_fd != -1) close(_fd);};

    void paul(std::vector<Client>& cliList);
};

class ServerManager : public AManager {
    std::string _configFile;

    std::vector<Client> clients;
    ServerPoll paul;

    friend class ServerPoll;
public:
    ServerManager(int port, std::string configFile): _configFile(configFile), paul(port) {};
    ~ServerManager() = default;

    void Update(const std::vector<IObject> objects, const std::vector<ILight> lights, std::vector<std::vector<Tile>>& map);
};

}

#endif
