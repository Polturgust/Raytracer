/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** ServerManager.cpp
*/

#include "ServerManager.hpp"

namespace raytracer {

static bool IsValidPluginPath(const std::string& p) {
    const std::string prefix = "plugins/raytracer_";
    const std::string suffix = ".so";

    if (p.find("..") != std::string::npos)
        return false;
    if (p.size() <= prefix.size() + suffix.size())
        return false;
    if (p.compare(0, prefix.size(), prefix) != 0)
        return false;
    if (p.compare(p.size() - suffix.size(), suffix.size(), suffix) != 0)
        return false;
    const std::string middle = p.substr(prefix.size(), p.size() - prefix.size() - suffix.size());
    if (middle.empty() || middle.find('/') != std::string::npos)
        return false;
    return true;
}

void ServerManager::Reply(Client& cl, const std::string& message) {
    _logger.LogSend(message, cl.Uid);
    cl.MsgQueu.push_back(message);
    cl.doWrite = true;
}

void ServerManager::HandleFGet(Client& cl, std::istringstream& iss) {
    std::string path, ip;
    std::uint16_t port = 0;
    if (!(iss >> path >> ip >> port)) {
        Reply(cl, "400 BAD REQUEST");
        return;
    }

    std::string resolved;
    std::string okFilename;
    if (path == "ConfigueFile") {
        resolved = _configFile;
        const auto slash = _configFile.find_last_of('/');
        okFilename = (slash == std::string::npos) ? _configFile : _configFile.substr(slash + 1);
    } else if (IsValidPluginPath(path)) {
        resolved = path;
    } else {
        Reply(cl, "404 NOT FOUND");
        return;
    }

    {
        std::ifstream test(resolved, std::ios::binary);
        if (!test.is_open()) {
            Reply(cl, "404 NOT FOUND");
            return;
        }
    }

    TcpSocket data;
    try {
        data.Connect(ip, port);
    } catch (const IError&) {
        Reply(cl, "503 NO CONNECT");
        return;
    }
    try {
        FileSender::Stream(data, resolved);
    } catch (const IError&) {
        Reply(cl, "500 ERROR");
        return;
    }
    data.Close();
    if (okFilename.empty())
        Reply(cl, "200 OK");
    else
        Reply(cl, "200 OK " + okFilename);
}

void ServerManager::HandleCommand(Client& cl, const std::string& line) {
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;
    _logger.LogReceive(line, cl.Uid);
    if (cmd == "FGET") {
        HandleFGet(cl, iss);
        return;
    }
    Reply(cl, "400 BAD REQUEST");
}

void ServerManager::DoPoll() {
    _poller.Clear();
    _poller.Watch(_listen.Fd(), POLLIN);
    for (const auto& c : _clients)
        _poller.Watch(c.socket.Fd(), static_cast<short>(c.doWrite ? (POLLIN | POLLOUT) : POLLIN));

    if (_poller.Wait(0) < 0)
        throw Error("ServerManager: poll failed.");

    std::vector<std::size_t> toRemove;
    for (std::size_t i = 0; i < _clients.size(); ++i) {
        Client& c = _clients[i];
        const short rev = _poller.Revents(i + 1);
        c.doRead = false;
        if (rev & (POLLERR | POLLHUP | POLLNVAL)) {
            toRemove.push_back(i);
            continue;
        }
        if (rev & POLLIN) {
            try {
                const std::string chunk = c.socket.Receive();
                if (chunk.empty()) {
                    toRemove.push_back(i);
                    continue;
                }
                c.cmd.append(chunk);
                c.doRead = true;
            } catch (const IError&) {
                toRemove.push_back(i);
                continue;
            }
        }
    }
    for (auto it = toRemove.rbegin(); it != toRemove.rend(); ++it) {
        _logger.LogReceive("Disconnected.", _clients[*it].Uid);
        _clients.erase(_clients.begin() + static_cast<std::ptrdiff_t>(*it));
    }

    if (_poller.Revents(0) & POLLIN) {
        try {
            Client cl(_listen.Accept(), GetUid());
            _logger.LogReceive("New conexion.", cl.Uid);
            _clients.emplace_back(std::move(cl));
        } catch (const IError& e) {
            std::cerr << "ServerManager: " << e.what();
        }
    }
}

std::size_t ServerManager::GetUid() {
    std::vector<std::size_t> uidTab;

    for (std::size_t i = 1; i <= _clients.size() + 1; i++)
        uidTab.push_back(i);
    for (std::size_t i = 0; i < _clients.size(); i++)
        uidTab[_clients[i].Uid - 1] = 0;
    for (std::size_t i = 0; i < uidTab.size(); i++)
        if (uidTab[i] != 0)
            return uidTab[i];
    return _clients.size() + 1;
}

void ServerManager::Update(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& map) {
    (void)map;
    (void)objects;
    (void)lights;
    (void)camera;

    DoPoll();

    for (auto& cl : _clients) {
        if (cl.doRead) {
            std::string::size_type pos;
            while ((pos = cl.cmd.find('\n')) != std::string::npos) {
                std::string line = cl.cmd.substr(0, pos);
                cl.cmd.erase(0, pos + 1);
                if (!line.empty() && line.back() == '\r')
                    line.pop_back();
                if (!line.empty())
                    HandleCommand(cl, line);
            }
            cl.doRead = false;
        }
        if (cl.doWrite && !cl.MsgQueu.empty()) {
            std::string message;
            for (const auto& entry : cl.MsgQueu) {
                std::string line = entry;
                while (!line.empty() && (line.back() == '\n' || line.back() == '\r'))
                    line.pop_back();
                message += line;
                message += "\r\n";
            }
            if (!message.empty()) {
                try {
                    cl.socket.SendAll(message.data(), message.size());
                } catch (const IError&) {
                    std::cerr << "ServerManager: cannot write to client.\n";
                }
            }
            cl.MsgQueu.clear();
            cl.doWrite = false;
        }
    }
}

}