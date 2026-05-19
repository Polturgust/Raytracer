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
    #include "printer.hpp"

namespace raytracer {

class CfgReader : public AReader {
    libconfig::Config _cfg;

    const libconfig::Setting& CameraExist();
    std::map<std::string, std::string> settingToParams(const libconfig::Setting& cfg, const std::string& prefix = "");
    void TakeOneValue(const libconfig::Setting& cfg, std::map<std::string, std::string>& params, const std::string& prefix);

    template<typename T>
    std::vector<std::unique_ptr<T>> loadLists(std::string name_list) {
        std::vector<std::unique_ptr<T>> list;
        const libconfig::Setting& root = _cfg.getRoot();
        if (!root.exists(name_list))
            throw Warning("Reader: No Objets found.\n");
        const libconfig::Setting& objects = _cfg.lookup(name_list);
        for (int i = 0; i < objects.getLength(); ++i) {
            const libconfig::Setting& group = objects[i];
            const char *rawName = group.getName();
            std::string groupName = rawName ? rawName : "";
            if (groupName.empty()) {
                if (!group.lookupValue("type", groupName) || groupName.empty())
                    throw Warning("Reader: missing plugin type in scene file.\n");
            }

            std::map<std::string, std::string> param = settingToParams(group);
            try {
                list.push_back(ldloader.load<T>(groupName, param));
            } catch (const IError& e) {
                if (e.code() == 0 && std::find(NoOpenFile.begin(), NoOpenFile.end(), groupName) == NoOpenFile.end())
                    NoOpenFile.push_back(groupName);
                if (e.code() == 84)
                    std::cout << Color::RED << "Error " << Color::RESET << e.what() << std::endl;
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

    std::vector<std::unique_ptr<IObject>> GetObjects() {return loadLists<IObject>("primitives");};
    std::vector<std::unique_ptr<ILight>> GetLights() {return loadLists<ILight>("lights");};

};

}

#endif
