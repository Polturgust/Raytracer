#include "Cylinder.hpp"

#include <cmath>
#include <iostream>

namespace raytracer::plugins {

Cylinder::Cylinder(const math::Point3D& center, const math::Vector3D& axis, double radius, double height, const std::array<int,3>& color)
    : AObject(nullptr), _center(center), _axis(axis.lengthSquared() == 0.0 ? math::Vector3D(0.0,0.0,1.0) : axis.normalized()), _radius(radius), _height(height), _color(color)
{
}

std::array<int,3> Cylinder::GetColor() const { return _color; }

math::Point3D Cylinder::GetPosition() const { return _center; }

bool Cylinder::Intersect(const core::Ray& ray, double& distance) const {
    constexpr double EPS = 1e-6;
    if (_radius <= 0.0) return false;

    const math::Vector3D d = ray.direction;
    const math::Vector3D oc = ray.origin - _center;

    const double dDotA = d.dot(_axis);
    const double ocDotA = oc.dot(_axis);

    const math::Vector3D dPerp = d - (_axis * dDotA);
    const math::Vector3D ocPerp = oc - (_axis * ocDotA);

    const double a = dPerp.dot(dPerp);
    const double b = 2.0 * dPerp.dot(ocPerp);
    const double c = ocPerp.dot(ocPerp) - _radius * _radius;

    if (std::abs(a) < EPS) return false;
    const double disc = b*b - 4*a*c;
    if (disc < 0.0) return false;
    const double sqrtD = std::sqrt(disc);
    const double t0 = (-b - sqrtD) / (2.0*a);
    const double t1 = (-b + sqrtD) / (2.0*a);

    auto valid = [&](double t) -> bool {
        if (t <= EPS) return false;
        if (_height > 0.0) {
            const double half = _height*0.5;
            const math::Point3D p = ray.origin + (ray.direction * t);
            const double proj = (p - _center).dot(_axis);
            return std::abs(proj) <= half;
        }
        return true;
    };

    double best = 1e30;

    if (valid(t0))
        best = std::min(best, t0);
    if (valid(t1))
        best = std::min(best, t1);

    // caps
    if (_height > 0.0) {
        double half = _height*0.5;
        double denom = d.dot(_axis);
        if (std::abs(denom) > EPS) {
            math::Point3D capCenter = _center + (_axis * half);
            double tcap = (capCenter - ray.origin).dot(_axis) / denom;
            if (tcap > EPS) {
                math::Point3D hit = ray.origin + d * tcap;
                math::Vector3D delta = hit - capCenter;
                math::Vector3D radial = delta - _axis * delta.dot(_axis);
                if (radial.lengthSquared() <= _radius*_radius) best = std::min(best, tcap);
            }
            capCenter = _center - (_axis * half);
            tcap = (capCenter - ray.origin).dot(_axis) / denom;
            if (tcap > EPS) {
                math::Point3D hit = ray.origin + d * tcap;
                math::Vector3D delta = hit - capCenter;
                math::Vector3D radial = delta - _axis * delta.dot(_axis);
                if (radial.lengthSquared() <= _radius*_radius) best = std::min(best, tcap);
            }
        }
    }

    if (best < 1e30) { distance = best; return true; }
    return false;
}

math::Vector3D Cylinder::GetNormal(const math::Point3D& point) const {
    // compute normal at point (body or cap)
    if (_height > 0.0) {
        const double half = _height*0.5;
        double proj = (point - _center).dot(_axis);
        if (std::abs(std::abs(proj) - half) < 1e-4) {
            // cap normal
            return proj > 0 ? _axis : (_axis * -1.0);
        }
    }
    math::Vector3D v = point - _center;
    double proj = v.dot(_axis);
    math::Point3D projPoint = _center + (_axis * proj);
    return (point - projPoint).normalized();
}

} // namespace raytracer::plugins

extern "C" {
    using namespace raytracer::plugins;
    using namespace raytracer;

    SoTypeEnum getSOType() { return OBJECT; }

    IObject* getObject(std::map<std::string,std::string> params) 
    {
        double x=0,y=0,z=0;
        double axisX=0,axisY=0,axisZ=1;
        double radius=1.0, height=-1.0;
        int r=200,g=200,b=200;

        try {
            if (params.count("x") && !params.at("x").empty())
                x = std::stod(params.at("x"));
            if (params.count("y") && !params.at("y").empty())
                y = std::stod(params.at("y"));
            if (params.count("z") && !params.at("z").empty())
                z = std::stod(params.at("z"));
    
            if (params.count("axisX") && !params.at("axisX").empty())
                axisX = std::stod(params.at("axisX"));
            if (params.count("axisY") && !params.at("axisY").empty())
                axisY = std::stod(params.at("axisY"));
            if (params.count("axisZ") && !params.at("axisZ").empty())
                axisZ = std::stod(params.at("axisZ"));

            if (params.count("radius") && !params.at("radius").empty())
                radius = std::stod(params.at("radius"));
            if (params.count("r") && !params.at("r").empty() && radius <= 0.0) 
                radius = std::stod(params.at("r"));
            if (params.count("height") && !params.at("height").empty())
                height = std::stod(params.at("height"));

            if (params.count("color.r") && !params.at("color.r").empty())
                r = std::stoi(params.at("color.r"));
            if (params.count("color.g") && !params.at("color.g").empty())
                g = std::stoi(params.at("color.g"));
            if (params.count("color.b") && !params.at("color.b").empty())
                b = std::stoi(params.at("color.b"));

            return new Cylinder(math::Point3D(x,y,z), math::Vector3D(axisX,axisY,axisZ), radius, height, {r,g,b});
        } catch (const std::exception &e) {
            throw Error(std::string("Error creating cylinder: ") + e.what());
        }
    }
}
