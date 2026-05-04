/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** ServerManager.cpp
*/

#include "ServerManager.hpp"

namespace raytracer {

void ServerManager::Update(const std::vector<IObject> objects, const std::vector<ILight> lights, std::vector<std::vector<Tile>>& map) {
    (void)map;
    (void)objects;
    (void)lights;
    try {
        paul.paul(clients);
    } catch (const IError& e) {
        if (e.code() == 84)
            throw Error(std::string("ServerManager: Poll error : ") + e.what());
        else
            std::cerr << "ServerManager: Poll warning : " << e.what();
    }
    for (auto it = clients.begin(); it != clients.end();) {
        Client& cl = *it;
        if (cl.doWrite && !cl.MsgQueu.empty()) {
            std::string message;
            for (const auto& entry : cl.MsgQueu) {
                message += entry;
                if (message.empty() || message.back() != '\n')
                    message += '\n';
            }
            if (!message.empty() && ::send(cl.fd, message.c_str(), message.size(), 0) < 0)
                throw Warning("ServerManager: cannot write to client.\n");
            cl.MsgQueu.clear();
            cl.doWrite = false;
        }
        if (cl.doRead) {
            if (cl.cmd.empty()) {
                it = clients.erase(it);
                continue;
            }
            cl.cmd.clear();
            cl.doRead = false;
        }
        ++it;
    }
    return;
}

}
