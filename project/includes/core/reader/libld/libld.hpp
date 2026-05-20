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
    #include "FileNotFound.hpp"

namespace raytracer {

class ld {
public:
    ld() {};
    ~ld() {};

    template<typename T>
    std::unique_ptr <T> load(std::string path, std::map<std::string, std::string> param) {
        std::string realpath("plugins/raytracer_" + path + ".so");
        void *handle = dlopen(realpath.c_str(), RTLD_LAZY);
        if (!handle)
            throw FileNotFound("No file match in plugin with name : " + realpath);
        dlerror();
        const char* err = dlerror();
        /*auto getSoType = reinterpret_cast<SoTypeEnum (*) ()> (dlsym(handle, "getSOType"));
        if (err) {
            dlclose(handle);
            throw Error("So type not found");
        }
        SoTypeEnum type = getSoType();*/
        T* (*getObject)(std::map<std::string, std::string>) = reinterpret_cast<T* (*)(std::map<std::string, std::string>)> (dlsym(handle, "getObject"));
        if (err) {
            dlclose(handle);
            throw Error("libld : getObject not found.");
        }
        return std::unique_ptr<T> (getObject(param));
    }
};

}

#endif
