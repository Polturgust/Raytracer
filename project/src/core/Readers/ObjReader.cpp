/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** ObjReader.cpp
*/

#include "ObjReader.hpp"

namespace raytracer {

void ObjReader::SetUp()
{
    std::ifstream file(_sceneFile);
    if (!file.is_open())
        throw Error("Core: cannot read obj file '\"" + _sceneFile + "\"'.\n");

    std::string line;
    while (std::getline(file, line)) {
        if (line.size() > 1 && line[0] == 'v' && line[1] == ' ')
            _hasVertices = true;
        if (line.size() > 1 && line[0] == 'f' && line[1] == ' ')
            _hasFaces = true;
        if (_hasVertices && _hasFaces)
            break;
    }

    if (!_hasVertices || !_hasFaces)
        throw Error("Core: invalid obj file '\"" + _sceneFile + "\"' (missing vertices or faces).\n");
}

std::pair<std::size_t, std::size_t> ObjReader::GetCameraResolution()
{
    return {800, 600};
}

std::array<int, 3> ObjReader::GetCameraPosition()
{
    return {0, 0, -10};
}

std::array<int, 3> ObjReader::GetCameraRotation()
{
    return {0, 0, 0};
}

double ObjReader::GetCameraFieldOfView()
{
    return 60.0;
}

std::vector<std::unique_ptr<IObject>> ObjReader::GetObjects()
{
    std::vector<std::unique_ptr<IObject>> objects;
    std::map<std::string, std::string> param;

    // ObjReader owns file-format parsing responsibility; mesh plugin only builds render object.
    param["name"] = "mesh";
    param["file"] = _sceneFile;
    param["scale"] = "1.0";
    param["position.x"] = "0.0";
    param["position.y"] = "0.0";
    param["position.z"] = "0.0";
    param["rotation.x"] = "0.0";
    param["rotation.y"] = "0.0";
    param["rotation.z"] = "0.0";
    param["color.r"] = "200";
    param["color.g"] = "200";
    param["color.b"] = "200";

    try {
        objects.push_back(ldloader.load<IObject>("mesh", param));
    } catch (const IError& e) {
        throw Error(std::string("Core: cannot create mesh from obj: ") + e.what() + "\n");
    }

    return objects;
}

std::vector<std::unique_ptr<ILight>> ObjReader::GetLights()
{
    // Current renderer uses a fixed directional light in shading.
    return {};
}

}
