/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** clientManager.cpp
*/

#include "ClientManager.hpp"
#include "CfgReader.hpp"

#include <sstream>

namespace raytracer {

ClientManager::ClientManager(const std::string& ip, std::uint16_t port): _ip(ip), _port(port){}

void ClientManager::Advance() {
    if (_stage < COMPUTE)
        _stage = static_cast<ClientStage>(_stage + 1);
}

void ClientManager::ConnectIfNeeded() {
    if (_connected)
        return;
    _socket.Connect(_ip, _port);
    _logger.LogSend("Connected to the server at " + _ip + " " +  std::to_string(_port));
    _connected = true;
    Advance();
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
    _poller.Clear();
    _poller.Watch(_socket.Fd(), static_cast<short>(POLLIN | (_outgoing.empty() ? 0 : POLLOUT)));
    _poller.Watch(STDIN_FILENO, POLLIN);
    if (_poller.Wait(0) < 0)
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
        std::string chunk;
        try {
            chunk = _socket.Receive();
        } catch (...) {}
        if (chunk.empty())
            throw Warning("Client: connection closed by server.");
        _recvBuf.append(chunk);
        std::string::size_type pos;
        while ((pos = _recvBuf.find('\n')) != std::string::npos) {
            std::string line = _recvBuf.substr(0, pos);
            _recvBuf.erase(0, pos + 1);
            if (!line.empty() && line.back() == '\r')
                line.pop_back();
            _incoming.push(line);
        }
        std::cout.flush();
    }
}

void ClientManager::ProcessActiveStates(Core& core) {
    while (true) {
        switch (_stage) {
            case ASKCONFIGUE: {
                _receiver = std::make_unique<FileReceiver>();
                std::ostringstream oss;
                oss << "FGET ConfigueFile " << _socket.LocalIp() << " " << _receiver->Port();
                _logger.LogSend(oss.str());
                QueueMessage(oss.str());
                Advance();
                break;
            }
            case TRYINIT: {
                core.Init();
                std::vector<std::string> missing = core.GetReader().GetNoOpenFile();
                if (missing.empty()) {
                    core.MarkInitialized();
                    _stage = COMPUTE;
                } else {
                    _pluginsToFetch = missing;
                    _pluginIndex = 0;
                    Advance();
                }
                break;
            }
            case ASKPLUGINS: {
                if (_pluginIndex >= _pluginsToFetch.size()) {
                    _stage = INITCORE;
                    break;
                }
                const std::string& name = _pluginsToFetch[_pluginIndex];
                _receiver = std::make_unique<FileReceiver>();
                std::ostringstream oss;
                oss << "FGET plugins/raytracer_" << name << ".so " << _socket.LocalIp() << " " << _receiver->Port();
                QueueMessage(oss.str());
                _logger.LogSend(oss.str());
                Advance();
                break;
            }
            case INITCORE: {
                core.ReloadObjectsAndLights();
                core.MarkInitialized();
                _stage = COMPUTE;
                break;
            }
            default:
                return;
        }
    }
}

void ClientManager::ProcessWaitingStates(Core& core) {
    while (true) {
        ClientStage before = _stage;
        switch (_stage) {
            case WAITCONFIGUE: {
                if (!HasMessage())
                    return;
                const std::string line = PopMessage();
                _logger.LogReceive(line);
                if (line.rfind("200", 0) != 0)
                    throw Error("Client: server refused config request: " + line);
                const std::string filename = line.substr(7);
                if (filename.empty())
                    throw Error("Client: missing filename in 200 OK response.");
                if (!_receiver)
                    throw Error("Client: no FileReceiver pending for config transfer.");
                _receiver->Receive(filename);
                _configFilename = filename;
                const auto extPos = filename.rfind('.');
                const std::string ext = (extPos == std::string::npos) ? "" : filename.substr(extPos);
                std::unique_ptr<IReader> reader;
                if (ext == ".cfg")
                    reader = std::make_unique<CfgReader>(filename);
                else
                    reader = nullptr;
                core.SetReader(std::move(reader));
                _receiver.reset();
                Advance();
                break;
            }
            case WAITPLUGINS: {
                if (!HasMessage())
                    return;
                const std::string line = PopMessage();
                _logger.LogReceive(line);
                if (_pluginIndex >= _pluginsToFetch.size()) {
                    _receiver.reset();
                    _stage = INITCORE;
                    break;
                }
                const std::string& name = _pluginsToFetch[_pluginIndex];
                if (line.rfind("200", 0) == 0) {
                    if (!_receiver)
                        std::cerr << "Client: no FileReceiver pending for plugin " << name << ".\n";
                    else {
                        try {
                            _receiver->Receive("plugins/raytracer_" + name + ".so");
                        } catch (...) {}
                    }
                } else
                    std::cerr << "Client: plugin " << name << " unavailable (" << line << ").\n";
                _receiver.reset();
                _pluginIndex++;
                if (_pluginIndex < _pluginsToFetch.size())
                    _stage = ASKPLUGINS;
                else
                    _stage = INITCORE;
                break;
            }
            default:
                return;
        }
        if (_stage == before)
            return;
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
    ConnectIfNeeded();
    if (_stage == COMPUTE)
        return;
    ProcessWaitingStates(core);
    DoPoll();
    ProcessActiveStates(core);
}

}
