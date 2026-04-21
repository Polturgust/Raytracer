/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** args.hpp
*/

#ifndef ARGS_HPP
    #define ARGS_HPP
    #include <string.h>
    #include <filesystem>
    #include <array>

    #include "Error.hpp"
    #include "Warning.hpp"

namespace raytracer {

typedef enum {
    DEFAULT = 0,
    SERVER,
    CLIENT,
}raytracer_type;

class Argument {
    raytracer_type _type = DEFAULT;
    std::string _ip = "";
    std::size_t _port = 0;

    std::size_t TestPort(std::string port);
    void TestIp(std::string ip);
    void TestFile(std::string);

public:
    Argument(int argc, char **argv);
    ~Argument() = default;

    raytracer_type GetType() {return _type;};
    std::size_t GetPort() {return _port;};
    std::string GetIp() {return _ip;};
};

}

#endif
