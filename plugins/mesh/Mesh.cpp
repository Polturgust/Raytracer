/*
** EPITECH PROJECT, 2025
** raytracer mesh plugin
** File description:
** Mesh.cpp
*/

#include "Mesh.hpp"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "Error.hpp"
#include "SoType.hpp"

namespace raytracer::plugins {

bool Triangle::intersect(const core::Ray& ray, double& distance, math::Vector3D& normal) const {
    const double EPSILON = 0.0001;
    math::Vector3D edge1 = v1 - v0;
    math::Vector3D edge2 = v2 - v0;
    math::Vector3D h = ray.direction.cross(edge2);
    double a = edge1.dot(h);
    if (std::abs(a) < EPSILON)
        return false;
    double f = 1.0 / a;
    math::Vector3D s = ray.origin - v0;
    double u = f * s.dot(h);
    if (u < 0.0 || u > 1.0)
        return false;
    math::Vector3D q = s.cross(edge1);
    double v = f * ray.direction.dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    double t = f * edge2.dot(q);
    if (t > EPSILON) {
        distance = t;
        normal = getNormal();
        return true;
    }
    return false;
}

math::Vector3D Triangle::getNormal() const {
    math::Vector3D edge1 = v1 - v0;
    math::Vector3D edge2 = v2 - v0;
    return edge1.cross(edge2).normalized();
}

Mesh::Mesh(const std::array<int, 3>& color, const math::Point3D& position, const math::Vector3D& rotation)
    : _color(color), _position(position), _rotation(rotation) {}

void Mesh::addTriangle(const Triangle& triangle) {
    _triangles.push_back(triangle);
}

std::array<int, 3> Mesh::GetColor() const {
    return _color;
}

math::Point3D Mesh::GetPosition() const {
    return _position;
}

math::Vector3D Mesh::GetRotation() const {
    return _rotation;
}

bool Mesh::Intersect(const core::Ray& ray, double& distance) const {
    if (_bvhNodes.empty()) {
        double closestDistance = std::numeric_limits<double>::max();
        bool hit = false;
        for (const auto& triangle : _triangles) {
            double t;
            math::Vector3D normal;
            if (triangle.intersect(ray, t, normal)) {
                if (t < closestDistance) {
                    closestDistance = t;
                    distance = t;
                    hit = true;
                }
            }
        }
        return hit;
    }

    double closest = std::numeric_limits<double>::max();
    bool hitAny = false;
    std::vector<int> stack;
    stack.reserve(64);
    stack.push_back(0);

    auto rayIntersectsAABBWithT = [&](const AABB& b, double& outTmin)->bool {
        double tmin = -std::numeric_limits<double>::infinity();
        double tmax = std::numeric_limits<double>::infinity();
        auto check = [&](double origin, double dir, double minB, double maxB)->bool {
            if (std::abs(dir) < 1e-12) {
                if (origin < minB || origin > maxB) return false;
                return true;
            }
            double inv = 1.0 / dir;
            double t1 = (minB - origin) * inv;
            double t2 = (maxB - origin) * inv;
            if (t1 > t2) std::swap(t1, t2);
            if (t1 > tmin) tmin = t1;
            if (t2 < tmax) tmax = t2;
            return tmin <= tmax;
        };
        if (!check(ray.origin.x, ray.direction.x, b.min.x, b.max.x)) return false;
        if (!check(ray.origin.y, ray.direction.y, b.min.y, b.max.y)) return false;
        if (!check(ray.origin.z, ray.direction.z, b.min.z, b.max.z)) return false;
        outTmin = tmin;
        return true;
    };

    while (!stack.empty()) {
        int nodeIdx = stack.back(); stack.pop_back();
        const BVHNode& node = _bvhNodes[nodeIdx];
        double nodeTmin = 0.0;
        if (!rayIntersectsAABBWithT(node.bounds, nodeTmin)) continue;
        if (nodeTmin > closest) continue;
        if (node.count > 0) {
            for (int i = 0; i < node.count; ++i) {
                int triIdx = _triIndices[node.start + i];
                const Triangle& tri = _triangles[triIdx];
                double t;
                math::Vector3D normal;
                if (tri.intersect(ray, t, normal)) {
                    if (t < closest) {
                        closest = t;
                        distance = t;
                        hitAny = true;
                    }
                }
            }
        } else {
            double tL = std::numeric_limits<double>::infinity();
            double tR = std::numeric_limits<double>::infinity();
            if (node.left != -1) rayIntersectsAABBWithT(_bvhNodes[node.left].bounds, tL);
            if (node.right != -1) rayIntersectsAABBWithT(_bvhNodes[node.right].bounds, tR);
            if (tL < tR) {
                if (node.right != -1 && tR <= closest) stack.push_back(node.right);
                if (node.left != -1 && tL <= closest) stack.push_back(node.left);
            } else {
                if (node.left != -1 && tL <= closest) stack.push_back(node.left);
                if (node.right != -1 && tR <= closest) stack.push_back(node.right);
            }
        }
    }
    return hitAny;
}

math::Vector3D Mesh::GetNormal(const math::Point3D& point) const {
    double closestDist = std::numeric_limits<double>::max();
    math::Vector3D closestNormal(0, 1, 0);
    for (const auto& triangle : _triangles) {
        double d = (point - triangle.v0).length();
        if (d < closestDist) {
            closestDist = d;
            closestNormal = triangle.getNormal();
        }
    }
    return closestNormal;
}

std::vector<math::Point3D> parseOBJFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open())
        throw Error("Mesh: cannot open OBJ file '" + filepath + "'");
    std::vector<math::Point3D> vertices;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#' || line[0] == 'm' || line[0] == 'u' || line[0] == 's' || line[0] == 'o')
            continue;
        if (line[0] == 'v' && line[1] == ' ') {
            std::istringstream iss(line.substr(2));
            double x, y, z;
            if (iss >> x >> y >> z) {
                vertices.push_back(math::Point3D(x, y, z));
            }
        }
        else if (line[0] == 'f' && line[1] == ' ') {
            std::istringstream iss(line.substr(2));
            std::vector<int> indices;
            std::string vertex;
            while (iss >> vertex) {
                size_t slashPos = vertex.find('/');
                if (slashPos != std::string::npos)
                    vertex = vertex.substr(0, slashPos);
                try {
                    int idx = std::stoi(vertex) - 1;
                    if (idx >= 0 && idx < static_cast<int>(vertices.size()))
                        indices.push_back(idx);
                } catch (...) {
                    continue;
                }
            }
            for (size_t i = 1; i + 1 < indices.size(); ++i) {
                if (indices[0] < static_cast<int>(vertices.size()) &&
                    indices[i] < static_cast<int>(vertices.size()) &&
                    indices[i + 1] < static_cast<int>(vertices.size())) {
                }
            }
        }
    }
    file.close();
    return vertices;
}

static math::Point3D triangleCentroid(const Triangle& t) {
    return math::Point3D((t.v0.x + t.v1.x + t.v2.x) / 3.0,
                         (t.v0.y + t.v1.y + t.v2.y) / 3.0,
                         (t.v0.z + t.v1.z + t.v2.z) / 3.0);
}

void Mesh::buildBVH() {
    _triIndices.resize(_triangles.size());
    for (size_t i = 0; i < _triIndices.size(); ++i) _triIndices[i] = static_cast<int>(i);
    _bvhNodes.clear();
    if (_triIndices.empty()) return;
    buildBVHNode(0, static_cast<int>(_triIndices.size()));
}

int Mesh::buildBVHNode(int start, int end) {
    BVHNode node;
    node.start = start;
    node.count = end - start;
    for (int i = start; i < end; ++i) {
        const Triangle& t = _triangles[_triIndices[i]];
        node.bounds.expand(t.v0);
        node.bounds.expand(t.v1);
        node.bounds.expand(t.v2);
    }
    int myIndex = static_cast<int>(_bvhNodes.size());
    _bvhNodes.push_back(node);

    if (node.count <= 4) {
        _bvhNodes[myIndex] = node;
        return myIndex;
    }

    int axis = node.bounds.longestAxis();
    std::sort(_triIndices.begin() + start, _triIndices.begin() + end, [&](int a, int b){
        math::Point3D ca = triangleCentroid(_triangles[a]);
        math::Point3D cb = triangleCentroid(_triangles[b]);
        if (axis == 0) return ca.x < cb.x;
        if (axis == 1) return ca.y < cb.y;
        return ca.z < cb.z;
    });
    int mid = (start + end) / 2;
    int left = buildBVHNode(start, mid);
    int right = buildBVHNode(mid, end);
    _bvhNodes[myIndex].left = left;
    _bvhNodes[myIndex].right = right;
    _bvhNodes[myIndex].count = 0;
    _bvhNodes[myIndex].start = 0;
    _bvhNodes[myIndex].bounds = _bvhNodes[left].bounds;
    _bvhNodes[myIndex].bounds.expand(_bvhNodes[right].bounds);
    return myIndex;
}

} // namespace raytracer::plugins

extern "C" {

using namespace raytracer::plugins;
using namespace raytracer;

SoTypeEnum getSOType() {
    return OBJECT;
}

IObject* getObject(std::map<std::string, std::string> params) {
    std::string filepath = "";
    double scale = 1.0;
    double pos_x = 0.0, pos_y = 0.0, pos_z = 0.0;
    int r = 200, g = 200, b = 200;
    try {
        if (params.count("file") && !params.at("file").empty())
            filepath = params.at("file");
        else
            throw Error("Mesh: missing 'file' parameter (OBJ file path)");
        if (params.count("scale") && !params.at("scale").empty())
            scale = std::stod(params.at("scale"));
        if (params.count("position.x") && !params.at("position.x").empty())
            pos_x = std::stod(params.at("position.x"));
        if (params.count("position.y") && !params.at("position.y").empty())
            pos_y = std::stod(params.at("position.y"));
        if (params.count("position.z") && !params.at("position.z").empty())
            pos_z = std::stod(params.at("position.z"));
        if (params.count("color.r") && !params.at("color.r").empty())
            r = std::stoi(params.at("color.r"));
        if (params.count("color.g") && !params.at("color.g").empty())
            g = std::stoi(params.at("color.g"));
        if (params.count("color.b") && !params.at("color.b").empty())
            b = std::stoi(params.at("color.b"));
        double rot_x = 0.0, rot_y = 0.0, rot_z = 0.0;
        if (params.count("rotation.x") && !params.at("rotation.x").empty())
            rot_x = std::stod(params.at("rotation.x"));
        if (params.count("rotation.y") && !params.at("rotation.y").empty())
            rot_y = std::stod(params.at("rotation.y"));
        if (params.count("rotation.z") && !params.at("rotation.z").empty())
            rot_z = std::stod(params.at("rotation.z"));

        Mesh* mesh = new Mesh({r, g, b}, math::Point3D(pos_x, pos_y, pos_z), math::Vector3D(rot_x, rot_y, rot_z));

        std::ifstream file(filepath);
        if (!file.is_open())
            throw Error("Mesh: cannot open OBJ file '" + filepath + "'");
        std::vector<math::Point3D> vertices;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#' || line[0] == 'm' || line[0] == 'u' || line[0] == 's' || line[0] == 'o') {
                continue;
            }
            if (line[0] == 'v' && line[1] == ' ') {
                    std::istringstream iss(line.substr(2));
                    double x, y, z;
                    if (iss >> x >> y >> z) {
                        vertices.push_back(math::Point3D(x * scale, y * scale, z * scale));
                    }
                }
            else if (line[0] == 'f' && line[1] == ' ') {
                std::istringstream iss(line.substr(2));
                std::vector<int> indices;
                std::string vertex;
                while (iss >> vertex) {
                    size_t slashPos = vertex.find('/');
                    if (slashPos != std::string::npos)
                        vertex = vertex.substr(0, slashPos);
                    try {
                        int idx = std::stoi(vertex) - 1;
                        if (idx >= 0 && idx < static_cast<int>(vertices.size()))
                            indices.push_back(idx);
                    } catch (...) {
                        continue;
                    }
                }
                const double deg2rad = 3.14159265358979323846 / 180.0;
                double rx = rot_x * deg2rad;
                double ry = rot_y * deg2rad;
                double rz = rot_z * deg2rad;

                auto rotatePoint = [&](const math::Point3D& p)->math::Point3D {
                    double x1 = p.x;
                    double y1 = p.y * std::cos(rx) - p.z * std::sin(rx);
                    double z1 = p.y * std::sin(rx) + p.z * std::cos(rx);
                    double x2 = x1 * std::cos(ry) + z1 * std::sin(ry);
                    double y2 = y1;
                    double z2 = -x1 * std::sin(ry) + z1 * std::cos(ry);
                    double x3 = x2 * std::cos(rz) - y2 * std::sin(rz);
                    double y3 = x2 * std::sin(rz) + y2 * std::cos(rz);
                    double z3 = z2;
                    return math::Point3D(x3 + pos_x, y3 + pos_y, z3 + pos_z);
                };

                for (size_t i = 1; i + 1 < indices.size(); ++i) {
                    if (indices[0] < static_cast<int>(vertices.size()) &&
                        indices[i] < static_cast<int>(vertices.size()) &&
                        indices[i + 1] < static_cast<int>(vertices.size())) {
                        math::Point3D p0 = rotatePoint(vertices[indices[0]]);
                        math::Point3D p1 = rotatePoint(vertices[indices[i]]);
                        math::Point3D p2 = rotatePoint(vertices[indices[i + 1]]);
                        mesh->addTriangle(Triangle(p0, p1, p2));
                    }
                }
            }
        }
        file.close();
        mesh->buildBVH();
        return mesh;
    } catch (const std::exception& e) {
        throw Error(std::string("Error creating mesh: ") + e.what());
        return nullptr;
    }
}

}
