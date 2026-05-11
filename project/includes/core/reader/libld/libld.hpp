/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** libld.hpp
*/

#ifndef LIBLD_HPP
    #define LIBLD_HPP
    #define LD_HPP
    #include <dlfcn.h>
    #include <memory>
    #include <iostream>
    #include <map>

    #include "IObject.hpp"
    #include "SoType.hpp"
    #include "Warning.hpp"
    #include "Error.hpp"

namespace raytracer {

class ld {
public:
    ld() {};
    ~ld() {};

    template<typename T>
    std::unique_ptr <T> load(std::string path, std::map<std::string, std::string> param) {
        std::string realpath("plugins/raytracer_" + path + ".so");
        void *handle = dlopen(realpath.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "dlopen: " << dlerror() << "\n";
            throw Warning("No file match in plugin with name : raytracer_" + path + ".so.\n");
        }
        dlerror();
        auto getSoType = reinterpret_cast<SoTypeEnum (*) ()> (dlsym(handle, "getSOType"));
        const char* err = dlerror();
        if (err) {
            dlclose(handle);
            throw Error("So type not found");
        }
        SoTypeEnum type = getSoType();
        T* (*getObject)(std::map<std::string, std::string>) = nullptr;
        switch (type) {
            case OBJECT:
                getObject = reinterpret_cast<T* (*)(std::map<std::string, std::string>)> (dlsym(handle, "getObject")); break;
            case LIGHT:
                getObject = reinterpret_cast<T* (*)(std::map<std::string, std::string>)> (dlsym(handle, "getLight")); break;
            case Texture:
                getObject = reinterpret_cast<T* (*)(std::map<std::string, std::string>)> (dlsym(handle, "getTexture")); break;
            default:
                break;
        }
        if (err) {
            dlclose(handle);
            throw Error("getObject type not works");
        }
        return std::unique_ptr<T> (getObject(param));
    }
};

}

#endif
