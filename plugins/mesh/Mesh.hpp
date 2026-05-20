/*
** EPITECH PROJECT, 2025
** raytracer mesh plugin
** File description:
** Mesh.hpp
*/

#ifndef MESH_HPP
    #define MESH_HPP

    #include <vector>
    #include <map>
    #include <string>
    #include <memory>

    #include "IObject.hpp"
    #include "Point3D.hpp"
    #include "Vector3D.hpp"
    #include "Ray.hpp"

namespace raytracer::plugins {

class Triangle {
public:
    math::Point3D v0, v1, v2;
    Triangle(const math::Point3D& a, const math::Point3D& b, const math::Point3D& c)
        : v0(a), v1(b), v2(c) {}
    bool intersect(const core::Ray& ray, double& distance, math::Vector3D& normal) const;
    math::Vector3D getNormal() const;
};

class Mesh : public raytracer::IObject {
private:
    std::vector<Triangle> _triangles;
    struct AABB {
        math::Point3D min;
        math::Point3D max;
        AABB() { min = math::Point3D(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()); max = math::Point3D(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()); }
        void expand(const math::Point3D& p) {
            if (p.x < min.x) min.x = p.x;
            if (p.y < min.y) min.y = p.y;
            if (p.z < min.z) min.z = p.z;
            if (p.x > max.x) max.x = p.x;
            if (p.y > max.y) max.y = p.y;
            if (p.z > max.z) max.z = p.z;
        }
        void expand(const AABB& o) { expand(o.min); expand(o.max); }
        int longestAxis() const {
            double dx = max.x - min.x; double dy = max.y - min.y; double dz = max.z - min.z;
            if (dx >= dy && dx >= dz) return 0;
            if (dy >= dx && dy >= dz) return 1;
            return 2;
        }
    };
    struct BVHNode {
        AABB bounds;
        int left = -1, right = -1;
        int start = 0, count = 0;
    };
    std::vector<int> _triIndices;
    std::vector<BVHNode> _bvhNodes;
    std::array<int, 3> _color;
    math::Point3D _position;
    math::Vector3D _rotation;

public:
    Mesh(const std::array<int, 3>& color, const math::Point3D& position = {0, 0, 0}, const math::Vector3D& rotation = {0, 0, 0});
    ~Mesh() override = default;

    void buildBVH();
    int buildBVHNode(int start, int end);

    void addTriangle(const Triangle& triangle);
    
    std::array<int, 3> GetColor() const override;
    math::Point3D GetPosition() const override;
    math::Vector3D GetRotation() const;
    bool Intersect(const core::Ray& ray, double& distance) const override;
    math::Vector3D GetNormal(const math::Point3D& point) const override;
};

}

#endif
