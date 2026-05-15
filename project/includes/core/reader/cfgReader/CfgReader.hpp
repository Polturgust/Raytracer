/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** CfgReader.hpp
*/

#ifndef CFGREADER_HPP
    #define CFGREADER_HPP
    #include <libconfig.h++>
    #include <algorithm>

    #include "AReader.hpp"
    #include "Warning.hpp"

namespace raytracer {

class CfgReader : public AReader {
    libconfig::Config _cfg;

    const libconfig::Setting& CameraExist();
    std::map<std::string, std::string> settingToParams(const libconfig::Setting& cfg, const std::string& prefix = "");

    template<typename T>
    std::vector<std::unique_ptr<T>> loadSubList(const std::string& section, const std::string& name_list) {
        std::vector<std::unique_ptr<T>> list;
        const libconfig::Setting& root = _cfg.getRoot();
        if (!root.exists(section))
            return list;
        const libconfig::Setting& sec = root[section.c_str()];
        if (!sec.exists(name_list))
            return list;
        const libconfig::Setting& objects = sec[name_list.c_str()];
        for (int i = 0; i < objects.getLength(); ++i) {
            const libconfig::Setting& group = objects[i];
            std::map<std::string, std::string> param = settingToParams(group);
            try {
                list.push_back(ldloader.load<T>(name_list, param));
            } catch (const IError& e) {
                if (e.code() == 0 && std::find(NoOpenFile.begin(), NoOpenFile.end(), name_list) == NoOpenFile.end())
                    NoOpenFile.push_back(name_list);
            }
        }
        return list;
    }

    template<typename T>
    std::vector<std::unique_ptr<T>> loadLists(std::string name_list) {
        std::vector<std::unique_ptr<T>> list;
        const libconfig::Setting& root = _cfg.getRoot();
        if (!root.exists(name_list))
            throw Warning("Reader: No Objets found.\n");
        const libconfig::Setting& objects = _cfg.lookup(name_list);
        for (int i = 0; i < objects.getLength(); ++i) {
            const libconfig::Setting& group = objects[i];
            std::string name = group.getName();
            std::map<std::string, std::string> param = settingToParams(group);
            try {
                list.push_back(ldloader.load<T>(name, param));
            } catch (const IError& e) {
                if (e.code() == 0 && std::find(NoOpenFile.begin(), NoOpenFile.end(), name) == NoOpenFile.end())
                    NoOpenFile.push_back(name);
            }
        }
        return list;
    }

public:
    CfgReader(std::string NameFile): AReader(NameFile) {SetUp();};
    ~CfgReader() = default;

    void SetUp();
    std::pair<std::size_t, std::size_t> GetCameraResolution();
    std::array<int, 3> GetCameraPosition();
    std::array<int, 3> GetCameraRotation();
    double GetCameraFieldOfView();

    double GetAmbientLight();
    double GetDiffuseLight();

    std::vector<std::unique_ptr<IObject>> GetObjects() { return loadLists<IObject>("primitives"); };
    std::vector<std::unique_ptr<ILight>> GetLights();
};

}

#endif