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
            throw Warning("No file match in plugin with name : " + realpath);
        dlerror();
        /* Attempt to locate the factory function in the plugin. Use dlerror()
           after dlsym to detect errors, and guard against calling a null
           function pointer which would cause a SIGSEGV. */
        T* (*getObject)(std::map<std::string, std::string>) = reinterpret_cast<T* (*)(std::map<std::string, std::string>)>(dlsym(handle, "getObject"));
        const char* dlsym_err = dlerror();
        if (dlsym_err || getObject == nullptr) {
            dlclose(handle);
            throw Error(std::string("libld : getObject not found: ") + (dlsym_err ? dlsym_err : "null pointer"));
        }
        return std::unique_ptr<T>(getObject(param));
    }
};

}

#endif
