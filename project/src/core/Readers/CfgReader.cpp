/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** CfgReader.cpp
*/

#include "CfgReader.hpp"

namespace raytracer {

void CfgReader::SetUp() {
    try {
        _cfg.readFile(_sceneFile.c_str());
    } catch (const libconfig::FileIOException&) {
        throw Error("Core: cannot read scene file '\"" + _sceneFile + "\"'.\n");
    } catch (const libconfig::ParseException& ex) {
        throw Error("Core: parse error in scene file '\"" + _sceneFile + "\"' at line "
            + std::to_string(ex.getLine()) + ": " + ex.getError() + "\n");
    }
}

const libconfig::Setting& CfgReader::CameraExist() {
    const libconfig::Setting& root = _cfg.getRoot();
    if (!root.exists("camera"))
        throw Error("Core: missing 'camera' section in scene file.\n");
    return root;
}

std::pair<std::size_t, std::size_t> CfgReader::GetCameraResolution() {
    const libconfig::Setting& root = CameraExist();
    const libconfig::Setting& camera = root["camera"];
    if (!camera.exists("resolution"))
        throw Error("Core: missing 'camera.resolution' section in scene file.\n");
    const libconfig::Setting& resolution = camera["resolution"];

    int width = 0, height = 0;
    if (!resolution.lookupValue("width", width) || !resolution.lookupValue("height", height))
        throw Error("Core: resolution width/height are missing in scene file.\n");
    if (width <= 0 || height <= 0)
        throw Error("Core: resolution width/height must be positive values.\n");

    return {static_cast<std::size_t>(width), static_cast<std::size_t>(height)};
}

std::array<int, 3> CfgReader::GetCameraPosition() {
    const libconfig::Setting& root = CameraExist();
    const libconfig::Setting& camera = root["camera"];
    if (!camera.exists("position"))
        throw Error("Core: missing 'camera.position' section in scene file.\n");
    const libconfig::Setting& position = camera["position"];

    int x = 0, y = 0, z = 0;
    if (!position.lookupValue("x", x) || !position.lookupValue("y", y) || !position.lookupValue("z", z))
        throw Error("Core: position are missing in scene file.\n");
    return {x, y, z};
}

std::array<int, 3> CfgReader::GetCameraRotation() {
    const libconfig::Setting& root = CameraExist();
    const libconfig::Setting& camera = root["camera"];
    if (!camera.exists("rotation"))
        throw Error("Core: missing 'camera.rotation' section in scene file.\n");
    const libconfig::Setting& position = camera["rotation"];

    int x = 0, y = 0, z = 0;
    if (!position.lookupValue("x", x) || !position.lookupValue("y", y) || !position.lookupValue("z", z))
        throw Error("Core: position are missing in scene file.\n");
    return {x, y, z};
}

double CfgReader::GetCameraFieldOfView() {
    const libconfig::Setting& root = CameraExist();
    const libconfig::Setting& camera = root["camera"];
    double fov = 0;
    if (!camera.lookupValue("fieldOfView", fov))
        throw Error("Core: missing 'camera.fieldOfView' section in scene file.\n");
    return fov;
}

double CfgReader::GetAmbientLight() {
    const libconfig::Setting& root = _cfg.getRoot();
    if (!root.exists("lights"))
        return 0.0;
    double ambient = 0.0;
    root["lights"].lookupValue("ambient", ambient);
    return ambient;
}

double CfgReader::GetDiffuseLight() {
    const libconfig::Setting& root = _cfg.getRoot();
    if (!root.exists("lights"))
        return 0.0;
    double diffuse = 0.0;
    root["lights"].lookupValue("diffuse", diffuse);
    return diffuse;
}

std::vector<std::unique_ptr<ILight>> CfgReader::GetLights() {
    std::vector<std::unique_ptr<ILight>> lights;

    const std::vector<std::string> lightTypes = {"directional", "point"};

    for (const auto& type : lightTypes) {
        auto sub = loadSubList<ILight>("lights", type);
        for (auto& l : sub)
            lights.push_back(std::move(l));
    }
    return lights;
}

std::map<std::string, std::string> CfgReader::settingToParams(const libconfig::Setting& cfg, const std::string& prefix) {
    std::map<std::string, std::string> params;
    for (int i = 0; i < cfg.getLength(); ++i) {
        const libconfig::Setting& field = cfg[i];
        const char* rawName = field.getName();
        std::string key = rawName ? (prefix.empty() ? rawName : prefix + "." + rawName) : prefix;
        if (field.isGroup()) {
            for (auto& kv : settingToParams(field, key))
                params.emplace(kv.first, kv.second);
            continue;
        }
        switch (field.getType()) {
            case libconfig::Setting::TypeString:
                params[key] = std::string(field.c_str() ? field.c_str() : "");
                break;
            case libconfig::Setting::TypeInt:
                params[key] = std::to_string(static_cast<int>(field));
                break;
            case libconfig::Setting::TypeFloat:
                params[key] = std::to_string(static_cast<double>(field));
                break;
            case libconfig::Setting::TypeBoolean:
                params[key] = static_cast<bool>(field) ? "true" : "false";
                break;
            default:
                params[key] = std::string(field.c_str() ? field.c_str() : "");
        }
    }
    return params;
}

}